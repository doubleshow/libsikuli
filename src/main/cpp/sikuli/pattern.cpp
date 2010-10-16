/*
 *  pattern.cpp
 *  sikuli
 *
 *  Created by Tom Yeh on 8/5/10.
 *  Copyright 2010 sikuli.org. All rights reserved.
 *
 */

#include <string>
#include <iostream>
#include <vector>

#include "opencv.hpp"


#include "ui.h"
#include "pattern.h"
#include "settings.h"

using namespace std;
using namespace sikuli;

Pattern::Pattern(){
   init();
}

Pattern::Pattern(const Pattern& p){
   (*this) = p;
}

Pattern::Pattern(const char* str_){
   init();
   str = string(str_);   
   bText = str.length()>3 && str[0] == '/' && str[str.length()-1] == '/';
   if (bText)
      str = str.substr(1, str.length()-2);
   else{
      _imageURL = str;
      _resolved_imageURL = string(ImageReadHelper::instance()->resolveImageFilename(str));
      _screen_image = ScreenImage(cv::imread(getImageURL()));

   }
}

Pattern::Pattern(ScreenImage simg){
   init();
   _screen_image = simg;
   str = "<ScreenImage>";
}

void
Pattern::init(){
   str = "";
   similarity = 0.8f;
   dx = 0;
   dy = 0;
   bText = false;
   _bAll = false;
   _ordering = SCORE;
   _position = ANYWHERE;
   _limit = 999999;
   _imageURL = "";
   _resolved_imageURL = string("");
   _screen_image = ScreenImage();   
}

ScreenImage
Pattern::getScreenImage(){
   return _screen_image;
}

bool
Pattern::isText(){
   return bText;
}

bool
Pattern::isImageURL(){
   return !isText();
}

Pattern 
Pattern::similar(float similarity_){
   Pattern ret(*this);
   ret.similarity = similarity_;
   return ret;
}

Pattern 
Pattern::exact(){
   Pattern ret(*this);
   ret.similarity = 1.0f;
   return ret;
}

Pattern 
Pattern::targetOffset(int dx_, int dy_){
   Pattern ret(*this);
   ret.dx = dx_;
   ret.dy = dy_;
   return ret;
}


Pattern 
Pattern::all(){
   Pattern ret(*this);
   ret._bAll = true;
   return ret;
}

Pattern 
Pattern::one(){
   Pattern ret(*this);
   ret._bAll = false;
   return ret;
}

Pattern 
Pattern::limit(int n){
   Pattern ret(*this);
   if (n>0)
      ret._limit = max(1,n);  // can not be fewer than one match
   return ret;
}

Pattern 
Pattern::order(int ordering){
   Pattern ret(*this);
   ret._ordering = ordering;
   return ret;
}

Pattern 
Pattern::topMost(){
   Pattern ret(*this);
   ret._position = TOPMOST;
   return ret;
}

Pattern 
Pattern::bottomMost(){
   Pattern ret(*this);
   ret._position = BOTTOMMOST;
   return ret;
}

Pattern 
Pattern::leftMost(){
   Pattern ret(*this);
   ret._position = LEFTMOST;
   return ret;
}


Pattern 
Pattern::rightMost(){
   Pattern ret(*this);
   ret._position = RIGHTMOST;
   return ret;
}

Location 
Pattern::getTargetOffset(){
   return Location(dx, dy);
}

float
Pattern::getSimilarity(){
   return similarity;
}

const char* 
Pattern::getImageURL() { 
   return _resolved_imageURL.c_str();
}

int
Pattern::getOrdering(){
   return _ordering;
}

int
Pattern::getLimit(){
   return _limit;
}

const char* 
Pattern::getText() { 
   return str.c_str();
}

#include <sstream>
string 
Pattern::tostring() const{
   stringstream ret;
   ret << "Pattern(\"" << str + "\")";
   ret << ".similar(" << similarity << ")";
   if(dx!=0 || dy!=0)
      ret << ".targetOffset(" << dx << "," << dy << ")";
   return ret.str();
}
