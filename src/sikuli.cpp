/*
 *  sikuli.cpp
 *  sikuli
 *
 *  Created by Tom Yeh on 8/5/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "sikuli.h"
#include "vision.h"

namespace sikuli{

void initOCR(const char* ocrData){
   Vision::initOCR(ocrData);
}

};