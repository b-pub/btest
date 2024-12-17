/*
 * Test assertions that should pass.
 *
 * Copyright (c) 2018,2024 Brent Burton
 *
 * SDPX-License-Identifier: ISC
 */
#include "embtest.hpp"

// ---- Boolean

TEST(AssertionPass, BooleanExpectEQ)
{
    EXPECT_EQ(true, true);
}

TEST(AssertionPass, BooleanAssertEQ)
{
    ASSERT_EQ(true, true);
}

TEST(AssertionPass, BooleanExpectNE)
{
    EXPECT_NE(true, false);
}

TEST(AssertionPass, BooleanAssertNE)
{
    ASSERT_NE(true, false);
}

// ---- signed int

TEST(AssertionPass, SignedExpectEQ)
{
    EXPECT_EQ(1, 1);
}

TEST(AssertionPass, SignedAssertEQ)
{
    ASSERT_EQ(1, 1);
}

TEST(AssertionPass, SignedExpectNE)
{
    EXPECT_NE(1, 0);
}

TEST(AssertionPass, SignedAssertNE)
{
    ASSERT_NE(1, 0);
}

TEST(AssertionPass, SignedExpectLE_EQ)
{
    EXPECT_LE(0, 0);
}

TEST(AssertionPass, SignedExpectLE_LT)
{
    EXPECT_LE(0, 1);
}

TEST(AssertionPass, SignedAssertLE_EQ)
{
    ASSERT_LE(0, 0);
}

TEST(AssertionPass, SignedAssertLE_LT)
{
    ASSERT_LE(0, 1);
}

TEST(AssertionPass, SignedExpectLT)
{
    EXPECT_LT(0, 1);
}

TEST(AssertionPass, SignedAssertLT)
{
    ASSERT_LT(0, 1);
}

TEST(AssertionPass, SignedExpectGE_EQ)
{
    EXPECT_GE(0, 0);
}

TEST(AssertionPass, SignedExpectGE_GT)
{
    EXPECT_GE(1, 0);
}

TEST(AssertionPass, SignedAssertGE_EQ)
{
    ASSERT_GE(0, 0);
}

TEST(AssertionPass, SignedAssertGE_GT)
{
    ASSERT_GE(1, 0);
}

TEST(AssertionPass, SignedExpectGT)
{
    EXPECT_GT(1, 0);
}

TEST(AssertionPass, SignedAssertGT)
{
    ASSERT_GT(1, 0);
}

// ---- unsigned int

TEST(AssertionPass, UnsignedExpectEQ)
{
    EXPECT_EQ(1u, 1u);
}

TEST(AssertionPass, UnsignedAssertEQ)
{
    ASSERT_EQ(1u, 1u);
}

TEST(AssertionPass, UnsignedExpectNE)
{
    EXPECT_NE(1u, 0u);
}

TEST(AssertionPass, UnsignedAssertNE)
{
    ASSERT_NE(1u, 0u);
}

TEST(AssertionPass, UnsignedExpectLE_EQ)
{
    EXPECT_LE(0u, 0u);
}

TEST(AssertionPass, UnsignedExpectLE_LT)
{
    EXPECT_LE(0u, 1u);
}

TEST(AssertionPass, UnsignedAssertLE_EQ)
{
    ASSERT_LE(0u, 0u);
}

TEST(AssertionPass, UnsignedAssertLE_LT)
{
    ASSERT_LE(0u, 1u);
}

TEST(AssertionPass, UnsignedExpectLT)
{
    EXPECT_LT(0u, 1u);
}

TEST(AssertionPass, UnsignedAssertLT)
{
    ASSERT_LT(0u, 1u);
}

TEST(AssertionPass, UnsignedExpectGE_EQ)
{
    EXPECT_GE(0u, 0u);
}

TEST(AssertionPass, UnsignedExpectGE_GT)
{
    EXPECT_GE(1u, 0u);
}

TEST(AssertionPass, UnsignedAssertGE_EQ)
{
    ASSERT_GE(0u, 0u);
}

TEST(AssertionPass, UnsignedAssertGE_GT)
{
    ASSERT_GE(1u, 0u);
}

TEST(AssertionPass, UnsignedExpectGT)
{
    EXPECT_GT(1u, 0u);
}

TEST(AssertionPass, UnsignedAssertGT)
{
    ASSERT_GT(1u, 0u);
}
