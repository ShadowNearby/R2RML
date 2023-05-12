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
    ConKVStore result;
    SelectQuery selectQuery;

    Handle(ConKVStore &store, int thread_num, std::string database, std::string user, std::string password);


    void replaceTemplate(std::string sub, std::string pre, std::string obj, std::vector<std::vector<size_t>> *queryRes,
                         const std::unordered_map<std::string, int> &queryIndex,
                         const std::vector<std::pair<size_t, size_t>> &subPairPos,
                         const std::vector<std::pair<size_t, size_t>> &prePairPos,
                         const std::vector<std::pair<size_t, size_t>> &objPairPos, bool join, int thread_num);

    void findBrace(std::string src, std::vector<std::pair<size_t, size_t>> &pairPos,
                   std::vector<std::string> &columnNames);

};