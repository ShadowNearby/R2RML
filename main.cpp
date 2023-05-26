//
// Created by yanjs on 2022/10/3.
//
#include "mapping/R2RMLParser.h"
#include "parser/RDFParser.h"
#include "mapping/Handle.h"
#include "mapping/SelectQuery.h"
#include "KVstore/ConKVStore.h"
#include "omp.h"
#include <sstream>
using namespace std;
auto _100 = R"(..\tiny_example)";
auto _100k = R"(..\example)";
auto _10m = R"(D:\Download\Claros)";

void outputTriples(string path, Handle &handle, int thread_num) {
    auto& id2string = handle.result.id2string;
    int notriples = handle.result.triple2id.size();
    vector<vector<size_t>> tripleids(notriples, vector<size_t>(3));
    int index = 0;
    for (const auto& item : handle.result.triple2id) {
        tripleids[index][0] = std::get<0>(item.first);
        tripleids[index][1] = std::get<1>(item.first);
        tripleids[index][2] = std::get<2>(item.first);
        index++;
    }
    int numberpersection = (notriples / thread_num) + 1;
#pragma omp parallel for schedule(static)
    for (int i = 0; i < thread_num; i++) {
        fstream local;
        local.open(path + "result-" + to_string(i), std::ios::out);
        stringstream s;
        for (int j = numberpersection * i; j < numberpersection * (i + 1) && j < notriples; j++) {
            s << id2string[tripleids[j][0]] << " " << id2string[tripleids[j][1]] << " " << id2string[tripleids[j][2]] << " ." << endl;
        }
        local << s.str();
        local.close();
    }
}

int main(int argc, char *argv[])
{

//    Logger logger;
    std::fstream f;
//    f.open("../test/testin");
    std::string user(argv[2]), password(argv[3]), schema_name(argv[4]), mapping_file(argv[5]);
    int thread_num = strtol(argv[1], nullptr, 10);
//    std::cout << "std::cin >> thread_num >> user >> password >> schema_name >> mapping_file;" << std::endl;
//    std::cin >> thread_num >> user >> password >> schema_name >> mapping_file;
    f.open(mapping_file);
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
    Handle handle(kvstore, thread_num, schema_name, user, password);
    //;
    outputTriples("C:/IST/R2RML/Materilzation/Fingr/", handle, thread_num);
 
    //f.close();
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
    int out;
    std::cin >> out;
    return 0;
}
