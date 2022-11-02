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

void Mapping::masterParser() {
    char c;
    file.open("E:\\r2rml_test.txt");
    while (!file.eof()) {
        std::getline(file, f);
        readStart = &f[0];
        readEnd = &f[f.size() - 1];
        switch (readFile(c)) {
            case Token_colon:
                readProperties();
        }
    }

}

void Mapping::readProperties() {
    auto p = prefixes[curOp];
    if (!p.empty()) {
        std::string pref = curOp;
        curOp.resize(0);
        while (readFile(c) != Token_space);
        if (pref == "rr") {
            // curOp = propertyName
            judgeOperations();
        }
    } else {
        std::cout << "no such prefix\n";
    }
}


void Mapping::readPrefixes() {
    char c;
    std::string str, str2;
    int mode = 1;//1:@prefix    2: ??:  3: <xxx#>
    while (readFile(c) != Token_eof) {
        if (mode == 1) {
            while (readFile(c) != Token_space) {
                str += c;
            }
            if (str == "prefix") {
                mode++;
                str.resize(0);
                continue;
            } else {
                std::cout << "wrong input, expect prefix after @\n";
            }
        } else if (mode == 2) {
            while (readFile(c) != Token_colon) {
                str += c;
            }
            mode++;
        } else if (mode == 3) {
            while (readFile(c) != Token_uri_start);
            while (readFile(c) != Token_uri_end) {
                str2 += c;
            }
            prefixes.insert(std::make_pair(str, str2));
        }
    }
    curOp.resize(0);
}

void Mapping::readTriplesMap() {
    curTriplesMap.resize(0);
    char c;
    while (readFile(c) != Token_logical_table_view_end) {
        curTriplesMap += c;
    }
}

Mapping::Token Mapping::readFile(char &c) {
//    char c;
    while (read(c)) {
        switch (c) {
            case '@':
                status = _Prefixes;
                readPrefixes();
                status = _TripleMap;
                break;
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
                    readTriplesMap();
                    continue;
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
                return Token_logical_table_view_end;
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

void Mapping::judgeOperations() {
    ///TODO: rr:xxx
    std::string op = curOp;
    if (op == "logicalTable") {
        curLogicalTable = new LogicalTable();
        curOp.resize(0);
        while (readFile(c) != Token_bracket_start);
        status = _LogicalTable;
        readLogicalTable();

    } else if (op == "subjectMap") {
        curOp.resize(0);
        while (readFile(c) != Token_bracket_start);

        curSubjectMap = new SubjectMap();
        curLogicalTable->setSubjectMap(*curSubjectMap);

        status = _SubjectMap;

        if (readEndOfMap())
            status = _TripleMap;
    } else if (op == "predicateObjectMap") {

    } else if (op == "predicate") {

    } else if (op == "objectMap") {

    } else if (op == "tableName") {
        readTableName();
    } else if (op == "column") {

    } else if (op == "tableName") {
        readTableName();
    } else if (op == "class") {

    } else if (op == "template") {
        curTemplate = new Template();
        while (readFile(c) != Token_quote);
        curOp.resize(0);
        while (readFile(c) != Token_quote);
        curTemplate->setText(curOp);
        curTemplate->getFields();
        if (status == _SubjectMap) curSubjectMap->setTemplate(*curTemplate);
        if (readEndOfMap())
            status = _TripleMap;
    }
}

void Mapping::readLogicalTable() {
    while (readFile(c) != Token_bracket_start);
    curOp.resize(0);
    while (readFile(c) != Token_colon);
    readProperties();
}

void Mapping::readTableName() {
    while (readFile(c) != Token_quote);
    curOp.resize(0);
    curTableName.resize(0);
    while (readFile(c) != Token_quote) {
        curTableName += c;
    }
    curLogicalTable->setUri(curTableName);
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
