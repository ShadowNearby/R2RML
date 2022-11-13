//
// Created by yanjs on 2022/11/1.
//

#pragma once

#include "../util/Utility.h"

class SelectTable
{
    std::string name;
    std::string alias;

    SelectTable()
    {
        name = "";
        alias = "";
    };

    SelectTable(const std::string &name, const std::string &alias)
    {
        this->name = name;
        this->alias = alias;
    }

    SelectTable(const SelectTable &table)
    {
        this->name = table.name;
        this->alias = table.alias;
    }
};

class SelectField
{
public:
    std::string name;
    std::string alias;
    SelectTable table;
};

class SelectQuery
{

public:
//    std::vector<SelectTable> tables;
//    std::vector<SelectField> fields;
    mysqlx::Session session;
    mysqlx::Schema db;
    std::vector<std::unordered_map<std::string, mysqlx::Value>> result;

    SelectQuery();

    ~SelectQuery();

    void getRows(std::string &tableName, std::vector<std::string> &columnNames);

    void getRows(std::string &tableName, std::string &subjectColName, std::vector<std::string> &columnNames);

    void getRows(std::string &tableName);

    void getRowsBySql(std::string &sql);

    void clearResult();
};
