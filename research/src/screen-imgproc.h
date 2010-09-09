/*
 *  screen-imgproc.h
 *  sikuli
 *
 *  Created by Tom Yeh on 9/8/10.
 *  Copyright 2010 sikuli.org. All rights reserved.
 *
 */

#include "opencv.hpp"


using namespace cv;


class Blob{
   
public:
   
   double area;
   Rect bound;
   
};


class cvgui {
   
public:
   
   static Mat removeGrayBackground(const Mat& input);
   static Mat obtainGrayBackgroundMask(const Mat& input);
   static void findLongLines(const Mat& binary, Mat& dest);
   static void findLongLines_Horizontal(const Mat& binary, Mat& dest);
   static void extractRects(const Mat& src, vector<Rect>& rects);

   
   static void extractBlobs(const Mat& src, vector<Blob>& blobs);

   
   static void extractSmallRects(const Mat& src, vector<Rect>& rects);
   
   
   static bool hasMoreThanNUniqueColors(const Mat& src, int n);
   
   
   static bool areHorizontallyAligned(const vector<Rect>& rects);
   
   
   
   static void segmentScreenshot(const Mat& screen, vector<Blob>& text_blobs, vector<Blob>& image_blobs);
      
};


