/* Generated file, do not edit */

#ifndef CXXTEST_RUNNING
#define CXXTEST_RUNNING
#endif

#include <cxxtest/TestListener.h>
#include <cxxtest/TestTracker.h>
#include <cxxtest/TestRunner.h>
#include <cxxtest/RealDescriptions.h>

#include "cvgui_test.h"

static CVGUITestSuite suite_CVGUITestSuite;

static CxxTest::List Tests_CVGUITestSuite = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_CVGUITestSuite( "cvgui_test.h", 15, "CVGUITestSuite", suite_CVGUITestSuite, Tests_CVGUITestSuite );

static class TestDescription_CVGUITestSuite_testSegmentation : public CxxTest::RealTestDescription {
public:
 TestDescription_CVGUITestSuite_testSegmentation() : CxxTest::RealTestDescription( Tests_CVGUITestSuite, suiteDescription_CVGUITestSuite, 23, "testSegmentation" ) {}
 void runTest() { suite_CVGUITestSuite.testSegmentation(); }
} testDescription_CVGUITestSuite_testSegmentation;

static class TestDescription_CVGUITestSuite_testIndexImages : public CxxTest::RealTestDescription {
public:
 TestDescription_CVGUITestSuite_testIndexImages() : CxxTest::RealTestDescription( Tests_CVGUITestSuite, suiteDescription_CVGUITestSuite, 31, "testIndexImages" ) {}
 void runTest() { suite_CVGUITestSuite.testIndexImages(); }
} testDescription_CVGUITestSuite_testIndexImages;

static class TestDescription_CVGUITestSuite_testReadWriteImageRecord : public CxxTest::RealTestDescription {
public:
 TestDescription_CVGUITestSuite_testReadWriteImageRecord() : CxxTest::RealTestDescription( Tests_CVGUITestSuite, suiteDescription_CVGUITestSuite, 39, "testReadWriteImageRecord" ) {}
 void runTest() { suite_CVGUITestSuite.testReadWriteImageRecord(); }
} testDescription_CVGUITestSuite_testReadWriteImageRecord;

