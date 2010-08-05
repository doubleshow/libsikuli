/*
 *  pattern.cpp
 *  sikuli
 *
 *  Created by Tom Yeh on 8/5/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "pattern.h"

using namespace sikuli;

Pattern::Pattern(){
   str = "";
   similarity = 0.8f;
   dx = 0;
   dy = 0;
   _bAll = false;
   _ordering = SCORE;
   _position = ANYWHERE;
   _limit = 1;
}

Pattern::Pattern(const Pattern& p){
   str = p.str;
   bText = p.bText;
   similarity = p.similarity;
   dx = p.dx;
   dy = p.dy;
   
   _bAll = p._bAll;
   _ordering = p._ordering;
   _position = p._position;
   _limit= p._limit;
}

Pattern::Pattern(const char* str_){
   str = string(str_);
   
   bText = str.length()>3 && str[0] == '/' && str[str.length()-1] == '/';
   if (bText)
      str = str.substr(1, str.length()-2);
   
   
   similarity = 0.8f;
   dx = 0;
   dy = 0;   
   
   _bAll = false;
   _ordering = SCORE;
   _position = ANYWHERE;
   _limit = 1;   
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
Pattern::orderBy(int ordering){
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
   return str.c_str();
}

int
Pattern::getOrdering(){
   return _ordering;
}

const char* 
Pattern::getText() { 
   return str.c_str();
}

#include <sstream>
string 
Pattern::toString(){
   stringstream ret;
   ret << "Pattern(\"" << str + "\")";
   ret << ".similar(" << similarity << ")";
   if(dx!=0 || dy!=0)
      ret << ".targetOffset(" << dx << "," << dy << ")";
   return ret.str();
}
