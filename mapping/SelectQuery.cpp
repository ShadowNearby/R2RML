//
// Created by yanjs on 2022/11/1.
//

#include "SelectQuery.h"


SelectQuery::SelectQuery() : session(mysqlx::Session("localhost", 33060, "yanjs", "yjs135790")),
                             db(session.getSchema("test"))
{

}

void
SelectQuery::getRows(std::string &tableName,
                     std::vector<std::string> &columnNames)
{
    clearResult();
    auto table = db.getTable(tableName);
    auto rows = table.select(columnNames).execute();
    for (const auto &item: rows.fetchAll()) {
        std::unordered_map<std::string, mysqlx::Value> row;
        for (auto i = 0; i < item.colCount(); ++i) {
            row[columnNames[i]] = item[i];
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
    std::string sql = "SELECT * FROM ";

    std::remove(tableName.begin(), tableName.end(), '"');

    sql += tableName;
    sql += ";";
    getRowsBySql(sql);
}

SelectQuery::~SelectQuery()
{
    session.close();
}

void SelectQuery::getRowsBySql(std::string &sql)
{
    clearResult();
    session.sql("USE test;").execute();
    auto sqlResult = session.sql(sql).execute();
    auto &columns = sqlResult.getColumns();
    for (const auto &item: sqlResult) {
        std::unordered_map<std::string, mysqlx::Value> row;
        for (int i = 0; i < item.colCount(); ++i) {
            row[columns[i].getColumnLabel()] = item[i];
        }
        result.emplace_back(row);
    }
}

void SelectQuery::clearResult()
{
    result.clear();
}
