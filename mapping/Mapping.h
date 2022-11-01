//
// Created by yanjs on 2022/10/29.
//

#pragma once

#include "../util/Utility.h"


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
    };

    class SubjectMap {
    private:
        Template tem;
        SelectQuery selectQuery;
        std::vector<std::string> Uris;

    public:
        void setTemplate(Template t) {
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

//    解析template
        std::vector<std::string> getColumns();
    };

    class PredicateObjectMap {
    private:
        std::vector<std::string> predicates;
        //chose one
        Template objectTemplate;
        std::string objectColumn;
    public:
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

        void setSubjects(std::vector<std::string> subs) {
            subjects = subs;
        }

        std::vector<PredicateObjectMap> getPredicateObjectMaps() {
            return predicateObjectMaps;
        }

        void setPredicateObjectMap(const std::vector<PredicateObjectMap> &pom) {
            predicateObjectMaps = pom;
        }
    };

    std::unordered_map<std::string, std::string> prefixes;
    std::unordered_map<std::string, LogicalTable> logicalTables;
};
