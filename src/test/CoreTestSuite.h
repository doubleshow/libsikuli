/*
 *  CoreTestSuite.h
 *  sikuli
 *
 *  Created by Tom Yeh on 7/29/10.
 *  Copyright 2010 sikuli.org. All rights reserved.
 *
 */

#include <cxxtest/TestSuite.h>

#include "sikuli.h"

using namespace cv;
using namespace sikuli;

class CoreTestSuite : public CxxTest::TestSuite 
{
public:
   
   Screen s;
   
   void setUp() {
     // Robot::openApp("Google Chrome.app");
      Settings::addImagePath("flash/images");
   }   
   
   void testFindInterfaces(void)
   {
      char str[] = "SikuliTester.png";
      try{
      s.find(Pattern(str));
      s.find(str);
      
      s.findAll(Pattern(str));
      s.findAll(str);
      }catch(...){}
   }
   
   void testReadImageFromURL(void)
   {
      Settings::resetImagePaths();
      Settings::addImagePath("http://localhost:4567/");
      
      Screen s;
      s.click("apple.png");
      s.click("http://localhost:4567/mac.png");
      s.click("apple_in_workdir.png");
   }
   
   void testPageUpDown(void)
   {
      switchApp("firefox.app");
      s.press(PAGE_DOWN);
      s.press(PAGE_DOWN);
      
      
      
   }
   
   
};
   

  
