//
// Created by yanjs on 2022/11/6.
//
#pragma once

#include "R2RMLParser.h"
#include "SelectQuery.h"


class MapTemplate
{
    std::string text;
    std::vector<std::string> slices;

};

class Handle
{
public:
    R2RMLParser parser;
    std::vector<Triple> result;
    SelectQuery selectQuery;

    Handle(KVstore &store);


    void replaceTemplate(std::string &sub, std::string &pre, std::string &obj,
                         std::vector<folly::ConcurrentHashMap<std::string, mysqlx::Value> *> &queryRes,
                         std::vector<std::pair<size_t, size_t>> &subPairPos,
                         std::vector<std::pair<size_t, size_t>> &prePairPos,
                         std::vector<std::pair<size_t, size_t>> &objPairPos);

    void findBrace(folly::ConcurrentHashMap<std::string, std::vector<mysqlx::Value>> &temMap, std::string src,
                   std::vector<folly::ConcurrentHashMap<std::string, mysqlx::Value> *> &queryRes,
                   std::vector<std::pair<size_t, size_t>> &pairPos);

    void addValueType(std::string &src, mysqlx::Value &value);

    std::string toStdString(mysqlx::Value &value);
};