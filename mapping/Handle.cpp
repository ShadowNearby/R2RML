//
// Created by yanjs on 2022/11/6.
//
#include "Handle.h"

std::string &removeQuot(std::string &str)
{
    str.erase(std::remove(str.begin(), str.end(), '\"'), str.end());
    return str;
}

Handle::Handle(KVstore &store)
{

    parser.parse(store);
    for (auto &item: R2RMLParser::triplesMaps) {
        std::vector<Triple> templateTripleList;
        auto &tripleMap = item.second;
        auto &key = item.first;
        std::string subject, predicate, object;
        std::string logicalTableName;
        std::string logicalTableSqlQuery;
        if (!tripleMap.logicalTable.empty()) {
            if (!tripleMap.logicalTable.tableName.empty()) {
                logicalTableName = tripleMap.logicalTable.tableName;
//                    removeQuot(logicalTableName);
            }
        }
        /// subjectMap
        subject = tripleMap.subjectMap.getSubject();
        if (!tripleMap.subjectMap.subjectClass.empty())
            templateTripleList.emplace_back(Triple(subject, rrPrefix::type_, tripleMap.subjectMap.subjectClass));
        /// predicateObjectMap
        for (auto predicateObjectMap: tripleMap.predicateObjectMaps) {
            predicate = predicateObjectMap.getPredicate();
            object = predicateObjectMap.getObject();
            templateTripleList.emplace_back(Triple(subject, predicate, object));
        }
        std::string sql = "SELECT * FROM " + removeQuot(tripleMap.logicalTable.tableName) + ";";
        selectQuery.getRowsBySql(sql);
        for (const auto &triple: templateTripleList) {
            std::unordered_map<std::string, std::vector<mysqlx::Value>> storedTemplate;
            std::string sub = triple.getSubject();
            std::string pre = triple.getPredicate();
            std::string obj = triple.getObject();
            std::vector<std::pair<size_t, size_t>> subPairPos;
            std::vector<std::pair<size_t, size_t>> prePairPos;
            std::vector<std::pair<size_t, size_t>> objPairPos;
            auto &queryRes = selectQuery.result;
            findBrace(storedTemplate, sub, queryRes, subPairPos);
            findBrace(storedTemplate, pre, queryRes, prePairPos);
            findBrace(storedTemplate, obj, queryRes, objPairPos);
            replaceTemplate(sub, pre, obj, queryRes, subPairPos, prePairPos, objPairPos);
        }
    }

}

std::string Handle::toStdString(mysqlx::Value &value)
{
    switch (value.getType()) {
        case mysqlx::Value::INT64:
            return std::to_string(value.operator int());
        case mysqlx::Value::VNULL:
            return {};
        case mysqlx::Value::UINT64:
            return std::to_string(value.operator uint64_t());
        case mysqlx::Value::FLOAT:
            return std::to_string(value.operator float());
        case mysqlx::Value::DOUBLE:
            return std::to_string(value.operator double());
        case mysqlx::Value::BOOL:
            return std::to_string(value.operator bool());
        case mysqlx::Value::STRING:
        case mysqlx::Value::DOCUMENT:
        case mysqlx::Value::RAW:
        case mysqlx::Value::ARRAY:
            return value.operator std::string();
    }
}

void Handle::addValueType(std::string &src, mysqlx::Value &value)
{

}

void Handle::findBrace(std::unordered_map<std::string, std::vector<mysqlx::Value>> &temMap, std::string src,
                       std::vector<std::unordered_map<std::string, mysqlx::Value>> &queryRes,
                       std::vector<std::pair<size_t, size_t>> &pairPos)
{
    size_t preLength = 0;
    while (!src.empty()) {
        auto posLeft = src.find('{');
        auto posRight = src.find('}');
        if (posRight == std::string::npos || posLeft == std::string::npos)
            return;
        pairPos.emplace_back(std::make_pair(posLeft + preLength, posRight + preLength));
        auto columnName = src.substr(posLeft + 2, posRight - posLeft - 3);
        if (temMap[columnName].empty()) {
            auto valueList = std::vector<mysqlx::Value>();
            for (auto &row: queryRes) {
                valueList.emplace_back(row[columnName]);
            }
            temMap[columnName] = valueList;
        }
        src = src.substr(posRight + 1, std::string::npos);
        preLength += posRight + 1;
//        columns.emplace_back(columnName);
    }
}

void Handle::replaceTemplate(std::string &sub, std::string &pre, std::string &obj,
                             std::vector<std::unordered_map<std::string, mysqlx::Value>> &queryRes,
                             std::vector<std::pair<size_t, size_t>> &subPairPos,
                             std::vector<std::pair<size_t, size_t>> &prePairPos,
                             std::vector<std::pair<size_t, size_t>> &objPairPos)
{

    for (auto &row: queryRes) {
        std::string subject = sub, predicate = pre, object = obj;
        int preLength = 0;
        for (auto &pairPos: subPairPos) {
            auto key = subject.substr(pairPos.first + preLength + 2, pairPos.second - pairPos.first - 3);
            subject = subject.replace(pairPos.first + preLength, pairPos.second - pairPos.first + 1,
                                      toStdString(row[key]));
            preLength += (int) toStdString(row[key]).size() - (int) key.length() - 4;
        }
        preLength = 0;
        for (auto &pairPos: prePairPos) {
            auto key = predicate.substr(pairPos.first + preLength + 2, pairPos.second - pairPos.first - 3);
            predicate = predicate.replace(pairPos.first + preLength, pairPos.second - pairPos.first + 1,
                                          toStdString(row[key]));
            preLength += (int) toStdString(row[key]).size() - (int) key.length() - 4;
        }

        preLength = 0;
        for (auto &pairPos: objPairPos) {
            auto key = object.substr(pairPos.first + preLength + 2, pairPos.second - pairPos.first - 3);
            object = object.replace(pairPos.first + preLength, pairPos.second - pairPos.first + 1,
                                    toStdString(row[key]));
            preLength += (int) toStdString(row[key]).size() - (int) key.length() - 4;
        }
        result.emplace_back(Triple(subject, predicate, object));
    }
}
