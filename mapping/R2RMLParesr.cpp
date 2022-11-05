//
// Created by yanjs on 2022/11/1.
//
#include "R2RMLParser.h"


R2RMLParser::R2RMLParser(std::vector<Triple> &input)
{

}

R2RMLParser::R2RMLParser()
{

}

R2RMLParser::R2RMLParser(KVstore &store)
{

    std::vector<Triple> tripleFromStore;
    store.getTripleByPreObj(tripleFromStore, rr.type_, rr.triplesMap_);
    for (const auto &item: tripleFromStore) {
        auto tripleMapName = item.getSubject();
        triplesMaps[tripleMapName] = TriplesMap();
    }
    for (auto &triplesMap: triplesMaps) {
        ///     logicalTable
        store.getTripleBySubPre(tripleFromStore, triplesMap.first, rr.logicalTable_);
        auto logicalTable = tripleFromStore.at(0).getObject();
        ///     logicalTable tableName
        store.getTripleBySubPre(tripleFromStore, logicalTable, rr.tableName_);
        if (!tripleFromStore.empty()) {
            auto tableName = tripleFromStore.at(0).getObject();
            triplesMap.second.logicalTable.tableName = tableName;
        }
        ///     logicalTable sqlQuery
        store.getTripleBySubPre(tripleFromStore, logicalTable, rr.sqlQuery_);
        if (!tripleFromStore.empty()) {
            auto sqlQuery = tripleFromStore.at(0).getObject();
            triplesMap.second.logicalTable.sqlQuery = sqlQuery;
        }
        ///     subjectMap
        store.getTripleBySubPre(tripleFromStore, triplesMap.first, rr.subjectMap_);
        if (!tripleFromStore.empty()) {
            auto subjectMap = tripleFromStore.at(0).getObject();
            ///     subjectMap class
            store.getTripleBySubPre(tripleFromStore, subjectMap, rr.class_);
            if (!tripleFromStore.empty()) {
                auto subjectMapClass = tripleFromStore.at(0).getObject();
                triplesMap.second.subjectMap.subjectClass = subjectMapClass;
            }
            ///     subjectMap template
            store.getTripleBySubPre(tripleFromStore, subjectMap, rr.template_);
            if (!tripleFromStore.empty()) {
                auto subjectMapTemplate = tripleFromStore.at(0).getObject();
                triplesMap.second.subjectMap.termMap.type = template_;
                triplesMap.second.subjectMap.termMap.template_ = subjectMapTemplate;
            }
            ///     subjectMap constant
            store.getTripleBySubPre(tripleFromStore, subjectMap, rr.constant_);
            if (!tripleFromStore.empty()) {
                auto constant = tripleFromStore.at(0).getObject();
                triplesMap.second.subjectMap.termMap.type = constant_;
                triplesMap.second.subjectMap.termMap.constant = constant;
            }
            ///     subjectMap column
            store.getTripleBySubPre(tripleFromStore, subjectMap, rr.column_);
            if (!tripleFromStore.empty()) {
                auto column = tripleFromStore.at(0).getObject();
                triplesMap.second.subjectMap.termMap.type = column_;
                triplesMap.second.subjectMap.termMap.column = column;
            }
            ///     subjectMap graph
            store.getTripleBySubPre(tripleFromStore, subjectMap, rr.graph_);
            if (!tripleFromStore.empty()) {
                auto graph = tripleFromStore.at(0).getObject();
                triplesMap.second.subjectMap.graph = graph;
            }
            ///     subjectMap graphMap
            store.getTripleBySubPre(tripleFromStore, subjectMap, rr.graphMap_);
            if (!tripleFromStore.empty()) {
                auto graphMap = tripleFromStore.at(0).getObject();
                ///     subjectMap graphMap template
                store.getTripleBySubPre(tripleFromStore, graphMap, rr.template_);
                if (!tripleFromStore.empty()) {
                    auto graphMapTemplate = tripleFromStore.at(0).getObject();
                    triplesMap.second.subjectMap.graphMap.termMap.type = template_;
                    triplesMap.second.subjectMap.graphMap.termMap.template_ = graphMapTemplate;
                }
                ///     subjectMap graphMap constant
                store.getTripleBySubPre(tripleFromStore, graphMap, rr.constant_);
                if (!tripleFromStore.empty()) {
                    auto constant = tripleFromStore.at(0).getObject();
                    triplesMap.second.subjectMap.graphMap.termMap.type = constant_;
                    triplesMap.second.subjectMap.graphMap.termMap.template_ = constant;
                }
                ///     subjectMap graphMap column
                store.getTripleBySubPre(tripleFromStore, graphMap, rr.column_);
                if (!tripleFromStore.empty()) {
                    auto column = tripleFromStore.at(0).getObject();
                    triplesMap.second.subjectMap.graphMap.termMap.type = column_;
                    triplesMap.second.subjectMap.graphMap.termMap.template_ = column;
                }
            }
        }
        ///     predicateObjectMaps
        store.getTripleBySubPre(tripleFromStore, triplesMap.first, rr.predicateObjectMap_);
        std::vector<std::string> preobjMapIndexs;
        for (auto &item: tripleFromStore)
            preobjMapIndexs.emplace_back(item.getObject());
        for (auto &preobjMapIndex: preobjMapIndexs) {
            PredicateObjectMap preobjMap;
            /// predicateObjectMap predicate
            store.getTripleBySubPre(tripleFromStore, preobjMapIndex, rr.predicate_);
            if (!tripleFromStore.empty()) {
                auto pre = tripleFromStore.at(0).getObject();
                preobjMap.predicate = pre;
            } else {
                /// predicateObjectMap predicateMap
                store.getTripleBySubPre(tripleFromStore, preobjMapIndex, rr.predicateMap_);
                auto preMap = tripleFromStore.at(0).getObject();
                /// predicateObjectMap predicateMap column
                store.getTripleBySubPre(tripleFromStore, preMap, rr.column_);
                if (!tripleFromStore.empty()) {
                    auto column = tripleFromStore.at(0).getObject();
                    preobjMap.predicateMap.termMap.type = column_;
                    preobjMap.predicateMap.termMap.column = column;
                }
                /// predicateObjectMap predicateMap template
                store.getTripleBySubPre(tripleFromStore, preMap, rr.template_);
                if (!tripleFromStore.empty()) {
                    auto mapTemplate = tripleFromStore.at(0).getObject();
                    preobjMap.predicateMap.termMap.type = template_;
                    preobjMap.predicateMap.termMap.template_ = mapTemplate;
                }
                /// predicateObjectMap predicateMap constant
                store.getTripleBySubPre(tripleFromStore, preMap, rr.constant_);
                if (!tripleFromStore.empty()) {
                    auto constant = tripleFromStore.at(0).getObject();
                    preobjMap.predicateMap.termMap.type = constant_;
                    preobjMap.predicateMap.termMap.constant = constant;
                }
            }
            /// predicateObjectMap object
            store.getTripleBySubPre(tripleFromStore, preobjMapIndex, rr.object_);
            if (!tripleFromStore.empty()) {
                auto obj = tripleFromStore.at(0).getObject();
                preobjMap.predicate = obj;
            } else {
                /// predicateObjectMap objectMap
                store.getTripleBySubPre(tripleFromStore, preobjMapIndex, rr.objectMap_);
                auto objMap = tripleFromStore.at(0).getObject();
                /// predicateObjectMap objectMap column_
                store.getTripleBySubPre(tripleFromStore, objMap, rr.column_);
                if (!tripleFromStore.empty()) {
                    auto column = tripleFromStore.at(0).getObject();
                    preobjMap.objectMap.termMap.type = column_;
                    preobjMap.objectMap.termMap.column = column;
                }
                /// predicateObjectMap objectMap template
                store.getTripleBySubPre(tripleFromStore, objMap, rr.template_);
                if (!tripleFromStore.empty()) {
                    auto objMapTemplate = tripleFromStore.at(0).getObject();
                    preobjMap.objectMap.termMap.type = template_;
                    preobjMap.objectMap.termMap.template_ = objMapTemplate;
                }
                /// predicateObjectMap objectMap constant_
                store.getTripleBySubPre(tripleFromStore, objMap, rr.constant_);
                if (!tripleFromStore.empty()) {
                    auto constant = tripleFromStore.at(0).getObject();
                    preobjMap.objectMap.termMap.type = constant_;
                    preobjMap.objectMap.termMap.constant = constant;
                }
                /// predicateObjectMap objectMap RefObjectMap
                store.getTripleBySubPre(tripleFromStore, objMap, rr.RefObjectMap_);
                if (!tripleFromStore.empty()) {
                    auto refObjectMap = tripleFromStore.at(0).getObject();
                    /// predicateObjectMap objectMap RefObjectMap parentTriplesMap
                    store.getTripleBySubPre(tripleFromStore, objMap, rr.parentTriplesMap_);
                    if (!tripleFromStore.empty()) {
                        auto parentTriplesMap = tripleFromStore.at(0).getObject();
                        preobjMap.objectMap.refObjectMap.parentTripleMap = parentTriplesMap;
                    }
                    /// predicateObjectMap objectMap RefObjectMap joinCondition
                    store.getTripleBySubPre(tripleFromStore, objMap, rr.joinCondition_);
                    if (!tripleFromStore.empty()) {
                        auto join = tripleFromStore.at(0).getObject();
                        /// predicateObjectMap objectMap RefObjectMap joinCondition child
                        store.getTripleBySubPre(tripleFromStore, join, rr.child_);
                        auto child = tripleFromStore.at(0).getObject();
                        preobjMap.objectMap.refObjectMap.join.child = child;
                        /// predicateObjectMap objectMap RefObjectMap joinCondition parent
                        store.getTripleBySubPre(tripleFromStore, join, rr.parent_);
                        auto parent = tripleFromStore.at(0).getObject();
                        preobjMap.objectMap.refObjectMap.join.parent = parent;
                    }
                }
            }
            triplesMap.second.predicateObjectMaps.emplace_back(preobjMap);
        }
    }
    store.getTripleByPreObj(tripleFromStore, rr.type_, rr.RefObjectMap_);
    for (const auto &item: tripleFromStore) {
        auto refObjectMap = item.getSubject();
        refObjectMaps[refObjectMap] = RefObjectMap();
    }
    for (auto &refObjectMap: refObjectMaps) {
        /// predicateObjectMap objectMap RefObjectMap parentTriplesMap
        store.getTripleBySubPre(tripleFromStore, refObjectMap.first, rr.parentTriplesMap_);
        if (!tripleFromStore.empty()) {
            auto parentTriplesMap = tripleFromStore.at(0).getObject();
            refObjectMap.second.parentTripleMap = parentTriplesMap;
        }
        /// predicateObjectMap objectMap RefObjectMap joinCondition
        store.getTripleBySubPre(tripleFromStore, refObjectMap.first, rr.joinCondition_);
        if (!tripleFromStore.empty()) {
            auto join = tripleFromStore.at(0).getObject();
            /// predicateObjectMap objectMap RefObjectMap joinCondition child
            store.getTripleBySubPre(tripleFromStore, join, rr.child_);
            auto child = tripleFromStore.at(0).getObject();
            refObjectMap.second.join.child = child;
            /// predicateObjectMap objectMap RefObjectMap joinCondition parent
            store.getTripleBySubPre(tripleFromStore, join, rr.parent_);
            auto parent = tripleFromStore.at(0).getObject();
            refObjectMap.second.join.parent = parent;
        }
    }

}
