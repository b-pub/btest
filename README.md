# btest
A simple unit test framework

---

## So what is *btest*?

`Btest` is a small and simple C++ unit test framework that only
implements the basics.

The `btest` library was started out of curiosity about how GoogleTest
worked -- how does one define unit tests easily, with no configuration,
and have simple execution? `Btest` has not only scratched that itch,
but has become useful in its own right.

The internals of `btest` (and GoogleTest, Catch2, etc) must:
* define tests during static initialization
* register tests into a central registry/controller
* provide runtime mechanisms to flag tests' results in a registry
* use abstract classes and object factories to create test instances

## Features

`btest` has a drastically reduced feature set compared to GoogleTest.
However, it implements what are arguably the most important testing
features, listed in this table.

Feature                   | btest | gtest
------------------------- | ----- | -----
basic tests               | yes   | yes
test fixtures             | yes   | yes
DISABLED_ tests           | yes   | yes
Test filtering            | no    | yes
Global environment        | no    | yes
Run order randomization   | no    | yes
Death tests               | no    | yes
Value-parameterized tests | no    | yes
XML or JSON format output | no    | yes

Of these missing features, I'd probably focus on the
following additions next.
1. Run order randomization (including repeats)
2. Test output formatting

Other features of `btest` that are appealing are:
* very small footprint - minimal increase in code size and compilation times
* familiar API semantics for creating tests and fixtures
* `btest` output can be redirected to any `std::ostream` provided by the application
* no assumptions about platform
* no configuration
* intelligible symbols to help with debugging

## Applications of *btest*

As mentioned, `btest` was originally more of a framework for
trying implementation ideas, and not a replacement for
GoogleTest. However, as it developed, I began integrating `btest`
into my own projects because it was such as simple test framework.

`Btest` has been used for:
* C++ development on both Mac OS and Linux (x86_64 and ARM)
* Embedded on-device unit tests on ARM Cortex-M processors

The `btest` library uses only C++11, and makes no assumptions
about execution platform. This, combined with its small footprint,
makes `btest` particularly suited for embedded systems, with the
only requirement is that `std::ostream` is available. Further,
the library creates clear class names from the test suite name and
test name, making inspection via a debugger that much easier.

## Example Usage

A unit test executable often consists of the `main()` entry point
in one compilation unit, and tests defined in possibly several
other files. The included test program in `btest/test`, `test_unittest`,
is an example of usage that includes isolated tests, disabled tests,
and a fixture.

The simplest test program might be the following.

```cpp
#include <iostream>
#include "btest.hpp"

BTEST(Example, trueFalseAssertNE)
{
    ASSERT_NE(true, false);
}

int main(int argc, char **argv)
{
    return btest::runAndReport(std::cout);
}
```

At the start, the `btest.hpp` header is included. The only test
is defined by the `BTEST()` macro with the test code in the block immediately
following. And finally, in `main()` the tests are run, and a final report is
printed to the provided stream, `std::cout`.

The output of this executable is:

```
BTEST Starting. 1 tests to run
[--------] Example.trueFalseAssertNE
[running ]
[ PASSED ] Example.trueFalseAssertNE
[  DONE  ]
-- Test results --
 Total tests: 1
 Disabled:    0
 Failed:      0
 Passed:      1
```

If the `trueFalseAssertNE` test needed to be visited in a debugger,
the `BTEST()` macro creates a straightforward class that would be named
`Example_trueFalseAssertNE_Test`, with the test body in method `::TestBody()`.

## Building btest

`btest` is currently managed with cmake, and uses C++11 for its
implementation. Recent CMake 3.x should be fine. From the
toplevel directory, run these commands:

```sh
$ cd path/to/btest
$ mkdir build && cd build
$ cmake ..
$ make -j
$ ./test_unittests
```

---

Brent - Nov 2018
