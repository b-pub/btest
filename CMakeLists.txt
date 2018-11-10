cmake_minimum_required(VERSION 2.8)

project(btest CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++03 -Wall -Werror")

include_directories(include)

# Define the btest static library

add_library(btest STATIC
    src/btest_main.cpp
)

# Add one test executable as a demo

add_executable(test_unittests
    tests/sample_1.cpp
    tests/sample_2.cpp
)

target_link_libraries(test_unittests btest)
