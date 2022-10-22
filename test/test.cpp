//
// Created by yanjs on 2022/10/14.
//

#include "test.h"


Test::Test(const std::string &_file_path)
{
    fStream_.open(_file_path, std::ios::in);
}

std::list<Triple> Test::getVector()
{
    return this->triple_vec_;
}

RunTimeTest::RunTimeTest(const std::string &_file_path) : Test(_file_path), parser_(fStream_)
{
    clock_t start, finish;
    start = clock();
    size_t nums = parser_.parseFile(triple_vec_);
    std::cout << "total triple: " << nums << std::endl;
    fStream_.close();
    finish = clock();
    double cost = double(finish - start) / CLOCKS_PER_SEC;
    double speed = (1000000 / double(nums)) * cost;
    std::cout << "time cost: " << cost << "s" << std::endl;
    std::cout << "speed: " << speed << "s / 1million" << std::endl;
}
