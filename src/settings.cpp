/*
 *  settings.cpp
 *  sikuli
 *
 *  Created by Tom Yeh on 8/5/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "settings.h"
#include <string>

using namespace sikuli;

int Settings::DelayBeforeDrop = 10;
int Settings::DelayAfterDrag = 10;
int Settings::WaitScanRate = 1;
bool Settings::ThrowException = true;
double Settings::AutoWaitTimeout = 3.0;
vector<string> Settings::_image_paths = vector<string>();

void
Settings::addImagePath(const char* image_path){
   _image_paths.push_back(string(image_path));
}

vector<const char*> 
Settings::getImagePaths(){
   vector<const char*> ret;
   for (int i=0; i<_image_paths.size(); ++i)
      ret.push_back(_image_paths[i].c_str());
   return ret;
}

void 
Settings::resetImagePaths(){
   _image_paths.clear();
}
