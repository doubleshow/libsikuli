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
   

   
//   void testMultipleMonitor(void)
//   {
//      Screen s0(0);
//      Screen s1(1);
//      
//      //s0.click("apple.png");
//      cout << s0.toString() << endl;
//      cout << s1.toString() << endl;
//      
//      Region r0 = s0.crop(50,50,100,100);
//      Region r1 = s1.crop(5,5,200,200);
//      
//
//      cout << r0.toString() << endl;
//      cout << r1.toString() << endl;
//      //
//    //  ScreenImage rim0 = r0.capture();
// //     namedWindow("Test0");
////      imshow("Test0",rim0.getMat());
////      
////      ScreenImage rim1 = r1.capture();
////      namedWindow("Test1");
////      imshow("Test1",rim1.getMat());
//
//      
//      //waitKey();
//      
//   //   r0.click(Location(10,10));
////      r1.click(Location(10,10));
//
//      r0.click("firefox_refresh.png");
//      r1.click("overview.png");
//      
//      //Screen::getNumberScreens();
//      
//      //s0.hover(Location(10,10));
//      
//      //s.capture();
//      
//      
// //     ScreenImage img = s1.capture();
////      namedWindow("Test");
////      imshow("Test",img.getMat());
////      waitKey();
////      
////      TS_ASSERT_THROWS(Pattern p1("tesdc.png"), sikuli::FileNotFound);
////      Pattern p2("apple.png");
//      
//      //Screen s;
//      //s.click("magnifying_glass.png");
//      //sleep(1);
//      //s.paste("test");
////test      Robot::paste("test");
////      Settings::resetImagePaths();
////      Settings::addImagePath("http://localhost:4567/");
////      
////      Screen s;
////      s.click("apple.png");
////      s.click("http://localhost:4567/mac.png");
////      s.click("apple_in_workdir.png");
//   }
   
};
   

  
