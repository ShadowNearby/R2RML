//
// Created by yanjs on 2022/11/1.
//

#include "SelectQuery.h"
#include "omp.h"

const std::string schema_name = "gtfs-sql";

//namespace
SelectQuery::SelectQuery() : session(mysqlx::Session("localhost", 33060, "yanjs", "yjs135790")),
                             db(session.getSchema(schema_name))
{
//    auto temp = db.getTable("stop_times");
//    auto table = temp.select("*").execute();
//    for (const auto &item: table.fetchAll()) {
//        for (auto i = 0; i < item.colCount(); ++i) {
//            std::cout << item[i] << std::endl;
//        }
//    }
//    std::cout << "yes" << std::endl;
}


void SelectQuery::getRows(std::string tableName)
{
    result.clear();
    auto table = db.getTable(tableName);
    auto sqlResult = table.select("*").execute();
    auto &columns = sqlResult.getColumns();
    auto start = std::chrono::steady_clock::now();
    std::vector<std::string> labels;
    for (const auto &item: columns) {
        labels.emplace_back(item.getColumnLabel());
    }
//    printf("tableName:%s\n", tableName.c_str());
//    printf("labels:%d\n", labels.size());
    auto it = sqlResult.begin();
    std::vector<decltype(*it)> itVec;
    for (; it != sqlResult.end(); ++it) {
        itVec.push_back(*it);
    }
    auto tableData = new folly::ConcurrentHashMap<size_t, folly::ConcurrentHashMap<std::string, mysqlx::Value> *>();
//#pragma omp parallel for
    for (int j = 0; j < itVec.size(); ++j) {
        auto row = new folly::ConcurrentHashMap<std::string, mysqlx::Value>();
        for (int i = 0; i < labels.size(); ++i) {
            row->insert_or_assign(labels[i], itVec[j][i]);
        }
        result.insert_or_assign(j, row);
    }
    auto end = std::chrono::steady_clock::now();
    auto runTime = std::chrono::duration<double>(end - start).count();
//    printf("SQL RunTime: %fs\n", runTime);
}

void SelectQuery::getJoinRows(std::string tableName, const RefObjectMap &refObjectMap)
{
    result.clear();
    std::string child_table = tableName;
    std::string parent_table = refObjectMap.parentTableName.substr(1, refObjectMap.parentTableName.size() - 2);
    std::vector<std::string> child_columns;
    std::vector<std::string> parent_columns;
    for (const auto &item: refObjectMap.join) {
        child_columns.emplace_back(item.child.substr(1, item.child.size() - 2));
        parent_columns.emplace_back(item.parent.substr(1, item.parent.size() - 2));
    }
    std::string sql = "select * from (select * from `" + schema_name + "`." + child_table + ") as child,\n" +
                      "  (select * from `" + schema_name + "`." + parent_table +
                      ") as parent\n" +
                      "  where child." + child_columns.front() + "=parent." + parent_columns.front() + "\n";
    for (int i = 1; i < child_columns.size(); ++i) {
        std::string and_sql = "and child." + child_columns[i] + "=parent." + parent_columns[i] + "\n";
        sql += and_sql;
    }
    sql.pop_back();
    sql += ';';
    auto sqlResult = session.sql(sql).execute();
    auto &columns = sqlResult.getColumns();
    auto column_count = sqlResult.getColumnCount();
    std::vector<std::string> labels;
    auto child_column_count = db.getTable(child_table).select("*").limit(1).execute().getColumnCount();
    for (int i = 0; i < child_column_count; ++i) {
        std::string column_name = columns[i].getColumnName();
        column_name = "$" + column_name;
        labels.emplace_back(column_name);
    }
    for (int i = child_column_count; i < column_count; ++i) {
        std::string column_name = columns[i].getColumnName();
        column_name = "#" + column_name;
        labels.emplace_back(column_name);
    }
    auto it = sqlResult.begin();
    std::vector<decltype(*it)> itVec;
    for (; it != sqlResult.end(); ++it) {
        itVec.push_back(*it);
    }
//#pragma omp parallel for
    auto tableData = new folly::ConcurrentHashMap<size_t, folly::ConcurrentHashMap<std::string, mysqlx::Value> *>();
    for (int j = 0; j < itVec.size(); ++j) {
        auto row = new folly::ConcurrentHashMap<std::string, mysqlx::Value>();
        for (int i = 0; i < labels.size(); ++i) {
            row->insert_or_assign(labels[i], itVec[j][i]);
        }
        result.insert_or_assign(j, row);
    }
}

SelectQuery::~SelectQuery()
{
    session.close();
}


void SelectQuery::clearResult()
{
    result.clear();
}
