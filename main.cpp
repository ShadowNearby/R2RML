//
// Created by yanjs on 2022/10/3.
//
#include "mapping/R2RMLParser.h"
#include "parser/RDFParser.h"
#include "mapping/Handle.h"
#include "mapping/SelectQuery.h"
#include "KVstore/ConKVStore.h"
#include "omp.h"
#include <sys\types.h>
#include <sstream>
using namespace std;
auto _100 = R"(..\tiny_example)";
auto _100k = R"(..\example)";
auto _10m = R"(D:\Download\Claros)";

void outputTriples(string path, Handle &handle, int thread_num) {
    auto& id2string = handle.result.id2string;
    int notriples = handle.result.triple2id.size();
    int index = 0,start = 0,table = 0;
    int numberpersection = (notriples / 16) + 1, numberperthread = numberpersection / thread_num +1;
    vector<vector<vector<size_t>>> tripleids(2,vector<vector<size_t>>(numberpersection, vector<size_t>(3)));
   // vector<stringstream*> sstreams(thread_num,new stringstream());
    //std::cout << "Iteration begin\n";
    vector<mutex> locks(thread_num);
    for (const auto& item : handle.result.triple2id) {
        tripleids[table][index][0] = std::get<0>(item.first);
        tripleids[table][index][1] = std::get<1>(item.first);
        tripleids[table][index][2] = std::get<2>(item.first);
        index++;
        
        if ((index > 0 && index % numberpersection == 0)) {
            int end = index;
            int tempTable = table;
            int mainthread = omp_get_thread_num();
            omp_set_num_threads(thread_num);
            omp_set_nested(1);
#pragma omp parallel 
#pragma omp for nowait
            for (int k = 0; k < 2; k++) {
                if (mainthread == omp_get_thread_num()&&thread_num !=1) {
                    //std::cout << "main thread!\n";
                    break;
                }

#pragma omp parallel for schedule(static)
                for (int i = 0; i < thread_num; i++) {
                    fstream local;
                    local.open(path + "result-" + to_string(i), std::ios_base::app);
                    stringstream s;
                    for (int j = numberperthread * i; j < numberperthread * (i + 1) && j < end; j++) {
                        s << id2string[tripleids[tempTable][j][0]] << " " << id2string[tripleids[tempTable][j][1]] << " " << id2string[tripleids[tempTable][j][2]] << " ." << endl;
                       // if (j == numberperthread * i + start )std::cout << "thread_num " << omp_get_thread_num() << "\n";
                    }
                    locks[i].lock();
                    local << s.str();
                    local.close();
                    locks[i].unlock();
                }
                //tripleids[tempTable].clear();
            }
            table = 1-table;
            index = 0;
        }
        
    }
    int end = index;
#pragma omp parallel for schedule(static)
    for (int i = 0; i < thread_num; i++) {
        fstream local;
        local.open(path + "result-" + to_string(i), std::ios_base::app);
        stringstream s;
        for (int j = numberperthread * i; j < numberperthread * (i + 1) && j < end; j++) {
            s << id2string[tripleids[table][j][0]] << " " << id2string[tripleids[table][j][1]] << " " << id2string[tripleids[table][j][2]] << " ." << endl;
            // if (j == numberperthread * i + start )std::cout << "thread_num " << omp_get_thread_num() << "\n";
        }
        locks[i].lock();
        local << s.str();
        local.close();
        locks[i].unlock();
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
    Handle handle(kvstore, thread_num, schema_name, user, password,start);
    //;
    printf("begin output\n");
    outputTriples("D:/IST/R2RML/Output/bsbm/fingr/", handle, thread_num);
 
    //f.close();
    auto end = std::chrono::steady_clock::now();
    auto runTime = std::chrono::duration<double>(end - start).count();
//    handle.result.getAllTriples(result);
    printf("sum: %zu runtime:%f\n", handle.result.triple2id.size(), runTime);
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
