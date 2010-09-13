/*
 *  runocr.cpp
 *  sikuli
 *
 *  Created by Tom Yeh on 9/12/10.
 *  Copyright 2010 sikuli.org. All rights reserved.
 *
 */

#include "myocr.h"
#include "ocr.h"


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

#include "vision.h"
using namespace sikuli;
int match_main(int argc, const char* argv[]){
   
   ScreenImage screen(argv[1]);
   Pattern ptn(argv[2]);
   
   vector<FindResult> results;
   results = Vision::find(screen, ptn);
   
   for (vector<FindResult>::iterator r = results.begin(); 
        r != results.end(); ++r){
      
      FindResult& result = *r;
      
      cout << result.x << " " << result.y;
      
   }
   
   return 1;
}

#include "cvgui.h"
int index_main(int argc, const char* argv[]){
   
   Database db;
   db.insert_file("research/testdata/amazon.png",1);
   
      
   ofstream out("test.bin", ios::binary);
   db.write(out);
   out.close();
   
   return 1;
}


int main(int argc, const char* argv[]){
   
   if (argc < 2)
      return 0;
   
   const char* command = argv[1];
   
   if (strcmp(command, "OCR") == 0)
      return ocr_main(argc-1, argv+1);

   else if (strcmp(command, "MATCH") == 0)
      return match_main(argc-1, argv+1);

   else if (strcmp(command, "INDEX") == 0)
      return index_main(argc-1, argv+1);
   
   
   return 0;
}