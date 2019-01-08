cmake_minimum_required(VERSION 3.1)

project(kob CXX C)

option(O_BUILD_TEST "build unit test case, default on" ON)

include(cmake/compiler.cmake)
include_directories(${PROJECT_SOURCE_DIR})

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

if(O_BUILD_TEST)
    enable_testing()
    add_subdirectory(third)
    add_subdirectory(test)
endif(O_BUILD_TEST)