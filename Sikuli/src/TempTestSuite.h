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
   

  
