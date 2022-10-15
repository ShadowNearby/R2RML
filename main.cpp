//
// Created by yanjs on 2022/10/3.
//

#include "test/test.h"
#include "Database.h"
#include "parser/RDFParser.h"

int main()
{
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
    auto triple = &vec[1];
    auto flag = database.handleTriple(triple);
    std::cout << flag << std::endl;
    return 0;

}