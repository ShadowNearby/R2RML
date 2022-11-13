//
// Created by yanjs on 2022/11/9.
//
#include "TriplesMap.h"
#include "R2RMLParser.h"

std::string &PredicateObjectMap::getObject()
{
    if (!object.empty())
        return object;
    else if (!objectMap.constant.empty()) {
        auto key = objectMap.constant;
        auto &refObjectMap = R2RMLParser::refObjectMaps[objectMap.constant];
        auto &objObjectMap = R2RMLParser::objectMaps[objectMap.constant];
        if (!refObjectMap.empty()) {
            auto &parentTripleMap = refObjectMap.parentTripleMap;
            auto &triplesMap = R2RMLParser::triplesMaps[parentTripleMap];
            return triplesMap.subjectMap.getSubject();
        } else if (!objObjectMap.empty()) {
            return objObjectMap.termMap.getValue();
        }
    } else if (!objectMap.termMap.empty())
        return objectMap.termMap.getValue();
    auto &parentTripleMap = objectMap.refObjectMap.parentTripleMap;
    auto &triplesMap = R2RMLParser::triplesMaps[parentTripleMap];
//        TriplesMap triplesMap;
    return triplesMap.subjectMap.getSubject();

}

std::string &PredicateObjectMap::getPredicate()
{
    if (!predicate.empty())
        return predicate;
    else return predicateMap.termMap.constant_;
}

std::string &PredicateObjectMap::getGraph()
{
    return graph;
}

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
    else {
        template_ = columnToTemplate(column_);
        column_.clear();
        return template_;
    }
}