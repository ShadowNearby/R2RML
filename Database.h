//
// Created by 刘洋 on 2022/10/14.
//
#pragma once

#include "util/Utility.h"
#include "./Structures.h"

typedef std::pair<std::string, std::vector<Triple *>> STR_TRI_PAIR;
typedef std::pair<std::string, std::string> STR_STR_PAIR;
typedef std::pair<std::string, SUB_TYPE *> STR_SUB_PAIR;
typedef std::unordered_map<std::string, size_t> STR_ID_MAP;
typedef std::pair<std::string, size_t> STR_ID_PAIR;
typedef std::unordered_map<std::pair<size_t, size_t>, Triple *, pair_hash> PAIR_TRIPLE_MAP;

class Database
{
    std::vector<std::string> all_subs;

    //hash map for s,p,o
#if 1
    std::unordered_map<std::string, SUB_TYPE *> sub_map;
    std::unordered_map<std::string, PRE_TYPE *> pre_map;
    std::unordered_map<std::string, OBJ_TYPE *> obj_map;
#endif
#if 0
    //method2
    size_t sub_len = 0, pre_len = 0, obj_len = 0;
    STR_ID_MAP sub_map;
    STR_ID_MAP pre_map;
    STR_ID_MAP obj_map;
    //two->triple
    PAIR_TRIPLE_MAP sub_pre_to_triple;
    PAIR_TRIPLE_MAP pre_obj_to_triple;
    PAIR_TRIPLE_MAP obj_sub_to_triple;
#endif
public:
    Database();

    // load data
    bool handleTriple(Triple *triple);

    std::vector<Triple *> &findTriple(std::string, std::string, std::string);


private:

};
