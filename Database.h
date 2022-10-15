//
// Created by 刘洋 on 2022/10/14.
//
#pragma once
#ifndef R2RML_DATABASE_H
#define R2RML_DATABASE_H

#include "util/Utility.h"
#include "./Structures.h"
#include "util/Triple.h"

typedef std::pair<std::string,std::vector<Triple*>> STR_TRI_PAIR;
typedef std::pair<std::string,std::string> STR_STR_PAIR;

class Database {
    std::vector<std::string> all_subs;

    //hash map for s,p,o
    std::unordered_map<std::string,SUB_TYPE*> sub_map;
    std::unordered_map<std::string,PRE_TYPE*> pre_map;
    std::unordered_map<std::string,OBJ_TYPE*> obj_map;

public:
    Database();
    // load data
    bool handleTriple(Triple* triple);


private:

};

#endif //R2RML_DATABASE_H
