/*
 *  screen.h
 *  sikuli
 *
 *  Created by Tom Yeh on 7/30/10.
 *  Copyright 2010 sikuli.org. All rights reserved.
 *
 */

#ifndef _SCREEN_H_
#define _SCREEN_H_
#include "region.h"

namespace sikuli{


class Screen : public Region{
   
public:
   
   Screen();
   Screen(int id);
   ~Screen();
   
//   ScreenImage capture(int x, int y, int w, int h);
   
   //Rectangle getBounds();

   //static int getNumberScreens();
   
};



};

#endif // _SCREEN_H_