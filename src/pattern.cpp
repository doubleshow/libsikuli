/*
 *  pattern.cpp
 *  sikuli
 *
 *  Created by Tom Yeh on 8/5/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#define dout if (0) cout

#include "pattern.h"
#include "settings.h"

#include <vector>
using namespace std;
using namespace sikuli;


#include <stdio.h>
#include <curl/curl.h>
#include <curl/types.h>
#include <curl/easy.h>
#include <string>

#include "exceptions.h"
#include <iostream>

#include <sys/stat.h> 
bool FileExists(string strFilename) {
   struct stat stFileInfo;
   bool blnReturn;
   int intStat;
   
   // Attempt to get the file attributes
   intStat = stat(strFilename.c_str(),&stFileInfo);
   if(intStat == 0) {
      // We were able to get the file attributes
      // so the file obviously exists.
      blnReturn = true;
   } else {
      // We were not able to get the file attributes.
      // This may mean that we don't have permission to
      // access the folder which contains this file. If you
      // need to do that level of checking, lookup the
      // return values of stat which will give you
      // more details on why stat failed.
      blnReturn = false;
   }
   
   return(blnReturn);
}

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
   int written = fwrite(ptr, size, nmemb, stream);
   return written;
}

string downloadImageFromURL(string url){
   //CURL *curl;
   //FILE *fp;
   //CURLcode res;
   //char outfilename[FILENAME_MAX];
   //tmpnam(outfilename);
   //curl = curl_easy_init();
   //if (curl) {
   //   fp = fopen(outfilename,"wb");
   //   curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
   //   curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
   //   curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
   //   res = curl_easy_perform(curl);
   //   /* always cleanup */
   //   curl_easy_cleanup(curl);
   //   fclose(fp);
   //}
   //
   //return string(outfilename);
	return string();
}

string findImageHelper(string image_url) throw(FileNotFound){
   
   dout << "[Pattern] Resolving image url:  " << image_url << endl;
   
   string found_image_path;
   if (strncmp(image_url.c_str(), "http", 4) == 0)
      // reading the image from a web address
      found_image_path = downloadImageFromURL(image_url);
   else if (FileExists(image_url))
      // reading the image from the local file system
      return image_url;
   
   throw FileNotFound(image_url.c_str());
} 

string findImageFromPaths(string image_filename) throw(FileNotFound) {
   
   
   // First try to check the existence of the image using the filename as is   
   try {
      return findImageHelper(image_filename);
   }catch (...) {}
   
   // Then, try to read the image at each image path
   vector<const char*> image_paths = Settings::getImagePaths();
   
   for (int i=0; i<image_paths.size(); ++i){
      string image_filename_with_path = string(image_paths[i]) + 
      "\\" + string(image_filename);

      try { 
         return findImageHelper(image_filename_with_path);
      }catch (...) {}
   }
   
   throw FileNotFound(image_filename.c_str());
}




Pattern::Pattern(){
   str = "";
   similarity = 0.8f;
   dx = 0;
   dy = 0;
   bText = false;
   _bAll = false;
   _ordering = SCORE;
   _position = ANYWHERE;
   _limit = 999999;
   _imageURL = "";
   _resolved_imageURL = string("");
   
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
   
   _imageURL = p._imageURL;
   _resolved_imageURL = string(p._resolved_imageURL);
}

Pattern::Pattern(const char* str_){
   str = string(str_);
   
   bText = str.length()>3 && str[0] == '/' && str[str.length()-1] == '/';
   if (bText)
      str = str.substr(1, str.length()-2);
   else{
      _imageURL = str;
      _resolved_imageURL = findImageFromPaths(str);
   }
   
   similarity = 0.8f;
   dx = 0;
   dy = 0;   
   
   _bAll = false;
   _ordering = SCORE;
   _position = ANYWHERE;
   _limit = 999999;   
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
Pattern::limit(int n){
   Pattern ret(*this);
   if (n>0)
      ret._limit = max(1,n);  // can not be fewer than one match
   return ret;
}

Pattern 
Pattern::order(int ordering){
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
   return _resolved_imageURL.c_str();
}

int
Pattern::getOrdering(){
   return _ordering;
}

int
Pattern::getLimit(){
   return _limit;
}

const char* 
Pattern::getText() { 
   return str.c_str();
}

#include <sstream>
string 
Pattern::toString() const{
   stringstream ret;
   ret << "Pattern(\"" << str + "\")";
   ret << ".similar(" << similarity << ")";
   if(dx!=0 || dy!=0)
      ret << ".targetOffset(" << dx << "," << dy << ")";
   return ret.str();
}
