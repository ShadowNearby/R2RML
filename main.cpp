//
// Created by yanjs on 2022/10/3.
//

#include "parser/RDFParser.h"
#include "KVstore/KVstore.h"
#include "omp.h"


#define ly false
#define yjs true
auto _100 = R"(..\tiny_example)";
auto _100k = R"(..\example)";
auto _10m = R"(D:\Download\Claros)";
std::string subject = "<http://dag.org#node840>";
std::string predicate = "<http://dag.org#edge>";
std::string object = "<http://dag.org#node1726>";

int main()
{
    omp_set_num_threads(100);
//    RunTimeTest test(_10k);
    std::fstream f;
    clock_t start, end;
    start = clock();
    f.open(_100k, std::ios::in);
    RDFParser rdfParser(f);
    std::vector<Triple> list;
    rdfParser.parseFile(list);
#if ly
    Database database;
    for (auto & i : vec) {
        auto triple = &i;
        auto flag = database.handleTriple(triple);
        if (!flag) std::cout << "error";
    }
#endif
#if yjs

    KVstore kvstore;
#pragma omp parallel for default(none) shared(list) firstprivate(kvstore)

    for (size_t i = 0; i < list.size() / 2; ++i) {
        kvstore.insert(list[i]);
    }
    end = clock();
    printf("%f\n", (double) (end - start) / CLOCKS_PER_SEC);
//    kvstore.merge(_kvstore);
//    auto nums = kvstore.insert(list);
//    printf("%llu\t%llu\n", vec.size(), list.size());

#endif
    end = clock();
    printf("%f\n", (double) (end - start) / CLOCKS_PER_SEC);
    return 0;

}