/*
 *  robot.cpp
 *  sikuli
 *
 *  Created by Tom Yeh on 7/27/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "robot.h"

#include <ApplicationServices/ApplicationServices.h>

void
Robot::delay(int time){
}

void 
Robot::mouseMove(int x, int y)
{
   CGPoint newloc;
   CGEventRef eventRef;
   newloc.x = x;
   newloc.y = y;
   
   eventRef = CGEventCreateMouseEvent(NULL, kCGEventMouseMoved, newloc,
                                      kCGMouseButtonCenter);
   //Apparently, a bug in xcode requires this next line
   CGEventSetType(eventRef, kCGEventMouseMoved);
   CGEventPost(kCGSessionEventTap, eventRef);
   CFRelease(eventRef);
}

void 
Robot::mousePress(int buttons){
   CGPoint curloc;
   CGEventRef eventRef;
   
   CGEventRef ourEvent = CGEventCreate(NULL);
   
   curloc = CGEventGetLocation(ourEvent);
   
   CGEventType mouseEvent;
   if (buttons & BUTTON1_MASK)
      mouseEvent = kCGEventLeftMouseDown;
   else if (buttons & BUTTON2_MASK)
      mouseEvent = kCGEventOtherMouseDown;
   else if (buttons & BUTTON3_MASK)
      mouseEvent = kCGEventRightMouseDown;
   
   eventRef = CGEventCreateMouseEvent(NULL, mouseEvent, curloc,
                                      kCGMouseButtonCenter);
   //Apparently, a bug in xcode requires this next line
   CGEventSetType(eventRef, mouseEvent);
   CGEventPost(kCGSessionEventTap, eventRef);
   CFRelease(eventRef);
}

void 
Robot::mouseRelease(int buttons){
   CGPoint curloc;
   CGEventRef eventRef;
   
   CGEventRef ourEvent = CGEventCreate(NULL);   
   curloc = CGEventGetLocation(ourEvent);
   
   CGEventType mouseEvent;
   if (buttons & BUTTON1_MASK)
      mouseEvent = kCGEventLeftMouseUp;
   else if (buttons & BUTTON2_MASK)
      mouseEvent = kCGEventOtherMouseUp;   
   else if (buttons & BUTTON3_MASK)
      mouseEvent = kCGEventRightMouseUp;   
   
   eventRef = CGEventCreateMouseEvent(NULL, mouseEvent, curloc,
                                      kCGMouseButtonCenter);
   //Apparently, a bug in xcode requires this next line
   CGEventSetType(eventRef, mouseEvent);
   CGEventPost(kCGSessionEventTap, eventRef);
   CFRelease(eventRef);   
}

void 
Robot::keyPress(int keycode){
   CGEventRef eventRef;
   eventRef = CGEventCreateKeyboardEvent (NULL, (CGKeyCode)keycode, true);
   CGEventPost(kCGSessionEventTap, eventRef);
   CFRelease(eventRef);
}

void 
Robot::keyRelease(int keycode){
   CGEventRef eventRef;
   eventRef = CGEventCreateKeyboardEvent (NULL, (CGKeyCode)keycode, false);
   CGEventPost(kCGSessionEventTap, eventRef);
   CFRelease(eventRef);   
}


void 
Robot::waitForIdle(){
}

void 
Robot::paste(string text){
}

using namespace cv;

#include "glgrab.h"

Mat
Robot::capture(){
   
   CGSize size = CGDisplayScreenSize(0);
   CGRect rect = CGDisplayBounds(0);
   
   CGImageRef imgRef = grabViaOpenGL(0, rect);
   const int h = CGImageGetHeight(imgRef);
   const int w = CGImageGetWidth(imgRef);
   
   const int bpp = CGImageGetBitsPerPixel(imgRef); 
   const int bpr = CGImageGetBytesPerRow(imgRef);

   CFDataRef imageData = CGDataProviderCopyData(CGImageGetDataProvider(imgRef)); 
   UInt8* buffer = new UInt8[CFDataGetLength(imageData)];
   CFDataGetBytes(imageData, CFRangeMake(0,CFDataGetLength(imageData)), buffer);
   
   CGImageRelease(imgRef);
   
   IplImage* img = cvCreateImageHeader(cvSize(w,h),8,bpp/8); //create the "shell"   
   cvSetData(img, buffer, bpr);    //set the buffer
   
   Mat argb(img);
   Mat bgr( argb.rows, argb.cols, CV_8UC3 );
   Mat alpha( argb.rows, argb.cols, CV_8UC1 );
   
   Mat out[] = { bgr, alpha };
   int from_to[] = { 1,2,  2,1,  3,0,  0,3 };
   mixChannels( &argb, 1, out, 2, from_to, 4 ); 
 
   delete buffer;
   cvReleaseImageHeader(&img);
   
   return bgr;//bgr.clone();//(bgr,true);//bgr.clone();
}

