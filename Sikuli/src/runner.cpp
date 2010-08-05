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
#include "TempTestSuite.h"

static TempTestSuite suite_TempTestSuite;

static CxxTest::List Tests_TempTestSuite = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_TempTestSuite( "TempTestSuite.h", 16, "TempTestSuite", suite_TempTestSuite, Tests_TempTestSuite );

static class TestDescription_TempTestSuite_testFindTopMost : public CxxTest::RealTestDescription {
public:
 TestDescription_TempTestSuite_testFindTopMost() : CxxTest::RealTestDescription( Tests_TempTestSuite, suiteDescription_TempTestSuite, 25, "testFindTopMost" ) {}
 void runTest() { suite_TempTestSuite.testFindTopMost(); }
} testDescription_TempTestSuite_testFindTopMost;

#include <cxxtest/Root.cpp>
