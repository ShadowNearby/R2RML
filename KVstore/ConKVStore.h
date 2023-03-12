//
// Created by yanjs on 2023/3/8.
//

#pragma once

#include "../util/Utility.h"
#include "../util/Triple.h"

class ConKVStore
{
public:
    ConKVStore();

    bool insert(const std::string &subject, const std::string &predicate, const std::string &object);

    bool insert(const Triple &triple);

    size_t insert(const std::vector<Triple> &triples);

    size_t getTriplesBySubPreObj(folly::ConcurrentHashMap<size_t, Triple> &result, const std::string &subject,
                                 const std::string &predicate,
                                 const std::string &object);

    size_t getTriplesBySubPre(folly::ConcurrentHashMap<size_t, Triple> &result, const std::string &subject,
                              const std::string &predicate);

    size_t getTriplesBySubObj(folly::ConcurrentHashMap<size_t, Triple> &result, const std::string &subject,
                              const std::string &object);

    size_t getTriplesByPreObj(folly::ConcurrentHashMap<size_t, Triple> &result, const std::string &predicate,
                              const std::string &object);

    size_t getTriplesBySub(folly::ConcurrentHashMap<size_t, Triple> &result, const std::string &subject);

    size_t getTriplesByPre(folly::ConcurrentHashMap<size_t, Triple> &result, const std::string &predicate);

    size_t getTriplesByObj(folly::ConcurrentHashMap<size_t, Triple> &result, const std::string &object);

    size_t getAllTriples(folly::ConcurrentHashMap<size_t, Triple> &result);

    std::mutex id_lock;
    size_t id;
    size_t count;
    folly::ConcurrentHashMap<size_t, std::string> id2string;
    folly::ConcurrentHashMap<std::string, size_t> string2id;
    folly::ConcurrentHashMap<std::pair<size_t, size_t>, folly::ConcurrentHashMap<size_t, char> *, pair_hash> subidpreid2objidList;
    folly::ConcurrentHashMap<std::pair<size_t, size_t>, folly::ConcurrentHashMap<size_t, char> *, pair_hash> subidobjid2preidList;
    folly::ConcurrentHashMap<std::pair<size_t, size_t>, folly::ConcurrentHashMap<size_t, char> *, pair_hash> preidobjid2subidList;
    folly::ConcurrentHashMap<size_t, folly::ConcurrentHashMap<std::pair<size_t, size_t>, char, pair_hash> *> subid2preidobjidList;
    folly::ConcurrentHashMap<size_t, folly::ConcurrentHashMap<std::pair<size_t, size_t>, char, pair_hash> *> preid2subidobjidList;
    folly::ConcurrentHashMap<size_t, folly::ConcurrentHashMap<std::pair<size_t, size_t>, char, pair_hash> *> objid2subidpreidList;
    folly::ConcurrentHashMap<std::tuple<size_t, size_t, size_t>, size_t, tuple_hash> triple2id;
};