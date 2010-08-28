/*
 *  screen-image.h
 *  sikuli
 *
 *  Created by Tom Yeh on 8/28/10.
 *  Copyright 2010 sikuli.org. All rights reserved.
 *
 */

#ifndef _SCREEN_IMAGE_H_
#define _SCREEN_IMAGE_H_

#include "opencv.hpp"
#include <string>

namespace sikuli{

class ScreenImage{
   
public:
   
   ScreenImage(){};
   ScreenImage(cv::Mat image) : _image(image){};
   
   std::string getFilename(){
      return _filename;
   }
   
   cv::Mat getMat() { return _image;}
   
   // save the image as the given filename
   void save(const char* filename);
   
   // show the image for the given number of seconds
   void show(int seconds);
   
   // show the image until the user presses a key
   void show();
   
private:
   
   std::string _filename;
   cv::Mat _image;
   
};

};

#endif // _SCREEN_IMAGE_H_