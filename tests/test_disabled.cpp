/*
 * Example disabled unit test for the embtest library.
 *
 * Copyright (c) 2018,2024 Brent Burton
 *
 * SDPX-License-Identifier: ISC
 */
#include "embtest.hpp"

TEST(DisabledTest, DISABLED_testDisabled)
{
    // This will crash if not disabled correctly (on some platforms)
    *((volatile int*)0) = 42;
}
