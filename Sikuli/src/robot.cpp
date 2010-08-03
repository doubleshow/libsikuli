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



int Robot::_modifiers = 0;

#include <time.h>
void
Robot::delay(int msec){
   struct timespec interval, remainder; 
   interval.tv_sec = 0; 
   interval.tv_nsec = msec*1000; 
   nanosleep(&interval, &remainder);
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
   
   // Set modifier flags
   CGEventFlags flags = CGEventGetFlags(eventRef);
   flags |= _modifiers;
   CGEventSetFlags(eventRef, flags);   
   
   //Apparently, a bug in xcode requires this next line
   CGEventSetType(eventRef, mouseEvent);
   CGEventPost(kCGSessionEventTap, eventRef);
   CFRelease(eventRef);
}

void
Robot::singleClick(int button){
   mousePress(button);
   mouseRelease(button);
}

void
Robot::drag(){
   mousePress(BUTTON1_MASK);
   sleep(2);
}

void
Robot::drop(){
   CGEventRef ourEvent = CGEventCreate(NULL);   
   CGPoint curloc = CGEventGetLocation(ourEvent);
   CGEventRef eventRef = CGEventCreateMouseEvent(NULL, kCGEventLeftMouseDragged, curloc,
                                                 kCGMouseButtonCenter);
   CGEventSetType(eventRef, kCGEventLeftMouseDragged);
   CGEventPost(kCGSessionEventTap, eventRef);
   
   CFRelease(eventRef);  
   
   mouseRelease(BUTTON1_MASK);
}

void
Robot::doubleClick(int buttons){
   CGPoint curloc;
   CGEventRef eventRef;
   
   CGEventRef ourEvent = CGEventCreate(NULL);   
   curloc = CGEventGetLocation(ourEvent);
   
   CGEventType mouseEventUp, mouseEventDown;
   if (buttons & BUTTON1_MASK){
      mouseEventUp = kCGEventLeftMouseUp;
      mouseEventDown = kCGEventLeftMouseDown;      
   }
   else if (buttons & BUTTON2_MASK){
      mouseEventUp = kCGEventOtherMouseUp;   
      mouseEventDown = kCGEventOtherMouseDown;         
   }
   else if (buttons & BUTTON3_MASK){
      mouseEventUp = kCGEventRightMouseUp;   
      mouseEventDown = kCGEventRightMouseDown;    
   }
   
   eventRef = CGEventCreateMouseEvent(NULL, mouseEventUp, curloc,
                                      kCGMouseButtonCenter);
   
   // Set modifier flags
   CGEventFlags flags = CGEventGetFlags(eventRef);
   flags |= _modifiers;
   CGEventSetFlags(eventRef, flags);
   
   CGEventTapLocation tap = kCGSessionEventTap;
  
   CGEventSetIntegerValueField(eventRef, kCGMouseEventClickState, 1);      
   CGEventSetType(eventRef, mouseEventDown);
   CGEventPost(tap, eventRef);     
   CGEventSetType(eventRef, mouseEventUp);
   CGEventPost(tap, eventRef);

   CGEventSetIntegerValueField(eventRef, kCGMouseEventClickState, 2);         
   CGEventSetType(eventRef, mouseEventDown);
   CGEventPost(tap, eventRef);
   CGEventSetType(eventRef, mouseEventUp);
   CGEventPost(tap, eventRef);
    
   
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
   switch (keycode){
      case VK_SHIFT:    _modifiers |= kCGEventFlagMaskShift; break;
      case VK_CONTROL:  _modifiers |= kCGEventFlagMaskControl; break;
      case VK_META:     _modifiers |= kCGEventFlagMaskCommand; break;
      case VK_ALT:   _modifiers |= kCGEventFlagMaskAlternate; break;
      default:

         CGEventRef eventRef;
         eventRef = CGEventCreateKeyboardEvent (NULL, (CGKeyCode)keycode, true);
         
         CGEventFlags flags = CGEventGetFlags(eventRef);
         flags |= _modifiers;
         CGEventSetFlags(eventRef, flags);
   
         CGEventPost(kCGSessionEventTap, eventRef);
         CFRelease(eventRef);
   }
}

void 
Robot::keyRelease(int keycode){
   switch (keycode){
      case VK_SHIFT:    _modifiers &= !kCGEventFlagMaskShift; break;
      case VK_CONTROL:  _modifiers &= !kCGEventFlagMaskControl; break;
      case VK_META:     _modifiers &= !kCGEventFlagMaskCommand; break;
      case VK_ALT:   _modifiers &= !kCGEventFlagMaskAlternate; break;         
      default:

         CGEventRef eventRef;
         eventRef = CGEventCreateKeyboardEvent (NULL, (CGKeyCode)keycode, false);
         CGEventPost(kCGSessionEventTap, eventRef);
         CFRelease(eventRef);   
   }
}

void
Robot::keyPressRelease(int keycode, int modifiers){
   CGEventRef eventRef;
   eventRef = CGEventCreateKeyboardEvent (NULL, (CGKeyCode)keycode, true);
   
   CGEventFlags flags = CGEventGetFlags(eventRef);
   switch (modifiers){
      case VK_SHIFT:
         flags |= kCGEventFlagMaskShift;
         break;
      case VK_CONTROL:
         flags |= kCGEventFlagMaskControl;
         break;
   }
   CGEventSetFlags(eventRef, flags);

   CGEventPost(kCGSessionEventTap, eventRef);
   CFRelease(eventRef);   
   
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

void
Robot::getDisplayBounds(int displayId, int& x, int& y, int& w, int& h){
   CGRect r = CGDisplayBounds(displayId);
   CGPoint p = r.origin;
   CGSize s = r.size;
   x = p.x;
   y = p.y;
   w = s.width;
   h = s.height;
   //return Rectangle(p.x,p.y,p.width,p.height);
}

using namespace cv;

#include "glgrab.h"
Mat
Robot::capture(int displayId){
   CGRect rect = CGDisplayBounds(displayId);
   return capture(displayId,(int)rect.origin.x,(int)rect.origin.y,
                  (int)rect.size.width,(int)rect.size.height);
}

Mat
Robot::capture(int displayId, int x, int y, int w, int h){
   
   CGRect bounds = CGDisplayBounds(displayId);   
   
   // flip y coordinate vertically
   int y0 = bounds.size.height - y - h;
   CGRect rect = CGRectMake(x,y0,w,h);
   CGImageRef imgRef = grabViaOpenGL(displayId, rect);

   const int imgh = CGImageGetHeight(imgRef);
   const int imgw = CGImageGetWidth(imgRef);
   
   const int bpp = CGImageGetBitsPerPixel(imgRef); 
   const int bpr = CGImageGetBytesPerRow(imgRef);

   CFDataRef imageData = CGDataProviderCopyData(CGImageGetDataProvider(imgRef)); 
   UInt8* buffer = new UInt8[CFDataGetLength(imageData)];
   CFDataGetBytes(imageData, CFRangeMake(0,CFDataGetLength(imageData)), buffer);
   
   CGImageRelease(imgRef);
   
   IplImage* img = cvCreateImageHeader(cvSize(imgw,imgh),8,bpp/8); //create the "shell"   
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

