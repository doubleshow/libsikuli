/* Generated file, do not edit */

#ifndef CXXTEST_RUNNING
#define CXXTEST_RUNNING
#endif

#define _CXXTEST_HAVE_STD
#define _CXXTEST_HAVE_EH
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
CxxTest::StaticSuiteDescription suiteDescription_FlashTestSuite( "FlashTestSuite.h", 50, "FlashTestSuite", suite_FlashTestSuite, Tests_FlashTestSuite );

static class TestDescription_FlashTestSuite_testBejeweled : public CxxTest::RealTestDescription {
public:
 TestDescription_FlashTestSuite_testBejeweled() : CxxTest::RealTestDescription( Tests_FlashTestSuite, suiteDescription_FlashTestSuite, 273, "testBejeweled" ) {}
 void runTest() { suite_FlashTestSuite.testBejeweled(); }
} testDescription_FlashTestSuite_testBejeweled;

#include <cxxtest/Root.cpp>
