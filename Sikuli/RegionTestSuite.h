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
#include "screen.h"
#include "vision.h"
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
   {//
 
 //     Region r = Region::getFullScreen();
//      Match m = r.find("region.png");
//      r.doubleClick("folder.png");
//      
////localhost/Users/tomyeh/Desktop/Videos/
      //Region r = Region::getFullScreen();
      
      FullScreen r;
      //r.click("folder.png");
      
      Vision::trainOCR("arial.png");
//      r.click("/Project/");
//      //sleep(1);
//      r.hover("/New Smart Group/");
//      r.hover("/Simple Filter Smart Group/");

      
      r.click("apple.png");
      //r.click("/System Preferences/");
      r.type("sys\n");
      sleep(1);
      r.click("/Trackpad/");
//      Match m = r.find("displays.png");
//      m.hover("/Displays/");
      
      //Match m = r.find("flash.png");
      //m.above().wider(50).doubleClick("folder.png");
      //m.left().taller(50).doubleClick("folder.png");
      
      //r.find("folder.png").right().taller(50).doubleClick("flash.png");
      
      //Region r;
      //r.click(Location(10,10), 1);
      //Pattern p("dropbox.png");
      //r.doubleClick(p,0);
      //sleep(2);
//     
//      r.click("apple.png"); 
//      r.type("sys\n");
//      sleep(3);
//      r.type("MouSe\n");
//     
      //r.click("tool.png");
      //int ret = r.rightClick("apple.png");
      
      //r.rightClick("tool.png");
      
      //TS_ASSERT_EQUALS(ret, 1);
     // r.hover("apple.png");
      //sleep(1);
      
      //r.doubleClick("t1.png", K_CTRL);
      
      //r.doubleClick("t1.png");
      
      // Long Click
      //r.hover("apple.png");//
//      r.mouseDown(BUTTON1_MASK);
//      sleep(2);
//      r.mouseUp(BUTTON1_MASK);
      //
//      r.click(BUTTON1_MASK);
//      sleep(2);
//      r.click(BUTTON1_MASK);
//      
//      //r.click("t1.png");
//      r.hover("t1.png");
//      sleep(1);
//      r.mouseDown(BUTTON1_MASK);
//      sleep(2);
//      r.mouseUp(BUTTON1_MASK);
//      
      
      //Match m = r.find("t1.png");
      
      //r.waitVanish("t1.png", 5);
      
      //r.drag("flash.png");
      //r.dropAt("t1.png",0.1);
      
      //Location(m.getTarget().x,m.getTarget().y-100),0.1);
//      sleep(5);
//      
      
      //r.hover("t1.png");
      //r.click(K_CTRL);
      //r.rightClick("t1.png");
//      //r.click("t2.png", K_SHIFT);
      //r.hover("tool.png");
      //r.mouseDown(BUTTON3_MASK);
      
//      Screen scr;
//      scr.getNumberScreens();
      
      
      //
//      Robot rb;
//      int x,y,h,w;
//      rb.getDisplayBounds(x,y,w,h);
      
   }
   
   
   
};
