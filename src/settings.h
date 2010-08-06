/*
 *  settings.h
 *  sikuli
 *
 *  Created by Tom Yeh on 8/5/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef _SETTINGS_H_
#define _SETTINGS_H_

#include <vector>
using namespace std;

namespace sikuli{
   
   class Settings{
   public:
      static int DelayAfterDrag;
      static int DelayBeforeDrop;
      static int WaitScanRate;
      static bool ThrowException;
      static double AutoWaitTimeout;
      
      
      static void addImagePath(const char* image_path);
      static vector<const char*> getImagePaths();
      static void resetImagePaths();
      
   private:
      
      static vector<string> _image_paths;
   };
   
};

#endif //_SETTINGS_H_