//
// Created by yanjs on 2022/10/3.
//

#include "test/test.h"
#include "Database.h"
#include "parser/RDFParser.h"

int main() {
    auto _100 = R"(..\tiny_example)";
    auto _10k = R"(..\example)";
//    auto _10m = R"(D:\Download\Claros)";
//    RunTimeTest test(_10k);
    std::fstream f;
    f.open(_100, std::ios::in);
    RDFParser rdfParser(f);
    std::vector<Triple> vec;
    rdfParser.parseFile(vec);
    Database database;
    clock_t start, end;
    start = clock();
    for (int i = 0; i < vec.size(); ++i) {
        auto triple = &vec[i];
        auto flag = database.handleTriple(triple);
        if (!flag) std::cout << "error";
    }
    end = clock();
    std::cout << (double) (end - start) / CLOCKS_PER_SEC << std::endl;
    std::string sub, pre, obj;
    std::cin >> sub >> pre >> obj;
    database.findTriple(sub, pre, obj);
    return 0;

}