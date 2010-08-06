/*
 *  pattern.h
 *  sikuli
 *
 *  Created by Tom Yeh on 8/5/10.
 *  Copyright 2010 sikuli.org. All rights reserved.
 *
 */

#ifndef _PATTERN_H_
#define _PATTERN_H_

#include <string>
#include "location.h"

using namespace std;

namespace sikuli{
   
enum where{
   ANYWHERE,
   TOPMOST,
   BOTTOMMOST,
   LEFTMOST,
   RIGHTMOST
};
   
enum ordering{
   SCORE,
   TOPDOWN,
   BOTTOMUP,
   LEFTRIGHT,
   RIGHTLEFT
};

class Pattern{
   
public:
   
   Pattern();
   Pattern(const Pattern& p);
   Pattern(const char* str);
   
   Pattern all();
   Pattern one();
   
   Pattern similar(float similarity_);
   Pattern exact();
   Pattern targetOffset(int dx_, int dy_);
   
   Location getTargetOffset();
   float getSimilarity();
   
   
   Pattern topMost();
   Pattern bottomMost();
   Pattern leftMost();
   Pattern rightMost();
   
   // no effect if n < 1
   Pattern limit(int n);
   
   
   Pattern orderBy(int ordering);
   
   string toString();
   
   const char* getText();
   const char* getImageURL();
   
   bool isImageURL();
   bool isText();
   
   bool bAll() { return _bAll;};
   bool bOne() { return !_bAll;};
   
   int where() { return _position; };
   
   
   int getOrdering();
   int getLimit();
   
   //int getDesiredLocation();
   
private:
   
   bool bText;
   string str;
   float similarity;
   int dx;
   int dy;
   
   bool _bAll;
   int  _limit;
   int  _ordering;
   int  _position;
   
}; 

};
#endif // _PATTERN_H_