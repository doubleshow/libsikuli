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
   
   
   void tearDown() {
      // sleep to let previous test complete
      sleep(1);
   }
   
   void testCreation( void )
   {
      Region r(10,10,10,10);
      TS_ASSERT_EQUALS( r.getH(), 10);
   }
   
   void testExists(void)
   {
      Screen s;
      TS_ASSERT(s.exists("apple.png"));
      TS_ASSERT(!s.exists("vista.png",0));      
   }
   
   void testModifiers(void)
   {
      Screen s;
      s.type("q", SHIFT + META);
      TS_ASSERT(s.exists("logout.png"));
      s.click("cancel.png");
   }   

   void testESC(void)
   {
      Screen s;
      s.type("q", SHIFT + META);
      TS_ASSERT(s.exists("logout.png"));
      s.press(ESC);
   }
   
   void testClick(void)
   {
      Screen s;
      s.click("apple.png");
      TS_ASSERT(s.exists("about.png"));
      s.press(ESC);
   }
   
   void testHoverAndClick(void)
   {  
      Screen s;
      s.hover("magnifying_glass.png");
      sleep(1);
      s.click();
      TS_ASSERT(s.exists("spotlight.png"));
      s.press(ESC);      
      s.press(ESC);
   }

   void testType(void)
   {
      Screen s;
      s.click("magnifying_glass.png");
      s.wait("spotlight_label.png");
      s.type("test TEST test TEST test");
      TS_ASSERT(s.exists("spotlight_input.png"));
      s.press(ESC);
      s.press(ESC);
   }  
   
   void testTrackPadWaitForToolTip(void)
   {
      Screen s;
      s.click("apple.png");
      s.type("sys\n");
      s.click("trackpad.png");
      s.find("tracking_speed.png").below(100).wider(100).hover("thumb.png");
      TS_ASSERT_THROWS_NOTHING(s.wait("tracking_speed_tooltip.png",5));
      s.type("w", META);
   }
   
   void testDragDrop(void)
   {
      Screen s;
      s.click("apple.png");
      s.type("sys\n");
      s.click("keyboard.png");
      
      Region slider = s.find("turn_off_label.png").below(100).wider(300);
      Region t0 = slider.find("thumb.png");
      
      Region never   = s.find("never.png");
      Region one_min = s.find("one_min.png");
      
      s.drag(t0);
      s.dropAt(never);
      
      TS_ASSERT(s.exists(Pattern("thumb_at_never.png").similar(0.95)));
      
      Region t1 = slider.find("thumb.png");
      s.drag(t1);
      s.dropAt(t0);
      
      sleep(1);
      
      s.dragDrop(t0,never);
      
      TS_ASSERT(s.exists(Pattern("thumb_at_never.png").similar(0.95)));
      
      s.dragDrop(t1,t0);
      
      
      sleep(1);
      s.type("q", META);
   }   
   
   void testOpenApp(void)
   {
      Screen s;
      Robot r;
      r.openApp("firefox.app");
      TS_ASSERT(s.exists("firefox_menu.png"));
   } 
   
};


   
   
  
