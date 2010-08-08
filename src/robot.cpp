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
bool Robot::_dragged = false;

#include <time.h>
void
Robot::delay(int msec){
   usleep(1000*msec);
}

void
Robot::mouseMoveTo(int x, int y, bool dragged)
{
   CGPoint newloc;
   CGEventRef eventRef;
   newloc.x = x;
   newloc.y = y;
   
   if(dragged){
      
      eventRef = CGEventCreateMouseEvent(NULL, kCGEventLeftMouseDragged, newloc,
                                         kCGMouseButtonCenter);
      CGEventSetType(eventRef, kCGEventLeftMouseDragged);
      CGEventPost(kCGSessionEventTap, eventRef);
      CFRelease(eventRef);  
      
   }else{
      
      eventRef = CGEventCreateMouseEvent(NULL, kCGEventMouseMoved, newloc,
                                         kCGMouseButtonCenter);
      CGEventSetType(eventRef, kCGEventMouseMoved);
      CGEventPost(kCGSessionEventTap, eventRef);
      CFRelease(eventRef);
   }
}
   
void 
Robot::mouseMoveFromTo(int x0, int y0, int x1, int y1, bool dragged){
   
   int stepsize = 50;
   
   int xsteps = (x1-x0)/stepsize;
   int ysteps = (y1-y0)/stepsize;
   
   int steps = max(max(abs(xsteps),abs(ysteps)),1);
 
   int xstep = (x1-x0)/steps;
   int ystep = (y1-y0)/steps;
   
   for (int i=0; i < steps; i++){
      int xi = x0 + i * xstep;
      int yi = y0 + i * ystep;
      mouseMoveTo(xi,yi,dragged);
      Robot::delay(50);
   }
   mouseMoveTo(x1,y1,dragged);
   Robot::delay(50);
}

void 
Robot::mouseMove(int x, int y)
{
   
   
   CGPoint curloc;
   CGEventRef eventRef;
   
   eventRef = CGEventCreate(NULL);
   curloc = CGEventGetLocation(eventRef);
   CFRelease(eventRef);
   
   
   mouseMoveFromTo(curloc.x,curloc.y,x,y,_dragged);
}

void 
Robot::mousePress(int buttons){
   CGPoint curloc;
   CGEventRef eventRef;
   
   CGEventRef ourEvent = CGEventCreate(NULL);
   curloc = CGEventGetLocation(ourEvent);
   CFRelease(ourEvent);
   
   
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
   _dragged = true;
}

void
Robot::drop(){
   mouseRelease(BUTTON1_MASK);
   _dragged = false;
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
Robot::waitForIdle(){
   //TODO:
}

void 
Robot::paste(const char* text){
   
   // Put the string data to the pasteboard
   OSStatus err;
   PasteboardRef pbRef;
   err = PasteboardCreate(kPasteboardClipboard,
                          &pbRef);
   PasteboardClear(pbRef);
   CFStringRef strRef;
   strRef = CFStringCreateWithCString(kCFAllocatorDefault,
                                   text,
                                   kCFStringEncodingMacRoman);
   CFDataRef dataRef;
   dataRef = CFStringCreateExternalRepresentation(NULL, strRef,                                                 
                                                 kCFStringEncodingUTF8, '?');
   
   PasteboardPutItemFlavor(pbRef, (PasteboardItemID)1, 
                           kUTTypeUTF8PlainText, dataRef, 
                           kPasteboardFlavorNoFlags);
   
   CFRelease(pbRef);
   CFRelease(strRef);
   CFRelease(dataRef);
   
   
   // Simulate CMD+V to paste the text at the current cursor location
   {
      CGEventRef eventRef;
      eventRef = CGEventCreateKeyboardEvent(NULL, (CGKeyCode)VK_V, true);
      CGEventFlags flags = CGEventGetFlags(eventRef);
      flags |= kCGEventFlagMaskCommand;
      CGEventSetFlags(eventRef, flags);
      CGEventPost(kCGSessionEventTap, eventRef);
      CFRelease(eventRef);
   }   
   {
      CGEventRef eventRef;
      eventRef = CGEventCreateKeyboardEvent(NULL, (CGKeyCode)VK_V, false);
      CGEventFlags flags = CGEventGetFlags(eventRef);
      flags |= kCGEventFlagMaskCommand;
      CGEventSetFlags(eventRef, flags);
      CGEventPost(kCGSessionEventTap, eventRef);
      CFRelease(eventRef);
   }

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
//#include <Carbon/Carbon.h>
//#include "LSInfo.h"
void
Robot::openApp(const char* appname){
   OSStatus err;
   FSRef appRef;
   

   // Convert c string to CFStringRef
   // http://www.carbondev.com/site/?page=CStrings+
   CFStringRef 
   str = CFStringCreateWithCString(kCFAllocatorDefault,
                                   appname,
                                   kCFStringEncodingMacRoman);

   // Follow example code at
   // http://developer.apple.com/mac/library/samplecode/LoginItemsAE/Listings/LoginItemsAE_c.html#//apple_ref/doc/uid/DTS10003788-LoginItemsAE_c-DontLinkElementID_4
   
   LSFindApplicationForInfo(kLSUnknownCreator,
                            NULL,
                            str,
                            &appRef,
                            NULL
                            ); 
   
   LSApplicationParameters     appParams;
   memset(&appParams, 0, sizeof(appParams));
   appParams.version = 0;
   appParams.flags = kLSLaunchDefaults;
   appParams.application = &appRef;

   ProcessSerialNumber psnPtr;
   err = LSOpenApplication(&appParams, &psnPtr);
   
   
   CFRelease(str);
}

/* Some useful windows code examples 
 
 #include <windows.h>    //windows api is nice things
 
 //text to send
 LPCTSTR sText = "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<";
 
 int main()
 {
 if (!OpenClipboard(NULL)){ return 1; }    //if failed to open clipboard, die
 EmptyClipboard();    //empty the clipboard
 
 HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, (lstrlen(sText)+1));    //allocate memory as big as the text-string
 LPTSTR sMem = (TCHAR*)GlobalLock(hMem);        //make memory-data space, lock the memory
 memcpy(sMem, sText, (lstrlen(sText)+1));    //copy text-data into memory-data
 GlobalUnlock(hMem);        //unlock the memory
 SetClipboardData(CF_TEXT, hMem);    //put the data (text) to the clipboard
 
 CloseClipboard();    //we don't want to put anymore data to it so..
 
 while(!GetAsyncKeyState(VK_F5)){ Sleep(100); }    //wait until F5 is pressed
 
 bool bEnd = false;    //wheter to stop spamming
 while( bEnd == false ){    //while not told to stop, run the loop
 if(GetAsyncKeyState(VK_F8)){ bEnd = !bEnd; }    //if F8 is pressed, stop spamming
 
 //CTRL+V is standard Windows-shortcut for Paste
 keybd_event(VK_CONTROL, 0, 0, 0);    //press down CTRL (control)
 keybd_event(VkKeyScan('v'),0xB2,0 , 0);    //press down the key 'v'
 keybd_event(VkKeyScan('v'),0xB2, KEYEVENTF_KEYUP,0);    //release the key 'v'
 keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0);    //release CTRL (control)
 
 //and press Return to send text (in MSN)
 keybd_event(VK_RETURN, 0, 0, 0);    //press down Return-key
 keybd_event(VK_RETURN, 0, KEYEVENTF_KEYUP, 0);    //release Return-key
 }
 
 EmptyClipboard();    //empty the clipboard
 
 return 0;
 } 
 
 */
