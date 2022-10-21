//
// Created by yanjs on 2022/10/3.
//

#include "test/test.h"
#include "Database.h"
#include "parser/RDFParser.h"
#include "KVstore/KVstore.h"

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

//    RunTimeTest test(_10k);
    std::fstream f;
    clock_t start, end;
    start = clock();
    f.open(_10m, std::ios::in);
    RDFParser rdfParser(f);
    std::vector<Triple> vec;
    rdfParser.parseFile(vec);
    Database database;
#if ly
    for (auto & i : vec) {
        auto triple = &i;
        auto flag = database.handleTriple(triple);
        if (!flag) std::cout << "error";
    }
#endif
#if yjs
    KVstore kVstore;
    for (const auto &item: vec)
        kVstore.insert(item);
    std::vector<Triple> result;
//    auto nums = kVstore.getTripleBySubObj(result, subject, object);
//    for (auto item: result)
//        printf("%s\n", item.to_string().c_str());
//    printf("%llu\n", nums);
#endif
    end = clock();
    std::cout << (double) (end - start) / CLOCKS_PER_SEC << std::endl;
    return 0;

}