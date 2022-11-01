//
// Created by yanjs on 2022/10/29.
//

#include "Mapping.h"
#include "../util/Utility.h"

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
    }
}

std::vector<std::string> Mapping::SubjectMap::getColumns() {
    return tem.getFields();
}