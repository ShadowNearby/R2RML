//
// Created by yanjs on 2022/10/29.
//

#pragma once

#include "../util/Utility.h"
#include "../util/Triple.h"


class Mapping {
public:
//    std::string sql;


//    subjectmap to sql(query)


    enum TermType {
        IRI,

        BLANKNODE,

        LITERAL,

        AUTO
    };

    enum Token {
        Token_eof,
        Token_at,
        Token_space,
        Token_dot,
        Token_colon,
        Token_bracket_start,
        Token_bracket_end,
        Token_brace,
        Token_uri_start,
        Token_uri_end,
        Token_logical_table_view_start,
        Token_logical_table_view_end,
        Token_semicolon,
        Token_quote
    };

    enum Status {
        _Prefixes,
        _TripleMap,
        _LogicalTable,
        _SubjectMap,
        _PredicateObjectMap,
        _Predicate,
        _ObjectMap,
        _Null
    };


private:
    std::ifstream file;
    std::string f;
    std::string curTriplesMap;
    std::string curTableName;
    std::string curOp;
    Status status = _Null;
public:
    Token readFile(char &c);

    bool read(char &c);

private:
    char *readStart;
    char *readEnd;
    char c;

    bool readEndOfMap();

    void readPrefixes();

    void readLogicalTable();

    void readTriplesMap();

    void readProperties();

    void judgeOperations();

    void readTableName();


public:
//    std::string sql;


//    subjectmap to sql(query)
    class SelectQuery {
    };


    class Template {
    private:
        std::string text;
        std::vector<std::string> fields;
        std::string nameSpace;
        TermType termType;
    public:
        Template() {}

        Template(std::string text, TermType termType, std::string nameSpace);

        void createTemplateFields();

        std::vector<std::string> getFields() { return fields; }

        void setFields(const std::vector<std::string> &Fields) { this->fields = Fields; }

        TermType getTermType() { return termType; }

        void setTermType(TermType termType1) { this->termType = termType1; }

        std::string getNameSpace() { return nameSpace; }

        void setNameSpace(std::string ns) { this->nameSpace = ns; }

        std::string getText() { return text; }

        void setText(std::string t) { text = t; }

        void replaceText(std::vector<std::string> &replace);
    };

    class SubjectMap {
    private:
        Template tem;
        SelectQuery selectQuery;
        std::vector<std::string> Uris;

    public:
        SubjectMap() {}

        void setTemplate(Template &t) {
            tem = t;
        };

        Template getTemplate() {
            return tem;
        };

        void setSelectQuery(SelectQuery sq) {
            selectQuery = sq;
        };

        SelectQuery getSelectQuery() {
            return selectQuery;
        };

        void setURIs(const std::vector<std::string> &uris) {
            Uris = uris;
        };

        std::vector<std::string> getURIs() {
            return Uris;
        };

        std::vector<std::string> getColumns();
    };

    class PredicateObjectMap {
    private:
        std::vector<std::string> predicates;
        //chose one
        Template objectTemplate;
        std::string objectColumn;
    public:
        PredicateObjectMap(std::vector<PredicateObjectMap> vector) {}

        std::vector<std::string> getPredicates() {
            return predicates;
        }

        void setPredicates(const std::vector<std::string> &ps) {
            predicates = ps;
        }

        Template getObjectTemplate() {
            return objectTemplate;
        }

        void setObjectTemplate(Template t) {
            objectTemplate = t;
        }

        std::string getObjectColumn() {
            return objectColumn;
        }

        void setObjectColumn(std::string objc) {
            objectColumn = objc;
        }
    };

    class LogicalTableView {
        std::string uri;
        SelectQuery selectQuery;
    public:
        LogicalTableView() {}

        std::string getUri() {
            return uri;
        }

        void setUri(std::string Uri) {
            uri = Uri;
        }

        SelectQuery getSelectQuery() {
            return selectQuery;
        }

        void setSelectQuery(SelectQuery sq) {
            selectQuery = sq;
        }
    };

    class LogicalTable {
        LogicalTableView logicalTableView;
        std::string uri;
        SubjectMap subjectMap;
        std::vector<std::string> subjects;
        std::vector<PredicateObjectMap> predicateObjectMaps;
    public:
        LogicalTable() {}

        LogicalTableView getLogicalTableView() {
            return logicalTableView;
        }

        void setLogicalTableView(const LogicalTableView &ltv) {
            logicalTableView = ltv;
        }

        std::string getUri() {
            return uri;
        }

        void setUri(std::string Uri) {
            uri = Uri;
        }

        std::vector<std::string> getSubjects() {
            return subjects;
        }

        void setSubjects(std::vector<std::string> &subs) {
            subjects = subs;
        }

        void setSubjectMap(SubjectMap &sbm) {
            subjectMap = sbm;
        }

        std::vector<PredicateObjectMap> getPredicateObjectMaps() {
            return predicateObjectMaps;
        }

        void setPredicateObjectMap(const std::vector<PredicateObjectMap> &pom) {
            predicateObjectMaps = pom;
        }

        void addPRedicateObjectMap(std::vector<PredicateObjectMap> &pom) {
            predicateObjectMaps.emplace_back(pom);
        }
    };

    LogicalTable *curLogicalTable = nullptr;
    SubjectMap *curSubjectMap = nullptr;
    Template *curTemplate = nullptr;


    std::unordered_map<std::string, std::string> prefixes;
    std::unordered_map<std::string, LogicalTable> logicalTables;

    void masterParser();
};
