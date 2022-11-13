//
// Created by yanjs on 2022/10/13.
//

#pragma once

#include "../util/Utility.h"
#include "../util/Triple.h"
#include "TurtleParser.h"

class RDFParser
{
private:
    std::stringstream sin_;
    TurtleParser turtleParser_;
public:
    static const int TRIPLE_NUM_PER_GROUP = 10 * 1000 * 1000;

    RDFParser();

    explicit RDFParser(std::istream &_fin);

    size_t parseFile(std::vector<Triple> &_triple_vector, const std::string &_error_log = "", size_t _startLine = 0);

    size_t parseLines(std::vector<Triple> &_triple_vector, const std::string &_lines);
};
