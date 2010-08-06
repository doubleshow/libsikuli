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

/*
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
*/
#include <stdio.h>
#include <curl/curl.h>
#include <curl/types.h>
#include <curl/easy.h>
#include <string>

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
   int written = fwrite(ptr, size, nmemb, stream);
   return written;
}

Mat imread_url(const char* url){
   CURL *curl;
   FILE *fp;
   CURLcode res;
   //const char* url = "http://localhost/aaa.txt";
   char outfilename[FILENAME_MAX];
   tmpnam(outfilename);
   curl = curl_easy_init();
   if (curl) {
      fp = fopen(outfilename,"wb");
      curl_easy_setopt(curl, CURLOPT_URL, url);//"http://localhost:4567/apple.png");
      curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
      curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
      res = curl_easy_perform(curl);
      /* always cleanup */
      curl_easy_cleanup(curl);
      fclose(fp);
   }
   
   Mat im = imread(outfilename);
   return im;
}

Mat imread_helper(const char* image_url){
   Mat im;
   if (strncmp(image_url, "http", 4) == 0)
      // reading the image from a web address
      im = imread_url(image_url);
   else 
      // reading the image from the local file system
      im = imread(image_url);
 
   return im;
}

Mat readImageFromPaths(const char* image_filename) throw(FindFailed) {

   Mat im;

   // First try to read the image using the filename as is
   im = imread_helper(image_filename);
   if (im.data != NULL)
      return im;
   
   // Then, try to read the image at each image path
   vector<const char*> image_paths = Settings::getImagePaths();
   
   for (int i=0; i<image_paths.size(); ++i){
      string fullpath = string(image_paths[i]) + 
      "/" + string(image_filename);
      
      im = imread_helper(fullpath.c_str());
      if (im.data != NULL)
         return im;
   }
   
   throw FindFailed(Pattern(image_filename));
}

vector<FindResult> 
getFindResults(TemplateFinder& f, const char* image_filename, bool all, double similarity) 
   throw(FindFailed) { 

   
   vector<FindResult> results;
      
   Mat image = readImageFromPaths(image_filename);
   
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

   if (results.empty())
      throw FindFailed(Pattern(image_filename));
   else
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
   
   TemplateFinder f(simg.getMat());   
   
   vector<FindResult> results;
   
   if (ptn.bAll()){
      
      results = getFindResults(f, ptn.getImageURL(), true, ptn.getSimilarity());
      
      
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
      
         results = getFindResults(f, ptn.getImageURL(), true, ptn.getSimilarity());      
            
         
         cout << "found " << results.size() << " matches." << endl;
         
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
      else {

         results = getFindResults(f, ptn.getImageURL(), false, ptn.getSimilarity());
            
      }
      
   }
   
   
   // Convert FindResults to Matches
   vector<Match> matches;
   int n = min((int)results.size(), (int)ptn.getLimit());
   for (int i=0; i< n; ++i){
      FindResult& r = results[i];
      matches.push_back(Match(r.x,r.y,r.w,r.h,r.score));
   }
   
   return matches;
}
/*
vector<Match> 
Vision::find1(ScreenImage simg, Pattern ptn) throw(FindFailed){ 
   
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
   
   vector<FindResult> results;
   while (f.hasNext()){      
      FindResult r = f.next();
      results.push_back(r);
   }
   
   
   // Filter Results
   if (ptn.where() != ANYWHERE && !results.empty()){
      
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
   
   
   
   // Convert FindResults to Matches
   vector<Match> matches;
   for (int i=0; i<results.size(); ++i){
      FindResult& r = results[i];
      matches.push_back(Match(r.x,r.y,r.w,r.h,r.score));
   }
   
   if (!matches.empty())
      return matches;
   else
      throw FindFailed(ptn);
}
*/
