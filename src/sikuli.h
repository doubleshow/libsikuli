/*
 *  sikuli.h
 *  sikuli
 *
 *  Created by Tom Yeh on 8/5/10.
 *  Copyright 2010 sikuli.org. All rights reserved.
 *
 */

#include "region.h"
#include "keys.h"
#include "screen.h"
#include "settings.h"
#include "event-manager.h"

namespace sikuli{

   void initOCR(const char* ocrData);
   
   void switchApp(const char* app);
   
   void addImagePath(const char* imagePath);
   
   void wait(double seconds);
   
};

