/*
 *  TempTestSuite.h
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


class TempTestSuite : public CxxTest::TestSuite 
{
public:
   
   void testAddImagePaths(void)
   {
      Settings::resetImagePaths();
      Settings::addImagePath("test");
      Settings::addImagePath("images");
      Settings::addImagePath("images/a");
      TS_ASSERT_EQUALS(Settings::getImagePaths().size(), 3);
      TS_ASSERT_SAME_DATA(Settings::getImagePaths()[0], "test", 4);
      TS_ASSERT_SAME_DATA(Settings::getImagePaths()[1], "images", 4);
      
      Screen s;
      TS_ASSERT_THROWS_NOTHING(s.find("aapple.png"));
      TS_ASSERT_THROWS_NOTHING(s.findAll("aapple.png"));
      
   }

   
   
};
   

  
