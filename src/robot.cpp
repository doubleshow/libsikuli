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


#include "keys.h"

int Robot::_modifiers = 0;
bool Robot::_dragged = false;



int
Robot::click(int screen, int x, int y, int buttons, int modifiers, bool dblClick){
   mouseMove(screen, x, y);
   delay(10);
   return click(buttons, modifiers, dblClick);
}

int
Robot::click(int x, int y, int buttons, int modifiers, bool dblClick){
   return click(0,x,y,buttons,modifiers,dblClick);
}


int
Robot::click(int buttons, int modifiers, bool dblClick){
   
   pressModifiers(modifiers);
   if( dblClick ){
      doubleClick(buttons);
   }else{
      singleClick(buttons);
   }
   releaseModifiers(modifiers);
   waitForIdle();
   return 1;
}

void 
Robot::pressModifiers(int modifiers){
   if(modifiers & SHIFT) Robot::keyPress(VK_SHIFT);
   if(modifiers & CTRL) Robot::keyPress(VK_CONTROL);
   if(modifiers & ALT) Robot::keyPress(VK_ALT);
   if(modifiers & CMD) Robot::keyPress(VK_META);
   if(modifiers & META) {
      //     if( Env.getOS() == OS.WINDOWS )
      //         Robot::keyPress(KeyEvent.VK_WINDOWS);
      //      else
      Robot::keyPress(VK_META);
   }
}

void 
Robot::releaseModifiers(int modifiers){
   if(modifiers & SHIFT) Robot::keyRelease(VK_SHIFT);
   if(modifiers & CTRL) Robot::keyRelease(VK_CONTROL);
   if(modifiers & ALT) Robot::keyRelease(VK_ALT);//
   if(modifiers & CMD) Robot::keyRelease(VK_META);   
   if(modifiers & META){ 
      //      if( Env.getOS() == OS.WINDOWS )
      //         Robot::keyRelease(KeyEvent.VK_WINDOWS);
      //      else
      Robot::keyRelease(VK_META);
   }
}


int 
Robot::hover(int x, int y){
   return hover(0,x,y);
}

int 
Robot::hover(int screen, int x, int y){
   mouseMove(screen,x,y);
   delay(100);
   return 1;
}


int
Robot::dragDrop(int screen1, int x1, int y1, int screen2, int x2, int y2, int modifiers){
   int ret = 0;
   pressModifiers(modifiers);
   if (drag(screen1,x1,y1) != 0){
      //delay((int) Settings::DelayAfterDrag*1000);
      ret = dropAt(screen2,x2,y2);
   }
   releaseModifiers(modifiers);
   return ret;   
}

int
Robot::dragDrop(int x1, int y1, int x2, int y2, int modifiers){
   return dragDrop(0,x1,y1,0,x2,y2,modifiers);
}

int
Robot::drag(int x, int y){
   return drag(0,x,y);
}

int
Robot::drag(int screen, int x, int y){
   mouseMove(screen,x,y);
   delay(100);
   drag();
   delay(100);
   return 1;  
}

int
Robot::dropAt(int x, int y, double seconds){
   return dropAt(0,x,y,seconds);
}

int
Robot::dropAt(int screen, int x, int y, double seconds){
   mouseMove(screen,x, y);
   delay((int) seconds*1000);
   drop();
   return 1;
}

int 
Robot::paste(const char* text){
   if (strlen(text)==0){
      return 0;
   }
   else{
      pasteText(text);
      return 1;
   }
}

int 
Robot::paste(int x, int y, const char* text){
   return paste(0,x,y,text);
}

int 
Robot::paste(int screen, int x, int y, const char* text){
   click(screen,x,y,BUTTON1_MASK,0,false); 
   delay(100);
   return paste(text);
}

int
Robot::press(int key, int modifiers){
   pressModifiers(modifiers);  
   type_key(key, PRESS_RELEASE);
   releaseModifiers(modifiers);
   waitForIdle();
   return 1;
}



int
Robot::type(const char* text, int modifiers){
   if (strlen(text) < 0)
      return 0;
   
   for (int i=0; i < strlen(text); i++){
      pressModifiers(modifiers);  
      type_ch(text[i], PRESS_RELEASE);
      releaseModifiers(modifiers);
      delay(20);
   }   
   waitForIdle();
   return 1;   
}

int
Robot::type(int x, int y, const char* text, int modifiers){
   return type(0,x,y,text,modifiers);
}

int
Robot::type(int screen, int x, int y, const char* text, int modifiers){
   click(screen,x,y,BUTTON1_MASK,0,false);
   delay(50);
   return type(text, modifiers);
}


void 
Robot::type_ch(char character, int mode){
   switch (character) {
      case 'a': doType(mode,VK_A); break;
      case 'b': doType(mode,VK_B); break;
      case 'c': doType(mode,VK_C); break;
      case 'd': doType(mode,VK_D); break;
      case 'e': doType(mode,VK_E); break;
      case 'f': doType(mode,VK_F); break;
      case 'g': doType(mode,VK_G); break;
      case 'h': doType(mode,VK_H); break;
      case 'i': doType(mode,VK_I); break;
      case 'j': doType(mode,VK_J); break;
      case 'k': doType(mode,VK_K); break;
      case 'l': doType(mode,VK_L); break;
      case 'm': doType(mode,VK_M); break;
      case 'n': doType(mode,VK_N); break;
      case 'o': doType(mode,VK_O); break;
      case 'p': doType(mode,VK_P); break;
      case 'q': doType(mode,VK_Q); break;
      case 'r': doType(mode,VK_R); break;
      case 's': doType(mode,VK_S); break;
      case 't': doType(mode,VK_T); break;
      case 'u': doType(mode,VK_U); break;
      case 'v': doType(mode,VK_V); break;
      case 'w': doType(mode,VK_W); break;
      case 'x': doType(mode,VK_X); break;
      case 'y': doType(mode,VK_Y); break;
      case 'z': doType(mode,VK_Z); break;
      case 'A': doType(mode,VK_SHIFT, VK_A); break;
      case 'B': doType(mode,VK_SHIFT, VK_B); break;
      case 'C': doType(mode,VK_SHIFT, VK_C); break;
      case 'D': doType(mode,VK_SHIFT, VK_D); break;
      case 'E': doType(mode,VK_SHIFT, VK_E); break;
      case 'F': doType(mode,VK_SHIFT, VK_F); break;
      case 'G': doType(mode,VK_SHIFT, VK_G); break;
      case 'H': doType(mode,VK_SHIFT, VK_H); break;
      case 'I': doType(mode,VK_SHIFT, VK_I); break;
      case 'J': doType(mode,VK_SHIFT, VK_J); break;
      case 'K': doType(mode,VK_SHIFT, VK_K); break;
      case 'L': doType(mode,VK_SHIFT, VK_L); break;
      case 'M': doType(mode,VK_SHIFT, VK_M); break;
      case 'N': doType(mode,VK_SHIFT, VK_N); break;
      case 'O': doType(mode,VK_SHIFT, VK_O); break;
      case 'P': doType(mode,VK_SHIFT, VK_P); break;
      case 'Q': doType(mode,VK_SHIFT, VK_Q); break;
      case 'R': doType(mode,VK_SHIFT, VK_R); break;
      case 'S': doType(mode,VK_SHIFT, VK_S); break;
      case 'T': doType(mode,VK_SHIFT, VK_T); break;
      case 'U': doType(mode,VK_SHIFT, VK_U); break;
      case 'V': doType(mode,VK_SHIFT, VK_V); break;
      case 'W': doType(mode,VK_SHIFT, VK_W); break;
      case 'X': doType(mode,VK_SHIFT, VK_X); break;
      case 'Y': doType(mode,VK_SHIFT, VK_Y); break;
      case 'Z': doType(mode,VK_SHIFT, VK_Z); break;
      case '`': doType(mode,VK_BACK_QUOTE); break;
      case '0': doType(mode,VK_0); break;
      case '1': doType(mode,VK_1); break;
      case '2': doType(mode,VK_2); break;
      case '3': doType(mode,VK_3); break;
      case '4': doType(mode,VK_4); break;
      case '5': doType(mode,VK_5); break;
      case '6': doType(mode,VK_6); break;
      case '7': doType(mode,VK_7); break;
      case '8': doType(mode,VK_8); break;
      case '9': doType(mode,VK_9); break;
      case '-': doType(mode,VK_MINUS); break;
      case '=': doType(mode,VK_EQUALS); break;
      case '~': doType(mode,VK_SHIFT, VK_BACK_QUOTE); break;
      case '!': doType(mode,VK_SHIFT, VK_1); break;
      case '@': doType(mode,VK_SHIFT, VK_2); break;
      case '#': doType(mode,VK_SHIFT, VK_3); break;
      case '$': doType(mode,VK_SHIFT, VK_4); break;
      case '%': doType(mode,VK_SHIFT, VK_5); break;
      case '^': doType(mode,VK_SHIFT, VK_6); break;
      case '&': doType(mode,VK_SHIFT, VK_7); break;
      case '*': doType(mode,VK_SHIFT, VK_8); break;
      case '(': doType(mode,VK_SHIFT, VK_9); break;
      case ')': doType(mode,VK_SHIFT, VK_0); break;
      case '_': doType(mode,VK_SHIFT, VK_MINUS); break;
      case '+': doType(mode,VK_SHIFT, VK_EQUALS); break;
      case '\b': doType(mode,VK_BACK_SPACE); break;
      case '\t': doType(mode,VK_TAB); break;
      case '\r': doType(mode,VK_ENTER); break;
      case '\n': doType(mode,VK_ENTER); break;
      case '[': doType(mode,VK_OPEN_BRACKET); break;
      case ']': doType(mode,VK_CLOSE_BRACKET); break;
      case '\\': doType(mode,VK_BACK_SLASH); break;
      case '{': doType(mode,VK_SHIFT, VK_OPEN_BRACKET); break;
      case '}': doType(mode,VK_SHIFT, VK_CLOSE_BRACKET); break;
      case '|': doType(mode,VK_SHIFT, VK_BACK_SLASH); break;
      case ';': doType(mode,VK_SEMICOLON); break;
      case ':': doType(mode,VK_SHIFT, VK_SEMICOLON); break;
      case '\'': doType(mode,VK_QUOTE); break;
      case '"': doType(mode,VK_SHIFT, VK_QUOTE); break;
      case ',': doType(mode,VK_COMMA); break;
      case '<': doType(mode,VK_SHIFT, VK_COMMA); break;
      case '.': doType(mode,VK_PERIOD); break;
      case '>': doType(mode,VK_SHIFT, VK_PERIOD); break;
      case '/': doType(mode,VK_SLASH); break;
      case '?': doType(mode,VK_SHIFT, VK_SLASH); break;
      case ' ': doType(mode,VK_SPACE); break;
   }
}

void 
Robot::type_key(int key, int mode){
   switch (key) {
      case ESC: doType(mode,VK_ESCAPE); break;
      case ENTER: doType(mode,VK_ENTER); break;
      case TAB: doType(mode, VK_TAB); break;
      case UP: doType(mode,VK_UP); break;
      case RIGHT: doType(mode,VK_RIGHT); break;
      case DOWN: doType(mode,VK_DOWN); break;
      case LEFT: doType(mode,VK_LEFT); break;
      case PAGE_UP: doType(mode,VK_PAGE_UP); break;
      case PAGE_DOWN: doType(mode,VK_PAGE_DOWN); break;
      case DELETE: doType(mode,VK_DELETE); break;
      case BACKSPACE: doType(mode,VK_BACK_SPACE); break;
      case END: doType(mode,VK_END); break;
      case HOME: doType(mode,VK_HOME); break;
      case INSERT: doType(mode,VK_INSERT); break;
      case CAPSLOCK: doType(mode,VK_CAPSLOCK); break;
      case SPACE: doType(mode,VK_SPACE); break;
      case F1: doType(mode,VK_F1); break;
      case F2: doType(mode,VK_F2); break;
      case F3: doType(mode,VK_F3); break;
      case F4: doType(mode,VK_F4); break;
      case F5: doType(mode,VK_F5); break;
      case F6: doType(mode,VK_F6); break;
      case F7: doType(mode,VK_F7); break;
      case F8: doType(mode,VK_F8); break;
      case F9: doType(mode,VK_F9); break;
      case F10: doType(mode,VK_F10); break;
      case F11: doType(mode,VK_F11); break;
      case F12: doType(mode,VK_F12); break;
      case F13: doType(mode,VK_F13); break;
      case F14: doType(mode,VK_F14); break;
      case F15: doType(mode,VK_F15); break;
   }
}

void 
Robot::doType(int mode, int keycode){
   if(mode==PRESS_ONLY){
      keyPress(keycode);
   }
   else if(mode==RELEASE_ONLY){
      keyRelease(keycode);
   }
   else{
      keyPress(keycode);
      keyRelease(keycode);
   }
}

void 
Robot::doType(int mode, int modifier, int keycode){
   if(mode==PRESS_ONLY){
      keyPress(modifier);
      keyPress(keycode);      
   }
   else if(mode==RELEASE_ONLY){
      keyRelease(keycode);      
      keyRelease(modifier);      
   }
   else{
      keyPress(modifier);
      keyPress(keycode);      
      keyRelease(keycode);
      keyRelease(modifier);  
   }
}

void 
Robot::mouseDown(int button){
   //_hold_buttons = buttons;
   mousePress(button);
   waitForIdle();
}

void 
Robot::mouseUp(int button){
   //if (buttons == 0)
   mouseRelease(button);//_hold_buttons);
   //else
   //   mouseRelease(buttons);
   waitForIdle();
}

void 
Robot::keyDown(int key){
//   
//   if (keys.length() > 0){
//      for (int i=0; i < keys.length(); ++i){
//         char key = keys[i];
//         // TODO: if this key has not already been pressed down
//         //if (_hold_keys.indexOf(keys.charAt(i)) == -1)
//         if (true){
//            //  type_ch(key, PRESS_ONLY);
//            _hold_keys += key;
//         }
//      }
//      Robot::waitForIdle();
//   }
}


// TODO: Re-implement using vector
void 
Robot::keyUp(int key){
 //  string keys){
 //  if (keys.empty())
//      keys = _hold_keys;
//   for (int i=0; i < keys.length(); ++i){
//      char key = keys[i];
//      int pos;
//      pos = 1;
//      // TODO:
//      // pos=_hold_keys.indexOf(keys.charAt(i))
//      if (pos != -1){
//         //   type_ch(key, RELEASE_ONLY);
//         // TODO: remove key from _hold_keys
//         //         _hold_keys = _hold_keys.substring(0,pos) + 
//         //         _hold_keys.substring(pos+1);
//      }
//   }
//   Robot::waitForIdle();
}


//
// OS-depdenet Implemntation
//

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
      //Robot::delay(50);
   }
   mouseMoveTo(x1,y1,dragged);
   //Robot::delay(50);
}


void 
Robot::mouseMove(int x, int y){
   mouseMove(0,x,y);
}

void 
Robot::mouseMove(int screen, int x, int y){
      
   CGPoint curloc;
   CGEventRef eventRef;
   
   eventRef = CGEventCreate(NULL);
   curloc = CGEventGetLocation(eventRef);
   CFRelease(eventRef);
   
   int x0,y0,w0,h0;
   getScreenBounds(screen, x0, y0, w0, h0);
   
   x += x0;
   y += y0;
   
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
   delay(50);
}

void 
Robot::pasteText(const char* text){
   
   // Put the string data to the pasteboard
   OSStatus err;
   PasteboardRef pbRef;
   err = PasteboardCreate(kPasteboardClipboard,
                          &pbRef);
   PasteboardClear(pbRef);
   
   // Convert c char array into CFString. We use kCFStringEncodingUTF8 
   // to tell the program to interpret the content of the char array
   // as in UTF8 encoding
   CFStringRef strRef;
   strRef = CFStringCreateWithCString(kCFAllocatorDefault, text,
                                      kCFStringEncodingUTF8);
   
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


int convert_user_screen_id_to_mac_display_id(int screen){
   CGDisplayErr err;
   CGDirectDisplayID ids[10];
   CGDisplayCount dspyCnt;
   err = CGGetActiveDisplayList(10,ids,&dspyCnt); 
   
   if (screen < dspyCnt)
      return ids[screen];
   else
      return 0;
}


void
Robot::getScreenBounds(int screen, int& x, int& y, int& w, int& h){
   CGDirectDisplayID dspyID;
   dspyID = convert_user_screen_id_to_mac_display_id(screen); 
   
   CGRect r = CGDisplayBounds(dspyID);
   CGPoint p = r.origin;
   CGSize s = r.size;
   x = p.x;
   y = p.y;
   w = s.width;
   h = s.height;
}

void
Robot::getScreenSize(int screen, int& w, int& h){
   CGDirectDisplayID dspyID;
   dspyID = convert_user_screen_id_to_mac_display_id(screen); 
   
   CGRect r = CGDisplayBounds(dspyID);
   CGSize s = r.size;
   w = s.width;
   h = s.height;
}

using namespace cv;

#include "glgrab.h"

Mat
Robot::capture(int screen){
   CGDirectDisplayID dspyID;
   dspyID = convert_user_screen_id_to_mac_display_id(screen); 
   
   CGRect rect = CGDisplayBounds(dspyID);
   return capture(screen,(int)rect.origin.x,(int)rect.origin.y,
                  (int)rect.size.width,(int)rect.size.height);
}

Mat
Robot::capture(int screen, int x, int y, int w, int h){
   
   CGDirectDisplayID dspyID;
   dspyID = convert_user_screen_id_to_mac_display_id(screen);
   
   CGRect bounds = CGDisplayBounds(dspyID);   
   
   // flip y coordinate vertically
   //int y0 = bounds.size.height - y - h;
   
   CGRect rect = CGRectMake(x+bounds.origin.x,y+bounds.origin.y,w,h);

   CGImageRef imgRef;
   imgRef = CGWindowListCreateImage(rect, 
                                    kCGWindowListOptionOnScreenOnly, 
                                    kCGNullWindowID, kCGWindowImageDefault);
   
   const int imgh = CGImageGetHeight(imgRef);
   const int imgw = CGImageGetWidth(imgRef);
   
   const int bpp = CGImageGetBitsPerPixel(imgRef); 
   const int bpr = CGImageGetBytesPerRow(imgRef);

   CFDataRef imageData = CGDataProviderCopyData(CGImageGetDataProvider(imgRef)); 
   UInt8* buffer = new UInt8[CFDataGetLength(imageData)];
   CFDataGetBytes(imageData, CFRangeMake(0,CFDataGetLength(imageData)), buffer);
   CGImageRelease(imgRef);
   CFRelease(imageData);
   
   
   
   Mat bgra(cv::Size(imgw,imgh), CV_8UC4, buffer, bpr);
   Mat bgr( bgra.rows, bgra.cols, CV_8UC3 );
   cvtColor(bgra,bgr,CV_RGBA2RGB);
   // 
   delete buffer;
   //namedWindow("test");
//   imshow("test",bgr);
//   waitKey(0);
//   
   return bgr;
   //
//   //
////   IplImage* img = cvCreateImageHeader(cvSize(imgw,imgh),8,bpp/8); //create the "shell"   
////   cvSetData(img, buffer, bpr);    //set the buffer
//   
//   //Mat argb(img);
//   Mat bgr( argb.rows, argb.cols, CV_8UC3 );
//   Mat alpha( argb.rows, argb.cols, CV_8UC1 );
//   
//   Mat out[] = { bgr, alpha };
//   int from_to[] = { 1,2,  2,1,  3,0,  0,3 };
//   mixChannels( &argb, 1, out, 2, from_to, 4 ); 
//   
//   namedWindow("test");
//   imshow("test",argb);
//   waitKey(0);
//// 
//   delete buffer;
//   //cvReleaseImageHeader(&img);
//   
//   return bgr;
}

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
