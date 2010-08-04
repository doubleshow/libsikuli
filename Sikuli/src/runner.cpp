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
#include "RegionTestSuite.h"

static RegionTestSuite suite_RegionTestSuite;

static CxxTest::List Tests_RegionTestSuite = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_RegionTestSuite( "RegionTestSuite.h", 16, "RegionTestSuite", suite_RegionTestSuite, Tests_RegionTestSuite );

static class TestDescription_RegionTestSuite_testCreation : public CxxTest::RealTestDescription {
public:
 TestDescription_RegionTestSuite_testCreation() : CxxTest::RealTestDescription( Tests_RegionTestSuite, suiteDescription_RegionTestSuite, 26, "testCreation" ) {}
 void runTest() { suite_RegionTestSuite.testCreation(); }
} testDescription_RegionTestSuite_testCreation;

static class TestDescription_RegionTestSuite_testExists : public CxxTest::RealTestDescription {
public:
 TestDescription_RegionTestSuite_testExists() : CxxTest::RealTestDescription( Tests_RegionTestSuite, suiteDescription_RegionTestSuite, 32, "testExists" ) {}
 void runTest() { suite_RegionTestSuite.testExists(); }
} testDescription_RegionTestSuite_testExists;

static class TestDescription_RegionTestSuite_testModifiers : public CxxTest::RealTestDescription {
public:
 TestDescription_RegionTestSuite_testModifiers() : CxxTest::RealTestDescription( Tests_RegionTestSuite, suiteDescription_RegionTestSuite, 39, "testModifiers" ) {}
 void runTest() { suite_RegionTestSuite.testModifiers(); }
} testDescription_RegionTestSuite_testModifiers;

static class TestDescription_RegionTestSuite_testESC : public CxxTest::RealTestDescription {
public:
 TestDescription_RegionTestSuite_testESC() : CxxTest::RealTestDescription( Tests_RegionTestSuite, suiteDescription_RegionTestSuite, 47, "testESC" ) {}
 void runTest() { suite_RegionTestSuite.testESC(); }
} testDescription_RegionTestSuite_testESC;

static class TestDescription_RegionTestSuite_testClick : public CxxTest::RealTestDescription {
public:
 TestDescription_RegionTestSuite_testClick() : CxxTest::RealTestDescription( Tests_RegionTestSuite, suiteDescription_RegionTestSuite, 55, "testClick" ) {}
 void runTest() { suite_RegionTestSuite.testClick(); }
} testDescription_RegionTestSuite_testClick;

static class TestDescription_RegionTestSuite_testHoverAndClick : public CxxTest::RealTestDescription {
public:
 TestDescription_RegionTestSuite_testHoverAndClick() : CxxTest::RealTestDescription( Tests_RegionTestSuite, suiteDescription_RegionTestSuite, 63, "testHoverAndClick" ) {}
 void runTest() { suite_RegionTestSuite.testHoverAndClick(); }
} testDescription_RegionTestSuite_testHoverAndClick;

static class TestDescription_RegionTestSuite_testType : public CxxTest::RealTestDescription {
public:
 TestDescription_RegionTestSuite_testType() : CxxTest::RealTestDescription( Tests_RegionTestSuite, suiteDescription_RegionTestSuite, 74, "testType" ) {}
 void runTest() { suite_RegionTestSuite.testType(); }
} testDescription_RegionTestSuite_testType;

static class TestDescription_RegionTestSuite_testTrackPadWaitForToolTip : public CxxTest::RealTestDescription {
public:
 TestDescription_RegionTestSuite_testTrackPadWaitForToolTip() : CxxTest::RealTestDescription( Tests_RegionTestSuite, suiteDescription_RegionTestSuite, 85, "testTrackPadWaitForToolTip" ) {}
 void runTest() { suite_RegionTestSuite.testTrackPadWaitForToolTip(); }
} testDescription_RegionTestSuite_testTrackPadWaitForToolTip;

static class TestDescription_RegionTestSuite_testDragDrop : public CxxTest::RealTestDescription {
public:
 TestDescription_RegionTestSuite_testDragDrop() : CxxTest::RealTestDescription( Tests_RegionTestSuite, suiteDescription_RegionTestSuite, 96, "testDragDrop" ) {}
 void runTest() { suite_RegionTestSuite.testDragDrop(); }
} testDescription_RegionTestSuite_testDragDrop;

static class TestDescription_RegionTestSuite_testOpenApp : public CxxTest::RealTestDescription {
public:
 TestDescription_RegionTestSuite_testOpenApp() : CxxTest::RealTestDescription( Tests_RegionTestSuite, suiteDescription_RegionTestSuite, 131, "testOpenApp" ) {}
 void runTest() { suite_RegionTestSuite.testOpenApp(); }
} testDescription_RegionTestSuite_testOpenApp;

#include <cxxtest/Root.cpp>
