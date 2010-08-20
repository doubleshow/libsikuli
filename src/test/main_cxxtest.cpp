/*
 *  main_cxxtest.cpp
 *  sikuli
 *
 *  Created by Tom Yeh on 8/20/10.
 *  Copyright 2010 sikuli.org. All rights reserved.
 *
 */

#include <cxxtest/ErrorPrinter.h>


int main() {
   //CxxTest::leaveOnly("FlashTestSuite","testDragDrop");
   CxxTest::leaveOnly("CoreTestSuite","testFindInterfaces");
   return CxxTest::ErrorPrinter().run();
}

#include <cxxtest/Root.cpp>