//
// Created by yanjs on 2022/11/5.
//

#pragma once

#include "../util/Utility.h"

//const std::string
struct rrPrefix
{
//   const std::string  = "<http://www.w3.org/ns/r2rml#>";
    const std::string type_ = "<http://www.w3.org/1999/02/22-rdf-syntax-ns#type>";
    const std::string triplesMap_ = "<http://www.w3.org/ns/r2rml#TriplesMap>";
    const std::string tableName_ = "<http://www.w3.org/ns/r2rml#tableName>";
    const std::string template_ = "<http://www.w3.org/ns/r2rml#template>";
    const std::string subjectMap_ = "<http://www.w3.org/ns/r2rml#subjectMap>";
    const std::string predicateObjectMap_ = "<http://www.w3.org/ns/r2rml#predicateObjectMap>";
    const std::string object_ = "<http://www.w3.org/ns/r2rml#object>";
    const std::string predicate_ = "<http://www.w3.org/ns/r2rml#predicate>";
    const std::string logicalTable_ = "<http://www.w3.org/ns/r2rml#logicalTable>";
    const std::string column_ = "<http://www.w3.org/ns/r2rml#column_>";
    const std::string parentTriplesMap_ = "<http://www.w3.org/ns/r2rml#parentTriplesMap>";
    const std::string joinCondition_ = "<http://www.w3.org/ns/r2rml#joinCondition>";
    const std::string graph_ = "<http://www.w3.org/ns/r2rml#graph>";
    const std::string graphMap_ = "<http://www.w3.org/ns/r2rml#graphMap>";
    const std::string constant_ = "<http://www.w3.org/ns/r2rml#constant_>";
    const std::string predicateMap_ = "<http://www.w3.org/ns/r2rml#predicateMap>";
    const std::string child_ = "<http://www.w3.org/ns/r2rml#child>";
    const std::string parent_ = "<http://www.w3.org/ns/r2rml#parent>";
    const std::string datatype_ = "<http://www.w3.org/ns/r2rml#datatype>";
    const std::string TriplesMap_ = "<http://www.w3.org/ns/r2rml#TriplesMap>";
    const std::string RefObjectMap_ = "<http://www.w3.org/ns/r2rml#RefObjectMap>";
    const std::string inverseExpression_ = "<http://www.w3.org/ns/r2rml#inverseExpression>";
    const std::string sqlQuery_ = "<http://www.w3.org/ns/r2rml#sqlQuery>";
    const std::string class_ = "<http://www.w3.org/ns/r2rml#class>";
    const std::string objectMap_ = "<http://www.w3.org/ns/r2rml#objectMap>";
//    const std::string refObjectMap_ = "<http://www.w3.org/ns/r2rml#refObjectMap>";


};
enum TermMapType
{
    Null,
    Constant_,
    Column_,
    Template_
};

class TermMap
{
public:
    TermMapType type;
    std::string constant_;
    std::string column_;
    std::string template_;

    [[nodiscard]] bool empty() const
    {
        return constant_.empty() && column_.empty() && template_.empty();
    }
};

class LogicalTable
{
public:
    std::string tableName;
    std::string sqlQuery;

    [[nodiscard]] bool empty() const
    {
        return tableName.empty() && sqlQuery.empty();
    }
};

class GraphMap
{
public:
    TermMap termMap;

    [[nodiscard]] bool empty() const
    {
        return termMap.empty();
    }
};


class SubjectMap
{
public:
    std::string subjectClass;
    TermMap termMap;
    std::string graph;
    GraphMap graphMap;

    [[nodiscard]] bool empty() const
    {
        return subjectClass.empty() && termMap.empty() && graph.empty() && graphMap.empty();
    }
};

class PredicateMap
{
public:
    TermMap termMap;

    [[nodiscard]] bool empty() const
    {
        return termMap.empty();
    }
};

class Join
{
public:
    std::string child;
    std::string parent;

    [[nodiscard]] bool empty() const
    {
        return child.empty() && parent.empty();
    }
};

class RefObjectMap
{

public:
    std::string parentTripleMap;
    Join join;

    [[nodiscard]] bool empty() const
    {
        return join.empty() && parentTripleMap.empty();
    }
};

class ObjectMap
{
public:

    RefObjectMap refObjectMap;
    TermMap termMap;

    [[nodiscard]] bool empty() const
    {
        return termMap.empty() && refObjectMap.empty();
    }
};


class PredicateObjectMap
{
public:
    ObjectMap objectMap;
    PredicateMap predicateMap;
    std::string predicate;
    std::string object;

    [[nodiscard]] bool empty() const
    {
        return objectMap.empty() && predicate.empty() && object.empty() && predicate.empty();
    }

};

class TriplesMap
{
public:
    LogicalTable logicalTable;
    SubjectMap subjectMap;
    std::vector<PredicateObjectMap> predicateObjectMaps;

    [[nodiscard]] bool empty() const
    {
        return subjectMap.empty() && logicalTable.empty() && predicateObjectMaps.empty();
    }
};