//
// Created by yanjs on 2022/10/18.
//
#include "KVstore.h"

KVstore::KVstore()
{
    this->subjectIndex = 1;
    this->objectIndex = 1;
    this->predicateIndex = 1;
}

void
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
                {subjectIndex, std::vector<std::pair<size_t, size_t>>(1, std::make_pair(predicateIndex, objectIndex))});
        preid2subidobjidList.insert(
                {predicateIndex, std::vector<std::pair<size_t, size_t>>(1, std::make_pair(subjectIndex, objectIndex))});
        objid2subidpreidList.insert(
                {objectIndex, std::vector<std::pair<size_t, size_t>>(1, std::make_pair(subjectIndex, predicateIndex))});
        ++subjectIndex;
        ++predicateIndex;
        ++objectIndex;
        return;
    }
    if (!subExist && preExist && objExist) {
        auto subID = subject2id[subject];
        id2predicate.insert({predicateIndex, predicate});
        id2object.insert({objectIndex, object});
        subidpreid2objidList.insert({std::make_pair(subID, predicateIndex), std::vector<size_t>(1, objectIndex)});
        subidobjid2preidList.insert({std::make_pair(subID, objectIndex), std::vector<size_t>(1, predicateIndex)});
        preidobjid2subidList.insert(
                {std::make_pair(predicateIndex, objectIndex), std::vector<size_t>(1, subjectIndex)});
        subid2preidobjidList[subID].emplace_back(std::make_pair(predicateIndex, objectIndex));
        preid2subidobjidList.insert(
                {predicateIndex, std::vector<std::pair<size_t, size_t>>(1, std::make_pair(subjectIndex, objectIndex))});
        objid2subidpreidList.insert(
                {objectIndex, std::vector<std::pair<size_t, size_t>>(1, std::make_pair(subjectIndex, predicateIndex))});
        ++predicateIndex;
        ++objectIndex;
        return;
    }
    if (subExist && !preExist && objExist) {
        auto preID = predicate2id[predicate];
        id2subject.insert({subjectIndex, subject});
        id2object.insert({objectIndex, object});
        subidpreid2objidList.insert(
                {std::make_pair(subjectIndex, preID), std::vector<size_t>(1, objectIndex)});
        subidobjid2preidList.insert(
                {std::make_pair(subjectIndex, objectIndex), std::vector<size_t>(1, predicateIndex)});
        preidobjid2subidList.insert(
                {std::make_pair(preID, objectIndex), std::vector<size_t>(1, subjectIndex)});
        subid2preidobjidList.insert(
                {subjectIndex, std::vector<std::pair<size_t, size_t>>(1, std::make_pair(predicateIndex, objectIndex))});
        preid2subidobjidList[preID].emplace_back(std::make_pair(subjectIndex, objectIndex));
        objid2subidpreidList.insert(
                {objectIndex, std::vector<std::pair<size_t, size_t>>(1, std::make_pair(subjectIndex, predicateIndex))});
        ++subjectIndex;
        ++objectIndex;
        return;
    }
    if (subExist && preExist && !objExist) {
        auto objID = object2id[object];
        id2subject.insert({subjectIndex, subject});
        id2predicate.insert({predicateIndex, predicate});
        subidpreid2objidList.insert(
                {std::make_pair(subjectIndex, predicateIndex), std::vector<size_t>(1, objectIndex)});
        subidobjid2preidList.insert(
                {std::make_pair(subjectIndex, objID), std::vector<size_t>(1, predicateIndex)});
        preidobjid2subidList.insert(
                {std::make_pair(predicateIndex, objID), std::vector<size_t>(1, subjectIndex)});
        objid2subidpreidList[objID].emplace_back(std::make_pair(subjectIndex, predicateIndex));
        subid2preidobjidList.insert(
                {subjectIndex, std::vector<std::pair<size_t, size_t>>(1, std::make_pair(predicateIndex, objectIndex))});
        preid2subidobjidList.insert(
                {predicateIndex, std::vector<std::pair<size_t, size_t>>(1, std::make_pair(subjectIndex, objectIndex))});
        ++subjectIndex;
        ++predicateIndex;
        return;
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
                {objectIndex, std::vector<std::pair<size_t, size_t>>(1, std::make_pair(subID, preID))});
        ++objectIndex;
        return;
    }
    if (!subExist && preExist && !objExist) {
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
        subid2preidobjidList[subID].emplace_back(std::make_pair(subID, objID));
        objid2subidpreidList[objID].emplace_back(std::make_pair(subID, predicateIndex));
        preid2subidobjidList.insert(
                {predicateIndex, std::vector<std::pair<size_t, size_t>>(1, std::make_pair(subID, objID))});
        ++predicateIndex;
        return;
    }
    if (subExist && !preExist && !objExist) {
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
                {subjectIndex, std::vector<std::pair<size_t, size_t>>(1, std::make_pair(preID, objID))});
        ++subjectIndex;
        return;
    }
    auto subID = subject2id[subject];
    auto preID = predicate2id[predicate];
    auto objID = object2id[object];
    auto &subIDList = preidobjid2subidList[std::make_pair(preID, objID)];
    auto &preIDList = subidobjid2preidList[std::make_pair(subID, objID)];
    auto &objIDList = subidpreid2objidList[std::make_pair(subID, preID)];
    auto &preIDobjIDList = subid2preidobjidList[subID];
    preIDobjIDList.emplace_back(std::make_pair(preID, objID));
    auto &subIDobjIDList = preid2subidobjidList[preID];
    subIDobjIDList.emplace_back(std::make_pair(subID, objID));
    auto &subIDpreIDList = objid2subidpreidList[objID];
    subIDpreIDList.emplace_back(std::make_pair(subID, preID));
    if (subIDList.empty())
        preidobjid2subidList.insert({std::make_pair(preID, objID), std::vector<size_t>(1, subID)});
    else
        subIDList.emplace_back(subID);
    if (preIDList.empty())
        subidobjid2preidList.insert({std::make_pair(subID, objID), std::vector<size_t>(1, preID)});
    else
        preIDList.emplace_back(preID);
    if (objIDList.empty())
        subidpreid2objidList.insert({std::make_pair(subID, preID), std::vector<size_t>(1, objID)});
    else
        objIDList.emplace_back(objID);
}

void
KVstore::insert(const Triple &triple)
{
    return insert(triple.getSubject(), triple.getPredicate(), triple.getObject());
}

size_t
KVstore::getTripleBySubPreObj(std::vector<Triple> &result, const std::string &subject, const std::string &predicate,
                              const std::string &object)
{
//    result.emplace_back(Triple(subject, predicate, object));
    return true;
}

size_t
KVstore::getTripleListBySubPre(std::vector<Triple> &result, const std::string &subject, const std::string &predicate)
{
    auto subID = subject2id[subject];
    auto preID = predicate2id[predicate];
    auto objIDList = subidpreid2objidList[std::make_pair(subID, preID)];
    for (const auto &item: objIDList) {
        auto object = id2object[item];
        result.emplace_back(Triple(subject, predicate, object));
    }
    return result.size();
}

size_t
KVstore::getTripleBySubObj(std::vector<Triple> &result, const std::string &subject, const std::string &object)
{
    auto subID = subject2id[subject];
    auto objID = object2id[object];
    auto preIDList = subidobjid2preidList[std::make_pair(subID, objID)];
    for (const auto &item: preIDList) {
        auto predicate = id2predicate[item];
        result.emplace_back(Triple(subject, predicate, object));
    }
    return result.size();

}

size_t
KVstore::getTripleByPreObj(std::vector<Triple> &result, const std::string &predicate, const std::string &object)
{
    auto preID = predicate2id[predicate];
    auto objID = object2id[object];
    auto subIDList = preidobjid2subidList[std::make_pair(preID, objID)];
    for (const auto &item: subIDList) {
        auto subject = id2subject[item];
        result.emplace_back(Triple(subject, predicate, object));
    }
    return result.size();
}

size_t
KVstore::getTripleBySub(std::vector<Triple> &result, const std::string &subject)
{
    auto subID = subject2id[subject];
    auto preIDobjIDList = subid2preidobjidList[subID];
    for (const auto &item: preIDobjIDList) {
        auto predicate = id2predicate[item.first];
        auto object = id2object[item.second];
        result.emplace_back(Triple(subject, predicate, object));
    }
    return result.size();
}

size_t
KVstore::getTripleByPre(std::vector<Triple> &result, const std::string &predicate)
{
    auto preID = predicate2id[predicate];
    auto subIDobjIDList = preid2subidobjidList[preID];
    for (const auto &item: subIDobjIDList) {
        auto subject = id2subject[item.first];
        auto object = id2object[item.second];
        result.emplace_back(Triple(subject, predicate, object));
    }
    return result.size();
}


size_t
KVstore::getTripleByObj(std::vector<Triple> &result, const std::string &object)
{
    auto objId = object2id[object];
    auto subIDpreIDList = objid2subidpreidList[objId];
    for (const auto &item: subIDpreIDList) {
        auto subject = id2subject[item.first];
        auto predicate = id2predicate[item.second];
        result.emplace_back(Triple(subject, predicate, object));
    }
    return result.size();
}


size_t
KVstore::getAllTriple(std::vector<Triple> &result)
{
    return false;
}




