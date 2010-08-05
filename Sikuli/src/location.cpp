/*
 *  location.cpp
 *  sikuli
 *
 *  Created by Tom Yeh on 8/5/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "location.h"

using namespace sikuli;

Location 
Location::offset(int dx, int dy){
   return Location(x+dx, y+dy);
}

Location 
Location::left(int dx){
   return Location(x-dx, y);
}

Location 
Location::right(int dx){
   return Location(x+dx, y);
}

Location 
Location::above(int dy){
   return Location(x, y-dy);
}

Location 
Location::below(int dy){
   return Location(x, y+dy);
}