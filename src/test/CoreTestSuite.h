/*
 *  TempTestSuite.h
 *  sikuli
 *
 *  Created by Tom Yeh on 7/29/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include <cxxtest/TestSuite.h>

#include "sikuli.h"
#include <opencv2/opencv.hpp>
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
   
};
   

  
