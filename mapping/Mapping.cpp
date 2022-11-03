//
// Created by yanjs on 2022/10/29.
//

#include "Mapping.h"

//Template
Mapping::Template::Template(std::string text, TermType termType, std::string nameSpace) {
    this->text = text;
    this->termType = termType;
    this->nameSpace = nameSpace;
    createTemplateFields();
}

void Mapping::Template::createTemplateFields() {
    std::string tmp = this->text;
    tmp.replace(tmp.begin(), tmp.end(), "\\", " ");
    while (tmp.find('{') != -1) {
        int from = tmp.find('{') + 1;
        int to = tmp.find('}');
        fields.push_back(tmp.substr((from, to - from)));
        tmp = tmp.substr(to + 1, tmp.length() - to - 1);
    }
}

void Mapping::Template::replaceText(std::vector<std::string> &replace) {
    int from = 0, to = 0;
    for (int i = 0; i < replace.size(); ++i) {
        from = text.find('{', to);
        to = text.find('}', to) + 1;
        text.replace(from, to - from, replace[i]);
    }
}

std::vector<std::string> Mapping::SubjectMap::getColumns() {
    return tem.getFields();
}

//--------------parser------------
Mapping::Token Mapping::readFile(char &c) {
//    char c;
    while (read(c)) {
        switch (c) {
            case '@':
                return Token_at;
            case ':':
                return Token_colon;
            case ';':
                return Token_semicolon;
            case '"':
                return Token_quote;
            case '<':
                if (read(c) && c != '#') {
                    readStart--;
                    return Token_uri_start;
                } else {
                    return Token_View_start;
                }
            case '#':
                if (read(c) && c == '>') {
                    readStart++;
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
            case '\t':
            case '\r':
                continue;
            default:
                if (c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z') {
                    curOp += c;
                }
        }
    }
    return Token_eof;
}

bool Mapping::read(char &c) {
    if (readStart < readEnd) {
        c = *(readStart++);
        return true;
    }
    return false;
}

void Mapping::masterParser() {
    file.open("E:\\r2rml_test.txt");
    while (!file.eof()) {
        std::getline(file, f);
        readStart = &f[0];
        readEnd = &f[f.size() - 1];
        Token next;
        curOp.resize(0);
        while (Token_eof != (next = readFile(c))) {
            switch (next) {
                case Token_colon:
                    readProperties();
                    curOp.resize(0);
                    break;
                case Token_at:
                    status = _Prefixes;
                    readPrefixes();
                    status = _Null;
                    curOp.resize(0);
                    break;
                case Token_View_start:
                    status = _TripleMap;
                    readTriplesMap();
                    curOp.resize(0);
                    break;
                case Token_bracket_end:
                    changeStatus();
                    break;
            }
        }

    }

}

void Mapping::readProperties() {
    if (status == _Null) {
        std::cout << "should in a tripleMap\n";
        return;
    }
    auto p = prefixes[curOp];
    std::string prev = curOp + ":";
    if (!p.empty()) {
        curOp.resize(0);
        if (readFile(c) == Token_bracket_start && status == _TripleMap) {
            judgeOperations();
        } else if (readFile(c) == Token_quote) {//maybe template
            judgeOperations();
        } else if (readFile(c) == Token_colon) {//e.g. rr:class ex:Department
            std::string tmp = prev + curOp + ":";
            curOp.resize(0);
            if (readFile(c) == Token_semicolon) {
                tmp += curOp;
                readText(tmp);
            }
        }
    } else {
        std::cout << "no such prefix\n";
        return;
    }
}


void Mapping::readPrefixes() {
    std::string str, str2;
    int mode = 1;//1:@prefix    2: <xxx#>
    curOp.resize(0);
    while (readFile(c) != Token_eof) {
        if (mode == 1) {
            while (readFile(c) != Token_colon) {
                if (curOp == "prefix") curOp.resize(0);
            }
            str = curOp;// prefix abbreviation
            curOp.resize(0);
            mode++;
        } else if (mode == 2) {
            while (readFile(c) != Token_uri_start);
            while (readFile(c) != Token_uri_end) {
                str2 += c;
            }
            auto t = prefixes.insert(std::make_pair(str, str2)).second;
            if (!t) std::cout << "already existed\n";
            mode++;
        }
    }
    curOp.resize(0);
}

void Mapping::readTriplesMap() {
    curTripleMap = new TripleMap;
    curOp.resize(0);
    while (readFile(c) != Token_View_end);
    curTripleMap->setName(curOp);
    allTripleMaps.emplace_back(*curTripleMap);
    TripleMapsIndex.insert(std::make_pair(curOp, TripleMapsNum++));

}


void Mapping::judgeOperations() {
    ///TODO: rr:xxx
    std::string op = curOp;
    if (op == "logicalTable") {
        curLogicalTable = new LogicalTable();
        curTripleMap->setLogicalTable(*curLogicalTable);
        status = _LogicalTable;
        Token next = readFile(c);
        if (next == Token_bracket_start) {
            curOp.resize(0);
            readLogicalTableName();
        } else if (next == Token_View_start) {
            curOp.resize(0);
            readLogicalTableView();
        }
    } else if (op == "subjectMap") {
        status = _SubjectMap;
        curSubjectMap = new SubjectMap();
        curTripleMap->setSubjectMap(*curSubjectMap);
        readMap();
    } else if (op == "predicateObjectMap") {
        status = _PredicateObjectMap;
        curPredicateObjectMap = new PredicateObjectMap();
        curTripleMap->addPredicateObjectMap(*curPredicateObjectMap);
        readMap();
    } else if (op == "predicateMap") {
        status = _PredicateMap;
        curPredicateMap = new PredicateMap();
        curPredicateObjectMap->setPredicateMap(*curPredicateMap);
        readMap();
    } else if (op == "objectMap") {

    } else if (op == "tableName") {
        readTableName();
    } else if (op == "column") {

    } else if (op == "tableName") {
        status = _TableName;
        readTableName();
        if (readEndOfMap()) status = _TripleMap;
    } else if (op == "sqlQuery") {

    } else if (op == "class") {

    } else if (op == "template") {
        curTemplate = new Template();
        curOp.resize(0);
        if (readFile(c) == Token_quote) {
            curTemplate->setText(curOp);
            curTemplate->getFields();
        }
        if (status == _SubjectMap) curSubjectMap->setTemplate(*curTemplate);
//        if (readEndOfMap())
//            status = _TripleMap;
    }
}

void Mapping::readLogicalTableName() {
    curOp.resize(0);
    if (readFile(c) == Token_colon) {
        curLogicalTable->setNameSpace(curOp);
        curOp.resize(0);
    } else {
        std::cout << "expect a namespace\n";
        return;
    }
    if (readFile(c) == Token_quote) {
        judgeOperations();
    }
}

void Mapping::readTableName() {
    curOp.resize(0);
    if (readFile(c) == Token_quote) {
        curLogicalTable->setLogicalTableName(curOp);
    }
}

void Mapping::readMap() {
    Token next;
    while (Token_eof != (next = readFile(c))) {
        if (next == Token_colon) {
            readProperties();
        } else if (next == Token_bracket_end) {
            changeStatus();
        } else if (next == Token_semicolon) {
            continue;
        }
        curOp.resize(0);
    }
}

void Mapping::readText(std::string p) {
    std::string str1, str2;
    int mode = 1;
    curOp.resize(0);
    for (int i = 0; i < p.size(); ++i) {
        if (mode == 1) {
            if (p[i] == ' ') {
                mode++;
                continue;
            }
            str1 += p[i];
        } else if (mode == 2) {
            str2 += p[i];
        }
    }
    if (str1 == "rr:class") str1 = "rdf:type";

    if (status == _SubjectMap) {
        curSubjectMap->setPredicate(str1);
        curSubjectMap->setObject(str2);
    } else if (status == _PredicateObjectMap) {
        if (str1 == "rr:predicate") {
            curPredicate = new Predicate();
            curPredicate->setText(str2);
            curPredicateObjectMap->setPredicate(*curPredicate);
        } else if (str1 == "rr:object") {
            curObject = new Object();
            curObject->setText(str2);
            curPredicateObjectMap->setObject(*curObject);
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


bool Mapping::readEndOfMap() {
    while (readFile(c) != Token_eof) {
        if (c == ';') return false;
        if (c == ']' && readFile(c) == Token_semicolon) {
            return true;
        }
    }
    return false;
}

void Mapping::changeStatus() {
    Token next = readFile(c);
    if (next == Token_semicolon) {
        switch (status) {
            case _LogicalTable:
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
    } else if (next == Token_eof && c == '.') {
        status = _Null;
    }
}