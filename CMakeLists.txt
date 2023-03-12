cmake_minimum_required(VERSION 3.23)
project(R2RML)
#SET(CMAKE_TOOLCHAIN_FILE "C:/src/vcpkg/scripts/buildsystems/vcpkg.cmake")
if (DEFINED ENV{VCPKG_DEFAULT_TRIPLET} AND NOT DEFINED VCPKG_TARGET_TRIPLET)
    set(VCPKG_TARGET_TRIPLET "$ENV{VCPKG_DEFAULT_TRIPLET}" CACHE STRING "")
endif ()
set(CMAKE_CXX_STANDARD 20)
set(FULL_PATH_TO_MYSQL_CONNECTOR_CPP_DIR "C:/Program Files/MySQL/mysql-connector-c++-8.0")
include(C:/src/vcpkg/scripts/buildsystems/vcpkg.cmake)
find_package(folly CONFIG REQUIRED)
find_package(OpenMP)
if (OPENMP_FOUND)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif ()
include_directories(
        ${FULL_PATH_TO_MYSQL_CONNECTOR_CPP_DIR}/include
        ${folly_INCLUDE_DIRS}
)
link_directories(
        ${FULL_PATH_TO_MYSQL_CONNECTOR_CPP_DIR}/lib64/vs14/debug
)
add_executable(${PROJECT_NAME} main.cpp
        util/Triple.h util/Triple.cpp
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
        KVstore/ConKVStore.cpp KVstore/ConKVStore.h
        )
#target_link_libraries(main PRIVATE Folly::follybenchmark Folly::folly_test_util)
target_link_libraries(${PROJECT_NAME} mysqlcppconn8 Folly::folly Folly::folly_deps)
