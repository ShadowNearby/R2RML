//
// Created by yanjs on 2022/10/14.
//

#pragma once

#include "../util/Utility.h"
#include "../parser/RDFParser.h"

class Test
{
private:

protected:
    explicit Test(const std::string &_file_path);

    std::fstream fStream_;
    std::vector<Triple> triple_vec_;
public:
    std::vector<Triple> getVector();
};

class RunTimeTest : public Test
{
private:
    RDFParser parser_;
public:
    explicit RunTimeTest(const std::string &_file_path);
};