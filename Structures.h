//
// Created by 刘洋 on 2022/10/14.
//

#pragma once

#include "util/Utility.h"
#include "util/Triple.h"

//unordered_map, using pair as index
template <typename T>
inline void hash_combine(std::size_t &seed, const T &val) {
    seed ^= std::hash<T>()(val) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}
// auxiliary generic functions to create a hash value using a seed
template <typename T> inline void hash_val(std::size_t &seed, const T &val) {
    hash_combine(seed, val);
}
template <typename T, typename... Types>
inline void hash_val(std::size_t &seed, const T &val, const Types &... args) {
    hash_combine(seed, val);
    hash_val(seed, args...);
}

template <typename... Types>
inline std::size_t hash_val(const Types &... args) {
    std::size_t seed = 0;
    hash_val(seed, args...);
    return seed;
}

struct pair_hash {
    template <class T1, class T2>
    std::size_t operator()(const std::pair<T1, T2> &p) const {
        return hash_val(p.first, p.second);
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

