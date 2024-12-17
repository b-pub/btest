cmake_minimum_required(VERSION 3.10)

project(embtest CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Werror")

include_directories(include)

# Define the embtest static library

add_library(embtest STATIC
    src/embtest_impl.cpp
)

# Add one test executable as a demo

file(GLOB EMBTEST_TEST_SOURCES tests/*.cpp)

add_executable(embtest_unittests
    ${EMBTEST_TEST_SOURCES}
)

target_link_libraries(embtest_unittests embtest)
