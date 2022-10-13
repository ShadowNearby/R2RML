//
// Created by yanjs on 2022/10/13.
//

#pragma once

#include "Utility.h"
#include "Triple.h"
#include "TurtleParser.h"

class RDFParser
{
private:
    std::stringstream sin_;
    TurtleParser turtleParser_;
public:
    RDFParser();

    RDFParser(std::ifstream &_fin);

    int
};
