/*
 * Example unit test for the embtest library.
 *
 * Copyright (c) 2018,2024 Brent Burton
 *
 * SDPX-License-Identifier: ISC
 */
#include <iostream>
#include "embtest.hpp"

int main(int argc, char **argv)
{
    /*
     * All tests defined in other compilation units (source files)
     * are executed in order with btest::runAndReport(). This function
     * executes the tests, tracks their results, and prints a final
     * report of passing, failing, and disabled tests.
     *
     * embtest::runAndReport() accepts a std::ostream& so that all test
     * output can be redirected as necessry.
     */
    int result = embtest::runAndReport(std::cout);

    std::cout
        << std::endl
        << "A successful run will have failing tests and 1 disabled test." << std::endl
        << "It is expected all FAILED tests will have _ShouldFail in the name." << std::endl
        << std::endl;
    
    return result;
}
