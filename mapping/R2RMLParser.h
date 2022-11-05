//
// Created by yanjs on 2022/11/1.
//

#pragma once

#include "../util/Utility.h"
#include "../KVstore/KVstore.h"
#include "TriplesMap.h"

class R2RMLParser
{
    rrPrefix rr;
public:
    std::unordered_map<std::string, TriplesMap> triplesMaps;
    std::unordered_map<std::string, RefObjectMap> refObjectMaps;
    std::vector<Triple> result;

    explicit R2RMLParser(std::vector<Triple> &input);

    explicit R2RMLParser(KVstore &store);

    R2RMLParser();
};


