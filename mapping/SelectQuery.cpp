//
// Created by yanjs on 2022/11/1.
//

#include "SelectQuery.h"


SelectQuery::SelectQuery() : session(mysqlx::Session("localhost", 33060, "yanjs", "yjs135790")),
                             db(session.getSchema("gtfs-sql"))
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

void
SelectQuery::getRows(std::string &tableName,
                     std::vector<std::string> &columnNames)
{
    clearResult();
    auto table = db.getTable(tableName);
    auto rows = table.select(columnNames).execute();
    for (const auto item: rows.fetchAll()) {
        auto row = new folly::ConcurrentHashMap<std::string, mysqlx::Value>();
        for (auto i = 0; i < item.colCount(); ++i) {
            row->insert_or_assign(columnNames[i], item[i]);
        }
        result.emplace_back(row);
    }
}

void
SelectQuery::getRows(std::string &tableName, std::string &subjectColName,
                     std::vector<std::string> &columnNames)
{
    clearResult();
    std::vector<std::string> par;
    par.emplace_back(subjectColName);
    par.insert(par.end(), columnNames.begin(), columnNames.end());
    getRows(tableName, par);
}

void SelectQuery::getRows(std::string &tableName)
{
    clearResult();
//    std::remove(tableName.begin(), tableName.end(), '"');
    auto table = db.getTable(tableName);
    auto sqlResult = table.select("*").execute();
    auto &columns = sqlResult.getColumns();
    for (const auto item: sqlResult) {
        auto row = new folly::ConcurrentHashMap<std::string, mysqlx::Value>();
        for (int i = 0; i < item.colCount(); ++i) {
            std::string key = columns[i].getColumnLabel();
            row->insert_or_assign(key, item[i]);
        }
        result.emplace_back(row);
    }
}

SelectQuery::~SelectQuery()
{
    session.close();
}

void SelectQuery::getRowsBySql(std::string &sql)
{
    clearResult();
//    std::cout << "yes" << std::endl;
    session.sql("USE gtfs-sql;").execute();
    auto table = db.getTable("stop_times");
    std::cout << sql << std::endl;
    auto sqlResult = session.sql(sql).execute();
    auto &columns = sqlResult.getColumns();
    std::cout << sqlResult.count() << std::endl;
    for (const auto item: sqlResult) {
        auto row = new folly::ConcurrentHashMap<std::string, mysqlx::Value>();
        for (int i = 0; i < item.colCount(); ++i) {
            std::string key = columns[i].getColumnLabel();
            row->insert_or_assign(key, item[i]);
        }
//        result.emplace_back(row);
    }
}

void SelectQuery::clearResult()
{
    result.clear();
}
