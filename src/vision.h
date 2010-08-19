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
#include "opencv2/opencv.hpp"
#include "finder.h"

namespace sikuli {

class Vision{
public:
   
   //static Match find(ScreenImage simg, Pattern ptn) throw(FindFailed);   
   //static vector<Match> findAll(ScreenImage simg, Pattern ptn) throw(FindFailed);
   static vector<FindResult> find(ScreenImage simg, Pattern ptn);

   static double compare(cv::Mat m1, cv::Mat m2);

   static void initOCR(const char* ocrDataPath) throw(FileNotFound);
   
};

}

#endif // _VISION_H_