/*
 *  sikuli.cpp
 *  sikuli
 *
 *  Created by Tom Yeh on 8/5/10.
 *  Copyright 2010 sikuli.org. All rights reserved.
 *
 */

#include "sikuli.h"
#include "vision.h"

namespace sikuli{

void initOCR(const char* ocrData){
   Vision::initOCR(ocrData);
}
   
void switchApp(const char* appName){
   Robot::openApp(appName);
}
   
void addImagePath(const char* imagePath){
   Settings::addImagePath(imagePath);
}
 
void wait(double seconds){
   Robot::delay(seconds*1000);
}
      
      
}