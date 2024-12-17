/*
 * Example unit test for the embtest library.
 *
 * Copyright (c) 2018,2024 Brent Burton
 *
 * SDPX-License-Identifier: ISC
 */
#include <stdexcept>
#include "embtest.hpp"

TEST(OtherFails, throwStdException_ShouldFail)
{
    throw std::logic_error("logic error occurred");
}

TEST(OtherFails, ExpectedFailure_ShouldFail)
{
    FAIL() << "This is a forced failure." << std::endl;
}
