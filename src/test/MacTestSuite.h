/*
 *  RegionTestSuite.h
 *  sikuli
 *
 *  Created by Tom Yeh on 7/29/10.
 *  Copyright 2010 sikuli.org. All rights reserved.
 *
 */

#include <cxxtest/TestSuite.h>
#include "sikuli.h"

using namespace sikuli;

class MacTestSuite : public CxxTest::TestSuite 
{
public:
   
   void setUp() {
      Settings::addImagePath("test/images");
   }
   
   void tearDown() {
      // sleep to let previous test complete
      sleep(1);
      
      Settings::resetImagePaths();
   }
   
   void testAddImagePaths(void)
   {
      Settings::resetImagePaths();
      Settings::addImagePath("test");
      Settings::addImagePath("test/images");
      Settings::addImagePath("test/images/a");
      TS_ASSERT_EQUALS(Settings::getImagePaths().size(), 3);
      TS_ASSERT_SAME_DATA(Settings::getImagePaths()[0], "test/test", 4);
      TS_ASSERT_SAME_DATA(Settings::getImagePaths()[1], "test/images", 4);
      
      Screen s;
      TS_ASSERT_THROWS_NOTHING(s.find("aapple.png"));
      TS_ASSERT_THROWS_NOTHING(s.findAll("aapple.png"));
      
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
      s.type("q", CMD);
   }
   
   void testDragDrop(void)
   {
      Screen s;
      s.click("apple.png");
      s.type("sys\n");
      s.click("keyboard.png");
      
      Region slider = s.find("turn_off_label.png").below(200).wider(300);
      Region t0 = slider.find("thumb.png");
      
      Region never   = slider.find("never.png");
      Region one_min = slider.find("one_min.png");
      
      slider.drag(t0);
      slider.dropAt(never);
      
      TS_ASSERT(s.exists(Pattern("thumb_at_never.png").similar(0.95)));
      
      Region t1 = slider.find("thumb.png");
      slider.drag(t1);
      slider.dropAt(t0);
      
      wait(1.0);
      
      slider.dragDrop(t0,never);
      
      TS_ASSERT(s.exists(Pattern("thumb_at_never.png").similar(0.95)));
      
      slider.dragDrop(t1,t0);
      
      
      wait(1.0);
      s.type("q", CMD);
   }   
   
   void testOpenApp(void)
   {
      Screen s;
      Robot r;
      r.openApp("firefox.app");
      TS_ASSERT(s.exists("firefox_menu.png"));
   } 
   
   void testFindAll(void)
   {
      Screen s;
      s.click("apple.png");
      s.type("sys\n");
      s.click("keyboard.png");
      
      Region r = s.find("keyboard_settings.png");
      vector<Match> ms = r.findAll("thumb.png");
      
      TS_ASSERT_EQUALS(ms.size(), 3);
      s.type("q",CMD);
   }
   
   void testOCR(void)
   {
      initOCR("arial.png");
      
      Screen s;
      s.click("apple.png");
      s.click("/System Preferences/");
      s.click("/Trackpad/");
      TS_ASSERT(s.exists(Pattern("tracking_speed.png").similar(0.95)));
      s.type("q",CMD);
      
   }   
   
   void testTargetOffset(void)
   {
      Screen s;
      s.click("apple.png");
      s.type("sys\n");
      
      Pattern p("trackpad.png");
      p = p.targetOffset(0,-100);
      s.click(p);
      TS_ASSERT(s.exists("security_tabs.png"));
      s.type("q",CMD);
   }
   
   void testPatternOrderBy(void)
   {
      
      Screen s;
      s.click("apple.png");
      s.type("sys\n");
      s.click("trackpad.png");
      
      sleep(1);
      
      Pattern p = Pattern("thumb.png").order(RIGHTLEFT);
      vector<Match> ms = s.findAll(p);
      s.hover(ms[2]);
      sleep(2);
      TS_ASSERT(s.exists("tracking_speed_tooltip.png"));
      
      s.hover(ms[0]);
      sleep(2);
      TS_ASSERT(s.exists("scrolling_speed_tooltip.png"));
      
      s.type("q", CMD);
      
   }
   
   void testPatternLimit(void)
   {
      Screen s;
      s.click("apple.png");
      s.type("sys\n");
      s.click("sharing.png");
      
      sleep(2);
      Pattern p = Pattern("checkbox.png").limit(5);
      vector<Match> ms = s.findAll(p);
      TS_ASSERT_EQUALS(ms.size(), 5);
      
      p = Pattern("checkbox.png");
      ms = s.findAll(p);
      TS_ASSERT(ms.size() > 5);
      
      p = Pattern("checkbox.png").limit(0);
      ms = s.findAll(p);
      TS_ASSERT(ms.size() > 5);
      
      
      s.type("q",CMD);
   }   
   
   
   
};


   
   
  
