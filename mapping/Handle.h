//
// Created by yanjs on 2022/11/6.
//
#pragma once

#include "R2RMLParser.h"
#include "SelectQuery.h"
#include "../util/PairHash.h"

class MapTemplate
{
    std::string text;
    std::vector<std::string> slices;

};

namespace std
{
    template<>
    struct hash<Triple>
    {
        std::size_t operator()(const Triple &t) const
        {
//            std::string s = t.getSubject() + t.getPredicate() + t.getObject();
            return hash_val(t.getSubject(), t.getPredicate(), t.getObject());
        }
    };
}

class Handle
{
public:
    R2RMLParser parser;
    folly::ConcurrentHashMap<Triple, char> result;
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

    std::string toStdString(mysqlx::Value &value);
};