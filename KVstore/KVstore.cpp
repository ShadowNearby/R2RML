//
// Created by yanjs on 2022/10/18.
//
#include "KVstore.h"

KVstore::KVstore()
{
    this->id = 1;
    this->count = 0;
}

bool
KVstore::insert(const std::string &subject, const std::string &predicate, const std::string &object)
{
    size_t subID = 0, preID = 0, objID = 0;
    if (!string2id[subject]) {
        subID = id;
        string2id[subject] = id;
        id2string[id] = subject;
        id++;
    } else
        subID = string2id[subject];
    if (!string2id[predicate]) {
        preID = id;
        string2id[predicate] = id;
        id2string[id] = predicate;
        id++;
    } else
        preID = string2id[predicate];
    if (!string2id[object]) {
        objID = id;
        string2id[object] = id;
        id2string[id] = object;
        id++;
    } else
        objID = string2id[object];
    auto &subIDList = preidobjid2subidList[std::make_pair(preID, objID)];
    auto &preIDList = subidobjid2preidList[std::make_pair(subID, objID)];
    auto &objIDList = subidpreid2objidList[std::make_pair(subID, preID)];
//    if (!subIDList.empty() && !preIDList.empty() && !objIDList.empty())
//        return false;
    auto &preIDobjIDList = subid2preidobjidList[subID];
    auto &subIDobjIDList = preid2subidobjidList[preID];
    auto &subIDpreIDList = objid2subidpreidList[objID];
    if (preIDobjIDList.empty())
        subid2preidobjidList[subID] = std::vector<std::pair<size_t, size_t>>(1, std::make_pair(preID, objID));
    else
        preIDobjIDList.emplace_back(std::make_pair(preID, objID));
    if (subIDobjIDList.empty())
        preid2subidobjidList[preID] = std::vector<std::pair<size_t, size_t>>(1, std::make_pair(subID, objID));
    else
        subIDobjIDList.emplace_back(std::make_pair(subID, objID));
    if (subIDpreIDList.empty())
        objid2subidpreidList[objID] = std::vector<std::pair<size_t, size_t>>(1, std::make_pair(subID, preID));
    else
        subIDpreIDList.emplace_back(std::make_pair(subID, preID));
    if (subIDList.empty())
        preidobjid2subidList[std::make_pair(preID, objID)] = std::vector<size_t>(1, subID);
    else
        subIDList.emplace_back(subID);
    if (preIDList.empty()) {
        subidobjid2preidList[std::make_pair(subID, objID)] = std::vector<size_t>(1, preID);
    } else
        preIDList.emplace_back(preID);
    if (objIDList.empty())
        subidpreid2objidList[std::make_pair(subID, preID)] = std::vector<size_t>(1, objID);
    else
        objIDList.emplace_back(objID);
    count++;
    triple2id[std::make_tuple(subID, preID, objID)] = count;
    return true;
}

bool
KVstore::insert(const Triple &triple)
{
    return insert(triple.getSubject(), triple.getPredicate(), triple.getObject());
}

size_t
KVstore::insert(const std::vector<Triple> &triples)
{
    auto start = std::chrono::steady_clock::now();
    size_t success = 0;
#pragma omp parallel for
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

size_t
KVstore::getTriplesBySubPreObj(std::vector<Triple> &result, const std::string &subject, const std::string &predicate,
                               const std::string &object)
{
    result.clear();
    auto subID = string2id[subject];
    auto preID = string2id[predicate];
    auto objID = string2id[object];
    auto tupleID = triple2id[std::make_tuple(subID, preID, objID)];
    if (tupleID != 0) {
        result.emplace_back(Triple(subject, predicate, object));
        return 1;
    }
    return 0;
}

size_t
KVstore::getTriplesBySubPre(std::vector<Triple> &result, const std::string &subject, const std::string &predicate)
{
    result.clear();
    auto subID = string2id[subject];
    auto preID = string2id[predicate];
    auto &objIDList = subidpreid2objidList[std::make_pair(subID, preID)];
    for (const auto &item: objIDList) {
        auto object = id2string[item];
        result.emplace_back(Triple(subject, predicate, object));
    }
    return objIDList.size();
}

size_t
KVstore::getTriplesBySubObj(std::vector<Triple> &result, const std::string &subject, const std::string &object)
{
    result.clear();
    auto subID = string2id[subject];
    auto objID = string2id[object];
    auto &preIDList = subidobjid2preidList[std::make_pair(subID, objID)];
    for (const auto &item: preIDList) {
        auto predicate = id2string[item];
        result.emplace_back(Triple(subject, predicate, object));
    }
    return preIDList.size();

}

size_t
KVstore::getTriplesByPreObj(std::vector<Triple> &result, const std::string &predicate, const std::string &object)
{
    result.clear();
    auto preID = string2id[predicate];
    auto objID = string2id[object];
    auto &subIDList = preidobjid2subidList[std::make_pair(preID, objID)];
    for (const auto &item: subIDList) {
        auto subject = id2string[item];
        result.emplace_back(Triple(subject, predicate, object));
    }
    return subIDList.size();
}

size_t
KVstore::getTriplesBySub(std::vector<Triple> &result, const std::string &subject)
{
    result.clear();
    auto subID = string2id[subject];
    auto &preIDobjIDList = subid2preidobjidList[subID];
    for (const auto &item: preIDobjIDList) {
        auto predicate = id2string[item.first];
        auto object = id2string[item.second];
        result.emplace_back(Triple(subject, predicate, object));
    }
    return preIDobjIDList.size();
}

size_t
KVstore::getTriplesByPre(std::vector<Triple> &result, const std::string &predicate)
{
    result.clear();
    auto preID = string2id[predicate];
    auto &subIDobjIDList = preid2subidobjidList[preID];
    for (const auto &item: subIDobjIDList) {
        auto subject = id2string[item.first];
        auto object = id2string[item.second];
        result.emplace_back(Triple(subject, predicate, object));
    }
    return subIDobjIDList.size();
}


size_t
KVstore::getTriplesByObj(std::vector<Triple> &result, const std::string &object)
{
    result.clear();
    auto objId = string2id[object];
    auto &subIDpreIDList = objid2subidpreidList[objId];
    for (const auto &item: subIDpreIDList) {
        auto subject = id2string[item.first];
        auto predicate = id2string[item.second];
        result.emplace_back(Triple(subject, predicate, object));
    }
    return subIDpreIDList.size();
}


size_t
KVstore::getAllTriples(std::vector<Triple> &result)
{
    result.clear();
    size_t sum = 0;
    for (const auto &preIt: preid2subidobjidList) {
        auto predicate = id2string[preIt.first];
        auto &subIDobjIDList = preid2subidobjidList[preIt.first];
        sum += subIDobjIDList.size();
        for (const auto &subIDobjId: subIDobjIDList) {
            auto subject = id2string[subIDobjId.first];
            auto object = id2string[subIDobjId.second];
            result.emplace_back(Triple(subject, predicate, object));
        }
    }
    return sum;
}

size_t KVstore::query(std::vector<Triple> &result, const std::string &subject, const std::string &predicate,
                      const std::string &object)
{
    result.clear();
    if (subject == "?" && predicate == "?" && object == "?")
        return getAllTriples(result);
    if (subject != "?" && predicate == "?" && object == "?")
        return getTriplesBySub(result, subject);
    if (subject == "?" && predicate != "?" && object == "?")
        return getTriplesByPre(result, predicate);
    if (subject == "?" && predicate == "?" && object != "?")
        return getTriplesByObj(result, object);
    if (subject != "?" && predicate != "?" && object == "?")
        return getTriplesBySubPre(result, subject, predicate);
    if (subject != "?" && predicate == "?" && object != "?")
        return getTriplesBySubObj(result, subject, object);
    if (subject == "?" && predicate != "?" && object != "?")
        return getTriplesByPreObj(result, predicate, object);
    return getTriplesBySubPreObj(result, subject, predicate, object);

}

//TODO
bool KVstore::remove(const std::string &subject, const std::string &predicate, const std::string &object)
{
//    std::vector<Triple> queryResult;
//    auto queryNums = query(queryResult, subject, predicate, object);
//    if (queryNums != 0) {
//        auto subID = subject2id[subject];
//        auto preID = predicate2id[predicate];
//        auto objID = object2id[object];
//        auto &subIDList = preidobjid2subidList[std::make_pair(preID, objID)];
//        auto &preIDList = subidobjid2preidList[std::make_pair(subID, objID)];
//        auto &objIDList = subidpreid2objidList[std::make_pair(subID, preID)];
//        if (objIDList.size() == 1)
//            subidpreid2objidList.erase(std::make_pair(subID, preID));
//        else
//            objIDList.remove(objID);
//        if (preIDList.size() == 1)
//            subidobjid2preidList.erase(std::make_pair(subID, objID));
//        else
//            preIDList.remove(preID);
//        if (1 == subIDList.size())
//            preidobjid2subidList.erase(std::make_pair(preID, objID));
//        else
//            subIDList.remove(subID);
//        auto &preIDobjIDList = subid2preidobjidList[subID];
//        if (1 == preIDobjIDList.size()) {
//            subid2preidobjidList.erase(subID);
//            id2subject.erase(subID);
//            subject2id.erase(subject);
//        } else
//            preIDobjIDList.remove(std::make_pair(preID, objID));
//        auto &subIDobjIDList = preid2subidobjidList[preID];
//        if (1 == subIDobjIDList.size()) {
//            preid2subidobjidList.erase(preID);
//            id2predicate.erase(preID);
//            predicate2id.erase(predicate);
//        } else
//            subIDobjIDList.remove(std::make_pair(subID, objID));
//        auto &subIDpreIDList = objid2subidpreidList[objID];
//        if (1 == subIDpreIDList.size()) {
//            objid2subidpreidList.erase(objID);
//            id2object.erase(objID);
//            object2id.erase(object);
//        } else
//            subIDpreIDList.remove(std::make_pair(subID, preID));
//        return true;
//    }
    return false;
}

bool KVstore::remove(const Triple &triple)
{
    return remove(triple.getSubject(), triple.getPredicate(), triple.getObject());
}

size_t KVstore::remove(const std::vector<Triple> &triples)
{
    size_t success = 0;
    for (const auto &item: triples)
        if (remove(item))
            ++success;
    return success;
}

bool KVstore::update(const Triple &triple, const std::string &subject, const std::string &predicate,
                     const std::string &object)
{
    return remove(triple) && insert(subject, predicate, object);
}

//TODO
void KVstore::merge(const KVstore &store)
{
    for (const auto &item: store.subidpreid2objidList) {
        auto &finalList = this->subidpreid2objidList[item.first];
        if (finalList.empty())
            finalList = item.second;
        else {
            finalList.insert(finalList.end(), item.second.begin(), item.second.end());
//            finalList.unique();
        }
    }
    for (const auto &item: store.subidobjid2preidList) {
        auto &finalList = this->subidobjid2preidList[item.first];
        if (finalList.empty())
            finalList = item.second;
        else {
            finalList.insert(finalList.end(), item.second.begin(), item.second.end());
//            finalList.unique();
        }
    }
    for (const auto &item: store.preidobjid2subidList) {
        auto &finalList = this->preidobjid2subidList[item.first];
        if (finalList.empty())
            finalList = item.second;
        else {
            finalList.insert(finalList.end(), item.second.begin(), item.second.end());
//            finalList.unique();
        }
    }
    for (const auto &item: store.subid2preidobjidList) {
        auto &finalList = this->subid2preidobjidList[item.first];
        if (finalList.empty())
            finalList = item.second;
        else {
            finalList.insert(finalList.end(), item.second.begin(), item.second.end());
//            finalList.unique();
        }
    }
    for (const auto &item: store.preid2subidobjidList) {
        auto &finalList = this->preid2subidobjidList[item.first];
        if (finalList.empty())
            finalList = item.second;
        else {
            finalList.insert(finalList.end(), item.second.begin(), item.second.end());
//            finalList.unique();
        }
    }
    for (const auto &item: store.objid2subidpreidList) {
        auto &finalList = this->objid2subidpreidList[item.first];
        if (finalList.empty())
            finalList = item.second;
        else {
            finalList.insert(finalList.end(), item.second.begin(), item.second.end());
//            finalList.unique();
        }
    }
}




