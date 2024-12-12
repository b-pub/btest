/*
 * SPDX-License-Identifier: ISC
 *
 * Example unit test for the btest library.
 */
#include <stdexcept>
#include "btest.hpp"

BTEST(First, trueFalseExpectNE)
{
    EXPECT_NE(true, false);
}

BTEST(First, trueFalseAssertNE)
{
    ASSERT_NE(true, false);
}

BTEST(First, trueFalseExpectEQ)
{
    EXPECT_EQ(true, false); // Expected failure
}

BTEST(First, trueFalseAssertEQ)
{
    ASSERT_EQ(true, false); // Expected failure
}

BTEST(First, trueTrueExpectEQ)
{
    EXPECT_EQ(true, true);
}

BTEST(First, falseFalseExpectEQ)
{
    EXPECT_EQ(false, false);
}

BTEST(Second, throwStdException)
{
    throw std::logic_error("logic error occurred"); // Expected failure
}

BTEST(Second, DISABLED_testDisabled)
{
    // This will crash if not disabled correctly
    *((volatile int*)0) = 42;
}

BTEST(Third, ExpectedFailure)
{
    FAIL() << "This is a forced failure." << std::endl; // Expected failure
}
