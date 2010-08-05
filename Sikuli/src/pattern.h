/*
 *  pattern.h
 *  sikuli
 *
 *  Created by Tom Yeh on 8/5/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _PATTERN_H_
#define _PATTERN_H_

#include <string>
#include "location.h"

using namespace std;

namespace sikuli{
   
   
class Pattern{
   
public:
   
   Pattern();
   Pattern(const Pattern& p);
   Pattern(const char* str);
   
   Pattern similar(float similarity_);
   Pattern exact();
   Pattern targetOffset(int dx_, int dy_);
   
   Location getTargetOffset();
   float getSimilarity();
   
   
   string toString();
   
   const char* getText();
   const char* getImageURL();
   
   bool isImageURL();
   bool isText();
   
private:
   
   bool bText;
   string str;
   float similarity;
   int dx;
   int dy;
   
}; 

};
#endif // _PATTERN_H_