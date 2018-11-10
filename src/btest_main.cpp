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

#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <memory>

#include "btest.h"

namespace btest {

/**
 * A RegisteredTest contains the internal setup data for a
 * test, including its name, the name of the suite it belongs to,
 * whether it is enabled or not, etc.
 */
class RegisteredTest
{
  public:
    RegisteredTest(std::string suiteName, std::string testName, TestFactoryBase *factory)
        : m_suiteName(suiteName)
        , m_testName(testName)
        , m_fullName(suiteName + "." + testName)
        , m_factory(factory)
        , m_token(-1)
        , m_enabled(true)
        , m_runstate(NOTRUN)
    { }

    ~RegisteredTest()
    {
        delete m_factory;
    }

    /*
     * Define various property accessors:
     */
    std::string const& suiteName() const { return m_suiteName; }
    std::string const& testName() const  { return m_testName; }
    std::string const& fullName() const  { return m_fullName; }

    void setToken(RegToken token)        { m_token = token; }
    RegToken token() const               { return m_token; }

    void enable()                        { m_enabled = true; }
    void disable()                       { m_enabled = false; }
    bool enabled() const                 { return m_enabled; }

    enum RunState {NOTRUN, PASSED, FAILED};
    void setRunstate(RunState rs)        { m_runstate = rs; }
    RunState runstate() const            { return m_runstate; }

    /**
     * Instantiate a new Test object from the provided test factory.
     */
    Test* makeTest() const               { return m_factory->makeTest(); }

  private:
    std::string      m_suiteName;
    std::string      m_testName;
    std::string      m_fullName;
    TestFactoryBase *m_factory;
    RegToken         m_token;
    bool             m_enabled;

    RunState         m_runstate;
};

/**
 * The TestRegistrar is the central registry of the testsuites
 * and tests, and provides access to the test factories, test
 * states, test control, etc.
 *
 * Tests are registered via the registerTest() framework
 * function, invoked by a BTEST() declaration and handled during
 * static initialization.
 */
class TestRegistrar
{
  public:
    TestRegistrar() {}

    ~TestRegistrar()
    {
        for (size_t i=0; i < m_alltests.size(); ++i)
            delete m_alltests[i];
    }

    /**
     * Return the total number of defined tests.
     */
    size_t getTestCount() const
    {
        return m_alltests.size();
    }

    /**
     * Count and return the number of disabled tests.
     */
    size_t getDisabledTestCount() const
    {
        size_t count = 0ul;
        for (size_t i=0; i < m_alltests.size(); ++i)
        {
            if (! m_alltests[i]->enabled())
                count++;
        }
        return count;
    }

    /**
     * Count and return the number of failed tests.
     */
    size_t getFailedTestCount() const
    {
        size_t count = 0ul;
        for (size_t i=0; i < m_alltests.size(); ++i)
        {
            if (m_alltests[i]->runstate() == RegisteredTest::FAILED)
                count++;
        }
        return count;
    }

    /**
     * Report the test names that failed
     */
    void reportFailedTests() const
    {
        std::cout << "[--------]" << std::endl;
        for (size_t i=0; i < m_alltests.size(); ++i)
        {
            if (m_alltests[i]->runstate() == RegisteredTest::FAILED)
                std::cout << "[FAILED  ] " << m_alltests[i]->fullName() << std::endl;
        }
        std::cout << "[--------]" << std::endl;
    }

    /**
     * Instantiate and run the test at index \c which.
     */
    void runTest(size_t which)
    {
        if (which >= m_alltests.size())
            return;

        RegisteredTest *rt = m_alltests[which];

        /*
         * Only run if enabled. If disabled, just ignore it.
         */
        if (rt->enabled())
        {
            std::cout << "[--------] " << rt->fullName() << std::endl;
            std::cout << "[running ]" << std::endl;
            Test *testInstance = rt->makeTest();

            rt->setRunstate(RegisteredTest::PASSED);

            /*
             * Test instance lifetime: ctor,SetUp,TestBody,TearDown,dtor
             */
            testInstance->SetUp();
            try {
                testInstance->TestBody();
            }
            catch (std::exception &e)
            {
                rt->setRunstate(RegisteredTest::FAILED);
                std::cout << "[EXCEPTED] Exception: " << e.what() << std::endl;
            }
            testInstance->TearDown();
            delete testInstance;

            /*
             * Report final test state
             */
            switch (rt->runstate()) {
                case RegisteredTest::PASSED:
                    std::cout << "[PASSED  ] " << rt->fullName() << std::endl;
                    break;
                case RegisteredTest::FAILED:
                    std::cout << "[FAILED  ] " << rt->fullName() << std::endl;
                    break;
                default:
                    std::cout << "[UNKNOWN ] " << rt->fullName() << std::endl;
            }
        }
    }

    /*
     * Given the suitename, testname, and test class factory,
     * register a new test into the framework.
     * Return the test's token on exit.
     */
    RegToken registerTest(std::string suite, std::string name, TestFactoryBase *factory)
    {
        // Create the test, enable it, and assign its token
        RegisteredTest *rt = new RegisteredTest(suite, name, factory);
        rt->enable();
        rt->setToken( static_cast<RegToken>(m_alltests.size()) );

        // Disable the test if named DISABLED_restOfTestName
        if (name.substr(0,9) == std::string("DISABLED_"))
            rt->disable();

        // Save it in our list and return the token
        m_alltests.push_back(rt);
        return rt->token();
    }

    /*
     * Record that a test has failed at least one condition.
     */
    void recordTestFailure(RegToken token)
    {
        size_t which = static_cast<size_t>(token);

        if (which >= m_alltests.size())
            return;                         // TODO: report or throw here

        m_alltests[which]->setRunstate(RegisteredTest::FAILED);
    }

  private:
    std::vector<RegisteredTest*> m_alltests; // just a flat list to start
};

static std::unique_ptr<TestRegistrar> s_testRegistrar;

/*
 * ::registerTest() is a forwarding function to the actual
 * registrar object. Since we don't have total control over
 * static initialization order due to linkage ordering, this
 * function does two things: it ensures the registrar object is
 * created by explicitly creating one, then forwards the test
 * registration to it. This way, linkage order doesn't matter.
 */
RegToken registerTest(char const *suitename, char const *testname, TestFactoryBase *factory)
{
    if (suitename == nullptr || testname == nullptr || factory == nullptr)
    {
        throw std::exception();
    }

    if (s_testRegistrar == nullptr)
    {
        std::unique_ptr<TestRegistrar> utr(new TestRegistrar());
        s_testRegistrar = std::move(utr);
    }

    return s_testRegistrar->registerTest(suitename, testname, factory);
}

/**
 * This function records that a test fails. When a test is started,
 * it is assumed to pass. At any point, a failure may be detected
 * which is marked by recordTestFailure(). This becomes a sticky state.
 *
 * This is a forwarding function to the registrar singleton.
 */
void recordTestFailure(RegToken token)
{
    if (s_testRegistrar == nullptr)
        return; // TODO: report internal error, throw, or something

    s_testRegistrar->recordTestFailure(token);
}


} // btest::


/**
 * Provide a basic function to run everything
 */
int main(int argc, char **argv)
{
    std::cout
        << "BTEST Starting. " << btest::s_testRegistrar->getTestCount() << " tests to run"
        << std::endl;

    size_t testCount = btest::s_testRegistrar->getTestCount();
    for (size_t i=0ul; i<testCount; ++i)
    {
        btest::s_testRegistrar->runTest(i);
    }

    size_t failedCount = btest::s_testRegistrar->getFailedTestCount();
    size_t disabledCount = btest::s_testRegistrar->getDisabledTestCount();
    size_t passedCount = testCount - disabledCount - failedCount;

    std::cout << "[  DONE  ]" << std::endl;

    if (failedCount > 0)
        btest::s_testRegistrar->reportFailedTests();

    std::cout << "-- Test results --" << std::endl
              << " Total tests: " << testCount << std::endl
              << " Disabled:    " << disabledCount << std::endl
              << " Failed:      " << failedCount << std::endl
              << " Passed:      " << passedCount << std::endl;

    return (failedCount > 0) ? 1 : 0;
}
