/*
 *  vision.cpp
 *  sikuli
 *
 *  Created by Tom Yeh on 8/1/10.
 *  Copyright 2010 sikuli.org. All rights reserved.
 *
 */

#include "vision.h"
#include "finder.h"
#include "settings.h"
#include "exceptions.h"

using namespace sikuli;

void
Vision::initOCR(const char* ocrDataPath) throw(FileNotFound) {
   Pattern p(ocrDataPath);
   Mat im = imread(p.getImageURL(),1);
   TextFinder::train(im);
}

vector<FindResult> 
getFindResults(Mat screenImage, Pattern ptn, bool all, double similarity) {
   
   vector<FindResult> results;
   
   if (ptn.isText()){
      
      
      TextFinder f(screenImage);      
      f.find(ptn.getText(), similarity);
      
      if (all){
         while (f.hasNext()){
            results.push_back(f.next());
         }
      }
      else{
         if (f.hasNext())
            results.push_back(f.next());
      }
      
      
      
   }else{
      
      TemplateFinder f(screenImage);      
      Mat image = imread(ptn.getImageURL());
      
      if (all){
         f.find_all(image, similarity);
         while (f.hasNext()){
            results.push_back(f.next());
         }
      }
      else{
         f.find(image, similarity);
         if (f.hasNext())
            results.push_back(f.next());
         
      }
   }
   return results;
}


bool sort_by_y_asc(const FindResult& r1, const FindResult& r2){
   return r1.y < r2.y;
}
bool sort_by_y_dsc(const FindResult& r1, const FindResult& r2){
   return r1.y > r2.y;
}
bool sort_by_x_asc(const FindResult& r1, const FindResult& r2){
   return r1.x < r2.x;
}
bool sort_by_x_dsc(const FindResult& r1, const FindResult& r2){
   return r1.x > r2.x;
}

vector<Match> 
Vision::find(ScreenImage simg, Pattern ptn) throw(FindFailed){ 
   
   //TemplateFinder f(simg.getMat());   
   
   Mat screenImage = simg.getMat();
   
//   Mat image = imread(ptn.getImageURL());
//   if (image.data == NULL)
//      throw FindFailed(ptn);
   
   vector<FindResult> results;
   if (ptn.bAll()){
      
      results = getFindResults(screenImage, ptn, true, ptn.getSimilarity());
      
      
      if (ptn.getOrdering() == TOPDOWN){
         sort(results.begin(), results.end(), sort_by_y_asc);
      }else if (ptn.getOrdering() == BOTTOMUP){
         sort(results.begin(), results.end(), sort_by_y_dsc);
      }else if (ptn.getOrdering() == LEFTRIGHT){
         sort(results.begin(), results.end(), sort_by_x_asc);
      }else if (ptn.getOrdering() == RIGHTLEFT){
         sort(results.begin(), results.end(), sort_by_x_dsc);
      }
   
   
   }else{
      
      if (ptn.where() != ANYWHERE){
      
         results = getFindResults(screenImage, ptn, true, ptn.getSimilarity());      
            
         
         cout << "found " << results.size() << " matches." << endl;
         
         if (!results.empty()){
            // Filter Results
            FindResult r = results[0];
            for (int i=1; i<results.size(); ++i){
               FindResult& ri = results[i];
               if ((ptn.where() == TOPMOST && ri.y < r.y) ||
                   (ptn.where() == BOTTOMMOST && ri.y > r.y) ||
                   (ptn.where() == LEFTMOST && ri.x < r.x) ||
                   (ptn.where() == RIGHTMOST && ri.x > r.x))
                  r = ri;
            }
            
            results.clear();
            results.push_back(r);
         }
      }
      else {

         results = getFindResults(screenImage, ptn, false, ptn.getSimilarity());
            
      }
      
   }
   
   
   if (results.empty())
      throw FindFailed(ptn);
   
   
   // Convert FindResults to Matches
   vector<Match> matches;
   int n = min((int)results.size(), (int)ptn.getLimit());
   for (int i=0; i< n; ++i){
      FindResult& r = results[i];
      matches.push_back(Match(r.x,r.y,r.w,r.h,r.score));
   }
   
   return matches;
}
