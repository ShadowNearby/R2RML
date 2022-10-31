cmake_minimum_required(VERSION 3.0)
project(R2RML)

set(CMAKE_CXX_STANDARD 17)
set(FULL_PATH_TO_MYSQL_CONNECTOR_CPP_DIR "C:\\Program Files\\MySQL\\MySQL Server 8.0")
include_directories(${FULL_PATH_TO_MYSQL_CONNECTOR_CPP_DIR}/include)
link_directories(${FULL_PATH_TO_MYSQL_CONNECTOR_CPP_DIR}/lib)
add_executable(R2RML main.cpp
        util/Triple.h util/Triple.cpp
        parser/Type.h parser/TurtleParser.h parser/TurtleParser.cpp
        util/Utility.h parser/RDFParser.cpp parser/RDFParser.h
        #        test/test.cpp test/test.h
        KVstore/KVstore.cpp KVstore/KVstore.h util/PairHash.h parser/cmdLine.h mapping/Mapping.cpp mapping/Mapping.h)
target_link_libraries(R2RML ${CMAKE_SOURCE_DIR}/bin/libmysql.dll)