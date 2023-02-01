cmake_minimum_required(VERSION 3.23)
project(R2RML)
#SET(CMAKE_TOOLCHAIN_FILE "C:/src/vcpkg/scripts/buildsystems/vcpkg.cmake")
if (DEFINED ENV{VCPKG_DEFAULT_TRIPLET} AND NOT DEFINED VCPKG_TARGET_TRIPLET)
    set(VCPKG_TARGET_TRIPLET "$ENV{VCPKG_DEFAULT_TRIPLET}" CACHE STRING "")
endif ()
set(CMAKE_CXX_STANDARD 17)
set(FULL_PATH_TO_MYSQL_CONNECTOR_CPP_DIR "C:/Program Files/MySQL/mysql-connector-c++-8.0")
include(C:/src/vcpkg/scripts/buildsystems/vcpkg.cmake)
find_package(folly CONFIG REQUIRED)
include_directories(
        ${FULL_PATH_TO_MYSQL_CONNECTOR_CPP_DIR}/include
        ${folly_INCLUDE_DIRS}
)
link_directories(
        ${FULL_PATH_TO_MYSQL_CONNECTOR_CPP_DIR}/lib64/vs14/debug
)
add_executable(${PROJECT_NAME} main.cpp
        util/Triple.h util/Triple.cpp
        util/Reclaimer.h util/LockFreeMap.h
        parser/Type.h parser/TurtleParser.h parser/TurtleParser.cpp
        util/Utility.h parser/RDFParser.cpp parser/RDFParser.h
        #        test/test.cpp test/test.h
        KVstore/KVstore.cpp KVstore/KVstore.h util/PairHash.h
        #        parser/cmdLine.h
        #        mapping/Mapping.cpp mapping/Mapping.h
        mapping/SelectQuery.cpp mapping/SelectQuery.h
        mapping/R2RMLParesr.cpp mapping/R2RMLParser.h
        util/Logger.h
        mapping/TriplesMap.h mapping/TriplesMap.cpp
        mapping/Handle.h mapping/Handle.cpp
        )
#target_link_libraries(main PRIVATE Folly::follybenchmark Folly::folly_test_util)
target_link_libraries(${PROJECT_NAME} mysqlcppconn8 Folly::folly Folly::folly_deps)
