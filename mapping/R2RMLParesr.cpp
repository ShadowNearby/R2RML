//
// Created by yanjs on 2022/11/1.
//
#include "R2RMLParser.h"

bool blankNode(const std::string &node)
{
    return node.substr(0, 4) == "<_:_";
}


std::unordered_map<std::string, TriplesMap> R2RMLParser::triplesMaps = std::unordered_map<std::string, TriplesMap>();
std::unordered_map<std::string, RefObjectMap> R2RMLParser::refObjectMaps = std::unordered_map<std::string, RefObjectMap>();
std::unordered_map<std::string, ObjectMap> R2RMLParser::objectMaps = std::unordered_map<std::string, ObjectMap>();

void R2RMLParser::parse(KVstore &store)
{
    std::vector<Triple> triplesFromStore;
    store.getTriplesByPreObj(triplesFromStore, rrPrefix::type_, rrPrefix::triplesMap_);
    for (const auto &item: triplesFromStore) {
        auto tripleMapName = item.getSubject();
        triplesMaps[tripleMapName] = TriplesMap();
    }
    for (auto &triplesMap: triplesMaps) {
        ///     logicalTable
        store.getTriplesBySubPre(triplesFromStore, triplesMap.first, rrPrefix::logicalTable_);
        auto logicalTable = triplesFromStore.front().getObject();
        ///     logicalTable tableName
        store.getTriplesBySubPre(triplesFromStore, logicalTable, rrPrefix::tableName_);
        if (!triplesFromStore.empty()) {
            auto tableName = triplesFromStore.front().getObject();
            triplesMap.second.logicalTable.tableName = tableName;
        }
        ///     logicalTable sqlQuery
        store.getTriplesBySubPre(triplesFromStore, logicalTable, rrPrefix::sqlQuery_);
        if (!triplesFromStore.empty()) {
            auto sqlQuery = triplesFromStore.front().getObject();
            triplesMap.second.logicalTable.sqlQuery = sqlQuery;
        }
        ///     subjectMap
        store.getTriplesBySubPre(triplesFromStore, triplesMap.first, rrPrefix::subjectMap_);
        if (!triplesFromStore.empty()) {
            auto subjectMap = triplesFromStore.front().getObject();
            ///     subjectMap class
            store.getTriplesBySubPre(triplesFromStore, subjectMap, rrPrefix::class_);
            if (!triplesFromStore.empty()) {
                auto subjectMapClass = triplesFromStore.front().getObject();
                triplesMap.second.subjectMap.subjectClass = subjectMapClass;
            }
            ///     subjectMap template
            store.getTriplesBySubPre(triplesFromStore, subjectMap, rrPrefix::template_);
            if (!triplesFromStore.empty()) {
                auto subjectMapTemplate = triplesFromStore.front().getObject();
                triplesMap.second.subjectMap.termMap.type_ = Template_;
                triplesMap.second.subjectMap.termMap.template_ = subjectMapTemplate;
            }
            ///     subjectMap Constant_
            store.getTriplesBySubPre(triplesFromStore, subjectMap, rrPrefix::constant_);
            if (!triplesFromStore.empty()) {
                auto constant = triplesFromStore.front().getObject();
                triplesMap.second.subjectMap.termMap.type_ = Constant_;
                triplesMap.second.subjectMap.termMap.constant_ = constant;
            }
            ///     subjectMap Column_
            store.getTriplesBySubPre(triplesFromStore, subjectMap, rrPrefix::column_);
            if (!triplesFromStore.empty()) {
                auto column = triplesFromStore.front().getObject();
                triplesMap.second.subjectMap.termMap.type_ = Column_;
                triplesMap.second.subjectMap.termMap.column_ = column;
            }
            ///     subjectMap graph
            store.getTriplesBySubPre(triplesFromStore, subjectMap, rrPrefix::graph_);
            if (!triplesFromStore.empty()) {
                auto graph = triplesFromStore.front().getObject();
                triplesMap.second.subjectMap.graph = graph;
            }
            ///     subjectMap graphMap
            store.getTriplesBySubPre(triplesFromStore, subjectMap, rrPrefix::graphMap_);
            if (!triplesFromStore.empty()) {
                auto graphMap = triplesFromStore.front().getObject();
                ///     subjectMap graphMap template
                store.getTriplesBySubPre(triplesFromStore, graphMap, rrPrefix::template_);
                if (!triplesFromStore.empty()) {
                    auto &graphMapTemplate = triplesFromStore.front().getObject();
                    triplesMap.second.subjectMap.graphMap.termMap.type_ = Template_;
                    triplesMap.second.subjectMap.graphMap.termMap.template_ = graphMapTemplate;
                }
                ///     subjectMap graphMap Constant_
                store.getTriplesBySubPre(triplesFromStore, graphMap, rrPrefix::constant_);
                if (!triplesFromStore.empty()) {
                    auto constant = triplesFromStore.front().getObject();
                    triplesMap.second.subjectMap.graphMap.termMap.type_ = Constant_;
                    triplesMap.second.subjectMap.graphMap.termMap.template_ = constant;
                }
                ///     subjectMap graphMap Column_
                store.getTriplesBySubPre(triplesFromStore, graphMap, rrPrefix::column_);
                if (!triplesFromStore.empty()) {
                    auto column = triplesFromStore.front().getObject();
                    triplesMap.second.subjectMap.graphMap.termMap.type_ = Column_;
                    triplesMap.second.subjectMap.graphMap.termMap.template_ = column;
                }
            }
        }
        ///     predicateObjectMaps
        store.getTriplesBySubPre(triplesFromStore, triplesMap.first, rrPrefix::predicateObjectMap_);
        std::vector<std::string> preobjMapIndexs;
        for (auto &item: triplesFromStore)
            preobjMapIndexs.emplace_back(item.getObject());
        for (auto &preobjMapIndex: preobjMapIndexs) {
            PredicateObjectMap preobjMap;
            /// predicateObjectMap predicate
            store.getTriplesBySubPre(triplesFromStore, preobjMapIndex, rrPrefix::predicate_);
            if (!triplesFromStore.empty()) {
                auto pre = triplesFromStore.front().getObject();
                preobjMap.predicate = pre;
            } else {
                /// predicateObjectMap predicateMap
                store.getTriplesBySubPre(triplesFromStore, preobjMapIndex, rrPrefix::predicateMap_);
                auto preMap = triplesFromStore.front().getObject();
                /// predicateObjectMap predicateMap Column_
                store.getTriplesBySubPre(triplesFromStore, preMap, rrPrefix::column_);
                if (!triplesFromStore.empty()) {
                    auto column = triplesFromStore.front().getObject();
                    preobjMap.predicateMap.termMap.type_ = Column_;
                    preobjMap.predicateMap.termMap.column_ = column;
                }
                /// predicateObjectMap predicateMap template
                store.getTriplesBySubPre(triplesFromStore, preMap, rrPrefix::template_);
                if (!triplesFromStore.empty()) {
                    auto mapTemplate = triplesFromStore.front().getObject();
                    preobjMap.predicateMap.termMap.type_ = Template_;
                    preobjMap.predicateMap.termMap.template_ = mapTemplate;
                }
                /// predicateObjectMap predicateMap Constant_
                store.getTriplesBySubPre(triplesFromStore, preMap, rrPrefix::constant_);
                if (!triplesFromStore.empty()) {
                    auto constant = triplesFromStore.front().getObject();
                    preobjMap.predicateMap.termMap.type_ = Constant_;
                    preobjMap.predicateMap.termMap.constant_ = constant;
                }
            }
            /// predicateObjectMap object
            store.getTriplesBySubPre(triplesFromStore, preobjMapIndex, rrPrefix::object_);
            if (!triplesFromStore.empty()) {
                auto obj = triplesFromStore.front().getObject();
                preobjMap.object = obj;
            } else {
                /// predicateObjectMap objectMap
                store.getTriplesBySubPre(triplesFromStore, preobjMapIndex, rrPrefix::objectMap_);
                auto objMap = triplesFromStore.front().getObject();
                /// predicateObjectMap objectMap Column_
                if (blankNode(objMap)) {
                    store.getTriplesBySubPre(triplesFromStore, objMap, rrPrefix::column_);
                    if (!triplesFromStore.empty()) {
                        auto column = triplesFromStore.front().getObject();
                        preobjMap.objectMap.termMap.type_ = Column_;
                        preobjMap.objectMap.termMap.column_ = column;
                    }
                    /// predicateObjectMap objectMap template
                    store.getTriplesBySubPre(triplesFromStore, objMap, rrPrefix::template_);
                    if (!triplesFromStore.empty()) {
                        auto objMapTemplate = triplesFromStore.front().getObject();
                        preobjMap.objectMap.termMap.type_ = Template_;
                        preobjMap.objectMap.termMap.template_ = objMapTemplate;
                    }
                    /// predicateObjectMap objectMap Constant_
                    store.getTriplesBySubPre(triplesFromStore, objMap, rrPrefix::constant_);
                    if (!triplesFromStore.empty()) {
                        auto constant = triplesFromStore.front().getObject();
                        preobjMap.objectMap.termMap.type_ = Constant_;
                        preobjMap.objectMap.termMap.constant_ = constant;
                    }
                } else
                    preobjMap.objectMap.constant = objMap;

                /// predicateObjectMap objectMap RefObjectMap
                store.getTriplesBySubPreObj(triplesFromStore, objMap, rrPrefix::type_, rrPrefix::RefObjectMap_);
                if (!triplesFromStore.empty()) {
                    auto refObjectMap = triplesFromStore.front().getSubject();
                    /// predicateObjectMap objectMap RefObjectMap parentTriplesMap
                    store.getTriplesBySubPre(triplesFromStore, refObjectMap, rrPrefix::parentTriplesMap_);
                    if (!triplesFromStore.empty()) {
                        auto parentTriplesMap = triplesFromStore.front().getObject();
                        preobjMap.objectMap.refObjectMap.parentTripleMap = parentTriplesMap;
                    }
                    /// predicateObjectMap objectMap RefObjectMap joinCondition
                    store.getTriplesBySubPre(triplesFromStore, refObjectMap, rrPrefix::joinCondition_);
                    if (!triplesFromStore.empty()) {
                        auto join = triplesFromStore.front().getObject();
                        /// predicateObjectMap objectMap RefObjectMap joinCondition child
                        store.getTriplesBySubPre(triplesFromStore, join, rrPrefix::child_);
                        auto child = triplesFromStore.front().getObject();
                        preobjMap.objectMap.refObjectMap.join.child = child;
                        /// predicateObjectMap objectMap RefObjectMap joinCondition parent
                        store.getTriplesBySubPre(triplesFromStore, join, rrPrefix::parent_);
                        auto parent = triplesFromStore.front().getObject();
                        preobjMap.objectMap.refObjectMap.join.parent = parent;
                    }
                }
            }
            triplesMap.second.predicateObjectMaps.emplace_back(preobjMap);
        }
    }
    store.getTriplesByPreObj(triplesFromStore, rrPrefix::type_, rrPrefix::RefObjectMap_);
    for (auto &item: triplesFromStore) {
        auto refObjectMap = item.getSubject();
        refObjectMaps[refObjectMap] = RefObjectMap();
    }
    for (auto &refObjectMap: refObjectMaps) {
        /// predicateObjectMap objectMap RefObjectMap parentTriplesMap
        store.getTriplesBySubPre(triplesFromStore, refObjectMap.first, rrPrefix::parentTriplesMap_);
        if (!triplesFromStore.empty()) {
            auto parentTriplesMap = triplesFromStore.front().getObject();
            refObjectMap.second.parentTripleMap = parentTriplesMap;
        }
        /// predicateObjectMap objectMap RefObjectMap joinCondition
        store.getTriplesBySubPre(triplesFromStore, refObjectMap.first, rrPrefix::joinCondition_);
        if (!triplesFromStore.empty()) {
            auto join = triplesFromStore.front().getObject();
            /// predicateObjectMap objectMap RefObjectMap joinCondition child
            store.getTriplesBySubPre(triplesFromStore, join, rrPrefix::child_);
            auto child = triplesFromStore.front().getObject();
            refObjectMap.second.join.child = child;
            /// predicateObjectMap objectMap RefObjectMap joinCondition parent
            store.getTriplesBySubPre(triplesFromStore, join, rrPrefix::parent_);
            auto parent = triplesFromStore.front().getObject();
            refObjectMap.second.join.parent = parent;
        }
    }
    store.getTriplesByPreObj(triplesFromStore, rrPrefix::type_, rrPrefix::ObjectMap_);
    for (auto &item: triplesFromStore) {
        objectMaps[item.getSubject()] = ObjectMap();
    }
    for (auto &item: objectMaps) {
        auto key = item.first;
        auto objectMap = item.second;
        store.getTriplesBySubPre(triplesFromStore, key, rrPrefix::column_);
        if (!triplesFromStore.empty()) {
            auto column = triplesFromStore.front().getObject();
            objectMap.termMap.type_ = Column_;
            objectMap.termMap.column_ = column;
        }
        /// predicateObjectMap objectMap template
        store.getTriplesBySubPre(triplesFromStore, key, rrPrefix::template_);
        if (!triplesFromStore.empty()) {
            auto objMapTemplate = triplesFromStore.front().getObject();
            objectMap.termMap.type_ = Template_;
            objectMap.termMap.template_ = objMapTemplate;
        }
        /// predicateObjectMap objectMap Constant_
        store.getTriplesBySubPre(triplesFromStore, key, rrPrefix::constant_);
        if (!triplesFromStore.empty()) {
            auto constant = triplesFromStore.front().getObject();
            objectMap.termMap.type_ = Constant_;
            objectMap.termMap.constant_ = constant;
        }
    }
}

void R2RMLParser::parse(std::vector<Triple> &triples)
{
    KVstore store;
    store.insert(triples);
    parse(store);
}

