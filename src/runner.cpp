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
CxxTest::StaticSuiteDescription suiteDescription_FlashTestSuite( "FlashTestSuite.h", 35, "FlashTestSuite", suite_FlashTestSuite, Tests_FlashTestSuite );

static class TestDescription_FlashTestSuite_testClick : public CxxTest::RealTestDescription {
public:
 TestDescription_FlashTestSuite_testClick() : CxxTest::RealTestDescription( Tests_FlashTestSuite, suiteDescription_FlashTestSuite, 58, "testClick" ) {}
 void runTest() { suite_FlashTestSuite.testClick(); }
} testDescription_FlashTestSuite_testClick;

static class TestDescription_FlashTestSuite_testDoubleClick : public CxxTest::RealTestDescription {
public:
 TestDescription_FlashTestSuite_testDoubleClick() : CxxTest::RealTestDescription( Tests_FlashTestSuite, suiteDescription_FlashTestSuite, 66, "testDoubleClick" ) {}
 void runTest() { suite_FlashTestSuite.testDoubleClick(); }
} testDescription_FlashTestSuite_testDoubleClick;

static class TestDescription_FlashTestSuite_testRightClick : public CxxTest::RealTestDescription {
public:
 TestDescription_FlashTestSuite_testRightClick() : CxxTest::RealTestDescription( Tests_FlashTestSuite, suiteDescription_FlashTestSuite, 73, "testRightClick" ) {}
 void runTest() { suite_FlashTestSuite.testRightClick(); }
} testDescription_FlashTestSuite_testRightClick;

static class TestDescription_FlashTestSuite_testType : public CxxTest::RealTestDescription {
public:
 TestDescription_FlashTestSuite_testType() : CxxTest::RealTestDescription( Tests_FlashTestSuite, suiteDescription_FlashTestSuite, 80, "testType" ) {}
 void runTest() { suite_FlashTestSuite.testType(); }
} testDescription_FlashTestSuite_testType;

static class TestDescription_FlashTestSuite_testKeys : public CxxTest::RealTestDescription {
public:
 TestDescription_FlashTestSuite_testKeys() : CxxTest::RealTestDescription( Tests_FlashTestSuite, suiteDescription_FlashTestSuite, 88, "testKeys" ) {}
 void runTest() { suite_FlashTestSuite.testKeys(); }
} testDescription_FlashTestSuite_testKeys;

static class TestDescription_FlashTestSuite_testOCR : public CxxTest::RealTestDescription {
public:
 TestDescription_FlashTestSuite_testOCR() : CxxTest::RealTestDescription( Tests_FlashTestSuite, suiteDescription_FlashTestSuite, 112, "testOCR" ) {}
 void runTest() { suite_FlashTestSuite.testOCR(); }
} testDescription_FlashTestSuite_testOCR;

static class TestDescription_FlashTestSuite_testDragDrop : public CxxTest::RealTestDescription {
public:
 TestDescription_FlashTestSuite_testDragDrop() : CxxTest::RealTestDescription( Tests_FlashTestSuite, suiteDescription_FlashTestSuite, 124, "testDragDrop" ) {}
 void runTest() { suite_FlashTestSuite.testDragDrop(); }
} testDescription_FlashTestSuite_testDragDrop;

static class TestDescription_FlashTestSuite_testFindAll : public CxxTest::RealTestDescription {
public:
 TestDescription_FlashTestSuite_testFindAll() : CxxTest::RealTestDescription( Tests_FlashTestSuite, suiteDescription_FlashTestSuite, 131, "testFindAll" ) {}
 void runTest() { suite_FlashTestSuite.testFindAll(); }
} testDescription_FlashTestSuite_testFindAll;

static class TestDescription_FlashTestSuite_testSpatialOp : public CxxTest::RealTestDescription {
public:
 TestDescription_FlashTestSuite_testSpatialOp() : CxxTest::RealTestDescription( Tests_FlashTestSuite, suiteDescription_FlashTestSuite, 149, "testSpatialOp" ) {}
 void runTest() { suite_FlashTestSuite.testSpatialOp(); }
} testDescription_FlashTestSuite_testSpatialOp;

static class TestDescription_FlashTestSuite_testOrdering : public CxxTest::RealTestDescription {
public:
 TestDescription_FlashTestSuite_testOrdering() : CxxTest::RealTestDescription( Tests_FlashTestSuite, suiteDescription_FlashTestSuite, 164, "testOrdering" ) {}
 void runTest() { suite_FlashTestSuite.testOrdering(); }
} testDescription_FlashTestSuite_testOrdering;

static class TestDescription_FlashTestSuite_testPaste : public CxxTest::RealTestDescription {
public:
 TestDescription_FlashTestSuite_testPaste() : CxxTest::RealTestDescription( Tests_FlashTestSuite, suiteDescription_FlashTestSuite, 192, "testPaste" ) {}
 void runTest() { suite_FlashTestSuite.testPaste(); }
} testDescription_FlashTestSuite_testPaste;

static class TestDescription_FlashTestSuite_testEvent : public CxxTest::RealTestDescription {
public:
 TestDescription_FlashTestSuite_testEvent() : CxxTest::RealTestDescription( Tests_FlashTestSuite, suiteDescription_FlashTestSuite, 199, "testEvent" ) {}
 void runTest() { suite_FlashTestSuite.testEvent(); }
} testDescription_FlashTestSuite_testEvent;

#include <cxxtest/Root.cpp>
