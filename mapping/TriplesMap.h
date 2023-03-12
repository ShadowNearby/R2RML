//
// Created by yanjs on 2022/11/5.
//

#pragma once

#include "../util/Utility.h"

//const std::string

namespace rrPrefix
{
//  static const std::string  = "<http://www.w3.org/ns/r2rml#>";
    static const std::string type_ = "<http://www.w3.org/1999/02/22-rdf-syntax-ns#type>";
    static const std::string triplesMap_ = "<http://www.w3.org/ns/r2rml#TriplesMap>";
    static const std::string tableName_ = "<http://www.w3.org/ns/r2rml#tableName>";
    static const std::string template_ = "<http://www.w3.org/ns/r2rml#template>";
    static const std::string subjectMap_ = "<http://www.w3.org/ns/r2rml#subjectMap>";
    static const std::string predicateObjectMap_ = "<http://www.w3.org/ns/r2rml#predicateObjectMap>";
    static const std::string object_ = "<http://www.w3.org/ns/r2rml#object>";
    static const std::string predicate_ = "<http://www.w3.org/ns/r2rml#predicate>";
    static const std::string logicalTable_ = "<http://www.w3.org/ns/r2rml#logicalTable>";
    static const std::string column_ = "<http://www.w3.org/ns/r2rml#column>";
    static const std::string parentTriplesMap_ = "<http://www.w3.org/ns/r2rml#parentTriplesMap>";
    static const std::string joinCondition_ = "<http://www.w3.org/ns/r2rml#joinCondition>";
    static const std::string graph_ = "<http://www.w3.org/ns/r2rml#graph>";
    static const std::string graphMap_ = "<http://www.w3.org/ns/r2rml#graphMap>";
    static const std::string constant_ = "<http://www.w3.org/ns/r2rml#constant>";
    static const std::string predicateMap_ = "<http://www.w3.org/ns/r2rml#predicateMap>";
    static const std::string child_ = "<http://www.w3.org/ns/r2rml#child>";
    static const std::string parent_ = "<http://www.w3.org/ns/r2rml#parent>";
    static const std::string datatype_ = "<http://www.w3.org/ns/r2rml#datatype>";
    static const std::string TriplesMap_ = "<http://www.w3.org/ns/r2rml#TriplesMap>";
    static const std::string RefObjectMap_ = "<http://www.w3.org/ns/r2rml#RefObjectMap>";
    static const std::string inverseExpression_ = "<http://www.w3.org/ns/r2rml#inverseExpression>";
    static const std::string sqlQuery_ = "<http://www.w3.org/ns/r2rml#sqlQuery>";
    static const std::string class_ = "<http://www.w3.org/ns/r2rml#class>";
    static const std::string ObjectMap_ = "<http://www.w3.org/ns/r2rml#ObjectMap>";
    static const std::string objectMap_ = "<http://www.w3.org/ns/r2rml#objectMap>";
//    static const std::string parentTriplesMap_ = "<http://www.w3.org/ns/r2rml#parentTriplesMap>";
//    const std::string refObjectMap_ = "<http://www.w3.org/ns/r2rml#refObjectMap>";
}
namespace xsdPrefix
{
    static const std::string integer_ = "<http://www.w3.org/2001/XMLSchema#integer>";
    static const std::string double_ = "<http://www.w3.org/2001/XMLSchema#double>";
    static const std::string date_ = "<http://www.w3.org/2001/XMLSchema#date>";
    static const std::string dateTime_ = "<http://www.w3.org/2001/XMLSchema#dateTime>";
    static const std::string hexBinary_ = "<http://www.w3.org/2001/XMLSchema#hexBinary>";
    static const std::string boolean_ = "<http://www.w3.org/2001/XMLSchema#boolean>";
}
class TriplesMap;

enum TermMapType
{
    Null_,
    Constant_,
    Column_,
    Template_
};

class TermMap
{
public:
    TermMapType type_;
    std::string constant_;
    std::string column_;
    std::string template_;

    TermMap() : type_(Null_)
    {};

    [[nodiscard]] bool empty() const
    {
        return constant_.empty() && column_.empty() && template_.empty();
    }

    std::string &getValue();
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

    std::string &getSubject();

    std::string &getGraph();
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
    std::string parentTableName;
    std::string parentNode;
    std::vector<Join> join;

    [[nodiscard]] bool empty() const
    {
        return join.empty() && parentTableName.empty();
    }
};

class ObjectMap
{
public:

    RefObjectMap refObjectMap;
    TermMap termMap;
    std::string constant;

    [[nodiscard]] bool empty() const
    {
        return termMap.empty() && refObjectMap.empty() && constant.empty();
    }

    [[nodiscard]] bool blankNode() const
    {
        return constant.empty();
    }

};

class PredicateObjectMap
{
public:
    std::vector<ObjectMap> objectMap;
    std::vector<PredicateMap> predicateMap;
    std::string graph;

    [[nodiscard]] bool empty() const
    {
        return objectMap.empty() && predicateMap.empty();
    }

//    std::string &getPredicate();
//
//    std::string &getObject();
//
//    std::string &getGraph();

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






