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

    size_t getTripleListBySubPre(std::vector<Triple> &result, const std::string &subject, const std::string &predicate);

    size_t getTripleBySubObj(std::vector<Triple> &result, const std::string &subject, const std::string &object);

    size_t getTripleByPreObj(std::vector<Triple> &result, const std::string &predicate, const std::string &object);

    size_t getTripleBySub(std::vector<Triple> &result, const std::string &subject);

    size_t getTripleByPre(std::vector<Triple> &result, const std::string &predicate);

    size_t getTripleByObj(std::vector<Triple> &result, const std::string &object);

    size_t getAllTriple(std::vector<Triple> &result);


    size_t subjectIndex;
    size_t predicateIndex;
    size_t objectIndex;
    std::unordered_map<size_t, std::string> id2subject;
    std::unordered_map<size_t, std::string> id2predicate;
    std::unordered_map<size_t, std::string> id2object;
    std::unordered_map<std::string, size_t> subject2id;
    std::unordered_map<std::string, size_t> predicate2id;
    std::unordered_map<std::string, size_t> object2id;
    std::unordered_map<std::pair<size_t, size_t>, std::vector<size_t>, pair_hash> subidpreid2objidList;
    std::unordered_map<std::pair<size_t, size_t>, std::vector<size_t>, pair_hash> subidobjid2preidList;
    std::unordered_map<std::pair<size_t, size_t>, std::vector<size_t>, pair_hash> preidobjid2subidList;
    std::unordered_map<size_t, std::vector<std::pair<size_t, size_t>>> subid2preidobjidList;
    std::unordered_map<size_t, std::vector<std::pair<size_t, size_t>>> preid2subidobjidList;
    std::unordered_map<size_t, std::vector<std::pair<size_t, size_t>>> objid2subidpreidList;

};
