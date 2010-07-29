/*
 *  RegionTestSuite.h
 *  sikuli
 *
 *  Created by Tom Yeh on 7/29/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include <cxxtest/TestSuite.h>
#include "region.h"
using namespace sikuli;

class RegionTestSuite : public CxxTest::TestSuite 
{
public:
   void testCreation( void )
   {
      Region r(10,10,10,10);
      TS_ASSERT_EQUALS( r.getH(), 10);
   }
   
   void testClick(void)
   {
      Region r;
      //r.click(Location(10,10), 1);
      //Pattern p("dropbox.png");
      //r.doubleClick(p,0);
      //sleep(2);
//      r.click("apple.png");
//      r.type("sys\n");
//      sleep(2);
//     r.type("mouse\n");
      //r.click("tool.png");
      //int ret = r.rightClick("apple.png");
      
      //r.rightClick("tool.png");
      
      //TS_ASSERT_EQUALS(ret, 1);
      r.hover("apple.png");
      sleep(1);
      r.click();
      //r.hover("tool.png");
      //r.mouseDown(BUTTON3_MASK);
   }
   
   
   
};
