/*
 *  screen.cpp
 *  sikuli
 *
 *  Created by Tom Yeh on 7/30/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "screen.h"

#include <ApplicationServices/ApplicationServices.h>

#include "robot.h"

using namespace sikuli;

Screen::Screen() : Region() {
   _id = 0;
   Robot::getDisplayBounds(_id,x,y,w,h);
}

Screen::Screen(int id) : Region() {   
   _id = id;
   Robot::getDisplayBounds(_id,x,y,w,h);
}

Screen::~Screen(){
}

ScreenImage
Screen::capture(int x, int y, int w, int h){
   cv::Mat img = Robot::capture(_id, x, y, w, h);
   ScreenImage screenimage(img);
   return screenimage;
}

int 
Screen::getNumberScreens(){
   CGDisplayErr err;
   CGDirectDisplayID ids[10];
   CGDisplayCount dspyCnt;
   err = CGGetActiveDisplayList(10,ids,&dspyCnt); 
   return (int) dspyCnt;
}

Rectangle
Screen::getBounds(){
   int x,y,w,h;
   Robot::getDisplayBounds(_id,x,y,w,h);
   return Rectangle(x,y,w,h);
}