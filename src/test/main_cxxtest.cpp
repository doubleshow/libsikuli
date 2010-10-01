/*
 *  main_cxxtest.cpp
 *  sikuli
 *
 *  Created by Tom Yeh on 8/20/10.
 *  Copyright 2010 sikuli.org. All rights reserved.
 *
 */

#include <cxxtest/ErrorPrinter.h>
#include <cxxtest/StdioPrinter.h>

int main() {

   //CxxTest::leaveOnly("CoreTestSuite","testPageUpDown");
   CxxTest::leaveOnly("CoreTestSuite","testAXAPI");
   //CxxTest::leaveOnly("CoreTestSuite","testGetMouseLocation");
   //CxxTest::leaveOnly("CoreTestSuite","testCapture");
   
   //CxxTest::leaveOnly("MacTestSuite","testHoverAndClick");
   //CxxTest::leaveOnly("MacTestSuite","testDragDrop");
   //CxxTest::leaveOnly("FlashTestSuite","testClick");
   //CxxTest::leaveOnly("FlashTestSuite","testFindAll");
   //CxxTest::leaveOnly("FlashTestSuite","testVanish");
   //CxxTest::leaveOnly("FlashTestSuite","testEvent");
	//CxxTest::leaveOnly("FlashTestSuite","testDragDrop");
	//CxxTest::leaveOnly("FlashTestSuite","testDoubleClick");
	//CxxTest::leaveOnly("FlashTestSuite","testType");
	//CxxTest::leaveOnly("FlashTestSuite","testKeys");
	//CxxTest::leaveOnly("FlashTestSuite","testPaste");
   
  //CxxTest::leaveOnly("FlashTestSuite");
   //CxxTest::leaveOnly("CoreTestSuite","");
   return CxxTest::StdioPrinter().run();
}

#include <cxxtest/Root.cpp>