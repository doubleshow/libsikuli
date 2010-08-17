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
#include "FlashTestSuite.h"

static FlashTestSuite suite_FlashTestSuite;

static CxxTest::List Tests_FlashTestSuite = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_FlashTestSuite( "FlashTestSuite.h", 44, "FlashTestSuite", suite_FlashTestSuite, Tests_FlashTestSuite );

static class TestDescription_FlashTestSuite_testPaste : public CxxTest::RealTestDescription {
public:
 TestDescription_FlashTestSuite_testPaste() : CxxTest::RealTestDescription( Tests_FlashTestSuite, suiteDescription_FlashTestSuite, 211, "testPaste" ) {}
 void runTest() { suite_FlashTestSuite.testPaste(); }
} testDescription_FlashTestSuite_testPaste;

#include <cxxtest/Root.cpp>
