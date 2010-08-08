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
using namespace sikuli;

class TempTestSuite : public CxxTest::TestSuite 
{
public:
   
   void setUp(void)
   {
      Settings::addImagePath("images");
   }
   
//   void testFindTopMost(void)
//   {
//      Screen s;
//      s.click("apple.png");
//      
//      sleep(2);
//      Pattern p = Pattern("mac.png").bottomMost();
//      Match m = s.find(p);
//      s.click(m);
//
//   }
   
   void testTemp(void)
   {
      Screen s;
      s.click("magnifying_glass.png");
      sleep(1);
      s.paste("test");
//test      Robot::paste("test");
//      Settings::resetImagePaths();
//      Settings::addImagePath("http://localhost:4567/");
//      
//      Screen s;
//      s.click("apple.png");
//      s.click("http://localhost:4567/mac.png");
//      s.click("apple_in_workdir.png");
   }
   
};
   

  
