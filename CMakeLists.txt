cmake_minimum_required(VERSION 3.0)
project(R2RML)

set(CMAKE_CXX_STANDARD 17)


add_executable(R2RML main.cpp
        util/Triple.h util/Triple.cpp
        parser/Type.h parser/TurtleParser.h parser/TurtleParser.cpp
        util/Utility.h parser/RDFParser.cpp parser/RDFParser.h
        #        test/test.cpp test/test.h
        Database.cpp Database.h Structures.h
        KVstore/KVstore.cpp KVstore/KVstore.h util/PairHash.h parser/cmdLine.h)
find_package(OpenMP)
if (OpenMP_CXX_FOUND)
    target_link_libraries(R2RML PUBLIC OpenMP::OpenMP_CXX)
endif ()