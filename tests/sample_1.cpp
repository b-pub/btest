/*
 * Example unit test for the embtest library.
 */
#include <stdexcept>
#include "embtest.hpp"

TEST(First, trueFalseExpectNE)
{
    EXPECT_NE(true, false);
}

TEST(First, trueFalseAssertNE)
{
    ASSERT_NE(true, false);
}

TEST(First, trueFalseExpectEQ)
{
    EXPECT_EQ(true, false); // Expected failure
}

TEST(First, trueFalseAssertEQ)
{
    ASSERT_EQ(true, false); // Expected failure
}

TEST(First, trueTrueExpectEQ)
{
    EXPECT_EQ(true, true);
}

TEST(First, falseFalseExpectEQ)
{
    EXPECT_EQ(false, false);
}

TEST(Second, throwStdException)
{
    throw std::logic_error("logic error occurred"); // Expected failure
}

TEST(Second, DISABLED_testDisabled)
{
    // This will crash if not disabled correctly
    *((volatile int*)0) = 42;
}

TEST(Third, ExpectedFailure)
{
    FAIL() << "This is a forced failure." << std::endl; // Expected failure
}
