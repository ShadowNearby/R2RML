//
// Created by yanjs on 2022/11/1.
//

#pragma once

#include "../util/Utility.h"
#include "TriplesMap.h"

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
    std::vector<mysqlx::Row> *join_table;
    std::unordered_map<std::string, int> join_index;
    std::unordered_map<std::string, std::vector<mysqlx::Row> *> tables;
    std::unordered_map<std::string, std::unordered_map<std::string, int>> tables_index;
    std::string schema_name;

    SelectQuery(std::string user, std::string password, std::string schema_name);

    ~SelectQuery();

//    void getRows(std::string &tableName, std::vector<std::string> &columnNames);

//    void getRows(std::string &tableName, std::string &subjectColName, std::vector<std::string> &columnNames);

//    void getRows(std::string tableName);

    void getJoinRows(std::string tableName, const RefObjectMap &refObjectMap,
                     const std::vector<std::string> &subject_columns,
                     const std::vector<std::string> &object_columns);

    void getAll();

    void clearResult();
};
