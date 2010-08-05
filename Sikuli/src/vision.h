/*
 *  vision.h
 *  sikuli
 *
 *  Created by Tom Yeh on 8/1/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _VISION_H_
#define _VISION_H_

#include "region.h"

namespace sikuli {

class Vision{
public:
   
   static Match find(ScreenImage simg, Pattern ptn) throw(FindFailed);   
   static vector<Match> findAll(ScreenImage simg, Pattern ptn) throw(FindFailed);
   
   static void initOCR(const char* ocrDataPath);
   
};

}

#endif // _VISION_H_