//
// Created by yanjs on 2022/11/1.
//

#include "SelectQuery.h"

#include <utility>
#include "omp.h"

//const std::string schema_name = "gtfs-sql";

//namespace
SelectQuery::SelectQuery(std::string user, std::string password, std::string schema_name) : session(
        mysqlx::Session("localhost", 33060, user, std::move(password))),
                                                                                            db(session.getSchema(
                                                                                                    schema_name))
{
    getAll();
    this->schema_name = schema_name;
}

void SelectQuery::getAll()
{
    for (const auto &tableName: db.getTableNames()) {
        std::string table_name = tableName;
        auto sqlResult = db.getTable(tableName).select("*").execute();
        auto &columns = sqlResult.getColumns();

        auto *table = new std::vector<mysqlx::Row>(sqlResult.fetchAll());

        std::unordered_map<std::string, int> labels;
        int i = 0;
        for (const auto &item: columns) {
            labels[item.getColumnLabel()] = i;
            ++i;
        }
        tables_index[table_name] = labels;
        tables[table_name] = table;
//        size_t column_count = labels.size();
//        auto table = new std::vector<std::vector<mysqlx::Value>>;
//        for (const auto &row: rows) {
//            std::vector<mysqlx::Value> vec_row(column_count);
//            for (int j = 0; j < column_count; ++j) {
//                vec_row[j] = row[j];
//            }
//            table->emplace_back(vec_row);
//        }
//        tables[table_name] = table;
    }
}


void SelectQuery::getJoinRows(std::string tableName, const RefObjectMap &refObjectMap,
                              const std::vector<std::string> &subject_columns,
                              const std::vector<std::string> &object_columns)
{
    static double cost = 0, clear_cost = 0;
    auto start = std::chrono::steady_clock::now();
//    printf("start join clear\n");
    delete join_table;
    join_table = nullptr;
    join_index.clear();
//    printf("end join clear\n");
    auto end = std::chrono::steady_clock::now();
    clear_cost += std::chrono::duration<double>(end - start).count();
//    printf("clear cost %f\n", clear_cost);
    std::string child_table = std::move(tableName);
    std::string parent_table = refObjectMap.parentTableName.substr(1, refObjectMap.parentTableName.size() - 2);
    std::vector<std::string> child_columns;
    std::vector<std::string> parent_columns;
    for (const auto &item: refObjectMap.join) {
        child_columns.emplace_back(item.child.substr(1, item.child.size() - 2));
        parent_columns.emplace_back(item.parent.substr(1, item.parent.size() - 2));
    }
    std::string child_get_columns, parent_get_columns;
    for (const auto &item: subject_columns) {
        child_get_columns += "child." + item + ",";
    }
    for (const auto &item: object_columns) {
        parent_get_columns += "parent." + item + ",";
    }
    parent_get_columns.pop_back();
    parent_get_columns.push_back(' ');
    std::string sql =
            "select " + child_get_columns + parent_get_columns + "from (select * from `" + schema_name + "`." +
            child_table + ") as child,\n" +
            "  (select * from `" + schema_name + "`." + parent_table +
            ") as parent\n" +
            "  where child." + child_columns.front() + "=parent." + parent_columns.front() + "\n";
    for (int i = 1; i < child_columns.size(); ++i) {
        std::string and_sql = "and child." + child_columns[i] + "=parent." + parent_columns[i] + "\n";
        sql += and_sql;
    }
    sql.pop_back();
    sql += ';';
    start = std::chrono::steady_clock::now();
    auto sqlResult = session.sql(sql).execute();
//    printf("%s\n", sql.c_str());
    auto &columns = sqlResult.getColumns();
    auto *table = new std::vector<mysqlx::Row>(sqlResult.fetchAll());
    end = std::chrono::steady_clock::now();
//    printf("%s\n%f\n", sql.c_str(), std::chrono::duration<double>(end - start).count());
    std::unordered_map<std::string, int> labels;
    int index = 0;
    size_t child_column_size = subject_columns.size();
    for (const auto &item: columns) {
        std::string column_name = index < child_column_size ? "$" : "#";
        column_name += item.getColumnName();
        labels[column_name] = index;
        ++index;
    }
    join_index = labels;
    join_table = table;
    end = std::chrono::steady_clock::now();
    cost += std::chrono::duration<double>(end - start).count();
//    printf("get child start\n");
//    auto child_column_count = tables_index[child_table].size();
//    printf("get child end\n");
//    for (int i = 0; i < child_column_count; ++i) {
//        std::string column_name = columns[i].getColumnName();
//        if (std::find(subject_columns.begin(), subject_columns.end(), column_name) != subject_columns.end()) {
//            column_name = "$" + column_name;
//            column_pos.emplace_back(i);
//            labels[column_name] = pos++;
//        }
//    }
//    for (size_t i = child_column_count; i < column_count; ++i) {
//        std::string column_name = columns[i].getColumnName();
//        if (std::find(object_columns.begin(), object_columns.end(), column_name) != object_columns.end()) {
//            column_name = "#" + column_name;
//            column_pos.emplace_back(i);
//            labels[column_name] = pos++;
//        }
//    }

//    size_t labels_size = labels.size();
//    int row_index = 0;
//    printf("start copy\n");
//    size_t row_size = sqlResult.count();
//    printf("count end\nrow_size:%zu\n", row_size);
//    start = std::chrono::steady_clock::now();
//    join_table = std::vector<std::vector<mysqlx::Value>>(row_size);
//    for (const auto &row: sqlResult) {
//        join_table[row_index] = std::vector<mysqlx::Value>(labels_size);
//        for (int i = 0; i < labels_size; ++i) {
//            join_table[row_index][i] = row[i];
//        }
//        ++row_index;
//    }

//    printf("end copy\n");
//    printf("join copy cost:%f\n", cost);
}

SelectQuery::~SelectQuery()
{
    session.close();
}

//void SelectQuery::getRows(std::string tableName)
//{
//    static double cost = 0;
//    for (auto &item: result) {
//        delete item.second;
//    }
//    result.clear();
//    auto start = std::chrono::steady_clock::now();
//    auto table = db.getTable(tableName);
//    auto sqlResult = table.select("*").execute();
//    auto &columns = sqlResult.getColumns();
//    std::vector<std::string> labels;
//    for (const auto &item: columns) {
//        labels.emplace_back(item.getColumnLabel());
//    }
//    auto it = sqlResult.begin();
//    std::vector<decltype(*it)> itVec;
//    for (; it != sqlResult.end(); ++it) {
//        itVec.push_back(*it);
//    }
//    omp_set_num_threads(3);
//#pragma omp parallel for
//    for (int j = 0; j < itVec.size(); ++j) {
//        auto row = new folly::ConcurrentHashMap<std::string, mysqlx::Value>();
//        for (int i = 0; i < labels.size(); ++i) {
//            row->insert_or_assign(labels[i], itVec[j][i]);
//        }
//        result.insert_or_assign(j, row);
//    }
//    auto end = std::chrono::steady_clock::now();
//    cost += std::chrono::duration<double>(end - start).count();
//    printf("non-join sql runTime: %f\n", cost);
//}


