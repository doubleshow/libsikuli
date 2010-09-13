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
#include "vision.h"

#include "myocr.h"
#include "ocr.h"


using namespace cv;
using namespace sikuli;

class CoreTestSuite : public CxxTest::TestSuite 
{
public:
   
   Screen s;
   
   void setUp() {
     // Robot::openApp("Google Chrome.app");
      Settings::addImagePath("test/flash/images");
      Settings::addImagePath("test/images");
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
      
      int x,y,w,h;
      Robot::getTopWindowBounds(x,y,w,h);
      return;
      
      switchApp("firefox.app");
      s.press(PAGE_DOWN);
      s.press(PAGE_DOWN);
      ScreenImage after, before;
      before = s.capture();
      
      
      bool can_scroll = true;  
      while (can_scroll){
         
         s.press(PAGE_DOWN);
         wait(0.5);
         
         after = s.capture();         
         double diff;
         diff = Vision::compare(before.getMat(), after.getMat());
         cout << diff << endl;
         
         can_scroll = diff > 0.001;
         
         before = after;
         //cout << same << endl;
         
         //simg = s.capture();
         //after = simg;
      }
   }
   
   void testAXAPI(void)
   {
      
      for (int i=0;i<=17;++i){
         char filename[50];
         sprintf(filename, "research/captured/screen%d.png",i);
         OCRText text = OCR::recognize_screenshot(filename);
         
         
         char buf[50];
         sprintf(buf,"%s.ocr.txt", filename);
         text.save(buf);

         sprintf(buf,"%s.ocr.loc", filename);
         text.save_with_location(buf);sik
         
      }
      
      
     // for (int i=1;i<=2;++i){
//         char filename[50];
//         sprintf(filename, "test/screenshots/finder%d.png",i);
//         OCRText text = OCR::recognize_screenshot(filename);
//         
//         
//         char buf[50];
//         sprintf(buf,"%s.ocr.txt", filename);
//         text.save(buf);
//         
//      }
//      
     // for (int i=1;i<=6;++i){
//         char filename[50];
//         sprintf(filename, "test/screenshots/amazon%d.png",i);
//         OCRText text = OCR::recognize_screenshot(filename);
//         
//         
//         char buf[50];
//         sprintf(buf,"%s.ocr.txt", filename);
//         text.save(buf);
//      
//      }
//      
//      for (int i=1;i<=4;++i){
//         char filename[50];
//         sprintf(filename, "test/screenshots/chi%d.png",i);
//         OCRText text = OCR::recognize_screenshot(filename);
//         
//         
//         char buf[50];
//         sprintf(buf,"%s.ocr.txt", filename);
//         text.save(buf);
//         
//      }

//      for (OCRText::iterator it = text.begin();
//           it != text.end(); ++it){
//         
//       
//         cout << it->str() << endl;
//         
//      }
      
   }
   
   void testGetMouseLocation(void)
   {
      s.hover(Location(150,150));
      Location loc = s.getMouseLocation();
      TS_ASSERT_EQUALS(loc.x, 150);
      TS_ASSERT_EQUALS(loc.y, 150);
      
      Region r = s.inner(100,100,100,100);
      loc = r.getMouseLocation();
      TS_ASSERT_EQUALS(loc.x, 50);
      TS_ASSERT_EQUALS(loc.y, 50);
      
//      for (int i=0;i<50;++i){
//         loc = s.getMouseLocation();
//         cout << "(" << loc.x << "," << loc.y << ")" << endl;
//         wait(0.1);
//      }
      
   }
   
   void testCapture(void)
   {
      // define a region
      Region r = s.inner(50,50,100,100);
      
      // capture a screen image of the region
      ScreenImage r_img = r.capture();
      
      // find the image
      Match m = s.find(r_img);
      
      // the match should be the same as the region
      TS_ASSERT(r == m);
      TS_ASSERT_EQUALS(r.getX(), m.getX());
      TS_ASSERT_EQUALS(r.getY(), m.getY());
      TS_ASSERT_EQUALS(r.getW(), m.getW());
      TS_ASSERT_EQUALS(r.getH(), m.getH());

   }
   
};
   

  
