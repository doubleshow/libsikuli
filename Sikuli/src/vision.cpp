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

using namespace sikuli;

void
Vision::trainOCR(const char* trainingImagePath){
   Mat im = imread(trainingImagePath,1);
   TextFinder::train(im);
}

Match 
Vision::find(ScreenImage simg, Pattern ptn) throw(FindFailed){ 
   
   BaseFinder* f;
   
   if (ptn.isText()){
      f = new TextFinder(simg.getMat());
      f->find(ptn.getText(), 0.6);//ptn.getSimilarity());
   }else if (ptn.isImageURL()){
      //namedWindow("screen");
      imwrite("/tmp/screen.png",simg.getMat());
      f = new TemplateFinder(simg.getMat());
      f->find(ptn.getImageURL(), ptn.getSimilarity());      
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
   Finder f(simg.getMat());
   f.find(ptn.getImageURL(), ptn.getSimilarity());
   while (f.hasNext()){      
      FindResult r = f.next();
      ms.push_back(Match(r.x,r.y,r.w,r.h,r.score));
   }
   
   if (!ms.empty())
      return ms;
   else
      throw FindFailed(ptn);
}

