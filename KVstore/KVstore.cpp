//
// Created by yanjs on 2022/10/18.
//
#include "KVstore.h"

KVstore::KVstore()
{
    this->subjectIndex = 1;
    this->objectIndex = 1;
    this->predicateIndex = 1;
    this->count = 0;
}

bool
KVstore::insert(const std::string &subject, const std::string &predicate, const std::string &object)
{
    bool subExist = subject2id.insert({subject, subjectIndex}).second;
    bool preExist = predicate2id.insert({predicate, predicateIndex}).second;
    bool objExist = object2id.insert({object, objectIndex}).second;
    if (subExist && preExist && objExist) {
        id2subject.insert({subjectIndex, subject});
        id2predicate.insert({predicateIndex, predicate});
        id2object.insert({objectIndex, object});
        subidpreid2objidList.insert(
                {std::make_pair(subjectIndex, predicateIndex), std::vector<size_t>(1, objectIndex)});
        subidobjid2preidList.insert(
                {std::make_pair(subjectIndex, objectIndex), std::vector<size_t>(1, predicateIndex)});
        preidobjid2subidList.insert(
                {std::make_pair(predicateIndex, objectIndex), std::vector<size_t>(1, subjectIndex)});
        subid2preidobjidList.insert(
                {subjectIndex, std::vector<std::pair<size_t,
                        size_t >>(1, std::make_pair(predicateIndex, objectIndex))});
        preid2subidobjidList.insert(
                {predicateIndex, std::vector<std::pair<size_t,
                        size_t >>(1, std::make_pair(subjectIndex, objectIndex))});
        objid2subidpreidList.insert(
                {objectIndex, std::vector<std::pair<size_t,
                        size_t >>(1, std::make_pair(subjectIndex, predicateIndex))});
        count++;
        triple2id[std::make_tuple(subjectIndex, predicateIndex, objectIndex)] = count;
        ++subjectIndex;
        ++predicateIndex;
        ++objectIndex;
        return true;
    }
    if (!subExist && preExist && objExist) {
        auto subID = subject2id[subject];
        id2predicate.insert({predicateIndex, predicate});
        id2object.insert({objectIndex, object});
        subidpreid2objidList.insert({std::make_pair(subID, predicateIndex), std::vector<size_t>(1, objectIndex)});
        subidobjid2preidList.insert({std::make_pair(subID, objectIndex), std::vector<size_t>(1, predicateIndex)});
        preidobjid2subidList.insert(
                {std::make_pair(predicateIndex, objectIndex), std::vector<size_t>(1, subID)});
        subid2preidobjidList[subID].emplace_back(std::make_pair(predicateIndex, objectIndex));
        preid2subidobjidList.insert(
                {predicateIndex, std::vector<std::pair<size_t,
                        size_t >>(1, std::make_pair(subID, objectIndex))});
        objid2subidpreidList.insert(
                {objectIndex, std::vector<std::pair<size_t, size_t >>(1, std::make_pair(subID, predicateIndex))});
        count++;
        triple2id[std::make_tuple(subID, predicateIndex, objectIndex)] = count;
        ++predicateIndex;
        ++objectIndex;
        return true;
    }
    if (subExist && !preExist && objExist) {
        auto preID = predicate2id[predicate];
        id2subject.insert({subjectIndex, subject});
        id2object.insert({objectIndex, object});
        subidpreid2objidList.insert(
                {std::make_pair(subjectIndex, preID), std::vector<size_t>(1, objectIndex)});
        subidobjid2preidList.insert(
                {std::make_pair(subjectIndex, objectIndex), std::vector<size_t>(1, preID)});
        preidobjid2subidList.insert(
                {std::make_pair(preID, objectIndex), std::vector<size_t>(1, subjectIndex)});
        subid2preidobjidList.insert(
                {subjectIndex, std::vector<std::pair<size_t, size_t >>(1, std::make_pair(preID, objectIndex))});
        preid2subidobjidList[preID].emplace_back(std::make_pair(subjectIndex, objectIndex));
        objid2subidpreidList.insert(
                {objectIndex, std::vector<std::pair<size_t,
                        size_t >>(1, std::make_pair(subjectIndex, preID))});
        count++;
        triple2id[std::make_tuple(subjectIndex, preID, objectIndex)] = count;
        ++subjectIndex;
        ++objectIndex;
        return true;
    }
    if (subExist && preExist) {
        auto objID = object2id[object];
        id2subject.insert({subjectIndex, subject});
        id2predicate.insert({predicateIndex, predicate});
        subidpreid2objidList.insert(
                {std::make_pair(subjectIndex, predicateIndex), std::vector<size_t>(1, objID)});
        subidobjid2preidList.insert(
                {std::make_pair(subjectIndex, objID), std::vector<size_t>(1, predicateIndex)});
        preidobjid2subidList.insert(
                {std::make_pair(predicateIndex, objID), std::vector<size_t>(1, subjectIndex)});
        objid2subidpreidList[objID].emplace_back(std::make_pair(subjectIndex, predicateIndex));
        subid2preidobjidList.insert(
                {subjectIndex, std::vector<std::pair<size_t,
                        size_t >>(1, std::make_pair(predicateIndex, objID))});
        preid2subidobjidList.insert(
                {predicateIndex, std::vector<std::pair<size_t,
                        size_t >>(1, std::make_pair(subjectIndex, objID))});
        count++;
        triple2id[std::make_tuple(subjectIndex, predicateIndex, objID)] = count;
        ++subjectIndex;
        ++predicateIndex;
        return true;
    }
    if (!subExist && !preExist && objExist) {
        auto preID = predicate2id[predicate];
        auto subID = subject2id[subject];
        id2object.insert({objectIndex, object});
        auto &objidList = subidpreid2objidList[std::make_pair(subID, preID)];
        if (objidList.empty())
            subidpreid2objidList.insert({std::make_pair(subID, preID), std::vector<size_t>(1, objectIndex)});
        else
            objidList.emplace_back(objectIndex);
        subidobjid2preidList.insert(
                {std::make_pair(subID, objectIndex), std::vector<size_t>(1, preID)});
        preidobjid2subidList.insert(
                {std::make_pair(preID, objectIndex), std::vector<size_t>(1, subID)});
        subid2preidobjidList[subID].emplace_back(std::make_pair(preID, objectIndex));
        preid2subidobjidList[preID].emplace_back(std::make_pair(subID, objectIndex));
        objid2subidpreidList.insert(
                {objectIndex, std::vector<std::pair<size_t, size_t >>(1, std::make_pair(subID, preID))});
        count++;
        triple2id[std::make_tuple(subID, preID, objectIndex)] = count;
        ++objectIndex;
        return true;
    }
    if (!subExist && preExist) {
        auto objID = object2id[object];
        auto subID = subject2id[subject];
        id2predicate.insert({predicateIndex, predicate});
        auto &preidList = subidpreid2objidList[std::make_pair(subID, objID)];
        if (preidList.empty())
            subidobjid2preidList.insert({std::make_pair(subID, objID), std::vector<size_t>(1, predicateIndex)});
        else
            preidList.emplace_back(predicateIndex);
        subidpreid2objidList.insert({std::make_pair(subID, predicateIndex), std::vector<size_t>(1, objID)});
        preidobjid2subidList.insert({std::make_pair(predicateIndex, objID), std::vector<size_t>(1, subID)});
        subid2preidobjidList[subID].emplace_back(std::make_pair(predicateIndex, objID));
        objid2subidpreidList[objID].emplace_back(std::make_pair(subID, predicateIndex));
        preid2subidobjidList.insert(
                {predicateIndex, std::vector<std::pair<size_t, size_t >>(1, std::make_pair(subID, objID))});
        count++;
        triple2id[std::make_tuple(subID, predicateIndex, objID)] = count;
        ++predicateIndex;
        return true;
    }
    if (subExist) {
        auto objID = object2id[object];
        auto preID = predicate2id[predicate];
        id2subject.insert({subjectIndex, subject});
        auto &subidList = preidobjid2subidList[std::make_pair(preID, objID)];
        if (subidList.empty())
            preidobjid2subidList.insert({std::make_pair(preID, objID), std::vector<size_t>(1, subjectIndex)});
        else
            subidList.emplace_back(subjectIndex);
        subidpreid2objidList.insert({std::make_pair(subjectIndex, preID), std::vector<size_t>(1, objID)});
        subidobjid2preidList.insert({std::make_pair(subjectIndex, objID), std::vector<size_t>(1, preID)});
        preid2subidobjidList[preID].emplace_back(std::make_pair(subjectIndex, objID));
        objid2subidpreidList[objID].emplace_back(std::make_pair(subjectIndex, preID));
        subid2preidobjidList.insert(
                {subjectIndex, std::vector<std::pair<size_t, size_t >>(1, std::make_pair(preID, objID))});
        count++;
        triple2id[std::make_tuple(subjectIndex, preID, objID)] = count;
        ++subjectIndex;
        return true;
    }
    auto subID = subject2id[subject];
    auto preID = predicate2id[predicate];
    auto objID = object2id[object];
    auto &subIDList = preidobjid2subidList[std::make_pair(preID, objID)];
    auto &preIDList = subidobjid2preidList[std::make_pair(subID, objID)];
    auto &objIDList = subidpreid2objidList[std::make_pair(subID, preID)];
    if (!subIDList.empty() && !preIDList.empty() && !objIDList.empty())
        return false;
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
    for (const auto &item: triples)
        if (insert(item))
            ++success;
    auto end = std::chrono::steady_clock::now();
    auto runTime = std::chrono::duration<double>(end - start).count();
    printf("Index established!\n"
           "Total Triples: %llu\n"
           "RunTime: %fs\n", success, runTime);
    return success;
}

size_t
KVstore::getTripleBySubPreObj(std::vector<Triple> &result, const std::string &subject, const std::string &predicate,
                              const std::string &object)
{
    auto subID = subject2id[subject];
    auto preID = predicate2id[predicate];
    auto objID = object2id[object];
    auto id = triple2id[std::make_tuple(subID, preID, objID)];
    if (id != 0) {
        result.emplace_back(Triple(subject, predicate, object));
        return 1;
    }
    return 0;
}

size_t
KVstore::getTripleListBySubPre(std::vector<Triple> &result, const std::string &subject, const std::string &predicate)
{
    auto subID = subject2id[subject];
    auto preID = predicate2id[predicate];
    auto &objIDList = subidpreid2objidList[std::make_pair(subID, preID)];
    for (const auto &item: objIDList) {
        auto object = id2object[item];
        result.emplace_back(Triple(subject, predicate, object));
    }
    return objIDList.size();
}

size_t
KVstore::getTripleBySubObj(std::vector<Triple> &result, const std::string &subject, const std::string &object)
{
    auto subID = subject2id[subject];
    auto objID = object2id[object];
    auto &preIDList = subidobjid2preidList[std::make_pair(subID, objID)];
    for (const auto &item: preIDList) {
        auto predicate = id2predicate[item];
        result.emplace_back(Triple(subject, predicate, object));
    }
    return preIDList.size();

}

size_t
KVstore::getTripleByPreObj(std::vector<Triple> &result, const std::string &predicate, const std::string &object)
{
    auto preID = predicate2id[predicate];
    auto objID = object2id[object];
    auto &subIDList = preidobjid2subidList[std::make_pair(preID, objID)];
    for (const auto &item: subIDList) {
        auto subject = id2subject[item];
        result.emplace_back(Triple(subject, predicate, object));
    }
    return subIDList.size();
}

size_t
KVstore::getTripleBySub(std::vector<Triple> &result, const std::string &subject)
{
    auto subID = subject2id[subject];
    auto &preIDobjIDList = subid2preidobjidList[subID];
    for (const auto &item: preIDobjIDList) {
        auto predicate = id2predicate[item.first];
        auto object = id2object[item.second];
        result.emplace_back(Triple(subject, predicate, object));
    }
    return preIDobjIDList.size();
}

size_t
KVstore::getTripleByPre(std::vector<Triple> &result, const std::string &predicate)
{
    auto preID = predicate2id[predicate];
    auto &subIDobjIDList = preid2subidobjidList[preID];
    for (const auto &item: subIDobjIDList) {
        auto subject = id2subject[item.first];
        auto object = id2object[item.second];
        result.emplace_back(Triple(subject, predicate, object));
    }
    return subIDobjIDList.size();
}


size_t
KVstore::getTripleByObj(std::vector<Triple> &result, const std::string &object)
{
    auto objId = object2id[object];
    auto &subIDpreIDList = objid2subidpreidList[objId];
    for (const auto &item: subIDpreIDList) {
        auto subject = id2subject[item.first];
        auto predicate = id2predicate[item.second];
        result.emplace_back(Triple(subject, predicate, object));
    }
    return subIDpreIDList.size();
}


size_t
KVstore::getAllTriple(std::vector<Triple> &result)
{
    size_t sum = 0;
    for (const auto &preIt: id2predicate) {
        auto predicate = preIt.second;
        auto &subIDobjIDList = preid2subidobjidList[preIt.first];
        sum += subIDobjIDList.size();
        for (const auto &subIDobjId: subIDobjIDList) {
            auto subject = id2subject[subIDobjId.first];
            auto object = id2object[subIDobjId.second];
            result.emplace_back(Triple(subject, predicate, object));
        }
    }
    return sum;
}

size_t KVstore::query(std::vector<Triple> &result, const std::string &subject, const std::string &predicate,
                      const std::string &object)
{
    if (subject == "?" && predicate == "?" && object == "?")
        return getAllTriple(result);
    if (subject != "?" && predicate == "?" && object == "?")
        return getTripleBySub(result, subject);
    if (subject == "?" && predicate != "?" && object == "?")
        return getTripleByPre(result, predicate);
    if (subject == "?" && predicate == "?" && object != "?")
        return getTripleByObj(result, object);
    if (subject != "?" && predicate != "?" && object == "?")
        return getTripleListBySubPre(result, subject, predicate);
    if (subject != "?" && predicate == "?" && object != "?")
        return getTripleBySubObj(result, subject, object);
    if (subject == "?" && predicate != "?" && object != "?")
        return getTripleByPreObj(result, predicate, object);
    return getTripleBySubPreObj(result, subject, predicate, object);

}

bool KVstore::remove(const std::string &subject, const std::string &predicate, const std::string &object)
{
    std::vector<Triple> queryResult;
    auto queryNums = query(queryResult, subject, predicate, object);
    if (queryNums != 0) {
        auto subID = subject2id[subject];
        auto preID = predicate2id[predicate];
        auto objID = object2id[object];
        auto &subIDList = preidobjid2subidList[std::make_pair(preID, objID)];
        auto &preIDList = subidobjid2preidList[std::make_pair(subID, objID)];
        auto &objIDList = subidpreid2objidList[std::make_pair(subID, preID)];
        if (objIDList.size() == 1)
            subidpreid2objidList.erase(std::make_pair(subID, preID));
        else
            objIDList.remove(objID);
        if (preIDList.size() == 1)
            subidobjid2preidList.erase(std::make_pair(subID, objID));
        else
            preIDList.remove(preID);
        if (1 == subIDList.size())
            preidobjid2subidList.erase(std::make_pair(preID, objID));
        else
            subIDList.remove(subID);
        auto &preIDobjIDList = subid2preidobjidList[subID];
        if (1 == preIDobjIDList.size()) {
            subid2preidobjidList.erase(subID);
            id2subject.erase(subID);
            subject2id.erase(subject);
        } else
            preIDobjIDList.remove(std::make_pair(preID, objID));
        auto &subIDobjIDList = preid2subidobjidList[preID];
        if (1 == subIDobjIDList.size()) {
            preid2subidobjidList.erase(preID);
            id2predicate.erase(preID);
            predicate2id.erase(predicate);
        } else
            subIDobjIDList.remove(std::make_pair(subID, objID));
        auto &subIDpreIDList = objid2subidpreidList[objID];
        if (1 == subIDpreIDList.size()) {
            objid2subidpreidList.erase(objID);
            id2object.erase(objID);
            object2id.erase(object);
        } else
            subIDpreIDList.remove(std::make_pair(subID, preID));
        return true;
    }
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

void KVstore::merge(const KVstore &store)
{
    for (const auto &item: store.subidpreid2objidList) {
        auto &finalList = this->subidpreid2objidList[item.first];
        if (finalList.empty())
            finalList = item.second;
        else {
            finalList.insert(finalList.end(), item.second.begin(), item.second.end());
            finalList.unique();
        }
    }
    for (const auto &item: store.subidobjid2preidList) {
        auto &finalList = this->subidobjid2preidList[item.first];
        if (finalList.empty())
            finalList = item.second;
        else {
            finalList.insert(finalList.end(), item.second.begin(), item.second.end());
            finalList.unique();
        }
    }
    for (const auto &item: store.preidobjid2subidList) {
        auto &finalList = this->preidobjid2subidList[item.first];
        if (finalList.empty())
            finalList = item.second;
        else {
            finalList.insert(finalList.end(), item.second.begin(), item.second.end());
            finalList.unique();
        }
    }
    for (const auto &item: store.subid2preidobjidList) {
        auto &finalList = this->subid2preidobjidList[item.first];
        if (finalList.empty())
            finalList = item.second;
        else {
            finalList.insert(finalList.end(), item.second.begin(), item.second.end());
            finalList.unique();
        }
    }
    for (const auto &item: store.preid2subidobjidList) {
        auto &finalList = this->preid2subidobjidList[item.first];
        if (finalList.empty())
            finalList = item.second;
        else {
            finalList.insert(finalList.end(), item.second.begin(), item.second.end());
            finalList.unique();
        }
    }
    for (const auto &item: store.objid2subidpreidList) {
        auto &finalList = this->objid2subidpreidList[item.first];
        if (finalList.empty())
            finalList = item.second;
        else {
            finalList.insert(finalList.end(), item.second.begin(), item.second.end());
            finalList.unique();
        }
    }
    this->id2object.insert(store.id2object.begin(), store.id2object.end());
    this->id2predicate.insert(store.id2predicate.begin(), store.id2predicate.end());
    this->id2subject.insert(store.id2object.begin(), store.id2object.end());
    this->subject2id.insert(store.subject2id.begin(), store.subject2id.end());
    this->predicate2id.insert(store.predicate2id.begin(), store.predicate2id.end());
    this->object2id.insert(store.object2id.begin(), store.object2id.end());
}




