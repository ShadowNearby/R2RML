//
// Created by yanjs on 2022/10/29.
//

#pragma once

#include "../util/Utility.h"

class Mapping
{
public:
//    std::string sql;


//    subjectmap to sql(query)
    class SelectQuery
    {
    };

    enum TermType
    {
        IRI,

        BLANKNODE,

        LITERAL,

        AUTO
    };

    class Template
    {
        std::string text;
        std::vector<std::string> fields;
        std::string prefix;
        TermType termType;
    };

    class SubjectMap
    {
        Template tem;
        SelectQuery selectQuery;
        std::vector<std::string> Uris;

//    解析template
        void getColumns();
    };

    class PredicateObjectMap
    {
        std::string predicates;

    };

    class LogicalTableView
    {
        std::string uri;
        SelectQuery selectQuery;
    };

    class LogicalTable
    {
        LogicalTableView logicalTableView;
        std::string uri;
        SubjectMap subjectMap;
        std::vector<std::string> subjects;
        std::vector<PredicateObjectMap> predicateObjectMaps;
    };

    std::unordered_map<std::string, std::string> prefixes;
    std::unordered_map<std::string, LogicalTable> logicalTables;
};
