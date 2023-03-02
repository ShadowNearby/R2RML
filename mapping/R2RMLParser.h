//
// Created by yanjs on 2022/11/1.
//

#pragma once

#include "../util/Utility.h"
#include "../KVstore/KVstore.h"
#include "TriplesMap.h"

class R2RMLParser
{
public:

    static folly::ConcurrentHashMap<std::string, TriplesMap> triplesMaps;
    static folly::ConcurrentHashMap<std::string, RefObjectMap> refObjectMaps;
    static folly::ConcurrentHashMap<std::string, ObjectMap> objectMaps;

    R2RMLParser()
    = default;

    void parse(KVstore &store);

    void parse(std::vector<Triple> &triples);

};




