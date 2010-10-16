/*
 *  keys.h
 *  sikuli
 *
 *  Created by Tom Yeh on 7/31/10.
 *  Copyright 2010 Sikuli. All rights reserved.
 *
 */

#ifndef _KEYS_H_
#define _KEYS_H_


#ifdef DELETE
#undef DELETE
#endif   

enum specialKeys{
   ALT = 1,
   CMD = 2,
   CTRL = 4,
   META = 8,
   SHIFT = 16,
   WIN = 32,
   ENTER,
   TAB,
   ESC,
   INSERT,
   BACKSPACE,
   DELETE,
   CAPSLOCK,
   SPACE,
   F1,
   F2,
   F3,
   F4,
   F5,
   F6,
   F7,
   F8,
   F9,
   F10,
   F11,
   F12,
   F13,
   F14,
   F15,
   HOME,
   END,
   LEFT,
   RIGHT,
   DOWN,
   UP,
   PAGE_DOWN,
   PAGE_UP
}; 


#endif // _KEYS_H_