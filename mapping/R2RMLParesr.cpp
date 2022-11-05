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

    std::vector<Triple> triplesFromStore;
    store.getTriplesByPreObj(triplesFromStore, rr.type_, rr.triplesMap_);
    for (const auto &item: triplesFromStore) {
        auto tripleMapName = item.getSubject();
        triplesMaps[tripleMapName] = TriplesMap();
    }
    for (auto &triplesMap: triplesMaps) {
        ///     logicalTable
        store.getTriplesBySubPre(triplesFromStore, triplesMap.first, rr.logicalTable_);
        auto logicalTable = triplesFromStore.at(0).getObject();
        ///     logicalTable tableName
        store.getTriplesBySubPre(triplesFromStore, logicalTable, rr.tableName_);
        if (!triplesFromStore.empty()) {
            auto tableName = triplesFromStore.at(0).getObject();
            triplesMap.second.logicalTable.tableName = tableName;
        }
        ///     logicalTable sqlQuery
        store.getTriplesBySubPre(triplesFromStore, logicalTable, rr.sqlQuery_);
        if (!triplesFromStore.empty()) {
            auto sqlQuery = triplesFromStore.at(0).getObject();
            triplesMap.second.logicalTable.sqlQuery = sqlQuery;
        }
        ///     subjectMap
        store.getTriplesBySubPre(triplesFromStore, triplesMap.first, rr.subjectMap_);
        if (!triplesFromStore.empty()) {
            auto subjectMap = triplesFromStore.at(0).getObject();
            ///     subjectMap class
            store.getTriplesBySubPre(triplesFromStore, subjectMap, rr.class_);
            if (!triplesFromStore.empty()) {
                auto subjectMapClass = triplesFromStore.at(0).getObject();
                triplesMap.second.subjectMap.subjectClass = subjectMapClass;
            }
            ///     subjectMap template
            store.getTriplesBySubPre(triplesFromStore, subjectMap, rr.template_);
            if (!triplesFromStore.empty()) {
                auto subjectMapTemplate = triplesFromStore.at(0).getObject();
                triplesMap.second.subjectMap.termMap.type = Template_;
                triplesMap.second.subjectMap.termMap.template_ = subjectMapTemplate;
            }
            ///     subjectMap Constant_
            store.getTriplesBySubPre(triplesFromStore, subjectMap, rr.constant_);
            if (!triplesFromStore.empty()) {
                auto constant = triplesFromStore.at(0).getObject();
                triplesMap.second.subjectMap.termMap.type = Constant_;
                triplesMap.second.subjectMap.termMap.constant_ = constant;
            }
            ///     subjectMap Column_
            store.getTriplesBySubPre(triplesFromStore, subjectMap, rr.column_);
            if (!triplesFromStore.empty()) {
                auto column = triplesFromStore.at(0).getObject();
                triplesMap.second.subjectMap.termMap.type = Column_;
                triplesMap.second.subjectMap.termMap.column_ = column;
            }
            ///     subjectMap graph
            store.getTriplesBySubPre(triplesFromStore, subjectMap, rr.graph_);
            if (!triplesFromStore.empty()) {
                auto graph = triplesFromStore.at(0).getObject();
                triplesMap.second.subjectMap.graph = graph;
            }
            ///     subjectMap graphMap
            store.getTriplesBySubPre(triplesFromStore, subjectMap, rr.graphMap_);
            if (!triplesFromStore.empty()) {
                auto graphMap = triplesFromStore.at(0).getObject();
                ///     subjectMap graphMap template
                store.getTriplesBySubPre(triplesFromStore, graphMap, rr.template_);
                if (!triplesFromStore.empty()) {
                    auto graphMapTemplate = triplesFromStore.at(0).getObject();
                    triplesMap.second.subjectMap.graphMap.termMap.type = Template_;
                    triplesMap.second.subjectMap.graphMap.termMap.template_ = graphMapTemplate;
                }
                ///     subjectMap graphMap Constant_
                store.getTriplesBySubPre(triplesFromStore, graphMap, rr.constant_);
                if (!triplesFromStore.empty()) {
                    auto constant = triplesFromStore.at(0).getObject();
                    triplesMap.second.subjectMap.graphMap.termMap.type = Constant_;
                    triplesMap.second.subjectMap.graphMap.termMap.template_ = constant;
                }
                ///     subjectMap graphMap Column_
                store.getTriplesBySubPre(triplesFromStore, graphMap, rr.column_);
                if (!triplesFromStore.empty()) {
                    auto column = triplesFromStore.at(0).getObject();
                    triplesMap.second.subjectMap.graphMap.termMap.type = Column_;
                    triplesMap.second.subjectMap.graphMap.termMap.template_ = column;
                }
            }
        }
        ///     predicateObjectMaps
        store.getTriplesBySubPre(triplesFromStore, triplesMap.first, rr.predicateObjectMap_);
        std::vector<std::string> preobjMapIndexs;
        for (auto &item: triplesFromStore)
            preobjMapIndexs.emplace_back(item.getObject());
        for (auto &preobjMapIndex: preobjMapIndexs) {
            PredicateObjectMap preobjMap;
            /// predicateObjectMap predicate
            store.getTriplesBySubPre(triplesFromStore, preobjMapIndex, rr.predicate_);
            if (!triplesFromStore.empty()) {
                auto pre = triplesFromStore.at(0).getObject();
                preobjMap.predicate = pre;
            } else {
                /// predicateObjectMap predicateMap
                store.getTriplesBySubPre(triplesFromStore, preobjMapIndex, rr.predicateMap_);
                auto preMap = triplesFromStore.at(0).getObject();
                /// predicateObjectMap predicateMap Column_
                store.getTriplesBySubPre(triplesFromStore, preMap, rr.column_);
                if (!triplesFromStore.empty()) {
                    auto column = triplesFromStore.at(0).getObject();
                    preobjMap.predicateMap.termMap.type = Column_;
                    preobjMap.predicateMap.termMap.column_ = column;
                }
                /// predicateObjectMap predicateMap template
                store.getTriplesBySubPre(triplesFromStore, preMap, rr.template_);
                if (!triplesFromStore.empty()) {
                    auto mapTemplate = triplesFromStore.at(0).getObject();
                    preobjMap.predicateMap.termMap.type = Template_;
                    preobjMap.predicateMap.termMap.template_ = mapTemplate;
                }
                /// predicateObjectMap predicateMap Constant_
                store.getTriplesBySubPre(triplesFromStore, preMap, rr.constant_);
                if (!triplesFromStore.empty()) {
                    auto constant = triplesFromStore.at(0).getObject();
                    preobjMap.predicateMap.termMap.type = Constant_;
                    preobjMap.predicateMap.termMap.constant_ = constant;
                }
            }
            /// predicateObjectMap object
            store.getTriplesBySubPre(triplesFromStore, preobjMapIndex, rr.object_);
            if (!triplesFromStore.empty()) {
                auto obj = triplesFromStore.at(0).getObject();
                preobjMap.predicate = obj;
            } else {
                /// predicateObjectMap objectMap
                store.getTriplesBySubPre(triplesFromStore, preobjMapIndex, rr.objectMap_);
                auto objMap = triplesFromStore.at(0).getObject();
                /// predicateObjectMap objectMap Column_
                store.getTriplesBySubPre(triplesFromStore, objMap, rr.column_);
                if (!triplesFromStore.empty()) {
                    auto column = triplesFromStore.at(0).getObject();
                    preobjMap.objectMap.termMap.type = Column_;
                    preobjMap.objectMap.termMap.column_ = column;
                }
                /// predicateObjectMap objectMap template
                store.getTriplesBySubPre(triplesFromStore, objMap, rr.template_);
                if (!triplesFromStore.empty()) {
                    auto objMapTemplate = triplesFromStore.at(0).getObject();
                    preobjMap.objectMap.termMap.type = Template_;
                    preobjMap.objectMap.termMap.template_ = objMapTemplate;
                }
                /// predicateObjectMap objectMap Constant_
                store.getTriplesBySubPre(triplesFromStore, objMap, rr.constant_);
                if (!triplesFromStore.empty()) {
                    auto constant = triplesFromStore.at(0).getObject();
                    preobjMap.objectMap.termMap.type = Constant_;
                    preobjMap.objectMap.termMap.constant_ = constant;
                }
                /// predicateObjectMap objectMap RefObjectMap
                store.getTriplesBySubPre(triplesFromStore, objMap, rr.RefObjectMap_);
                if (!triplesFromStore.empty()) {
                    auto refObjectMap = triplesFromStore.at(0).getObject();
                    /// predicateObjectMap objectMap RefObjectMap parentTriplesMap
                    store.getTriplesBySubPre(triplesFromStore, objMap, rr.parentTriplesMap_);
                    if (!triplesFromStore.empty()) {
                        auto parentTriplesMap = triplesFromStore.at(0).getObject();
                        preobjMap.objectMap.refObjectMap.parentTripleMap = parentTriplesMap;
                    }
                    /// predicateObjectMap objectMap RefObjectMap joinCondition
                    store.getTriplesBySubPre(triplesFromStore, objMap, rr.joinCondition_);
                    if (!triplesFromStore.empty()) {
                        auto join = triplesFromStore.at(0).getObject();
                        /// predicateObjectMap objectMap RefObjectMap joinCondition child
                        store.getTriplesBySubPre(triplesFromStore, join, rr.child_);
                        auto child = triplesFromStore.at(0).getObject();
                        preobjMap.objectMap.refObjectMap.join.child = child;
                        /// predicateObjectMap objectMap RefObjectMap joinCondition parent
                        store.getTriplesBySubPre(triplesFromStore, join, rr.parent_);
                        auto parent = triplesFromStore.at(0).getObject();
                        preobjMap.objectMap.refObjectMap.join.parent = parent;
                    }
                }
            }
            triplesMap.second.predicateObjectMaps.emplace_back(preobjMap);
        }
    }
    store.getTriplesByPreObj(triplesFromStore, rr.type_, rr.RefObjectMap_);
    for (const auto &item: triplesFromStore) {
        auto refObjectMap = item.getSubject();
        refObjectMaps[refObjectMap] = RefObjectMap();
    }
    for (auto &refObjectMap: refObjectMaps) {
        /// predicateObjectMap objectMap RefObjectMap parentTriplesMap
        store.getTriplesBySubPre(triplesFromStore, refObjectMap.first, rr.parentTriplesMap_);
        if (!triplesFromStore.empty()) {
            auto parentTriplesMap = triplesFromStore.at(0).getObject();
            refObjectMap.second.parentTripleMap = parentTriplesMap;
        }
        /// predicateObjectMap objectMap RefObjectMap joinCondition
        store.getTriplesBySubPre(triplesFromStore, refObjectMap.first, rr.joinCondition_);
        if (!triplesFromStore.empty()) {
            auto join = triplesFromStore.at(0).getObject();
            /// predicateObjectMap objectMap RefObjectMap joinCondition child
            store.getTriplesBySubPre(triplesFromStore, join, rr.child_);
            auto child = triplesFromStore.at(0).getObject();
            refObjectMap.second.join.child = child;
            /// predicateObjectMap objectMap RefObjectMap joinCondition parent
            store.getTriplesBySubPre(triplesFromStore, join, rr.parent_);
            auto parent = triplesFromStore.at(0).getObject();
            refObjectMap.second.join.parent = parent;
        }
    }

}
