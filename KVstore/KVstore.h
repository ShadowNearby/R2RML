//
// Created by yanjs on 2022/10/18.
//

#pragma once

#include "../util/Utility.h"

typedef size_t SubjectID;
typedef size_t PredicateID;
typedef size_t ObjectID;

class KVstore
{
public:
    size_t count;

    KVstore();

    void merge(const KVstore &store);

    bool insert(const std::string &subject, const std::string &predicate, const std::string &object);

    bool insert(const Triple &triple);

    size_t insert(const std::vector<Triple> &triples);

    bool remove(const std::string &subject, const std::string &predicate, const std::string &object);

    bool remove(const Triple &triple);

    size_t remove(const std::vector<Triple> &triples);

    bool
    update(const Triple &triple, const std::string &subject, const std::string &predicate, const std::string &object);

    size_t query(std::vector<Triple> &result, const std::string &subject, const std::string &predicate,
                 const std::string &object);


    size_t getTripleBySubPreObj(std::vector<Triple> &result, const std::string &subject, const std::string &predicate,
                                const std::string &object);

    size_t getTripleBySubPre(std::vector<Triple> &result, const std::string &subject, const std::string &predicate);

    size_t getTripleBySubObj(std::vector<Triple> &result, const std::string &subject, const std::string &object);

    size_t getTripleByPreObj(std::vector<Triple> &result, const std::string &predicate, const std::string &object);

    size_t getTripleBySub(std::vector<Triple> &result, const std::string &subject);

    size_t getTripleByPre(std::vector<Triple> &result, const std::string &predicate);

    size_t getTripleByObj(std::vector<Triple> &result, const std::string &object);

    size_t getAllTriple(std::vector<Triple> &result);


    size_t id;
    std::unordered_map<size_t, std::string> id2string;
    std::unordered_map<std::string, size_t> string2id;
    std::unordered_map<std::pair<size_t, size_t>, std::vector<size_t>, pair_hash> subidpreid2objidList;
    std::unordered_map<std::pair<size_t, size_t>, std::vector<size_t>, pair_hash> subidobjid2preidList;
    std::unordered_map<std::pair<size_t, size_t>, std::vector<size_t>, pair_hash> preidobjid2subidList;
    std::unordered_map<size_t, std::vector<std::pair<size_t, size_t>>> subid2preidobjidList;
    std::unordered_map<size_t, std::vector<std::pair<size_t, size_t>>> preid2subidobjidList;
    std::unordered_map<size_t, std::vector<std::pair<size_t, size_t>>> objid2subidpreidList;
    std::unordered_map<std::tuple<size_t, size_t, size_t>, size_t, tuple_hash> triple2id;
};
