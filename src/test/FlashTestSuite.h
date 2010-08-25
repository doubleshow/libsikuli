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
#include "event-manager.h"

using namespace sikuli;

void play_rome_puzzle();


void appear_callback(Event event){
   cout << event.pattern.getImageURL() << " appeared in " << event.region.toString() << endl;
   event.region.hover(event.match);
}

void vanish_callback(Event event){
   cout << event.pattern.getImageURL() << " vanished from " << event.region.toString() << endl;
}

void change_callback(Event event){
   cout << "change observed in " << event.region.toString() << endl;
}


void appear_and_stop_callback(Event event){
   cout << event.pattern.getImageURL() << " appeared in " << event.region.toString() << endl;
   EventManager::stop();
   cout << "stopping event manager.." << endl;
}

class VanishEventHandler : public SikuliEventHandler{
  
public:
   virtual void handle(Event event){
      cout << event.pattern.getImageURL() << " vanished from " << event.region.toString() << endl;  
   }
};

void wack(Event event){
   cout << event.pattern.getImageURL() << " appeared in " << event.region.toString() << endl;
   event.region.click(event.match);
}


class FlashTestSuite : public CxxTest::TestSuite 
{
public:
   
   Screen s;
   
   void setUp() {
      Robot::openApp("Google Chrome.app");
      Settings::addImagePath("test/flash/images");
      s = Screen(1);
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
      s.type("textinput.png",
      "abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ !@#$%^&*()_+ 1234567890-=`~;':,./<>?\"[]{}\\|");
      TS_ASSERT(s.exists("success.png"));
   }
   
   void testKeys(void)
   {
      switchToTest("TestKeys.png");
      s.click("textinput.png");
      s.press(DOWN);
      s.press(UP);
      s.press(LEFT);
      s.press(RIGHT);
      s.press(F1); s.press(F2); s.press(F3); s.press(F4); s.press(F5); s.press(F6);
      s.press(F7); s.press(F8); s.press(F9); s.press(F10); s.press(F11); s.press(F12);
      s.press(BACKSPACE);
      s.press(DELETE);
      s.press(END);
      s.press(ENTER);
      s.press(ESC);
      s.press(PAGE_DOWN);
      s.press(PAGE_UP);
      s.press(SPACE);
      s.press(CAPSLOCK);
      s.press(INSERT);
      s.press(HOME);
      s.press(TAB);
   }
   
   void testOCR(void)
   {
      switchToTest("TestOCR.png");
      initOCR("arial.png");
      s.click("/System Preferences/");
      s.click("/Configuration/");
      s.click("/Keyboard/");
      s.click("/My Favorites/");

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
   
   void testVanish(void)
   {
      switchToTest("TestVanish.png");
      s.waitVanish("flower.png");
      TS_ASSERT(!s.exists("flower.png",0)); 
   }
   
   void testSpatialOp(void)
   {
      switchToTest("TestSpatialOp.png");
      
      
      
      Match computer = s.find("computer.png");
      s.hover(computer);
      
      computer.left().find("clickButton.png").click(Location(10,10));
      computer.below().find("clickButton.png").click(Location(10,10));
      computer.right().find("clickButton.png").click(Location(10,10));
      computer.above().find("clickButton.png").click(Location(10,10));
      
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
   
   void testEvent(void)
   {
      switchToTest("TestEvent.png");
      
      Match m = s.find("SikuliTester.png");
      
      Region r = s.inner(m.x,m.y,230,250);
      
      EventManager::addObserver(r, Observer(APPEAR, Pattern("computer.png"), &appear_callback));
      
      VanishEventHandler ve;
      EventManager::addObserver(r, Observer(VANISH, Pattern("computer.png"), &ve));
      
      r.onAppear(Pattern("flower.png"), &appear_callback);
      r.onVanish(Pattern("flower.png"), &ve);
      
      r.onAppear(Pattern("bug.png"), &appear_and_stop_callback);      
      EventManager::observe(20);      
   }
   
};


   
   
  
