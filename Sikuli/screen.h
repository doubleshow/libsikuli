/*
 *  screen.h
 *  sikuli
 *
 *  Created by Tom Yeh on 7/30/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "region.h"

namespace sikuli{


class Screen : public Region{
   
public:
   
   Screen();
   Screen(int id);
   
   ScreenImage capture(int x, int y, int w, int h);
   
   Rectangle getBounds();

   static int getNumberScreens();
   
private:
   
   int _id;
};



};