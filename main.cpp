//
// Created by yanjs on 2022/10/3.
//

#include "parser/RDFParser.h"
#include "KVstore/KVstore.h"
#include "parser/cmdLine.h"
#include "omp.h"


auto _100 = R"(..\tiny_example)";
auto _100k = R"(..\example)";
auto _10m = R"(D:\Download\Claros)";

int main(int argc, char *argv[])
{
    cmdline::parser parser;
    parser.add<std::string>("path", 'p', "file path", true);
    parser.parse_check(argc, argv);
    auto path = parser.get<std::string>("path");
    std::fstream f;
    try {
        f.open(path, std::ios::in);
    }
    catch (const std::ifstream::failure &failure) {
        printf("Exception opening file!\n");
    }
    RDFParser rdfParser(f);
    std::vector<Triple> list;
    rdfParser.parseFile(list);
    KVstore kvstore;
    kvstore.insert(list);
    while (true) {
        int mode;
        printf("Please select mode:\n"
               "1:query   2:insert   3:remove   0:quit\n");
        std::cin >> mode;
        if (0 == mode) {
            printf("Bye!");
            break;
        }
        std::string subject, predicate, object;
        printf("Please input subject(? for variable)\n");
        std::cin >> subject;
        printf("Please input predicate(? for variable)\n");
        std::cin >> predicate;
        printf("Please input object(? for variable)\n");
        std::cin >> object;
        auto start = std::chrono::steady_clock::now();
        if (1 == mode) {
            std::vector<Triple> ResultList;
            auto result = kvstore.query(ResultList, subject, predicate, object);
            printf("Complete!\n"
                   "Total query: %llu\n", result);
        } else if (2 == mode) {
            auto result = kvstore.insert(subject, predicate, object);
            printf("Complete!\n");
        } else {
            kvstore.remove(subject, predicate, object);
            printf("Complete!\n");
        }
        auto end = std::chrono::steady_clock::now();
        auto runTime = std::chrono::duration<double>(end - start).count();
        printf("RunTime: %fs\n", runTime);

    }
    return 0;

}