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

static class TestDescription_RegionTestSuite_testAddImagePaths : public CxxTest::RealTestDescription {
public:
 TestDescription_RegionTestSuite_testAddImagePaths() : CxxTest::RealTestDescription( Tests_RegionTestSuite, suiteDescription_RegionTestSuite, 31, "testAddImagePaths" ) {}
 void runTest() { suite_RegionTestSuite.testAddImagePaths(); }
} testDescription_RegionTestSuite_testAddImagePaths;

static class TestDescription_RegionTestSuite_testCreation : public CxxTest::RealTestDescription {
public:
 TestDescription_RegionTestSuite_testCreation() : CxxTest::RealTestDescription( Tests_RegionTestSuite, suiteDescription_RegionTestSuite, 47, "testCreation" ) {}
 void runTest() { suite_RegionTestSuite.testCreation(); }
} testDescription_RegionTestSuite_testCreation;

static class TestDescription_RegionTestSuite_testExists : public CxxTest::RealTestDescription {
public:
 TestDescription_RegionTestSuite_testExists() : CxxTest::RealTestDescription( Tests_RegionTestSuite, suiteDescription_RegionTestSuite, 53, "testExists" ) {}
 void runTest() { suite_RegionTestSuite.testExists(); }
} testDescription_RegionTestSuite_testExists;

static class TestDescription_RegionTestSuite_testModifiers : public CxxTest::RealTestDescription {
public:
 TestDescription_RegionTestSuite_testModifiers() : CxxTest::RealTestDescription( Tests_RegionTestSuite, suiteDescription_RegionTestSuite, 60, "testModifiers" ) {}
 void runTest() { suite_RegionTestSuite.testModifiers(); }
} testDescription_RegionTestSuite_testModifiers;

static class TestDescription_RegionTestSuite_testESC : public CxxTest::RealTestDescription {
public:
 TestDescription_RegionTestSuite_testESC() : CxxTest::RealTestDescription( Tests_RegionTestSuite, suiteDescription_RegionTestSuite, 68, "testESC" ) {}
 void runTest() { suite_RegionTestSuite.testESC(); }
} testDescription_RegionTestSuite_testESC;

static class TestDescription_RegionTestSuite_testClick : public CxxTest::RealTestDescription {
public:
 TestDescription_RegionTestSuite_testClick() : CxxTest::RealTestDescription( Tests_RegionTestSuite, suiteDescription_RegionTestSuite, 76, "testClick" ) {}
 void runTest() { suite_RegionTestSuite.testClick(); }
} testDescription_RegionTestSuite_testClick;

static class TestDescription_RegionTestSuite_testHoverAndClick : public CxxTest::RealTestDescription {
public:
 TestDescription_RegionTestSuite_testHoverAndClick() : CxxTest::RealTestDescription( Tests_RegionTestSuite, suiteDescription_RegionTestSuite, 84, "testHoverAndClick" ) {}
 void runTest() { suite_RegionTestSuite.testHoverAndClick(); }
} testDescription_RegionTestSuite_testHoverAndClick;

static class TestDescription_RegionTestSuite_testType : public CxxTest::RealTestDescription {
public:
 TestDescription_RegionTestSuite_testType() : CxxTest::RealTestDescription( Tests_RegionTestSuite, suiteDescription_RegionTestSuite, 95, "testType" ) {}
 void runTest() { suite_RegionTestSuite.testType(); }
} testDescription_RegionTestSuite_testType;

static class TestDescription_RegionTestSuite_testTrackPadWaitForToolTip : public CxxTest::RealTestDescription {
public:
 TestDescription_RegionTestSuite_testTrackPadWaitForToolTip() : CxxTest::RealTestDescription( Tests_RegionTestSuite, suiteDescription_RegionTestSuite, 106, "testTrackPadWaitForToolTip" ) {}
 void runTest() { suite_RegionTestSuite.testTrackPadWaitForToolTip(); }
} testDescription_RegionTestSuite_testTrackPadWaitForToolTip;

static class TestDescription_RegionTestSuite_testDragDrop : public CxxTest::RealTestDescription {
public:
 TestDescription_RegionTestSuite_testDragDrop() : CxxTest::RealTestDescription( Tests_RegionTestSuite, suiteDescription_RegionTestSuite, 117, "testDragDrop" ) {}
 void runTest() { suite_RegionTestSuite.testDragDrop(); }
} testDescription_RegionTestSuite_testDragDrop;

static class TestDescription_RegionTestSuite_testOpenApp : public CxxTest::RealTestDescription {
public:
 TestDescription_RegionTestSuite_testOpenApp() : CxxTest::RealTestDescription( Tests_RegionTestSuite, suiteDescription_RegionTestSuite, 152, "testOpenApp" ) {}
 void runTest() { suite_RegionTestSuite.testOpenApp(); }
} testDescription_RegionTestSuite_testOpenApp;

static class TestDescription_RegionTestSuite_testFindAll : public CxxTest::RealTestDescription {
public:
 TestDescription_RegionTestSuite_testFindAll() : CxxTest::RealTestDescription( Tests_RegionTestSuite, suiteDescription_RegionTestSuite, 160, "testFindAll" ) {}
 void runTest() { suite_RegionTestSuite.testFindAll(); }
} testDescription_RegionTestSuite_testFindAll;

static class TestDescription_RegionTestSuite_testOCR : public CxxTest::RealTestDescription {
public:
 TestDescription_RegionTestSuite_testOCR() : CxxTest::RealTestDescription( Tests_RegionTestSuite, suiteDescription_RegionTestSuite, 174, "testOCR" ) {}
 void runTest() { suite_RegionTestSuite.testOCR(); }
} testDescription_RegionTestSuite_testOCR;

static class TestDescription_RegionTestSuite_testTargetOffset : public CxxTest::RealTestDescription {
public:
 TestDescription_RegionTestSuite_testTargetOffset() : CxxTest::RealTestDescription( Tests_RegionTestSuite, suiteDescription_RegionTestSuite, 187, "testTargetOffset" ) {}
 void runTest() { suite_RegionTestSuite.testTargetOffset(); }
} testDescription_RegionTestSuite_testTargetOffset;

#include <cxxtest/Root.cpp>
