/* Generated file, do not edit */

#ifndef CXXTEST_RUNNING
#define CXXTEST_RUNNING
#endif

#define _CXXTEST_HAVE_EH
#include <cxxtest/TestListener.h>
#include <cxxtest/TestTracker.h>
#include <cxxtest/TestRunner.h>
#include <cxxtest/RealDescriptions.h>

#include "CoreTestSuite.h"

static CoreTestSuite suite_CoreTestSuite;

static CxxTest::List Tests_CoreTestSuite = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_CoreTestSuite( "CoreTestSuite.h", 17, "CoreTestSuite", suite_CoreTestSuite, Tests_CoreTestSuite );

static class TestDescription_CoreTestSuite_testFindInterfaces : public CxxTest::RealTestDescription {
public:
 TestDescription_CoreTestSuite_testFindInterfaces() : CxxTest::RealTestDescription( Tests_CoreTestSuite, suiteDescription_CoreTestSuite, 28, "testFindInterfaces" ) {}
 void runTest() { suite_CoreTestSuite.testFindInterfaces(); }
} testDescription_CoreTestSuite_testFindInterfaces;

static class TestDescription_CoreTestSuite_testReadImageFromURL : public CxxTest::RealTestDescription {
public:
 TestDescription_CoreTestSuite_testReadImageFromURL() : CxxTest::RealTestDescription( Tests_CoreTestSuite, suiteDescription_CoreTestSuite, 40, "testReadImageFromURL" ) {}
 void runTest() { suite_CoreTestSuite.testReadImageFromURL(); }
} testDescription_CoreTestSuite_testReadImageFromURL;

