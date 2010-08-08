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
CxxTest::StaticSuiteDescription suiteDescription_FlashTestSuite( "FlashTestSuite.h", 15, "FlashTestSuite", suite_FlashTestSuite, Tests_FlashTestSuite );

static class TestDescription_FlashTestSuite_testClick : public CxxTest::RealTestDescription {
public:
 TestDescription_FlashTestSuite_testClick() : CxxTest::RealTestDescription( Tests_FlashTestSuite, suiteDescription_FlashTestSuite, 38, "testClick" ) {}
 void runTest() { suite_FlashTestSuite.testClick(); }
} testDescription_FlashTestSuite_testClick;

static class TestDescription_FlashTestSuite_testDoubleClick : public CxxTest::RealTestDescription {
public:
 TestDescription_FlashTestSuite_testDoubleClick() : CxxTest::RealTestDescription( Tests_FlashTestSuite, suiteDescription_FlashTestSuite, 46, "testDoubleClick" ) {}
 void runTest() { suite_FlashTestSuite.testDoubleClick(); }
} testDescription_FlashTestSuite_testDoubleClick;

static class TestDescription_FlashTestSuite_testRightClick : public CxxTest::RealTestDescription {
public:
 TestDescription_FlashTestSuite_testRightClick() : CxxTest::RealTestDescription( Tests_FlashTestSuite, suiteDescription_FlashTestSuite, 53, "testRightClick" ) {}
 void runTest() { suite_FlashTestSuite.testRightClick(); }
} testDescription_FlashTestSuite_testRightClick;

static class TestDescription_FlashTestSuite_testType : public CxxTest::RealTestDescription {
public:
 TestDescription_FlashTestSuite_testType() : CxxTest::RealTestDescription( Tests_FlashTestSuite, suiteDescription_FlashTestSuite, 60, "testType" ) {}
 void runTest() { suite_FlashTestSuite.testType(); }
} testDescription_FlashTestSuite_testType;

static class TestDescription_FlashTestSuite_testDragDrop : public CxxTest::RealTestDescription {
public:
 TestDescription_FlashTestSuite_testDragDrop() : CxxTest::RealTestDescription( Tests_FlashTestSuite, suiteDescription_FlashTestSuite, 67, "testDragDrop" ) {}
 void runTest() { suite_FlashTestSuite.testDragDrop(); }
} testDescription_FlashTestSuite_testDragDrop;

static class TestDescription_FlashTestSuite_testFindAll : public CxxTest::RealTestDescription {
public:
 TestDescription_FlashTestSuite_testFindAll() : CxxTest::RealTestDescription( Tests_FlashTestSuite, suiteDescription_FlashTestSuite, 74, "testFindAll" ) {}
 void runTest() { suite_FlashTestSuite.testFindAll(); }
} testDescription_FlashTestSuite_testFindAll;

#include <cxxtest/Root.cpp>
