//
// Created by yanjs on 2023/3/8.
//

#include "ConKVStore.h"

bool ConKVStore::insert(const std::string &subject, const std::string &predicate, const std::string &object)
{
    size_t subID = 0, preID = 0, objID = 0;
    size_t id = 0;
    id = lock(subject);
    if (!string2id[subject]) {
        subID = ids.at(id);
        ids.at(id)++;
        string2id.insert_or_assign(subject, subID);
        id2string.insert_or_assign(subID, subject);
    } else { subID = string2id[subject]; }
    unlock(subject);
    id = lock(predicate);
    if (!string2id[predicate]) {
        preID = ids.at(id);
        ids.at(id)++;
        string2id.insert_or_assign(predicate, preID);
        id2string.insert_or_assign(preID, predicate);
    } else
        preID = string2id[predicate];
    unlock(predicate);
    id = lock(object);
    if (!string2id[object]) {
        objID = ids.at(id);
        ids.at(id)++;
        string2id.insert_or_assign(object, objID);
        id2string.insert_or_assign(objID, object);
    } else
        objID = string2id[object];
    unlock(object);
    auto subIDList = preidobjid2subidList[std::make_pair(preID, objID)];
    auto preIDList = subidobjid2preidList[std::make_pair(subID, objID)];
    auto objIDList = subidpreid2objidList[std::make_pair(subID, preID)];
    auto preIDobjIDList = subid2preidobjidList[subID];
    auto subIDobjIDList = preid2subidobjidList[preID];
    auto subIDpreIDList = objid2subidpreidList[objID];
    if (preIDobjIDList == nullptr) {
        subid2preidobjidList.insert_or_assign(subID,
                                              new folly::ConcurrentHashMap<std::pair<size_t, size_t>, char, pair_hash>());
    }
    subid2preidobjidList[subID]->insert_or_assign(std::make_pair(preID, objID), ' ');
    if (subIDobjIDList == nullptr)
        preid2subidobjidList.insert_or_assign(preID,
                                              new folly::ConcurrentHashMap<std::pair<size_t, size_t>, char, pair_hash>());
    preid2subidobjidList[preID]->insert_or_assign(std::make_pair(subID, objID), ' ');
    if (subIDpreIDList == nullptr)
        objid2subidpreidList.insert_or_assign(objID,
                                              new folly::ConcurrentHashMap<std::pair<size_t, size_t>, char, pair_hash>());
    objid2subidpreidList[objID]->insert_or_assign(std::make_pair(subID, preID), ' ');
    if (subIDList == nullptr)
        preidobjid2subidList.insert_or_assign(std::make_pair(preID, objID),
                                              new folly::ConcurrentHashMap<size_t, char>());
    preidobjid2subidList[std::make_pair(preID, objID)]->insert_or_assign(subID, ' ');
    if (preIDList == nullptr)
        subidobjid2preidList.insert_or_assign(std::make_pair(subID, objID),
                                              new folly::ConcurrentHashMap<size_t, char>());
    subidobjid2preidList[std::make_pair(subID, objID)]->insert_or_assign(preID, ' ');
    if (objIDList == nullptr)
        subidpreid2objidList.insert_or_assign(std::make_pair(subID, preID),
                                              new folly::ConcurrentHashMap<size_t, char>());
    subidpreid2objidList[std::make_pair(subID, preID)]->insert_or_assign(objID, ' ');
    count++;
    triple2id.insert_or_assign(std::make_tuple(subID, preID, objID), ' ');
    return true;
}

bool ConKVStore::insert(const Triple &triple)
{
    return insert(triple.getSubject(), triple.getPredicate(), triple.getObject());
}

size_t ConKVStore::insert(const std::vector<Triple> &triples)
{
    auto start = std::chrono::steady_clock::now();
    size_t success = 0;
//#pragma omp parallel for
    for (int i = 0; i < triples.size(); ++i)
        if (insert(triples[i]))
            ++success;
    auto end = std::chrono::steady_clock::now();
    auto runTime = std::chrono::duration<double>(end - start).count();
    printf("Index established!\n"
           "Total Triples: %llu\n"
           "RunTime: %fs\n", success, runTime);
    return success;
}

size_t ConKVStore::getTriplesBySubPreObj(folly::ConcurrentHashMap<size_t, Triple> &result, const std::string &subject,
                                         const std::string &predicate, const std::string &object)
{
    result.clear();
    auto subID = string2id[subject];
    auto preID = string2id[predicate];
    auto objID = string2id[object];
    auto tupleID = triple2id[std::make_tuple(subID, preID, objID)];
    if (tupleID != 0) {
        result.insert_or_assign(result.size(), Triple(subject, predicate, object));
        return 1;
    }
    return 0;
}

size_t ConKVStore::getTriplesBySubPre(folly::ConcurrentHashMap<size_t, Triple> &result, const std::string &subject,
                                      const std::string &predicate)
{
    result.clear();
    auto subID = string2id[subject];
    auto preID = string2id[predicate];
    auto objIDList = subidpreid2objidList[std::make_pair(subID, preID)];
    if (!objIDList)
        return 0;
    for (const auto &item: *objIDList) {
        auto object = id2string[item.first];
        result.insert_or_assign(result.size(), Triple(subject, predicate, object));
    }
    return (*objIDList).size();
}

size_t ConKVStore::getTriplesBySubObj(folly::ConcurrentHashMap<size_t, Triple> &result, const std::string &subject,
                                      const std::string &object)
{
    result.clear();
    auto subID = string2id[subject];
    auto objID = string2id[object];
    auto preIDList = subidobjid2preidList[std::make_pair(subID, objID)];
    if (!preIDList)
        return 0;
    for (const auto &item: *preIDList) {
        auto predicate = id2string[item.first];
        result.insert_or_assign(result.size(), Triple(subject, predicate, object));
    }
    return (*preIDList).size();
}

size_t ConKVStore::getTriplesByPreObj(folly::ConcurrentHashMap<size_t, Triple> &result, const std::string &predicate,
                                      const std::string &object)
{
    result.clear();
    auto preID = string2id[predicate];
    auto objID = string2id[object];
    auto subIDList = preidobjid2subidList[std::make_pair(preID, objID)];
    if (!subIDList)
        return 0;
    for (const auto &item: *subIDList) {
        auto subject = id2string[item.first];
        result.insert_or_assign(result.size(), Triple(subject, predicate, object));
    }
    return (*subIDList).size();
}

size_t ConKVStore::getTriplesBySub(folly::ConcurrentHashMap<size_t, Triple> &result, const std::string &subject)
{
    result.clear();
    auto subID = string2id[subject];
    auto preIDobjIDList = subid2preidobjidList[subID];
    if (!preIDobjIDList)
        return 0;
    for (const auto &item: *preIDobjIDList) {
        auto predicate = id2string[item.first.first];
        auto object = id2string[item.first.second];
        result.insert_or_assign(result.size(), Triple(subject, predicate, object));
    }
    return (*preIDobjIDList).size();
}

size_t ConKVStore::getTriplesByPre(folly::ConcurrentHashMap<size_t, Triple> &result, const std::string &predicate)
{
    result.clear();
    auto preID = string2id[predicate];
    auto subIDobjIDList = preid2subidobjidList[preID];
    if (!subIDobjIDList)
        return 0;
    for (const auto &item: *subIDobjIDList) {
        auto subject = id2string[item.first.first];
        auto object = id2string[item.first.second];
        result.insert_or_assign(result.size(), Triple(subject, predicate, object));
    }
    return (*subIDobjIDList).size();
}

size_t ConKVStore::getTriplesByObj(folly::ConcurrentHashMap<size_t, Triple> &result, const std::string &object)
{
    result.clear();
    auto objId = string2id[object];
    auto subIDpreIDList = objid2subidpreidList[objId];
    if (!subIDpreIDList)
        return 0;
    for (const auto &item: *subIDpreIDList) {
        auto subject = id2string[item.first.first];
        auto predicate = id2string[item.first.second];
        result.insert_or_assign(result.size(), Triple(subject, predicate, object));
    }
    return (*subIDpreIDList).size();
}

size_t ConKVStore::getAllTriples(folly::ConcurrentHashMap<size_t, Triple> &result)
{
    result.clear();
    size_t sum = 0;
    for (const auto &preIt: preid2subidobjidList) {
        auto predicate = id2string[preIt.first];
        auto subIDobjIDList = preid2subidobjidList[preIt.first];
        sum += (*subIDobjIDList).size();
        for (const auto &subIDobjId: *subIDobjIDList) {
            auto subject = id2string[subIDobjId.first.first];
            auto object = id2string[subIDobjId.first.second];
            result.insert_or_assign(result.size(), Triple(subject, predicate, object));
        }
    }
    return sum;
}

ConKVStore::ConKVStore()
{
    this->count = 0;
    for (size_t i = 0; i < 128; ++i) {
        id_locks.push_back(new std::mutex());
        ids.push_back(i * (1 << 25) + 1);
    }
}

void ConKVStore::insert_dict(const std::string &subject, const std::string &predicate, const std::string &object)
{
    size_t subID = get(subject), preID = get(predicate), objID = get(object);
    triple2id.insert_or_assign(std::make_tuple(subID, preID, objID), ' ');
}
