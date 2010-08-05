/*
 *  vision.cpp
 *  sikuli
 *
 *  Created by Tom Yeh on 8/1/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "vision.h"
#include "finder.h"
#include "settings.h"

using namespace sikuli;

void
Vision::initOCR(const char* ocrDataPath){
   Mat im = imread(ocrDataPath,1);
   TextFinder::train(im);
}

Match 
Vision::find(ScreenImage simg, Pattern ptn) throw(FindFailed){ 
   
   BaseFinder* f;
   
   if (ptn.isText()){
      f = new TextFinder(simg.getMat());
      f->find(ptn.getText(), 0.6);//ptn.getSimilarity());
   }else if (ptn.isImageURL()){
      imwrite("/tmp/screen.png",simg.getMat());
      f = new TemplateFinder(simg.getMat());
      
         
      // TODO: make this implementation prettier and use os dependent 
      // file separators
      vector<const char*> image_paths = Settings::getImagePaths();
      
      for (int i=0; i<image_paths.size(); ++i){
         string image_url;
         image_url = string(image_paths[i]) + 
            "/" + ptn.getImageURL();
      
         try{
            f->find(image_url.c_str(), ptn.getSimilarity()); 
            break;
         }catch (cv::Exception) {
            
         }
      }
      
      
   }
   
   if (f->hasNext()){      
      FindResult r = f->next();
      delete f;
      return Match(r.x,r.y,r.w,r.h,r.score);
   }else {
      delete f;
      throw FindFailed(ptn);
   }   
   
}

vector<Match> 
Vision::findAll(ScreenImage simg, Pattern ptn) throw(FindFailed){ 
   vector<Match> ms;
   TemplateFinder f(simg.getMat());
   
   vector<const char*> image_paths = Settings::getImagePaths();
   
   for (int i=0; i<image_paths.size(); ++i){
      string image_url;
      image_url = string(image_paths[i]) + 
      "/" + ptn.getImageURL();
      
      try{
         f.find_all(image_url.c_str(), ptn.getSimilarity()); 
         break;
      }catch (cv::Exception) {
         
      }
   }
   
   
   while (f.hasNext()){      
      FindResult r = f.next();
      ms.push_back(Match(r.x,r.y,r.w,r.h,r.score));
   }
   
   if (!ms.empty())
      return ms;
   else
      throw FindFailed(ptn);
}

