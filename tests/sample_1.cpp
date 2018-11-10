#include <stdexcept>
#include "btest.h"

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
    EXPECT_EQ(true, false);
}

BTEST(First, trueFalseAssertEQ)
{
    ASSERT_EQ(true, false);
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
    throw std::logic_error("logic error occurred");
}

BTEST(Second, DISABLED_testDisabled)
{
    // This will crash if not disabled correctly
    *((volatile int*)0) = 42;
}
