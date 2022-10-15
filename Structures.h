//
// Created by 刘洋 on 2022/10/14.
//

#pragma once

#include "util/Utility.h"
#include "util/Triple.h"

//unordered_map, using pair as index
struct pair_hash
{
    template<typename U1, typename U2>
    std::size_t operator()(const std::pair<U1, U2> &p) const
    {
        auto hash1 = std::hash<U1>{}(p.first);
        auto hash2 = std::hash<U2>{}(p.second);
        return hash1 ^ hash2;
    }
};


struct obj_type;
struct pre_type;
typedef struct sub_type
{
    std::vector<Triple *> single_triple;
    std::vector<pre_type *> sub_to_pre;
    std::unordered_map<std::string, std::vector<Triple *>> obj_sub_triple;

    sub_type()
    {
        single_triple.clear();
        sub_to_pre.clear();
    }
} SUB_TYPE;
typedef struct pre_type
{
    std::vector<Triple *> single_triple;
    std::vector<obj_type *> pre_to_obj;
    std::unordered_map<std::string, std::vector<Triple *>> sub_pre_triple;

    pre_type()
    {
        single_triple.clear();
        pre_to_obj.clear();
    }
} PRE_TYPE;
typedef struct obj_type
{
    std::vector<Triple *> single_triple;
    std::vector<sub_type *> obj_to_sub;
    std::unordered_map<std::string, std::vector<Triple *>> pre_obj_triple;
    std::unordered_map<std::pair<std::string, std::string>, Triple *, pair_hash> spo_triple;

    obj_type()
    {
        single_triple.clear();
        obj_to_sub.clear();
    }
} OBJ_TYPE;

