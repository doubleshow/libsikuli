/*
 *  location.h
 *  sikuli
 *
 *  Created by Tom Yeh on 8/5/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _LOCATION_H_
#define _LOCATION_H_

namespace sikuli{

class Location {
   
public: 
   
   Location() : x(-1), y(-1), screen(0) {};
   Location(int _x, int _y) : x(_x), y(_y){};
   Location(int _x, int _y, int _screen) : x(_x), y(_y), screen(_screen) {}
   Location(const Location& loc) : x(loc.x), y(loc.y), screen(loc.screen){};
   
   Location offset(int dx, int dy);
   Location left(int dx);
   Location right(int dx);
   Location above(int dy);
   Location below(int dy);
   
   int x;
   int y;
   int screen;

};

};
#endif