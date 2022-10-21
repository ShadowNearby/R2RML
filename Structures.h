//
// Created by 刘洋 on 2022/10/14.
//

#pragma once

#include "util/Utility.h"
#include "util/Triple.h"


struct obj_type;
struct pre_type;
typedef struct sub_type
{
    std::vector<Triple *> single_triple;
    std::unordered_map<std::string, std::vector<Triple *>> obj_sub_triple;

    sub_type()
    {
        single_triple.clear();
    }
} SUB_TYPE;
typedef struct pre_type
{
    std::vector<Triple *> single_triple;
    std::unordered_map<std::string, std::vector<Triple *>> sub_pre_triple;

    pre_type()
    {
        single_triple.clear();
    }
} PRE_TYPE;
typedef struct obj_type
{
    std::vector<Triple *> single_triple;
    std::unordered_map<std::string, std::vector<Triple *>> pre_obj_triple;
    std::unordered_map<std::pair<std::string, std::string>, Triple *, pair_hash> spo_triple;

    obj_type()
    {
        single_triple.clear();
    }
} OBJ_TYPE;

