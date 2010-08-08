/*
 *  FlashTestSuite.h
 *  sikuli
 *
 *  Created by Tom Yeh on 7/29/10.
 *  Copyright 2010 sikuli.org. All rights reserved.
 *
 */

#include <cxxtest/TestSuite.h>
#include "sikuli.h"

using namespace sikuli;

class FlashTestSuite : public CxxTest::TestSuite 
{
public:
   
   Screen s;
   
   void setUp() {
      Robot::openApp("Google Chrome.app");
      Settings::addImagePath("flash/images");
   }
   
   void tearDown() {      
      // sleep to allow the previous test to complete
      sleep(1);
      Settings::resetImagePaths();
   }
   
   void switchToTest(const char* testTabImage)
   {
      s.click(testTabImage);
      sleep(1);
   }
   
   void testClick(void)
   {
      switchToTest("TestClick.png");
      s.click("clickHere.png");
      TS_ASSERT(s.exists("success.png"));
   }

   
   void testDoubleClick(void)
   {
      switchToTest("TestDoubleClick.png");
      s.doubleClick("clickHere.png"); 
      TS_ASSERT(s.exists("success.png"));
   }
   
   void testRightClick(void)
   {
      s.rightClick("clickHere.png");
      TS_ASSERT(s.exists("about_adobe_flash_player.png"));
      s.press(ESC);
   }
   
   void testType(void)
   {
      switchToTest("TestType.png");
      s.type("textinput.png","test TEST 1234567890 !@#$%^&*()");
      TS_ASSERT(s.exists("success.png"));
   }
   
   void testDragDrop(void)
   {
      switchToTest("TestDragDrop.png");
      s.dragDrop("hslider_thumb.png", "dropHere.png");
      TS_ASSERT(s.exists("success.png"));
   }

   void testFindAll(void)
   {
      switchToTest("TestFindAll.png");
      vector<Match> ms = s.findAll("flower.png");
      TS_ASSERT_EQUALS(ms.size(), 6);

      ms = s.findAll("folder.png");
      TS_ASSERT_EQUALS(ms.size(), 5);

      ms = s.findAll("checkbox.png");
      TS_ASSERT_EQUALS(ms.size(), 5);      

      for (int i=0; i< ms.size(); ++i)
         s.click(ms[i]);

      TS_ASSERT(s.exists("success.png"));      
   }
   
   void testSpatialOp(void)
   {
      switchToTest("TestSpatialOp.png");
      
      Match computer = s.find("computer.png");
      s.hover(computer);
      
      s.click(computer.left().find("clickButton.png"));
      s.click(computer.below().find("clickButton.png"));
      s.click(computer.right().find("clickButton.png"));
      s.click(computer.above().find("clickButton.png"));
      
      TS_ASSERT(s.exists("success.png"));
   }

   void testOrdering(void)
   {
      switchToTest("TestOrdering.png");

      
      Pattern p; 
      p = Pattern("button.png").leftMost();
      s.click(p);
      p = Pattern("button.png").bottomMost();
      s.click(p);
      p = Pattern("button.png").rightMost();
      s.click(p);
      p = Pattern("button.png").topMost();
      s.click(p);
      
      p = Pattern("button.png").order(LEFTRIGHT);
      s.click(s.findAll(p)[2]);
      p = Pattern("button.png").order(BOTTOMUP);
      s.click(s.findAll(p)[1]);
      p = Pattern("button.png").order(RIGHTLEFT);
      s.click(s.findAll(p)[2]);
      p = Pattern("button.png").order(TOPDOWN);
      s.click(s.findAll(p)[1]);
      
      TS_ASSERT(s.exists("success.png"));
      
   }
   
   void testPaste(void)
   {
      switchToTest("TestPaste.png");
      s.paste("paste_target.png", "paSte tHis text here!!!");
      TS_ASSERT(s.exists("success.png"));
   }
   
};


   
   
  
