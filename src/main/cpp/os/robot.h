/*
 *  robot.h
 *  sikuli
 *
 *  Created by Tom Yeh on 7/27/10.
 *  Copyright 2010 sikuli.org. All rights reserved.
 *
 */
#ifndef _SIKULI_ROBOT_
#define _SIKULI_ROBOT_

#include <string>

#include <opencv.hpp>

using namespace std;

namespace sikuli{
// BUTTON
#define BUTTON1_MASK 1
#define BUTTON2_MASK 2  
#define BUTTON3_MASK 4   

#ifdef WINDOWS

// http://msdn.microsoft.com/en-us/library/dd375731%28v=VS.85%29.aspx

#define VK_A   0x41
#define VK_B   0x42
#define VK_C   0x43
#define VK_D   0x44
#define VK_E   0x45
#define VK_F   0x46
#define VK_G   0x47
#define VK_H   0x48
#define VK_I   0x49
#define VK_J   0x4A
#define VK_K   0x4B
#define VK_L   0x4C
#define VK_M   0x4D
#define VK_N   0x4E
#define VK_O   0x4F
#define VK_P   0x50
#define VK_Q   0x51
#define VK_R   0x52
#define VK_S   0x53
#define VK_T   0x54
#define VK_U   0x55
#define VK_V   0x56
#define VK_W   0x57
#define VK_X   0x58
#define VK_Y   0x59
#define VK_Z   0x5A

#define VK_BACK_QUOTE 0xC0
#define VK_BACK_SLASH 0xDC
#define VK_OPEN_BRACKET  0xDB
#define VK_CLOSE_BRACKET 0xDD

#define VK_BACK_SPACE VK_BACK
#define VK_MINUS 0xBD
#define VK_EQUALS 0xBB
#define VK_QUOTE 0xDE
#define VK_COMMA 0xBC
#define VK_PERIOD 0xBE
#define VK_SLASH 0xBF
#define VK_ENTER 0x0D
#define VK_SEMICOLON 0xBA

#define VK_0   0x30
#define VK_1   0x31
#define VK_2   0x32
#define VK_3   0x33
#define VK_4   0x34
#define VK_5   0x35
#define VK_6   0x36
#define VK_7   0x37
#define VK_8   0x38
#define VK_9   0x39

#define VK_PAGE_UP VK_PRIOR
#define VK_PAGE_DOWN VK_NEXT
#define VK_CAPSLOCK VK_CAPITAL

#endif

#ifdef MAC

// VIRTUAL KEYS
// Mac: keyboard map
// http://boredzo.org/blog/wp-content/uploads/2007/05/imtx-virtual-keycodes.png

#define VK_A   0
#define VK_B   11
#define VK_C   8
#define VK_D   2
#define VK_E   14
#define VK_F   3
#define VK_G   5
#define VK_H   4
#define VK_I   34
#define VK_J   38
#define VK_K   40
#define VK_L   37
#define VK_M   46
#define VK_N   45
#define VK_O   31
#define VK_P   35
#define VK_Q   12
#define VK_R   15
#define VK_S   1
#define VK_T   17
#define VK_U   32
#define VK_V   9
#define VK_W   13
#define VK_X   7
#define VK_Y   16
#define VK_Z   6
#define VK_BACK_QUOTE   50
#define VK_0   29
#define VK_1   18
#define VK_2   19
#define VK_3   20
#define VK_4   21
#define VK_5   23
#define VK_6   22
#define VK_7   26
#define VK_8   28
#define VK_9   25
#define VK_MINUS  27
#define VK_EQUALS 24
#define VK_BACK_SPACE   51
#define VK_TAB 48
#define VK_ENTER  36
#define VK_RETURN 52
#define VK_OPEN_BRACKET 33
#define VK_CLOSE_BRACKET 30
#define VK_BACK_SLASH   42
#define VK_SEMICOLON 41
#define VK_QUOTE  39
#define VK_COMMA  43
#define VK_PERIOD 47
#define VK_SLASH  44
#define VK_SPACE  49
#define VK_ESCAPE 53
#define VK_UP  126
#define VK_RIGHT  124
#define VK_DOWN   125
#define VK_LEFT  123
#define VK_PAGE_UP   116
#define VK_PAGE_DOWN 121
#define VK_DELETE 117
#define VK_END 119
#define VK_HOME   115
#define VK_INSERT 114
#define VK_F1     122
#define VK_F2  120
#define VK_F3  99
#define VK_F4  118
#define VK_F5  96
#define VK_F6  97
#define VK_F7  98
#define VK_F8  100
#define VK_F9  101
#define VK_F10 109
#define VK_F11 103
#define VK_F12 111
#define VK_F13 105
#define VK_F14 107
#define VK_F15 113
#define VK_CAPSLOCK 57
#define VK_SHIFT  56
#define VK_CONTROL   59
#define VK_ALT 58
#define VK_META   55

#endif

// TYPE MODE
#define PRESS_RELEASE 0
#define PRESS_ONLY 1
#define RELEASE_ONLY 2


class Robot{
   
   // OS-dependent implementation   
private:
   
   static void mouseMove(int screen, int x, int y);
   static void mouseMove(int x, int y);
   
   
   static void mousePress(int buttons);
   static void mouseRelease(int buttons);
   static void keyPress(int key);
   static void keyRelease(int key);
   
   static void singleClick(int button);
   static void doubleClick(int button);
   static void drag();
   static void drop();
   static void pasteText(const char* text);
   
   static void waitForIdle();
   
public:

   static void openApp(const char* appname);
   static void delay(int seconds);
   
   static cv::Mat capture(int screen);   
   static cv::Mat capture(int screen, int x, int y, int w, int h);
   
   static void getScreenBounds(int screen, int& x, int& y, int& w, int& h);
   static void getScreenSize(int screen, int& w, int& h);
   
   static void getTopWindowBounds(int& x, int& y, int& w, int& h);
   
   
   // get the location of the mouse cursor in the desktop coordinate system
   static void getMouseLocation(int& x_out, int& y_out);
   
   
   // Cross-platform implementation
public:
   
   // get the location of the mouse cursor in the screen coordinate system
   static void getMouseLocation(int screen, int& x_out, int& y_out);

   
   static int click(int buttons, int modifiers, bool dblClick);
   static int click(int x, int y, int buttons, int modifiers, bool dblClick);
   static int click(int screen, int x, int y, int buttons, int modifiers, bool dblClick);
   
   static int dragDrop(int x1, int y1, int x2, int y2, int modifiers = 0);
   static int dragDrop(int screen1, int x1, int y1, int screen2, int x2, int y2, int modifiers = 0);
   
   static int drag(int x, int y);
   static int drag(int screen, int x, int y);

   static int dropAt(int x, int y, double delay = 0);
   static int dropAt(int screen, int x, int y, double delay = 0);
   
   static int hover(int x, int y);
   static int hover(int screen, int x, int y);
   
   static int type(const char* text, int modifiers = 0);
   static int type(int x, int y, const char* text, int modifiers = 0);
   static int type(int screen, int x, int y, const char* text, int modifiers = 0);
   
   static int press(int key, int modifiers = 0);
   static int paste(const char* text);
   static int paste(int x, int y, const char* text);
   static int paste(int screen, int x, int y, const char* text);
      
   static void mouseDown(int button);
   static void mouseUp(int button);
   
   static void keyDown(int key);
   static void keyUp(int key);
   
private:
   
   static void pressModifiers(int modifiers);
   static void releaseModifiers(int modifiers);

   static void doType(int mode, int keyCode);
   static void doType(int mode, int keyCode1, int keycode2);
   
   static void type_ch(char character, int mode);  
   static void type_key(int key, int mode);
   
   
private:
   
   // not sure whether these two can be cross-platform
   static void mouseMoveTo(int x, int y, bool dragged);
   static void mouseMoveFromTo(int x0, int y0, int x1, int y1, bool dragged);

   
private:
   
   static int _modifiers;
   static bool _dragged;
   
};

};

#endif