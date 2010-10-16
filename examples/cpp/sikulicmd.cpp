/*
 *  runocr.cpp
 *  sikuli
 *
 *  Created by Tom Yeh on 9/12/10.
 *  Copyright 2010 sikuli.org. All rights reserved.
 *
 */

#include "tessocr.h"
#include "sikuli.h"
#include "imgdb.h"

#include <stdio.h>

#define MAXLEN 80

using namespace sikuli;
int prefix_main(int argc, const char* argv[]){
   
   
//   Screen s;
//   s.capture();
//   ScreenImage image = s.capture();
//   OCR::find_word(image.getMat(),"namedWindow");
//   ScreenImage image = s.capture();
   Mat screen = imread("screen.png");
   //OCR::find_word(screen,"Keyboard");
   
   //
//   vector<string> phrase;
//   phrase.push_back("System");
//   phrase.push_back("Preferences");
//
//   
   
   vector<string> phrase;
   phrase.push_back("Software");
   phrase.push_back("Update");   
   OCR::find_phrase(screen,phrase);
   
   
   return 0;
   
}

int ocr_main(int argc, const char* argv[]){
   const char* filename = argv[1];
   OCRText text = OCR::recognize_screenshot(filename);

   char buf[200];
   sprintf(buf,"%s.ocr.txt", filename);
   text.save(buf);

   sprintf(buf,"%s.ocr.loc", filename);
   text.save_with_location(buf);
   
   return 1;
}



int ocrdir_main(int argc, const char* argv[]){
   
   
   const char* inputdir = argv[1];
   int n = atoi(argv[2]);
   
   
   
   for (int i=0;i<n;++i){
   
      char buf[200];
 
      sprintf(buf,"%s/screen%d.png", inputdir, i);
      cout << buf;
      OCRText text = OCR::recognize_screenshot(buf);      
      
      sprintf(buf,"%s/screen%d.png.ocr.txt", inputdir, i);
      text.save(buf);
      
      sprintf(buf,"%s/screen%d.png.ocr.loc", inputdir, i);
      text.save_with_location(buf);
   
   }
   
   return 1;
}



#include "vision.h"
using namespace sikuli;
int match_main(int argc, const char* argv[]){
     
   vector<FindResult> results;
   
   FindInput input(argv[1],argv[2]);
   input.setFindAll(true);
   results = Vision::find(input);
   
   for (vector<FindResult>::iterator r = results.begin(); 
        r != results.end(); ++r){
      
      FindResult& result = *r;
      
      cout << result.x << " " << result.y << endl;
      
   }
   
   return 1;
}

#include "cvgui.h"
int index_main(int argc, const char* argv[]){
   if (argc < 4)
      return -1;
   
   Database db;
   
   const char* inputdir = argv[1];
   int n = atoi(argv[2]);
   const char* output = argv[3];
   
   char filename[200];
   
   for (int i=0;i<n;++i){
      sprintf(filename, "%s/screen%d.png", inputdir, i);
      cout << "indexing " << filename << endl;
      
      db.insert_file(filename, i);
   }
      
   ofstream out(output, ios::binary);
   db.write(out);
   out.close();
   
   return 1;
}


int query_main(int argc, const char* argv[]){
   if (argc < 3)
      return -1;
   
   Database db;

   const char* index_filename = argv[1];   
   const char* query_image_filename = argv[2];
   
   ifstream in(index_filename, ios::binary);
   db.read(in);
   in.close();
 
   
   vector<ImageRecord> results = db.find(query_image_filename);
   
   for (vector<ImageRecord>::iterator r = results.begin(); 
        r != results.end(); ++r){
      
      ImageRecord& record = *r;
      
      cout << "ui" << record.id << " ";
      
   }
   
   return 1;
}

int compare_main(int argc, const char* argv[]){
   if (argc < 3)
      return -1;
   
   const char* image1_filename = argv[1];   
   const char* image2_filename = argv[2];
      
   Mat image1 = imread(image1_filename,1);
   Mat image2 = imread(image2_filename,1);
   
   double score = Vision::compare(image1,image2);
   
   cout << score << endl;
   
   return 1;
}


int capture_main(int argc, const char* argv[]){
   if (argc < 2)
      return -1;
   
   sikuli::Screen screen;
   
   const char* outputdir = argv[1];   
   
   char buf[200];
   int i=0;
   while (true){
      
      cout << "capturing frame no. " << i << endl;
      
      ScreenImage image = screen.capture();
      
      sprintf(buf,"%s/screen%d.png", outputdir, i);
      image.save(buf);
      
      OCRText text = OCR::recognize_screenshot(buf);      
      
      sprintf(buf,"%s/screen%d.png.ocr.txt", outputdir, i);
      text.save(buf);
      
      sprintf(buf,"%s/screen%d.png.ocr.loc", outputdir, i);
      text.save_with_location(buf);
      
      sprintf(buf,"%s/screen%d.png.ui.txt", outputdir, i);
      ofstream of1(buf);
      of1.close();
      
      sprintf(buf,"%s/screen%d.png.ui.loc", outputdir, i);
      ofstream of2(buf);
      of2.close();
      
      
      i++;
   }
   
   return 1;
}


int main(int argc, const char* argv[]){
   
   if (argc < 2)
      return 0;
   
   const char* command = argv[1];
   
   if (strcmp(command, "OCR") == 0)
      return ocr_main(argc-1, argv+1);
   
   else if (strcmp(command, "OCRDIR") == 0)
      return ocrdir_main(argc-1, argv+1);   

   else if (strcmp(command, "MATCH") == 0)
      return match_main(argc-1, argv+1);

   else if (strcmp(command, "INDEX") == 0)
      return index_main(argc-1, argv+1);
   
   else if (strcmp(command, "QUERY") == 0)
      return query_main(argc-1, argv+1);
   
   else if (strcmp(command, "COMPARE") == 0)
      return compare_main(argc-1, argv+1);
   
   else if (strcmp(command, "CAPTURE") == 0)
      return capture_main(argc-1, argv+1);
   
   else if (strcmp(command, "PREFIX") == 0)
      return prefix_main(argc-1, argv+1);
  
   
   return 0;
}