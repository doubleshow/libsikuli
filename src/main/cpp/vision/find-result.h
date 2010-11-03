/*
 *  find-result.h
 *  sikuli
 *
 *  Created by Tom Yeh on 8/1/10.
 *  Copyright 2010 sikuli.org. All rights reserved.
 *
 */
#ifndef _FIND_RESULT_
#define _FIND_RESULT_

#include <string>

struct FindResult {
   int x, y;
   int w, h;
   double score;
   FindResult(){
      x=0;y=0;w=0;h=0;score=-1;text = "";
   }
   FindResult(int _x, int _y, int _w, int _h, double _score){
      x = _x; y = _y;
      w = _w; h = _h;
      score = _score;
      text = "";
   }
   
   std::string text;
};

#endif //_FIND_RESULT_