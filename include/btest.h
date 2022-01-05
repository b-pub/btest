/*
 * ISC License
 *
 * Copyright (c) 2018 Brent Burton
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */
#ifndef BTEST_H
#define BTEST_H

#include <iostream>
#include <cstdint>

namespace btest {

/**
 * The Test class is an abstract base for all btest test
 * classes. A btest client needs to know the lifetime of
 * a test, and that test instances only exist to run a
 * single test.
 *
 * Lifetime:
 *  1. test construction
 *  2. ::SetUp() is called
 *  3. ::TestBody() is called. This is the actual test code.
 *  4. ::TearDown() is called
 *  5. destruction
 *
 * As an implementation note, the block of code that represents
 * a test is the body of the ::TestBody() method.
 */
class Test
{
  public:
    Test() {}

    virtual void SetUp() {}
    virtual void TestBody() = 0;
    virtual void TearDown() {}

    virtual ~Test() {}
};

/**
 * TestFactoryBase is an abstract base class that provides
 * a simple interface: a method to create instances of a
 * btest::Test.
 */
class TestFactoryBase
{
  public:
    /**
     * Destructor must be public and virtual.
     */
    virtual ~TestFactoryBase() {}

    /**
     * To get a new Test instance, call makeTest().
     */
    virtual Test* makeTest() = 0;

  protected:
    /**
     * Construction is only possible by subclasses.
     */
    TestFactoryBase() {}
};

/**
 * The TestFactory template class knows how to make one
 * type of Test. In fact, this is all it does.
 */
template <typename T>
class TestFactory : public TestFactoryBase
{
  public:
    virtual Test* makeTest() { return new T(); }
};

/**
 * A RegToken is essentially a handle into the internal
 * storage of tests and their registrations.
 */
typedef int RegToken;

/**
 * Internal function registerTest() accepts a test's suite name,
 * its own test name, and a factory object for this test type.
 *
 * registerTest() then registers the test with btest's test
 * registration mechanism, and if all is successful, returns a
 * RegToken for later use.
 */
RegToken registerTest(char const *suitename,
                      char const *testname,
                      TestFactoryBase *factory);
/**
 * Internally, tests are assumed to pass. Once a test condition
 * fails, the test is marked as failing.
 *
 * recordTestFailure() uses the RegToken from registerTest() to
 * note a test has failed.
 */
void recordTestFailure(RegToken token);

/**
 * Implementation detail.
 *
 * Templatized function assert_eq() provides a safe workspace
 * to evaluate equality of two operands (left and right).
 *
 * Since the left and right expressions are written as parameters
 * in the caller, they are guaranteed to be invoked once, while
 * maintaining any side effects they may have. Only their values
 * are compared here.
 *
 * If the operands are inequal, the error is reported and the
 * test is marked as failing.
 *
 * Return value is true if the two operands are equal.
 */
template <typename LType, typename RType>
bool assert_eq(bool asserted,
               char const* lstr, char const* rstr,
               LType lval, RType rval,
               int line, char const* file, RegToken token)
{
    bool failed = (lval != rval);
    if (failed)
    {
        std::cout
            << "Failure: (line " << line << ") " << file << std::endl
            << "       : It is " << (asserted ? "asserted":"expected")
            << " that these are equal:" << std::endl
            << "   left: " << lstr << std::endl
            << "  right: " << rstr << std::endl;
        recordTestFailure(token);
    }
    return !failed;
}

template <typename LType, typename RType>
bool assert_fpeq(bool asserted,
                 char const* lstr, char const* rstr,
                 LType lval, RType rval, float eps,
                 int line, char const* file, RegToken token)
{
    bool failed = !(std::abs(lval - rval) < eps);
    if (failed)
    {
        std::cout
            << "Failure: (line " << line << ") " << file << std::endl
            << "       : It is " << (asserted ? "asserted":"expected")
            << " that these are equal:" << std::endl
            << "   left: " << lstr << std::endl
            << "  right: " << rstr << std::endl;
        recordTestFailure(token);
    }
    return !failed;
}

/**
 * See assert_eq().
 * This method, assert_ne(), tests inequality of two operands.
 * Return value is true if the two operands are not equal.
 */
template <typename LType, typename RType>
bool assert_ne(bool asserted,
               char const* lstr, char const* rstr,
               LType lval, RType rval,
               int line, char const* file, RegToken token)
{
    bool failed = (lval == rval);
    if (failed)
    {
        std::cout
            << "Failure: (line " << line << ") " << file << std::endl
            << "       : It is " << (asserted ? "asserted":"expected")
            << " that these are not equal:" << std::endl
            << "   left: " << lstr << std::endl
            << "  right: " << rstr << std::endl;
        recordTestFailure(token);
    }
    return !failed;
}

/**
 * Assert that some expression is true
 */
template <typename LType>
bool assert_true(bool asserted,
                 char const* lstr,
                 LType lval,
                 int line, char const* file, RegToken token)
{
    bool failed = !(lval);
    if (failed)
    {
        std::cout
            << "Failure: (line " << line << ") " << file << std::endl
            << "       : It is " << (asserted ? "asserted":"expected")
            << " that this is true:" << std::endl
            << "   expr: " << lstr << std::endl;
        recordTestFailure(token);
    }
    return !failed;
}

/**
 * Force a test failure outside of an assertion.
 */
std::ostream& forceFailure(int line, char const* file, RegToken token);

} // btest::

/**
 * The BTEST_CLASS_NAME(suite,test) macro provides
 * a standard way to name Test classes in btest.
 * Calling this macro (a,b) yields symbol a_b_Test.
 */
#define BTEST_CLASS_NAME(suite,test) suite##_##test##_Test

/**
 * Declare a new test with BTEST(suitename, testname).
 *
 * This macro defines the test class, registers it with btest
 * internals, and implements the test body.
 *
 * This macro should not be followed by an immediate semicolon,
 * but rather a {block of code that implements the test}
 */
#define BTEST(suitename, testname)                                   \
/* Define test suite class */                                        \
class BTEST_CLASS_NAME(suitename,testname): public btest::Test       \
{                                                                    \
  public:                                                            \
    virtual void TestBody();                                         \
  private:                                                           \
    static btest::RegToken s_registrationToken;                      \
};                                                                   \
/* invoke static-initialization registration */                      \
btest::RegToken BTEST_CLASS_NAME(suitename,testname)::s_registrationToken =  \
btest::registerTest(#suitename, #testname,                           \
new btest::TestFactory< BTEST_CLASS_NAME(suitename,testname) >());   \
/* implement test body as following block */                         \
void BTEST_CLASS_NAME(suitename,testname)::TestBody()

/**
 * See documentation for BTEST(). BTEST_F() declares a new test
 * using a supporting fixture class.
 *
 * As with BTEST(), BTEST_F() should not be followed by anything
 * except the block of code that implements the test.
 */
#define BTEST_F(fixture, testname)                                   \
/* Define test suite class */                                        \
class BTEST_CLASS_NAME(fixture,testname): public fixture             \
{                                                                    \
  public:                                                            \
    virtual void TestBody();                                         \
  private:                                                           \
    static btest::RegToken s_registrationToken;                      \
};                                                                   \
/* invoke static-initialization registration */                      \
btest::RegToken BTEST_CLASS_NAME(fixture,testname)::s_registrationToken =  \
btest::registerTest(#fixture, #testname,                             \
new btest::TestFactory< BTEST_CLASS_NAME(fixture,testname) >());     \
/* implement test body as following block */                         \
void BTEST_CLASS_NAME(fixture,testname)::TestBody()


/*
 * Assertion types. These evaluate the one or two arguments given the
 * condition, and report errors if the condition is not satisfied.
 * ASSERT_<COND>() macros are fatal errors, that exit the current function.
 * EXPECT_<COND>() macros are nonfatal, and allow the test to continue.
 *
 * Arguments are evaluated once
 */
#define ASSERT_EQ(left,right) \
    if (!btest::assert_eq(true, #left, #right, left, right, __LINE__, __FILE__, s_registrationToken)) return
#define ASSERT_NE(left,right) \
    if (!btest::assert_ne(true, #left, #right, left, right, __LINE__, __FILE__, s_registrationToken)) return
#define EXPECT_EQ(left,right) \
    (void)btest::assert_eq(false, #left, #right, left, right, __LINE__, __FILE__, s_registrationToken)
#define EXPECT_NE(left,right) \
    (void)btest::assert_ne(false, #left, #right, left, right, __LINE__, __FILE__, s_registrationToken)
#define ASSERT_TRUE(expr) \
    if (!btest::assert_true(true, #expr, expr, __LINE__, __FILE__, s_registrationToken)) return

#define ASSERT_FPEQ(left,right,eps)                                        \
    if (!btest::assert_fpeq(true, #left, #right, left, right, eps, __LINE__, __FILE__, s_registrationToken)) return

#define FAIL() \
    btest::forceFailure(__LINE__, __FILE__, s_registrationToken)

#endif // BTEST_H
