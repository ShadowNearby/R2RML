//
// Created by yanjs on 2022/10/3.
//

//#include "parser/RDFParser.h"
//#include "KVstore/KVstore.h"
//#include "parser/cmdLine.h"
#include "mapping/Mapping.h"
#include "omp.h"
#include "util/Logger.h"


auto _100 = R"(..\tiny_example)";
auto _100k = R"(..\example)";
auto _10m = R"(D:\Download\Claros)";

int main(int argc, char *argv[]) {
//    auto con = new MYSQL();
//    mysql_init(con);
//    mysql_real_connect(con, "localhost", "yanjs", "yjs135790", "test", 3306, nullptr, 0);
//    con->db;
//    const char *query = "select * from student;";
//    size_t length = 100;
//    auto result = mysql_real_query(con, query, length);
//    printf("%d\n", result);
//    printf("%s\n", con->db);
//    printf("aa\n");
    Logger logger;
    try {

        Mapping m;
        m.masterParser();
        auto map = m.allTripleMaps;
        for (auto &item: map) {
            logger.log(item.getLogicalTable().getTemplate().getText());
        }
    } catch (std::exception &e) {
        logger.log(e.what());
    }

    return 0;

}