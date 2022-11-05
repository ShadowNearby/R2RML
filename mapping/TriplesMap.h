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
    constant_,
    column_,
    template_
};

class TermMap
{
public:
    TermMapType type;
    std::string constant;
    std::string column;
    std::string template_;
};

class LogicalTable
{
public:
    std::string tableName;
    std::string sqlQuery;
};

class GraphMap
{
public:
    TermMap termMap;
};


class SubjectMap
{
public:
    std::string subjectClass;
    TermMap termMap;
    std::string graph;
    GraphMap graphMap;
};

class PredicateMap
{
public:
    TermMap termMap;

};

class Join
{
public:
    std::string child;
    std::string parent;
};

class RefObjectMap
{

public:
    std::string parentTripleMap;
    Join join;
};

class ObjectMap
{
public:

    RefObjectMap refObjectMap;
    TermMap termMap;
};


class PredicateObjectMap
{
public:
    ObjectMap objectMap;
    PredicateMap predicateMap;
    std::string predicate;
    std::string object;

};

class TriplesMap
{
public:
    LogicalTable logicalTable;
    SubjectMap subjectMap;
    std::vector<PredicateObjectMap> predicateObjectMaps;
};