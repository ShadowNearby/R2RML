//
// Created by yanjs on 2022/11/1.
//

#pragma once

#include "../util/Utility.h"
#include "../KVstore/ConKVStore.h"
#include "TriplesMap.h"

class R2RMLParser
{
public:

    static std::unordered_map<std::string, TriplesMap> triplesMaps;
    static std::unordered_map<std::string, RefObjectMap> refObjectMaps;
    static std::unordered_map<std::string, ObjectMap> objectMaps;
    inline std::string getDataType(std::string node, ConKVStore& store);
    inline std::string getTermType(std::string node, ConKVStore& store);
    R2RMLParser()
    = default;

    void parse(ConKVStore &store);

//    void parse(std::vector<Triple> &triples);

};




