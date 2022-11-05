//
// Created by yanjs on 2022/10/29.
//

#pragma once

#include <utility>

#include "../util/Utility.h"
#include "../util/Triple.h"
#include "../util/PairHash.h"


class Mapping
{
public:
    Mapping()
    {}

public:
    enum TermType
    {
        IRI,
        BLANKNODE,
        LITERAL,
        AUTO
    };

    enum Token
    {
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
        Token_semicolon,
        Token_quote,
        Token_View_start,
        Token_View_end,
        Token_text
    };

    enum Status
    {
        _Prefixes,
        _TripleMap,
        _LogicalTable,
        _SubjectMap,
        _PredicateObjectMap,
        _Predicate,
        _PredicateMap,
        _ObjectMap,
        _Object,
        _SqlQuery,
        _Null
    };


    size_t TripleMapsNum = 0;
    std::ifstream file;
    std::string f;
    std::string curTableName;
    std::string curOp;
    char *readStart;
    char *readEnd;
    char c;
    Status status = _Null;
public:
    Token readFile(char &ch);

    bool read(char &ch);


    void readPrefixes();

    void readTriplesMap();

    void readProperties();

    void judgeOperations();

    void readTableName();

    void readMap();

    void readTemplate();

    void readText(std::string &p);

    void readSqlQuery();


    void readLogicalTableView();

    void changeStatus();

    void addLogicalTableView()
    {
        if (status == _SqlQuery) {
            curLogicalTableView->setSelectQuery(*curSelectQuery);
            curLogicalTable->setLogicalTableView(*curLogicalTableView);
            curSelectQuery = nullptr;
            curLogicalTableView = nullptr;
            return;
        }
    }

    void addLogicalTable()
    {
        curTripleMap->setLogicalTable(*curLogicalTable);
        curLogicalTable = nullptr;
    }

    void addSubjectMap()
    {
        if (curTemplate) {
            curSubjectMap->setTemplate(*curTemplate);
            curTemplate = nullptr;
        }
        curTripleMap->setSubjectMap(*curSubjectMap);
        curSubjectMap = nullptr;
    }

    void addPredicateObjectMap()
    {
        if (curPredicateObjectMap->usePreShortCut()) {
            curPredicateObjectMap->setPredicate(*curPredicate);
            curPredicate = nullptr;
        } else {
            curPredicateObjectMap->setPredicateMap(*curPredicateMap);
            curPredicateMap = nullptr;
        }

        if (curPredicateObjectMap->useObjShortCut()) {
            curPredicateObjectMap->setObject(*curObject);
            curObject = nullptr;
        } else {
            curPredicateObjectMap->setObjectMap(*curObjectMap);
            curObjectMap = nullptr;
        }
    }

    void addPredicateMap()
    {
        if (curTemplate) {
            curPredicateMap->setTemplate(*curTemplate);
            curTemplate = nullptr;
        }
    }

    void addObjectMap()
    {
        if (curTemplate) {
            curObjectMap->setTemplate(*curTemplate);
            curTemplate = nullptr;
        }
    }

    void addTripleMap()
    {
        if (curTripleMap) {
            allTripleMaps.emplace_back(*curTripleMap);
            curTripleMap = nullptr;
        }
    }


public:
//    std::string sql;


//    subjectmap to sql(query)
    class SelectQuery
    {
        std::string query;
    public:
        SelectQuery()
        {}

        void addQuery(std::string &q)
        { query += q; }

        std::string getQuery()
        { return query; }
    };


    class Template
    {
    private:
        std::string text;
        std::vector<std::string> fields;
        std::string nameSpace;
        TermType termType;
    public:
        Template(std::string t = "", std::string n = "", TermType tt = AUTO)
        {
            text = t;
            nameSpace = n;
            termType = tt;
            createTemplateFields();
        };

        void createTemplateFields();

        std::vector<std::string> getFields()
        { return fields; }

        void setFields(const std::vector<std::string> &Fields)
        { this->fields = Fields; }

        TermType getTermType()
        { return termType; }

        void setTermType(TermType termType1)
        { this->termType = termType1; }

        std::string getNameSpace()
        { return nameSpace; }

        void setNameSpace(std::string &ns)
        { this->nameSpace = ns; }

        std::string getText()
        { return text; }

        void setText(std::string &t)
        { text = t; }

        void replaceText(std::vector<std::string> &replace);
    };

    class SubjectMap
    {
    private:
        Template tem;
        std::string pre, obj;

    public:
        SubjectMap()
        {};

        void setTemplate(Template &t)
        {
            tem = t;
        };

        Template getTemplate()
        {
            return tem;
        };

        void setPredicate(std::string &p)
        { pre = p; }

        std::string getPredicate()
        { return pre; }

        void setObject(std::string &o)
        { obj = o; }

        std::string getObject()
        { return obj; }

//        SelectQuery getSelectQuery() {
//            return selectQuery;
//        };
//
//        void setURIs(const std::vector<std::string> &uris) {
//            Uris = uris;
//        };
//
//        std::vector<std::string> getURIs() {
//            return Uris;
//        };

        std::vector<std::string> getColumns();
    };

    class PredicateMap
    {
    protected:
        std::string property;
        std::string text;
        std::vector<std::string> column;
        Template tmp;
    public:
        PredicateMap()
        {};

        std::string getProperty()
        { return property; }

        std::string getText()
        { return text; }

        std::vector<std::string> getColumn()
        { return column; }

        Template getTemplate()
        { return tmp; }

        void setProperty(std::string &p)
        { property = p; }

        void setText(std::string &t)
        { text = t; }

        void addColumn(std::string &val)
        { column.emplace_back(val); }

        void setTemplate(Template &t)
        { tmp = t; }
    };

    class Predicate : public PredicateMap
    {
    public:
        Predicate()
        {};
    };

    class ObjectMap
    {
    protected:
        std::string property;
        std::string text;
        std::vector<std::string> column;
        Template tmp;
    public:
        ObjectMap()
        {};

        std::string getProperty()
        { return property; }

        std::string getText()
        { return text; }

        std::vector<std::string> getColumn()
        { return column; }

        Template getTemplate()
        { return tmp; }

        void setProperty(std::string &p)
        { property = p; }

        void setText(std::string &t)
        { text = t; }

        void addColumn(std::string &val)
        { column.emplace_back(val); }

        void setTemplate(Template &t)
        { tmp = t; }
    };

    class Object : public ObjectMap
    {
    public:
        Object()
        {};
    };

    class PredicateObjectMap
    {
    private:
        PredicateMap preMap;
        Predicate pre;
        ObjectMap objMap;
        Object obj;
        bool preShortcut = false, objShortcut = false;
    public:
        PredicateObjectMap()
        {};

        PredicateMap getPredicateMap()
        { return preMap; }

        Predicate getPredicate()
        { return pre; }

        ObjectMap getObjectMap()
        { return objMap; }

        Object getObj()
        { return obj; }

        bool usePreShortCut() const
        { return preShortcut; }

        bool useObjShortCut() const
        { return objShortcut; }

        void setPredicateMap(PredicateMap &pm)
        { preMap = pm; }

        void setPredicate(Predicate &p)
        { pre = p; }

        void setObjectMap(ObjectMap &om)
        { objMap = om; }

        void setObject(Object &o)
        { obj = o; }

        void setPreShortCut()
        { preShortcut = true; }

        void setObjShortCut()
        { objShortcut = true; }
    };

    class LogicalTableView
    {
        std::string viewName;
        SelectQuery selectQuery;
    public:
        LogicalTableView()
        {};

        std::string getViewName()
        {
            return viewName;
        }

        void setViewName(std::string &query)
        {
            viewName = query;
        }

        SelectQuery getSelectQuery()
        {
            return selectQuery;
        }

        void setSelectQuery(SelectQuery sq)
        {
            selectQuery = sq;
        }
    };

    class LogicalTable
    {
        LogicalTableView logicalTableView;
        std::string NameSpace;
        Template logicalTableTmp;
    public:
        LogicalTable()
        {};

        LogicalTableView getLogicalTableView()
        {
            return logicalTableView;
        }

        void setLogicalTableView(const LogicalTableView &ltv)
        {
            logicalTableView = ltv;
        }

        Template &getTemplate()
        {
            return logicalTableTmp;
        }

        void setTemplate(Template &ltt)
        {
            logicalTableTmp = ltt;
        }

        std::string getNameSpace()
        { return NameSpace; }

        void setNameSpace(std::string &ns)
        { NameSpace = ns; }

    };

    class TripleMap
    {
        std::string name;
        LogicalTable logicalTable;
        SubjectMap subjectMap;
        std::vector<PredicateObjectMap> predicateObjectMaps;
    public:
        TripleMap()
        {};

        void setName(std::string &n)
        { name = n; }

        void setLogicalTable(LogicalTable &lt)
        { logicalTable = lt; }

        void setSubjectMap(SubjectMap &sm)
        { subjectMap = sm; }

        void addPredicateObjectMap(PredicateObjectMap &pom)
        { predicateObjectMaps.emplace_back(pom); }

        std::string getName()
        { return name; }

        LogicalTable &getLogicalTable()
        { return logicalTable; }

        SubjectMap getSubjectMap()
        { return subjectMap; }

        std::vector<PredicateObjectMap> getPredicateObjectMap()
        { return predicateObjectMaps; }
    };

//    TriplesMap *curTripleMap = nullptr;
//    SubjectMap *curSubjectMap = nullptr;
//    LogicalTable *curLogicalTable = nullptr;
//    Template *curTemplate = nullptr;
//    PredicateObjectMap *curPredicateObjectMap = nullptr;
//    Predicate *curPredicate = nullptr;
//    Object *curObject = nullptr;
//    PredicateMap *curPredicateMap = nullptr;
//    ObjectMap *curObjectMap = nullptr;
    std::shared_ptr<TripleMap> curTripleMap;
    std::shared_ptr<SubjectMap> curSubjectMap;
    std::shared_ptr<LogicalTable> curLogicalTable;
    std::shared_ptr<Template> curTemplate;
    std::shared_ptr<PredicateObjectMap> curPredicateObjectMap;
    std::shared_ptr<Predicate> curPredicate;
    std::shared_ptr<Object> curObject;
    std::shared_ptr<PredicateMap> curPredicateMap;
    std::shared_ptr<ObjectMap> curObjectMap;
    std::shared_ptr<SelectQuery> curSelectQuery;
    std::shared_ptr<LogicalTableView> curLogicalTableView;


    std::unordered_map<std::string, std::string> prefixes;
    std::vector<TripleMap> allTripleMaps;
    std::unordered_map<std::string, size_t> TripleMapsIndex;

    void masterParser();
};
