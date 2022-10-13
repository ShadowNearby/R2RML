//
// Created by yanjs on 2022/10/3.
//
#include "Utility.h"
#include "parser/RDFParser.h"
#include <ctime>

int main()
{
    clock_t start, finish;
    start = clock();

    std::vector<Triple> v;
    std::fstream f;
    f.open(R"(..\example)", std::ios::in);
    RDFParser parser(f);
    size_t nums = parser.parseFile(v);
    std::cout << "total triple: " << nums << std::endl;
    f.close();

    finish = clock();
    double cost = double(finish - start) / CLOCKS_PER_SEC;
    double speed = (1000000 / double(nums)) * cost;
    std::cout << "time cost: " << cost << "s" << std::endl;
    std::cout << "speed: " << speed << "s / 1million" << std::endl;
    return 0;
}