//
// Created by yanjs on 2022/10/3.
//
#include "mapping/R2RMLParser.h"
#include "parser/RDFParser.h"
#include "mapping/Handle.h"
#include "mapping/SelectQuery.h"
#include "KVstore/ConKVStore.h"
#include "omp.h"

auto _100 = R"(..\tiny_example)";
auto _100k = R"(..\example)";
auto _10m = R"(D:\Download\Claros)";


int main(int argc, char *argv[])
{

//    Logger logger;
    std::fstream f;
//    f.open("../test/testin");
    f.open("../input");
    RDFParser parser(f);
    std::vector<Triple> vec;
    parser.parseFile(vec);
    ConKVStore kvstore;
    kvstore.insert(vec);
    f.close();
//    f.open("../output");
//    for (auto &item: vec) {
//        f << item.to_string().c_str() << "\n";
//    }
//    f.close();
    auto start = std::chrono::steady_clock::now();
//    omp_set_num_threads(16);
    Handle handle(kvstore);
//    f.open("../output", std::ios::out);
//    for (const auto &item: result)
//        f << item.second.getSubject() << " " << item.second.getPredicate() << " "
//          << item.second.getObject()
//          << " ." << std::endl;
//    f.close();
    auto end = std::chrono::steady_clock::now();
    auto runTime = std::chrono::duration<double>(end - start).count();
//    handle.result.getAllTriples(result);
    printf("sum:%zu runtime:%f\n", handle.result.triple2id.size(), runTime);
//    printf("RunTime: %fs\nCount:%zu\n", runTime, result.size());

//    folly::ConcurrentHashMap<size_t, Triple> result;
//    handle.result.getAllTriples(result);
//    for (auto it = result.begin(); it != result.end(); ++it) {
//        auto t = it->second;
//        printf("%s %s %s\n", t.getSubject().c_str(), t.getPredicate().c_str(), t.getObject().c_str());
//    }
    return 0;
}
