//
// Created by yanjs on 2022/11/9.
//
#include "TriplesMap.h"
#include "R2RMLParser.h"

std::string &SubjectMap::getSubject()
{
    return termMap.getValue();
}

std::string &SubjectMap::getGraph()
{
    if (!graph.empty())
        return graph;
    else return graphMap.termMap.getValue();
}

std::string &columnToTemplate(std::string &src)
{
    src = "\"{" + src.substr(1, src.size() - 2) + "}\"";
    return src;
}

std::string &TermMap::getValue()
{
    if (!constant_.empty())
        return constant_;
    else if (!template_.empty())
        return template_;
    template_ = columnToTemplate(column_);
    column_.clear();
    return template_;
}