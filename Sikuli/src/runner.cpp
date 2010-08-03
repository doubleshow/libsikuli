/* Generated file, do not edit */

#ifndef CXXTEST_RUNNING
#define CXXTEST_RUNNING
#endif

#define _CXXTEST_HAVE_STD
#include <cxxtest/TestListener.h>
#include <cxxtest/TestTracker.h>
#include <cxxtest/TestRunner.h>
#include <cxxtest/RealDescriptions.h>
#include <cxxtest/ErrorPrinter.h>

int main() {
 return CxxTest::ErrorPrinter().run();
}
#include "RegionTestSuite.h"

static RegionTestSuite suite_RegionTestSuite;

static CxxTest::List Tests_RegionTestSuite = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_RegionTestSuite( "RegionTestSuite.h", 14, "RegionTestSuite", suite_RegionTestSuite, Tests_RegionTestSuite );

static class TestDescription_RegionTestSuite_testCreation : public CxxTest::RealTestDescription {
public:
 TestDescription_RegionTestSuite_testCreation() : CxxTest::RealTestDescription( Tests_RegionTestSuite, suiteDescription_RegionTestSuite, 17, "testCreation" ) {}
 void runTest() { suite_RegionTestSuite.testCreation(); }
} testDescription_RegionTestSuite_testCreation;

static class TestDescription_RegionTestSuite_testClick : public CxxTest::RealTestDescription {
public:
 TestDescription_RegionTestSuite_testClick() : CxxTest::RealTestDescription( Tests_RegionTestSuite, suiteDescription_RegionTestSuite, 23, "testClick" ) {}
 void runTest() { suite_RegionTestSuite.testClick(); }
} testDescription_RegionTestSuite_testClick;

#include <cxxtest/Root.cpp>
