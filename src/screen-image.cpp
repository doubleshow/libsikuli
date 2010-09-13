/*
 *  screen-image.cpp
 *  sikuli
 *
 *  Created by Tom Yeh on 8/28/10.
 *  Copyright 2010 sikuli.org. All rights reserved.
 *
 */

#include "screen-image.h"
#include "robot.h"

using namespace sikuli;

ScreenImage::ScreenImage(const char* filename){
   _image = cv::imread(filename,3);
}

void
ScreenImage::show(int seconds){
   cv::namedWindow("sikuli",CV_WINDOW_AUTOSIZE);
   
   //   Mat m;
   //   cv::resize(_image,m,Size(150,150));
   
   cv::imshow("sikuli", _image);
   if (seconds == 0)
      cv::waitKey(0);
   else
      Robot::delay(seconds*1000);
   
   cvDestroyWindow("sikuli");
}

void
ScreenImage::show(){
   show(0);
}

void
ScreenImage::save(const char* filename){
   cv::imwrite(filename, _image);
}
