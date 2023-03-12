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

void R2RMLParser::parse(ConKVStore &store)
{
    folly::ConcurrentHashMap<size_t, Triple> triplesFromStore;
    store.getTriplesByPreObj(triplesFromStore, rrPrefix::type_, rrPrefix::triplesMap_);
    for (const auto &item: triplesFromStore) {
        auto tripleMapName = item.second.getSubject();
        triplesMaps.insert_or_assign(tripleMapName, TriplesMap());
    }
    for (auto it = triplesMaps.begin(); it != triplesMaps.end(); ++it) {
        auto key = it->first;
        auto &t = it->second;
        ///     logicalTable
        store.getTriplesBySubPre(triplesFromStore, it->first, rrPrefix::logicalTable_);
        auto logicalTable = triplesFromStore.begin()->second.getObject();
        ///     logicalTable tableName
        store.getTriplesBySubPre(triplesFromStore, logicalTable, rrPrefix::tableName_);
        if (!triplesFromStore.empty()) {
            auto tableName = triplesFromStore.begin()->second.getObject();
            it->second.logicalTable.tableName = tableName;
        }
        ///     logicalTable sqlQuery
        store.getTriplesBySubPre(triplesFromStore, logicalTable, rrPrefix::sqlQuery_);
        if (!triplesFromStore.empty()) {
            auto sqlQuery = triplesFromStore.begin()->second.getObject();
            it->second.logicalTable.sqlQuery = sqlQuery;
        }
        ///     subjectMap
        store.getTriplesBySubPre(triplesFromStore, it->first, rrPrefix::subjectMap_);
        if (!triplesFromStore.empty()) {
            auto subjectMap = triplesFromStore.begin()->second.getObject();
            ///     subjectMap class
            store.getTriplesBySubPre(triplesFromStore, subjectMap, rrPrefix::class_);
            if (!triplesFromStore.empty()) {
                auto subjectMapClass = triplesFromStore.begin()->second.getObject();
                it->second.subjectMap.subjectClass = subjectMapClass;
            }
            ///     subjectMap template
            store.getTriplesBySubPre(triplesFromStore, subjectMap, rrPrefix::template_);
            if (!triplesFromStore.empty()) {
                auto subjectMapTemplate = triplesFromStore.begin()->second.getObject();
                it->second.subjectMap.termMap.type_ = Template_;
                it->second.subjectMap.termMap.template_ = subjectMapTemplate;
            }
            ///     subjectMap Constant_
            store.getTriplesBySubPre(triplesFromStore, subjectMap, rrPrefix::constant_);
            if (!triplesFromStore.empty()) {
                auto constant = triplesFromStore.begin()->second.getObject();
                it->second.subjectMap.termMap.type_ = Constant_;
                it->second.subjectMap.termMap.constant_ = constant;
            }
            ///     subjectMap Column_
            store.getTriplesBySubPre(triplesFromStore, subjectMap, rrPrefix::column_);
            if (!triplesFromStore.empty()) {
                auto column = triplesFromStore.begin()->second.getObject();
                it->second.subjectMap.termMap.type_ = Column_;
                it->second.subjectMap.termMap.column_ = column;
            }
            ///     subjectMap graph
            store.getTriplesBySubPre(triplesFromStore, subjectMap, rrPrefix::graph_);
            if (!triplesFromStore.empty()) {
                auto graph = triplesFromStore.begin()->second.getObject();
                it->second.subjectMap.graph = graph;
            }
            ///     subjectMap graphMap
            store.getTriplesBySubPre(triplesFromStore, subjectMap, rrPrefix::graphMap_);
            if (!triplesFromStore.empty()) {
                auto graphMap = triplesFromStore.begin()->second.getObject();
                ///     subjectMap graphMap template
                store.getTriplesBySubPre(triplesFromStore, graphMap, rrPrefix::template_);
                if (!triplesFromStore.empty()) {
                    auto &graphMapTemplate = triplesFromStore.begin()->second.getObject();
                    it->second.subjectMap.graphMap.termMap.type_ = Template_;
                    it->second.subjectMap.graphMap.termMap.template_ = graphMapTemplate;
                }
                ///     subjectMap graphMap Constant_
                store.getTriplesBySubPre(triplesFromStore, graphMap, rrPrefix::constant_);
                if (!triplesFromStore.empty()) {
                    auto constant = triplesFromStore.begin()->second.getObject();
                    it->second.subjectMap.graphMap.termMap.type_ = Constant_;
                    it->second.subjectMap.graphMap.termMap.template_ = constant;
                }
                ///     subjectMap graphMap Column_
                store.getTriplesBySubPre(triplesFromStore, graphMap, rrPrefix::column_);
                if (!triplesFromStore.empty()) {
                    auto column = triplesFromStore.begin()->second.getObject();
                    it->second.subjectMap.graphMap.termMap.type_ = Column_;
                    it->second.subjectMap.graphMap.termMap.template_ = column;
                }
            }
            triplesMaps[key] = it->second;
        }
        ///     predicateObjectMaps
        store.getTriplesBySubPre(triplesFromStore, it->first, rrPrefix::predicateObjectMap_);
        std::vector<std::string> preobjMapIndexs;
        for (auto &item: triplesFromStore)
            preobjMapIndexs.emplace_back(item.second.getObject());
        for (auto &preobjMapIndex: preobjMapIndexs) {
            PredicateObjectMap preobjMap;
            /// predicateObjectMap predicate
            store.getTriplesBySubPre(triplesFromStore, preobjMapIndex, rrPrefix::predicate_);
            if (!triplesFromStore.empty()) {
                for (const auto &item: triplesFromStore) {
                    PredicateMap preMap;
                    preMap.termMap.constant_ = item.second.getObject();
                    preobjMap.predicateMap.emplace_back(preMap);
                }
            } else {
                /// predicateObjectMap predicateMap
                store.getTriplesBySubPre(triplesFromStore, preobjMapIndex, rrPrefix::predicateMap_);
                std::vector<std::string> preMapIndexs;
                for (const auto &preMapIt: triplesFromStore) {
                    preMapIndexs.emplace_back(preMapIt.second.getObject());
                }
                for (const auto &preMapNode: preMapIndexs) {
                    PredicateMap preMap;
                    /// predicateObjectMap predicateMap Column_
                    store.getTriplesBySubPre(triplesFromStore, preMapNode, rrPrefix::column_);
                    if (!triplesFromStore.empty()) {
                        auto column = triplesFromStore.begin()->second.getObject();
                        preMap.termMap.type_ = Column_;
                        preMap.termMap.column_ = column;
                    }
                    /// predicateObjectMap predicateMap template
                    store.getTriplesBySubPre(triplesFromStore, preMapNode, rrPrefix::template_);
                    if (!triplesFromStore.empty()) {
                        auto mapTemplate = triplesFromStore.begin()->second.getObject();
                        preMap.termMap.type_ = Template_;
                        preMap.termMap.template_ = mapTemplate;
                    }
                    /// predicateObjectMap predicateMap Constant_
                    store.getTriplesBySubPre(triplesFromStore, preMapNode, rrPrefix::constant_);
                    if (!triplesFromStore.empty()) {
                        auto constant = triplesFromStore.begin()->second.getObject();
                        preMap.termMap.type_ = Constant_;
                        preMap.termMap.constant_ = constant;
                    }
                    preobjMap.predicateMap.emplace_back(preMap);
                }
            }
            /// predicateObjectMap object
            store.getTriplesBySubPre(triplesFromStore, preobjMapIndex, rrPrefix::object_);
            if (!triplesFromStore.empty()) {
                for (const auto &item: triplesFromStore) {
                    ObjectMap objMap;
                    objMap.termMap.constant_ = item.second.getObject();
                    preobjMap.objectMap.emplace_back(objMap);
                }
            } else {
                /// predicateObjectMap objectMap
                store.getTriplesBySubPre(triplesFromStore, preobjMapIndex, rrPrefix::objectMap_);
                std::vector<std::string> objMapIndexs;
                for (const auto &objMapIt: triplesFromStore) {
                    objMapIndexs.emplace_back(objMapIt.second.getObject());
                }
                for (const auto &objMapNode: objMapIndexs) {
                    ObjectMap objMap;
                    /// predicateObjectMap objectMap Column_
                    if (!blankNode(objMapNode)) {
                        int a, b, c;
                        store.getTriplesBySubPre(triplesFromStore, objMapNode, rrPrefix::column_);
                        a = triplesFromStore.size();
                        if (!triplesFromStore.empty()) {
                            auto column = triplesFromStore.begin()->second.getObject();
                            objMap.termMap.type_ = Column_;
                            objMap.termMap.column_ = column;
                        }
                        /// predicateObjectMap objectMap template
                        store.getTriplesBySubPre(triplesFromStore, objMapNode, rrPrefix::template_);
                        b = triplesFromStore.size();
                        if (!triplesFromStore.empty()) {
                            auto objMapTemplate = triplesFromStore.begin()->second.getObject();
                            objMap.termMap.type_ = Template_;
                            objMap.termMap.template_ = objMapTemplate;
                        }
                        /// predicateObjectMap objectMap Constant_
                        store.getTriplesBySubPre(triplesFromStore, objMapNode, rrPrefix::constant_);
                        c = triplesFromStore.size();
                        if (!triplesFromStore.empty()) {
                            auto constant = triplesFromStore.begin()->second.getObject();
                            objMap.termMap.type_ = Constant_;
                            objMap.termMap.constant_ = constant;
                        }
                        /// predicateObjectMap objectMap RefObjectMap
                        if (a == 0 && b == 0 && c == 0) {
                            objMap.constant = objMapNode;
                            /// predicateObjectMap objectMap RefObjectMap parentTriplesMap
                            store.getTriplesBySubPre(triplesFromStore, objMapNode, rrPrefix::parentTriplesMap_);
                            auto parentTriplesMap = triplesFromStore.begin()->second.getObject();
                            objMap.refObjectMap.parentNode = parentTriplesMap;
                            store.getTriplesBySubPre(triplesFromStore, parentTriplesMap, rrPrefix::logicalTable_);
                            auto parentLogicalTable = triplesFromStore.begin()->second.getObject();
                            ///     logicalTable tableName
                            store.getTriplesBySubPre(triplesFromStore, parentLogicalTable, rrPrefix::tableName_);
                            if (!triplesFromStore.empty()) {
                                auto tableName = triplesFromStore.begin()->second.getObject();
                                objMap.refObjectMap.parentTableName = tableName;
                            }
                            /// predicateObjectMap objectMap RefObjectMap joinCondition
                            store.getTriplesBySubPre(triplesFromStore, objMapNode, rrPrefix::joinCondition_);
                            for (auto &triple: triplesFromStore) {
                                auto join = triple.second.getObject();
                                Join j;
                                store.getTriplesBySubPre(triplesFromStore, join, rrPrefix::child_);
                                j.child = triplesFromStore.begin()->second.getObject();
                                store.getTriplesBySubPre(triplesFromStore, join, rrPrefix::parent_);
                                j.parent = triplesFromStore.begin()->second.getObject();
                                objMap.refObjectMap.join.emplace_back(j);
                            }
                        }
                    } else { objMap.constant = objMapNode; }
                    preobjMap.objectMap.emplace_back(objMap);
                }
            }
            it->second.predicateObjectMaps.emplace_back(preobjMap);

        }
    }
    for (const auto &it: R2RMLParser::triplesMaps) {
        for (const auto &item: it.second.predicateObjectMaps) {
//            printf("%d %d %d\n", it.second.predicateObjectMaps.size(), item.predicateMap.size(), item.objectMap.size());
        }
    }
    /*
    store.getTriplesByPre(triplesFromStore, rrPrefix::parentTriplesMap_);
    for (auto &item: triplesFromStore) {
        auto refObjectMap = item.second.getSubject();
        refObjectMaps.insert_or_assign(refObjectMap, RefObjectMap());
    }
    for (auto &refObjectMap: refObjectMaps) {
        auto key = refObjectMap.first;
        /// predicateObjectMap objectMap parentTriplesMap joinCondition
        store.getTriplesBySubPre(triplesFromStore, refObjectMap.first, rrPrefix::joinCondition_);
        if (!triplesFromStore.empty()) {
            auto join = triplesFromStore.begin()->second.getObject();
            /// predicateObjectMap objectMap RefObjectMap joinCondition child
            store.getTriplesBySubPre(triplesFromStore, join, rrPrefix::child_);
            auto child = triplesFromStore.begin()->second.getObject();
            refObjectMap.second.join.child = child;
            /// predicateObjectMap objectMap RefObjectMap joinCondition parent
            store.getTriplesBySubPre(triplesFromStore, join, rrPrefix::parent_);
            auto parent = triplesFromStore.begin()->second.getObject();
            refObjectMap.second.join.parent = parent;
        }
        refObjectMaps[key] = refObjectMap.second;
    }
    store.getTriplesByPreObj(triplesFromStore, rrPrefix::type_, rrPrefix::ObjectMap_);
    for (auto &item: triplesFromStore) {
        objectMaps.insert_or_assign(item.second.getSubject(), ObjectMap());
    }
    for (auto &item: objectMaps) {
        auto key = item.first;
        auto objectMap = item.second;
        store.getTriplesBySubPre(triplesFromStore, key, rrPrefix::column_);
        if (!triplesFromStore.empty()) {
            auto column = triplesFromStore.begin()->second.getObject();
            objectMap.termMap.type_ = Column_;
            objectMap.termMap.column_ = column;
        }
        /// predicateObjectMap objectMap template
        store.getTriplesBySubPre(triplesFromStore, key, rrPrefix::template_);
        if (!triplesFromStore.empty()) {
            auto objMapTemplate = triplesFromStore.begin()->second.getObject();
            objectMap.termMap.type_ = Template_;
            objectMap.termMap.template_ = objMapTemplate;
        }
        /// predicateObjectMap objectMap Constant_
        store.getTriplesBySubPre(triplesFromStore, key, rrPrefix::constant_);
        if (!triplesFromStore.empty()) {
            auto constant = triplesFromStore.begin()->second.getObject();
            objectMap.termMap.type_ = Constant_;
            objectMap.termMap.constant_ = constant;
        }
    }
     */
}

//void R2RMLParser::parse(std::vector<Triple> &triples)
//{
//    ConKVStore store;
//    store.insert(triples);
//    parse(store);
//}

