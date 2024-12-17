/*
 * Public header for the embtest unit-test library.
 *
 * Copyright (c) 2018,2024 Brent Burton
 *
 * SDPX-License-Identifier: ISC
 */
#pragma once

#include <iostream>
#include <cstdint>

#define EMBTEST_VERSION_MAJOR 1
#define EMBTEST_VERSION_MINOR 2
#define EMBTEST_VERSION_PATCH 0

/*
 * While all declarations in this file are public
 * to some extent, many of them are more implementation
 * details but must be publicly accessible.
 *
 * The "definitely public" types are marked with
 * PUBLIC in their comments.
 */

/**
 * Define the embtest namespace to contain our machinery.
 */
namespace embtest {

/*
 * Version reporting
 *
 * Define a couple stringification macros.
 */
#define S(thing) S2(thing)
#define S2(thing) #thing

constexpr const char *versionString =
    S(EMBTEST_VERSION_MAJOR) "." S(EMBTEST_VERSION_MINOR) "." S(EMBTEST_VERSION_PATCH);

#undef S
#undef S2

/**
 * The Test class is an abstract base for all embtest test
 * classes and test fixtures. A embtest client needs to
 * know the lifecycle of a test, and that test instances
 * only exist long enough to run a single test.
 *
 * Lifecycle:
 *  1. test construction
 *  2. ::SetUp() is called
 *  3. ::TestBody() is called. This is the actual test code.
 *  4. ::TearDown() is called
 *  5. destruction
 *
 * As an implementation note, the block of code that represents
 * a test is the body of the ::TestBody() method.
 *
 * PUBLIC
 */
class Test
{
  public:
    Test() {}

    virtual void SetUp() {}       ///< SetUp() is not used for tests, but override for test fixtures
    virtual void TestBody() = 0;  ///< TestBody() contains the test code proper
    virtual void TearDown() {}    ///< TearDown() is not used for tests, but override for test fixtures

    virtual ~Test() {}
};

/**
 * TestFactoryBase is an abstract base class that provides
 * a simple interface: a method to create instances of a
 * embtest::Test.
 *
 * IMPLEMENTATION DETAIL
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
 *
 * IMPLEMENTATION DETAIL
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
 *
 * IMPLEMENTATION DETAIL
 */
typedef int RegToken;

/**
 * Internal function registerTest() accepts a test's suite name,
 * its own test name, and a factory object for this test type.
 *
 * registerTest() then registers the test with embtest's test
 * registration mechanism, and if all is successful, returns a
 * RegToken for later use.
 *
 * IMPLEMENTATION DETAIL
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
 *
 * IMPLEMENTATION DETAIL
 */
void recordTestFailure(RegToken token);

/**
 * Retrieve the current output stream.
 *
 * PUBLIC
 */
std::ostream& getOutstream();

/**
 * Templatized function to provide consistent error formatting
 * for assertion failures.
 */
template <typename LType, typename RType>
void logConditionFailure(bool asserted,
                        char const* lstr, char const* rstr,
                        LType const& lval, RType const& rval,
                        int line, char const* file, char const* oper)
{
    getOutstream()
        << "Failure: (line " << line << ") " << file << std::endl
        << "       : It is " << (asserted ? "asserted":"expected")
        << " that left " << oper << " right:" << std::endl
        << "   left: " << lstr << " = " << lval << std::endl
        << "  right: " << rstr << " = " << rval << std::endl;
}

/**
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
 *
 * IMPLEMENTATION DETAIL
 */
template <typename LType, typename RType>
bool assert_eq(bool asserted,
               char const* lstr, char const* rstr,
               LType lval, RType rval,
               int line, char const* file, RegToken token)
{
    bool failed = !(lval == rval);
    if (failed)
    {
        logConditionFailure(asserted, lstr, rstr, lval, rval, line, file, "==");
        recordTestFailure(token);
    }
    return !failed;
}

/**
 * See assert_eq().
 * This method, assert_ne(), tests inequality of two operands.
 * Return value is true if the two operands are not equal.
 *
 * IMPLEMENTATION DETAIL
 */
template <typename LType, typename RType>
bool assert_ne(bool asserted,
               char const* lstr, char const* rstr,
               LType lval, RType rval,
               int line, char const* file, RegToken token)
{
    bool failed = !(lval != rval);
    if (failed)
    {
        logConditionFailure(asserted, lstr, rstr, lval, rval, line, file, "!=");
        recordTestFailure(token);
    }
    return !failed;
}

template <typename LType, typename RType>
bool assert_lt(bool asserted,
               char const* lstr, char const* rstr,
               LType lval, RType rval,
               int line, char const* file, RegToken token)
{
    bool failed = !(lval < rval);
    if (failed)
    {
        logConditionFailure(asserted, lstr, rstr, lval, rval, line, file, "<");
        recordTestFailure(token);
    }
    return !failed;
}

template <typename LType, typename RType>
bool assert_le(bool asserted,
               char const* lstr, char const* rstr,
               LType lval, RType rval,
               int line, char const* file, RegToken token)
{
    bool failed = !(lval <= rval);
    if (failed)
    {
        logConditionFailure(asserted, lstr, rstr, lval, rval, line, file, "<=");
        recordTestFailure(token);
    }
    return !failed;
}

template <typename LType, typename RType>
bool assert_gt(bool asserted,
               char const* lstr, char const* rstr,
               LType lval, RType rval,
               int line, char const* file, RegToken token)
{
    bool failed = !(lval > rval);
    if (failed)
    {
        logConditionFailure(asserted, lstr, rstr, lval, rval, line, file, ">");
        recordTestFailure(token);
    }
    return !failed;
}

template <typename LType, typename RType>
bool assert_ge(bool asserted,
               char const* lstr, char const* rstr,
               LType lval, RType rval,
               int line, char const* file, RegToken token)
{
    bool failed = !(lval >= rval);
    if (failed)
    {
        logConditionFailure(asserted, lstr, rstr, lval, rval, line, file, ">=");
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
        logConditionFailure(asserted, lstr, rstr, lval, rval, line, file, "==");
        recordTestFailure(token);
    }
    return !failed;
}
/**
 * Assert that some expression is true
 *
 * IMPLEMENTATION DETAIL
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
        getOutstream()
            << "Failure: (line " << line << ") " << file << std::endl
            << "       : It is " << (asserted ? "asserted":"expected")
            << " that this is true:" << std::endl
            << "   expr: " << lstr << std::endl;
        recordTestFailure(token);
    }
    return !failed;
}

/**
 * Assert that some expression is false
 *
 * IMPLEMENTATION DETAIL
 */
template <typename LType>
bool assert_false(bool asserted,
                 char const* lstr,
                 LType lval,
                 int line, char const* file, RegToken token)
{
    bool failed = !!(lval);
    if (failed)
    {
        getOutstream()
            << "Failure: (line " << line << ") " << file << std::endl
            << "       : It is " << (asserted ? "asserted":"expected")
            << " that this is false:" << std::endl
            << "   expr: " << lstr << std::endl;
        recordTestFailure(token);
    }
    return !failed;
}

/**
 * Force a test failure outside of an assertion.
 *
 * Indicate a test failure by specifying the file and line number,
 * and the test RegToken for bookkeeping.
 *
 * @return Reference to the output stream for trailing messages.
 *
 * @see the @ref FAIL()
 *
 * IMPLEMENTATION DETAIL, use FAIL() instead.
 */
std::ostream& forceFailure(int line, char const* file, RegToken token);

/*============================================
 * Start of truly public functions and macros
 *
 * PUBLIC
 */

/**
 * Provide a basic function to run all tests, and
 * print a final summary of the number of passed,
 * failed, and disabled tests.
 *
 * @param[in] out The output stream to write progress and summary information.
 * @returns Integer suitable for an exit code (0=success, 1=error)
 *
 * PUBLIC
 */
int runAndReport(std::ostream &out);

} // embtest::

/*
 * Perform a few sanity checks to ensure names
 * from this library are not already declared:
 */
#if defined(TEST_CLASS_NAME)
#error TEST_CLASS_NAME macro already defined
#endif
#if defined(TEST)
#error TEST macro already defined
#endif
#if defined(TEST_F)
#error TEST_F macro already defined
#endif

/**
 * The TEST_CLASS_NAME(suite,test) macro provides
 * a standard way to name Test classes in embtest.
 * Calling this macro (a,b) yields symbol a_b_Test.
 *
 * PUBLIC
 */
#define TEST_CLASS_NAME(suite,test) suite##_##test##_Test

/**
 * Declare a new test with TEST(suitename, testname).
 *
 * This macro defines the test class, registers it with embtest
 * internals, and implements the test body method.
 *
 * This macro should not be followed by an immediate semicolon,
 * but rather a {block of code that implements the test}.
 * (Implementation note: the {block of test code} becomes Test::TestBody().)
 *
 * PUBLIC
 */
#define TEST(suitename, testname)                                    \
/* Define test suite class */                                        \
class TEST_CLASS_NAME(suitename,testname): public embtest::Test      \
{                                                                    \
  public:                                                            \
    void TestBody();                                                 \
  private:                                                           \
    static embtest::RegToken s_registrationToken;                    \
};                                                                   \
/* invoke static-initialization registration */                      \
embtest::RegToken TEST_CLASS_NAME(suitename,testname)::s_registrationToken =  \
embtest::registerTest(#suitename, #testname,                         \
new embtest::TestFactory< TEST_CLASS_NAME(suitename,testname) >());  \
/* implement test body as following block */                         \
void TEST_CLASS_NAME(suitename,testname)::TestBody()

/**
 * See documentation for TEST(). TEST_F() declares a new test
 * using a supporting fixture class.
 *
 * As with TEST(), TEST_F() should not be followed by anything
 * except the block of code that implements the test.
 *
 * PUBLIC
 */
#define TEST_F(fixture, testname)                                    \
/* Define test suite class */                                        \
class TEST_CLASS_NAME(fixture,testname): public fixture              \
{                                                                    \
  public:                                                            \
    void TestBody();                                                 \
  private:                                                           \
    static embtest::RegToken s_registrationToken;                    \
};                                                                   \
/* invoke static-initialization registration */                      \
embtest::RegToken TEST_CLASS_NAME(fixture,testname)::s_registrationToken =  \
embtest::registerTest(#fixture, #testname,                           \
new embtest::TestFactory< TEST_CLASS_NAME(fixture,testname) >());    \
/* implement test body as following block */                         \
void TEST_CLASS_NAME(fixture,testname)::TestBody()

/*
 * Assertion types. These evaluate the one or two arguments given the
 * condition, and report errors if the condition is not satisfied.
 * ASSERT_<COND>() macros are fatal errors, that exit the current function.
 * EXPECT_<COND>() macros are nonfatal, and allow the test to continue.
 *
 * Arguments are evaluated once.
 *
 * PUBLIC
 */
#define ASSERT_EQ(left,right) \
    if (!embtest::assert_eq(true, #left, #right, left, right, __LINE__, __FILE__, s_registrationToken)) return
#define EXPECT_EQ(left,right) \
    (void)embtest::assert_eq(false, #left, #right, left, right, __LINE__, __FILE__, s_registrationToken)

#define ASSERT_NE(left,right) \
    if (!embtest::assert_ne(true, #left, #right, left, right, __LINE__, __FILE__, s_registrationToken)) return
#define EXPECT_NE(left,right) \
    (void)embtest::assert_ne(false, #left, #right, left, right, __LINE__, __FILE__, s_registrationToken)

#define ASSERT_LT(left,right) \
    if (!embtest::assert_lt(true, #left, #right, left, right, __LINE__, __FILE__, s_registrationToken)) return
#define EXPECT_LT(left,right) \
    (void)embtest::assert_lt(false, #left, #right, left, right, __LINE__, __FILE__, s_registrationToken)

#define ASSERT_LE(left,right) \
    if (!embtest::assert_le(true, #left, #right, left, right, __LINE__, __FILE__, s_registrationToken)) return
#define EXPECT_LE(left,right) \
    (void)embtest::assert_le(false, #left, #right, left, right, __LINE__, __FILE__, s_registrationToken)

#define ASSERT_GT(left,right) \
    if (!embtest::assert_gt(true, #left, #right, left, right, __LINE__, __FILE__, s_registrationToken)) return
#define EXPECT_GT(left,right) \
    (void)embtest::assert_gt(false, #left, #right, left, right, __LINE__, __FILE__, s_registrationToken)

#define ASSERT_GE(left,right) \
    if (!embtest::assert_ge(true, #left, #right, left, right, __LINE__, __FILE__, s_registrationToken)) return
#define EXPECT_GE(left,right) \
    (void)embtest::assert_ge(false, #left, #right, left, right, __LINE__, __FILE__, s_registrationToken)

#define ASSERT_TRUE(expr) \
    if (!embtest::assert_true(true, #expr, expr, __LINE__, __FILE__, s_registrationToken)) return
#define EXPECT_TRUE(expr) \
    (void)embtest::assert_true(true, #expr, expr, __LINE__, __FILE__, s_registrationToken)

#define ASSERT_FALSE(expr) \
    if (!embtest::assert_false(true, #expr, expr, __LINE__, __FILE__, s_registrationToken)) return
#define EXPECT_FALSE(expr) \
    (void)embtest::assert_false(true, #expr, expr, __LINE__, __FILE__, s_registrationToken)

#define ASSERT_FPEQ(left,right,eps) \
    if (!embtest::assert_fpeq(true, #left, #right, left, right, eps, __LINE__, __FILE__, s_registrationToken)) return

/**
 * Force a test failure. If a condition cannot be cleanly
 * detected by an ASSERT* or EXPECT* macro, provide the
 * FAIL() mechanism to report a failure after detection.
 *
 * Note: FAIL() returns a reference to a
 * `std::ostream`-compatible type so a message can be
 * reported as test output. E.g.:
 *
 *     FAIL() << "World Domination Failed" << std::endl;
 *
 * A line ending may be used like above; the embtest library will not
 * append a newline.
 *
 * PUBLIC
 */
#define FAIL() \
    embtest::forceFailure(__LINE__, __FILE__, s_registrationToken)

