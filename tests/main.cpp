#include <iostream>
#include "btest.h"

int main(int argc, char **argv)
{
    /*
     * All tests defined in other compilation units (source files)
     * are executed in order with btest::runAndReport(). This function
     * executes the tests, tracks their results, and prints a final
     * report of passing, failing, and disabled tests.
     *
     * btest::runAndReport() accepts a std::ostream& so that all test
     * output can be redirected as necessry.
     */
    int result = btest::runAndReport(std::cout);

    std::cout
        << std::endl
        << "A successful run will have 4 failing tests and 1 disabled test"
        << std::endl;
    
    return result;
}
