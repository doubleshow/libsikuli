/*
 *  ui.cpp
 *  sikuli
 *
 *  Created by Tom Yeh on 8/20/10.
 *  Copyright 2010 sikuli.org. All rights reserved.
 *
 */

#include "ui.h"
#include <iostream>

using namespace sikuli;
using namespace std;

//SikuliUI* 
//sikuliUI = 0;

SikuliUI* SikuliUI::sikuliUI = 0;

class SikuliInitCleanup{
   
public:
   SikuliInitCleanup(){
      SikuliUI::sikuliUI  = new SikuliUI();
      ptr = SikuliUI::sikuliUI;
   }
   
   ~SikuliInitCleanup(){
      delete ptr;
   }
private:
   SikuliUI* ptr;
};
SikuliInitCleanup sic;

void 
SikuliUI::set(SikuliUI* ui){
   SikuliUI::sikuliUI  = ui;
}

int
SikuliUI::handleMatchFound(Region region, Pattern target, vector<Match> matches){
   
   int n = matches.size();
   cout << endl;
   cout << target.tostring() << ": " << n << " match(es)" << endl;
   for (int i=0;i <n; ++i){ 
      Match& match = matches[i];
      if (i<5)
         cout << '\t' << i+1 << ": (" << match.x << "," << match.y << "," << match.w << "," << match.h <<
         "), score = " << match.getScore()  << endl;
      else if (i==5) 
         cout << '\t' << "... more" << endl;
   }
   
   return 1;
}

int 
SikuliUI::handleFindFailedException(Region region, 
                                    Pattern target){
   cout << target.tostring() << " can not be found!!" << endl;
   
   while (true){
      char ret = 0;
      cout << "(S)kip, (R)etry, (A)bort?";
      cin >> ret;
      if (ret == 'S' || ret == 's'){
         return SKIP;
      }else if (ret == 'R' || ret == 'r' ){
         return RETRY;
      }else if (ret == 'A' || ret == 'a'){
         return ABORT;
      }
   }
}




#include <sys/stat.h> 
bool FileExists(const char* strFilename) {
   struct stat stFileInfo;
   bool blnReturn;
   int intStat;
   
   // Attempt to get the file attributes
   intStat = stat(strFilename,&stFileInfo);
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


//string findImageHelper(const char* image_filename) throw(FileNotFound){
//   
//   dout << "[Pattern] Resolving image url:  " << image_url << endl;
//   
//   if (FileExists(image_url))
//      // reading the image from the local file system
//      return image_url;
//   
//   throw FileNotFound(image_url.c_str());
//} 
//
//string findImageFromPaths(string image_filename)  {
//   
//   
//   if (FileExists(image_filename))
//      return string(image_filename);
//   
//   // First try to check the existence of the image using the filename as is   
//   try {
//      return findImageHelper(image_filename);
//   }catch (...) {}
//   
//   // Then, try to read the image at each image path
//   vector<const char*> image_paths = Settings::getImagePaths();
//   
//   for (int i=0; i<image_paths.size(); ++i){
//      string image_filename_with_path = string(image_paths[i]) + 
//#ifdef WINDOWS
//      "\\" + string(image_filename);
//#else
//      "/" + string(image_filename);
//#endif
//      
//      try { 
//         return findImageHelper(image_filename_with_path);
//      }catch (...) {}
//   }
//   
//   throw FileNotFound(image_filename.c_str());
//}
//

#include "settings.h"

string 
ImageReadHelper::resolveImageFilename(string image_filename){
   
   if (FileExists(image_filename.c_str()))
      return image_filename;
   
   // Then, look for the file in each image path
   vector<const char*> image_paths = Settings::getImagePaths();
   
   for (int i=0; i<image_paths.size(); ++i){
      string image_filename_with_path = string(image_paths[i]) + 
#ifdef WINDOWS
      "\\" 
#else
      "/"
#endif
      + string(image_filename);
      
      if (FileExists(image_filename_with_path.c_str()))
         return image_filename_with_path;
   }
   
   throw FileNotFound(image_filename.c_str());
}

ImageReadHelper* 
ImageReadHelper::instance(){
   static ImageReadHelper helper;
   if (ptr != 0){
      return ptr;
   }
   return &helper;
};

ImageReadHelper* ImageReadHelper::ptr = 0;

void 
ImageReadHelper::set(ImageReadHelper* ptr_){
   ptr = ptr_;
};

