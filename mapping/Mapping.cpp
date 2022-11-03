//
// Created by yanjs on 2022/10/29.
//

#include "Mapping.h"

//Template
Mapping::Template::Template(std::string &text, TermType &termType, std::string &nameSpace) {
    this->text = text;
    this->termType = termType;
    this->nameSpace = nameSpace;
    createTemplateFields();
}

void Mapping::Template::createTemplateFields() {
    std::string tmp = this->text;
    tmp.replace(tmp.begin(), tmp.end(), "\\", " ");
    while (tmp.find('{') != -1) {
        size_t from = tmp.find('{') + 1;
        size_t to = tmp.find('}');
        fields.push_back(tmp.substr((from, to - from)));
        tmp = tmp.substr(to + 1, tmp.length() - to - 1);
    }
}

void Mapping::Template::replaceText(std::vector<std::string> &replace) {
    size_t from = 0, to = 0;
    for (auto &i: replace) {
        from = text.find('{', to);
        to = text.find('}', to) + 1;
        text.replace(from, to - from, i);
    }
}

std::vector<std::string> Mapping::SubjectMap::getColumns() {
    return tem.getFields();
}

//--------------parser------------
//--------------util--------------
Mapping::Token Mapping::readFile(char &ch) {
//    char ch;
    while (read(ch)) {
        switch (ch) {
            case '@':
                return Token_at;
            case ':':
                return Token_colon;
            case ';':
                return Token_semicolon;
            case '"':
                return Token_quote;
            case '<':
                if (read(ch) && ch != '#') {
                    readStart--;
                    return Token_uri_start;
                } else {
                    return Token_View_start;
                }
            case '#':
                if (read(ch) && ch == '>') {
                    return Token_uri_end;
                } else {
                    std::cout << "wrong input, expect > after #\n";
                    break;
                }
            case '>':
                return Token_View_end;
            case '[':
                return Token_bracket_start;
            case ']':
                return Token_bracket_end;
            case '.':
                if (readStart < readEnd) return Token_dot;
                else break;
            case ' ':
                return Token_space;
            case '\t':
            case '\r':
                continue;
            default:
//                if (ch >= 'a' && ch <= 'z' || ch >= 'A' && ch <= 'Z' || ch <= '9' && ch >= '0')
//                curOp += ch;
                return Token_text;

        }
    }
    return Token_eof;
}

bool Mapping::read(char &ch) {
    if (readStart < readEnd) {
        ch = *(readStart++);
        return true;
    }
    return false;
}

//--------------reader-----------------
void Mapping::masterParser() {
//    file.open("E:\\r2rml_test.txt");
    while (!file.eof()) {
        std::getline(file, f);
        readStart = &f[0];
        readEnd = &f[f.size() - 1];
        Token next;
        curOp.resize(0);
        if (status == _SqlQuery) {
            readSqlQuery();
        }
        while (Token_eof != (next = readFile(c))) {
            switch (next) {
                case Token_text:
                    curOp += c;
                    break;
                case Token_colon:
                    readProperties();
                    curOp.resize(0);
                    break;
                case Token_at:
                    status = _Prefixes;
                    readPrefixes();
                    changeStatus();
                    break;
                case Token_View_start:
                    readTriplesMap();
                    curOp.resize(0);
                    break;
                case Token_bracket_end:
                    changeStatus();
                    break;
                default:
                    break;
            }
        }
        printf("%d\n", allTripleMaps.size());
    }
}


void Mapping::readProperties() {
//    if (status == _Null) {
//        std::cout << "should be in a tripleMap\n";
//        return;
//    }
    auto p = prefixes[curOp];
//    std::string prev = curOp + ":";
    curOp += ":";
    if (1 || !p.empty()) {
        Token next;
        bool isText = false;
        while (Token_eof != (next = readFile(c))) {
            if (next == Token_text || next == Token_colon) curOp += c;
            else if (next == Token_space) {
                next = readFile(c);
                if (next == Token_bracket_start && status == _TripleMap || next == Token_quote) {
                    judgeOperations();
                } else {
                    isText = true;
                    curOp += " ";
                    curOp += c;
                }
            } else if (next == Token_semicolon) {
                if (isText) readText(curOp);
                break;
            }
        }

    }
}

void Mapping::judgeOperations() {
    std::string op = curOp;
    curOp.resize(0);
    if (op == "rr:logicalTable") {
        status = _LogicalTable;
        curLogicalTable = std::make_shared<LogicalTable>();
        curTripleMap->setLogicalTable(*curLogicalTable);
        readMap();
    } else if (op == "rr:subjectMap") {
        status = _SubjectMap;
        curSubjectMap = std::make_shared<SubjectMap>();
        curTripleMap->setSubjectMap(*curSubjectMap);
        readMap();
    } else if (op == "rr:predicateObjectMap") {
        status = _PredicateObjectMap;
        curPredicateObjectMap = std::make_shared<PredicateObjectMap>();
        curTripleMap->addPredicateObjectMap(*curPredicateObjectMap);
        readMap();
    } else if (op == "rr:predicateMap") {
        status = _PredicateMap;
        curPredicateMap = std::make_shared<PredicateMap>();
        curPredicateObjectMap->setPredicateMap(*curPredicateMap);
        readMap();
    } else if (op == "rr:objectMap") {
        status = _ObjectMap;
        curObjectMap = std::make_shared<ObjectMap>();
        curPredicateObjectMap->setObjectMap(*curObjectMap);
        readMap();
    } else if (op == "rr:tableName") {
        readTemplate();
    } else if (op == "rr:sqlQuery") {
        status = _SqlQuery;
        curSelectQuery = std::make_shared<SelectQuery>();
        curLogicalTableView = std::make_shared<LogicalTableView>();
        curLogicalTableView->setSelectQuery(*curSelectQuery);
        curLogicalTable->setLogicalTableView(*curLogicalTableView);
        while (readFile(c) == Token_quote);
    } else if (op == "rr:template") {
        readTemplate();
    } else if (op == "ex:column") {
        if (status == _ObjectMap) {
            curTemplate = std::make_shared<Template>();
            readTemplate();
        } else {
            std::cout << "column property should in an objectMap\n";
        }
    }
}

void Mapping::readTemplate() {
    curTemplate = std::make_shared<Template>();
    Token next;
    while (Token_text == (next = readFile(c))) {
        curOp += c;
    }
    curTemplate->setText(curOp);
    switch (status) {
        case _LogicalTable:
            curLogicalTable->setTemplate(*curTemplate);
            break;
        case _ObjectMap:
            curObjectMap->setTemplate(*curTemplate);
            break;
        case _SubjectMap:
            curSubjectMap->setTemplate(*curTemplate);
            curTemplate->setText(curOp);
            curTemplate->createTemplateFields();
            break;
        default:
            break;
    }
}

void Mapping::readPrefixes() {
    std::string str, str2;
    int mode = 1;//1:@prefix    2: <xxx#>
//    curOp.resize(0);
    while (readFile(c) != Token_space);
    while (readFile(c) != Token_colon) {
        str += c;
    }
    while (readFile(c) != Token_uri_start);
    while (readFile(c) != Token_uri_end) {
        str2 += c;
    }
    prefixes[str] = str2;
    curOp.resize(0);
}

void Mapping::readTriplesMap() {
    if (status == _Null) {
        auto newTripleMap = std::make_shared<TripleMap>();
        curOp.resize(0);
        while (Token_View_end != readFile(c)) {
            curOp += c;
        }
        newTripleMap->setName(curOp);
        allTripleMaps.emplace_back(*newTripleMap);
        TripleMapsIndex[curOp] = TripleMapsNum++;
    }
}


void Mapping::readMap() {
    Token next;
    while (Token_eof != (next = readFile(c))) {
        switch (next) {
            case Token_text:
                curOp += c;
                break;
            case Token_colon:
                readProperties();
                break;
        }
        curOp.resize(0);
    }
}

void Mapping::readText(std::string &p) {
    std::string str1, str2;
    int mode = 1;
    for (char i: p) {
        if (mode == 1) {
            if (i == ' ') {
                mode++;
                continue;
            }
            str1 += i;
        } else if (mode == 2) {
            str2 += i;
        }
    }
    if (str1 == "rr:class") str1 = "rdf:type";

    if (status == _SubjectMap) {
        curSubjectMap->setPredicate(str1);
        curSubjectMap->setObject(str2);
    } else if (status == _PredicateObjectMap) {
        if (str1 == "rr:predicate") {
            curPredicate = std::make_shared<Predicate>();
            curPredicate->setText(str2);
            curPredicateObjectMap->setPredicate(*curPredicate);
            curPredicateObjectMap->setPreShortCut();
        } else if (str1 == "rr:object") {
            curObject = std::make_shared<Object>();
            curObject->setText(str2);
            curPredicateObjectMap->setObject(*curObject);
            curPredicateObjectMap->setObjShortCut();
        }
    } else if (status == _PredicateMap) {
        curPredicateMap->setProperty(str1);
        if (str1 == "rr:constant") {
            curPredicateMap->setText(str2);
        }
    } else if (status == _ObjectMap) {
        curObjectMap->setProperty(str1);
        if (str1 == "rr:constant") {
            curObjectMap->setText(str2);
        }
    }
}

void Mapping::readSqlQuery() {
    std::string str;
    Token next;
    while (Token_eof != (next = readFile(c))) {
        if (next != Token_quote)
            str += c;
        else {
            curSelectQuery->addQuery(str);
            while (Token_quote == readFile(c));
            return;
        }
    }
    str += '\n';
    curSelectQuery->addQuery(str);
}


void Mapping::changeStatus() {
    Token next = readFile(c);
    if (next == Token_semicolon) {
        switch (status) {
            case _LogicalTable:
            case _SqlQuery:
            case _SubjectMap:
            case _PredicateObjectMap:
                status = _TripleMap;
                break;
            case _Predicate:
            case _ObjectMap:
            case _PredicateMap:
            case _Object:
                status = _PredicateObjectMap;
            default:
                std::cout << "error\n";
                break;
        }
    } else if (c == '.') {
        status = _Null;
    }
}