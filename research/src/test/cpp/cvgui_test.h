/*
 *  cvgui_test.h
 *  sikuli
 *
 *  Created by Tom Yeh on 7/29/10.
 *  Copyright 2010 sikuli.org. All rights reserved.
 *
 */

#include <cxxtest/TestSuite.h>

#include <strstream>
#include "cvgui.h"

class CVGUITestSuite : public CxxTest::TestSuite 
{
public:
   
   void setUp() {
   }   
   
   
   void testSegmentation(void)
   {
      
      Mat screen = cv::imread("research/testdata/amazon.png");
      vector<Blob> image_blobs, text_blobs; 
      cvgui::segmentScreenshot(screen, text_blobs, image_blobs);
   }
   
   void testIndexImages(void)
   {
      
      Database db;
      db.insert_file("research/testdata/amazon.png",1);
      
      std::stringstream ss;
      db.write(ss);
      
      Database db1;
      db1.read(ss);
      
      db1.find("research/testdata/amazon_logo.png");
      
   }
   
   void testReadWriteImageRecord(void)
   {
      ImageRecord ir;
      ir.id = 50;
      ir.height = 30;
      ir.width = 20;
      
      std::stringstream ss;
      ir.write(ss);
      
      ImageRecord ir1;
      ir1.read(ss);
           
      TS_ASSERT_EQUALS(ir.id, ir1.id);
      
   }
   
};


                               
                               
                               
