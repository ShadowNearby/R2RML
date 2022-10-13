cmake_minimum_required(VERSION 3.0)
project(R2RML)

set(CMAKE_CXX_STANDARD 17)

add_executable(R2RML main.cpp Triple.h Triple.cpp parser/Type.h parser/TurtleParser.h parser/TurtleParser.cpp Utility.h parser/RDFParser.cpp parser/RDFParser.h)
