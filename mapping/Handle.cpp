//
// Created by yanjs on 2022/11/6.
//
#include "Handle.h"
#include "omp.h"

namespace mysqlx
{

    static inline std::vector<uint64_t>
    mysqlx_raw_as_u64_vector(const mysqlx::Value &in_value)
    {
        std::vector<uint64_t> out;

        const auto bytes = in_value.getRawBytes();
        auto ptr = reinterpret_cast<const std::byte *>(bytes.first);
        auto end = reinterpret_cast<const std::byte *>(bytes.first) + bytes.second;

        while (ptr != end) {
            static constexpr std::byte carry_flag{0b1000'0000};
            static constexpr std::byte value_mask{0b0111'1111};

            uint64_t v = 0;
            uint64_t shift = 0;
            bool is_carry;
            do {
                auto byte = *ptr;
                is_carry = (byte & carry_flag) == carry_flag;
                v |= std::to_integer<uint64_t>(byte & value_mask) << shift;

                ++ptr;
                shift += 7;
            } while (is_carry && ptr != end && shift <= 63);

            out.push_back(v);
        }

        return out;
    }

    static inline std::chrono::year_month_day
    read_date(const mysqlx::Value &value)
    {
        const auto vector = mysqlx_raw_as_u64_vector(value);
        if (vector.size() < 3)
            throw std::out_of_range{"Value is not a valid DATE"};

        return std::chrono::year{static_cast<int>(vector.at(0))} / static_cast<int>(vector.at(1)) /
               static_cast<int>(vector.at(2));
    }

    static inline std::chrono::system_clock::time_point
    read_date_time(const mysqlx::Value &value)
    {
        const auto vector = mysqlx_raw_as_u64_vector(value);
        if (vector.size() < 3)
            throw std::out_of_range{"Value is not a valid DATETIME"};

        auto ymd = std::chrono::year{static_cast<int>(vector.at(0))} / static_cast<int>(vector.at(1)) /
                   static_cast<int>(vector.at(2));
        auto sys_days = std::chrono::sys_days{ymd};

        auto out = std::chrono::system_clock::time_point(sys_days);

        auto it = vector.begin() + 2;
        auto end = vector.end();

        if (++it == end)
            return out;
        out += std::chrono::hours{*it};

        if (++it == end)
            return out;
        out += std::chrono::minutes{*it};

        if (++it == end)
            return out;
        out += std::chrono::seconds{*it};

        if (++it == end)
            return out;
        out += std::chrono::microseconds{*it};

        return out;
    }

}

std::string removeQuot(std::string str)
{
    str.erase(std::remove(str.begin(), str.end(), '\"'), str.end());
//    std::cout << str << std::endl;
    return str;
}

Handle::Handle(ConKVStore &store)
{
    double non_join_cost = 0;
    double join_cost = 0;
    double replace_cost = 0;
    parser.parse(store);
    auto it = R2RMLParser::triplesMaps.begin();
    it = R2RMLParser::triplesMaps.begin();
    for (int i = 0; i < R2RMLParser::triplesMaps.size(); ++i, ++it) {
        std::vector<Triple> templateTripleList;
        auto &tripleMap = it->second;
        auto key = it->first;
        std::string subject, predicate, object;
        std::string logicalTableName;
        std::string logicalTableSqlQuery;
        if (!tripleMap.logicalTable.empty()) {
            if (!tripleMap.logicalTable.tableName.empty()) {
                logicalTableName = removeQuot(tripleMap.logicalTable.tableName);
            }
        }
        /// subjectMap
        subject = tripleMap.subjectMap.getSubject();
        if (!tripleMap.subjectMap.subjectClass.empty()) {
            auto start = std::chrono::steady_clock::now();
            selectQuery.getRows(logicalTableName);
            auto end = std::chrono::steady_clock::now();
            non_join_cost += std::chrono::duration<double>(end - start).count();
            folly::ConcurrentHashMap<std::string, std::vector<mysqlx::Value>> storedTemplate;
            std::string sub = subject;
            std::string pre = rrPrefix::type_;
            std::string obj = tripleMap.subjectMap.subjectClass;
            std::vector<std::pair<size_t, size_t>> subPairPos;
            std::vector<std::pair<size_t, size_t>> prePairPos;
            std::vector<std::pair<size_t, size_t>> objPairPos;
            auto &queryRes = selectQuery.result;
            findBrace(storedTemplate, sub, queryRes, subPairPos);
            start = std::chrono::steady_clock::now();
            replaceTemplate(sub, pre, obj, queryRes, subPairPos, prePairPos, objPairPos, false);
            end = std::chrono::steady_clock::now();
            replace_cost += std::chrono::duration<double>(end - start).count();
        }     /// predicateObjectMap

        for (auto &predicateObjectMap: tripleMap.predicateObjectMaps) {
            for (auto &objectMap: predicateObjectMap.objectMap) {
                for (auto &predicateMap: predicateObjectMap.predicateMap) {
                    predicate = predicateMap.termMap.getValue();
                    bool join = false;
                    // non-parentTriplesMap
                    if (objectMap.constant.empty()) {
                        object = objectMap.termMap.getValue();
                        auto start = std::chrono::steady_clock::now();
                        selectQuery.getRows(logicalTableName);
                        auto end = std::chrono::steady_clock::now();
                        non_join_cost += std::chrono::duration<double>(end - start).count();
                    }
                        // parentTriplesMap
                    else {
                        object = R2RMLParser::triplesMaps[objectMap.refObjectMap.parentNode].subjectMap.getSubject();
                        auto start = std::chrono::steady_clock::now();
                        selectQuery.getJoinRows(logicalTableName, objectMap.refObjectMap);
                        auto end = std::chrono::steady_clock::now();
                        join_cost += std::chrono::duration<double>(end - start).count();
                        join = true;
                    }
                    folly::ConcurrentHashMap<std::string, std::vector<mysqlx::Value>> storedTemplate;
                    std::string sub = subject;
                    std::string pre = predicate;
                    std::string obj = object;
                    std::vector<std::pair<size_t, size_t>> subPairPos;
                    std::vector<std::pair<size_t, size_t>> prePairPos;
                    std::vector<std::pair<size_t, size_t>> objPairPos;
                    auto &queryRes = selectQuery.result;
                    findBrace(storedTemplate, sub, queryRes, subPairPos);
                    findBrace(storedTemplate, pre, queryRes, prePairPos);
                    findBrace(storedTemplate, obj, queryRes, objPairPos);
                    auto start = std::chrono::steady_clock::now();
                    replaceTemplate(sub, pre, obj, queryRes, subPairPos, prePairPos, objPairPos, join);
                    auto end = std::chrono::steady_clock::now();
                    replace_cost += std::chrono::duration<double>(end - start).count();
                }
            }
        }
    }
    printf("join cost:%f non join cost:%f replace cost:%f\n", join_cost, non_join_cost, replace_cost);
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
        case mysqlx::Value::ARRAY:
            return value.operator std::string();
        case mysqlx::Value::RAW:
            std::stringstream ss;
            ss << mysqlx::read_date(value);
            return ss.str();
    }
    return {};
}


void Handle::findBrace(folly::ConcurrentHashMap<std::string, std::vector<mysqlx::Value>> &temMap, std::string src,
                       const folly::ConcurrentHashMap<size_t, folly::ConcurrentHashMap<std::string, mysqlx::Value> *> &queryRes,
                       std::vector<std::pair<size_t, size_t>> &pairPos)
{
    size_t preLength = 0;
    while (!src.empty()) {
        auto posLeft = src.find('{');
        auto posRight = src.find('}');
        if (posRight == std::string::npos || posLeft == std::string::npos)
            return;
        pairPos.emplace_back(posLeft + preLength, posRight + preLength);
        auto columnName = src.substr(posLeft + 2, posRight - posLeft - 3);
        if (temMap[columnName].empty()) {
            auto valueList = std::vector<mysqlx::Value>();
            for (auto row: queryRes) {
                if (row.second->find(columnName) != row.second->end())
                    valueList.emplace_back((*(row.second))[columnName]);
            }
            temMap.insert_or_assign(columnName, valueList);
        }
        src = src.substr(posRight + 1, std::string::npos);
        preLength += posRight + 1;
//        columns.emplace_back(columnName);
    }
}


void Handle::replaceTemplate(std::string sub, std::string pre, std::string obj,
                             folly::ConcurrentHashMap<size_t, folly::ConcurrentHashMap<std::string, mysqlx::Value> *> &queryRes,
                             const std::vector<std::pair<size_t, size_t>> &subPairPos,
                             const std::vector<std::pair<size_t, size_t>> &prePairPos,
                             const std::vector<std::pair<size_t, size_t>> &objPairPos, bool join)
{
    size_t size = queryRes.size();
//    printf("%s %s %s\n", sub.c_str(), pre.c_str(), obj.c_str());
//    printf("size: %d\n", size)
    omp_set_num_threads(3);
#pragma omp parallel for
    for (int i = 0; i < size; ++i) {
        auto it = queryRes[i];
        std::string subject = sub, predicate = pre, object = obj;
        int preLength = 0;
        bool flag = true;
        for (auto &pairPos: subPairPos) {
            auto key = subject.substr(pairPos.first + preLength + 1, pairPos.second - pairPos.first - 1);
            if (join) key = "$" + key;
            auto value = it->at(key);
            if (value.isNull())
                flag = false;
            subject = subject.replace(pairPos.first + preLength, pairPos.second - pairPos.first + 1,
                                      toStdString(value));
            if (!join)
                preLength += (int) toStdString(value).size() - (int) key.length() - 2;
            else
                preLength += (int) toStdString(value).size() - (int) key.length() - 1;
        }
        preLength = 0;
        for (auto &pairPos: prePairPos) {
            auto key = predicate.substr(pairPos.first + preLength + 1, pairPos.second - pairPos.first - 1);
            if (join) key = "$" + key;
            auto tokey = it->at(key);
            if (tokey.isNull())
                flag = false;
            predicate = predicate.replace(pairPos.first + preLength, pairPos.second - pairPos.first + 1,
                                          toStdString(tokey));
            if (!join)
                preLength += (int) toStdString(tokey).size() - (int) key.length() - 2;
            else
                preLength += (int) toStdString(tokey).size() - (int) key.length() - 1;
        }
        preLength = 0;
        for (auto &pairPos: objPairPos) {
            auto key = object.substr(pairPos.first + preLength + 1, pairPos.second - pairPos.first - 1);
            if (join) key = "#" + key;
            auto tokey = it->at(key);
            if (tokey.isNull())
                flag = false;
            object = object.replace(pairPos.first + preLength, pairPos.second - pairPos.first + 1,
                                    toStdString(tokey));
            if (!join)
                preLength += (int) toStdString(tokey).size() - (int) key.length() - 2;
            else
                preLength += (int) toStdString(tokey).size() - (int) key.length() - 1;
        }
        if (!flag)
            continue;
        Triple triple(subject, predicate, object);
//        printf("%s %s %s\n", triple.getSubject().c_str(), triple.getPredicate().c_str(), triple.getObject().c_str());
        result.insert(triple);
    }
}
