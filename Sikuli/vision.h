/*
 *  vision.h
 *  sikuli
 *
 *  Created by Tom Yeh on 8/1/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */


#include "region.h"

namespace sikuli {
class Vision{
public:
   
   static Match find(ScreenImage simg, const char* search_string) throw(FindFailed); 
   static Match find(ScreenImage simg, Pattern ptn) throw(FindFailed);
   
   static vector<Match> findAll(ScreenImage simg, Pattern ptn) throw(FindFailed);
   
   static void trainOCR(const char* trainingImagePath);
   
};

}