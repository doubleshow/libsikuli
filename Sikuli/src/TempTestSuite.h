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
   
   void testFindTopMost(void)
   {
      Screen s;
      s.click("apple.png");
      s.type("sys\n");
      
      sleep(2);
      Pattern p = Pattern("checkbox.png").bottomMost();
      Match m = s.find(p);
      s.click(m);
      

   }

   
   
};
   

  
