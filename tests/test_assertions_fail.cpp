/*
 * Test assertions that should fail.
 *
 * Copyright (c) 2018,2024 Brent Burton
 *
 * SDPX-License-Identifier: ISC
 */
#include "embtest.hpp"

// ---- Boolean

TEST(AssertionFail, BooleanExpectEQ_ShouldFail)
{
    EXPECT_EQ(true, false);
}

TEST(AssertionFail, BooleanAssertEQ_ShouldFail)
{
    ASSERT_EQ(true, false);
}

TEST(AssertionFail, BooleanExpectNE_ShouldFail)
{
    EXPECT_NE(true, true);
}

TEST(AssertionFail, BooleanAssertNE_ShouldFail)
{
    ASSERT_NE(true, true);
}

// ---- signed int

TEST(AssertionFail, SignedExpectEQ_ShouldFail)
{
    EXPECT_EQ(1, 0);
}

TEST(AssertionFail, SignedAssertEQ_ShouldFail)
{
    ASSERT_EQ(1, 0);
}

TEST(AssertionFail, SignedExpectNE_ShouldFail)
{
    EXPECT_NE(1, 1);
}

TEST(AssertionFail, SignedAssertNE_ShouldFail)
{
    ASSERT_NE(1, 1);
}

TEST(AssertionFail, SignedExpectLT_GT_ShouldFail)
{
    EXPECT_LT(1, 0);
}

TEST(AssertionFail, SignedExpectLT_EQ_ShouldFail)
{
    EXPECT_LT(1, 1);
}

TEST(AssertionFail, SignedAssertLT_GT_ShouldFail)
{
    ASSERT_LT(1, 0);
}

TEST(AssertionFail, SignedAssertLT_EQ_ShouldFail)
{
    ASSERT_LT(1, 1);
}

TEST(AssertionFail, SignedExpectLE_ShouldFail)
{
    EXPECT_LE(1, 0);
}

TEST(AssertionFail, SignedAssertLE_ShouldFail)
{
    ASSERT_LE(1, 0);
}

TEST(AssertionFail, SignedExpectGT_LT_ShouldFail)
{
    EXPECT_GT(0, 1);
}

TEST(AssertionFail, SignedExpectGT_EQ_ShouldFail)
{
    EXPECT_GT(1, 1);
}

TEST(AssertionFail, SignedAssertGT_LT_ShouldFail)
{
    ASSERT_GT(0, 1);
}

TEST(AssertionFail, SignedAssertGT_EQ_ShouldFail)
{
    ASSERT_GT(1, 1);
}

TEST(AssertionFail, SignedExpectGE_ShouldFail)
{
    EXPECT_GE(0, 1);
}

TEST(AssertionFail, SignedAssertGE_ShouldFail)
{
    ASSERT_GE(0, 1);
}

// ---- unsigned int

TEST(AssertionFail, UnsignedExpectEQ_ShouldFail)
{
    EXPECT_EQ(1u, 0u);
}

TEST(AssertionFail, UnsignedAssertEQ_ShouldFail)
{
    ASSERT_EQ(1u, 0u);
}

TEST(AssertionFail, UnsignedExpectNE_ShouldFail)
{
    EXPECT_NE(1u, 1u);
}

TEST(AssertionFail, UnsignedAssertNE_ShouldFail)
{
    ASSERT_NE(1u, 1u);
}

TEST(AssertionFail, UnsignedExpectLT_GT_ShouldFail)
{
    EXPECT_LT(1u, 0u);
}

TEST(AssertionFail, UnsignedExpectLT_EQ_ShouldFail)
{
    EXPECT_LT(1u, 1u);
}

TEST(AssertionFail, UnsignedAssertLT_GT_ShouldFail)
{
    ASSERT_LT(1u, 0u);
}

TEST(AssertionFail, UnsignedAssertLT_EQ_ShouldFail)
{
    ASSERT_LT(1u, 1u);
}

TEST(AssertionFail, UnsignedExpectLE_ShouldFail)
{
    EXPECT_LE(1u, 0u);
}

TEST(AssertionFail, UnsignedAssertLE_ShouldFail)
{
    ASSERT_LE(1u, 0u);
}

TEST(AssertionFail, UnsignedExpectGT_LT_ShouldFail)
{
    EXPECT_GT(0u, 1u);
}

TEST(AssertionFail, UnsignedExpectGT_EQ_ShouldFail)
{
    EXPECT_GT(1u, 1u);
}

TEST(AssertionFail, UnsignedAssertGT_LT_ShouldFail)
{
    ASSERT_GT(0u, 1u);
}

TEST(AssertionFail, UnsignedAssertGT_EQ_ShouldFail)
{
    ASSERT_GT(1u, 1u);
}

TEST(AssertionFail, UnsignedExpectGE_ShouldFail)
{
    EXPECT_GE(0u, 1u);
}

TEST(AssertionFail, UnsignedAssertGE_ShouldFail)
{
    ASSERT_GE(0u, 1u);
}
