/*
 *  ui.h
 *  sikuli
 *
 *  Created by Tom Yeh on 8/20/10.
 *  Copyright 2010 sikuli.org. All rights reserved.
 *
 */

#ifndef _UI_H_
#define _UI_H_


#include "region.h"
#include "pattern.h"

namespace sikuli{

   enum{
      SKIP,
      RETRY,
      ABORT
   };
   

   
class SikuliUI{
   
public:
   
   SikuliUI(){};
   virtual ~SikuliUI(){};
   
   virtual int handleFindFailedException(Region region, 
                                         Pattern target);
   
   virtual int handleMatchFound(Region region,
                                Pattern target,
                                std::vector<Match> matches);

   static void set(SikuliUI* ui);
   static SikuliUI* sikuliUI;
   
private:
   
};
   
class ImageReadHelper{
   
public:
   
   ImageReadHelper(){};
   virtual ~ImageReadHelper(){};
   virtual string resolveImageFilename(string image_filename);
   
   
   static ImageReadHelper* instance();
   static void set(ImageReadHelper* ptr);

private:
   
   static ImageReadHelper* ptr;
   
};
   

   

};


#endif // _UI_H_
