/*
 *  screen.cpp
 *  sikuli
 *
 *  Created by Tom Yeh on 7/30/10.
 *  Copyright 2010 sikuli.org. All rights reserved.
 *
 */

#include "screen.h"
#include "robot.h"

using namespace sikuli;

Screen::Screen() : Region() {
   _screen_id = 0;
   Robot::getScreenSize(_screen_id,w,h);
   x = 0;
   y = 0;
   xo = 0;
   yo = 0;
}

Screen::Screen(int id) : Region() {   
   _screen_id = id;
   Robot::getScreenSize(_screen_id,w,h);
   x = 0;
   y = 0;
   xo = 0;
   yo = 0;
}

Screen::~Screen(){
}

//ScreenImage
//Screen::capture(int x, int y, int w, int h){
//   cv::Mat img = Robot::capture(_id, x, y, w, h);
//   ScreenImage screenimage(img);
//   return screenimage;
//}

//int 
//Screen::getNumberScreens(){
   //CGDisplayErr err;
   //CGDirectDisplayID ids[10];
   //CGDisplayCount dspyCnt;
   //err = CGGetActiveDisplayList(10,ids,&dspyCnt); 
   //return (int) dspyCnt;
//}

//Rectangle
//Screen::getBounds(){
//   int x,y,w,h;
//   Robot::getScreenBounds(_screen_id,x,y,w,h);
//   return Rectangle(x,y,w,h);
//}