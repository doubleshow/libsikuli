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
}

Pattern::Pattern(const Pattern& p){
   str = p.str;
   bText = p.bText;
   similarity = p.similarity;
   dx = p.dx;
   dy = p.dy;
}

Pattern::Pattern(const char* str_){
   str = string(str_);
   
   bText = str.length()>3 && str[0] == '/' && str[str.length()-1] == '/';
   if (bText)
      str = str.substr(1, str.length()-2);
   
   
   similarity = 0.8f;
   dx = 0;
   dy = 0;   
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
