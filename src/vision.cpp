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
	  //string str(ptn.getImageURL());
	  //char filename[] = "c:\\cygwin\\home\\tomyeh\\libsikuli\\examples\\images\\startpage.png";
	  //string name("examples\\images\\startpage.png"); 
	  Mat image = imread(ptn.getImageURL());

	  
	  //ptn.getImageURL());
	  
	/*namedWindow("Test");
	imshow("Test",image);
	waitKey();*/

      
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

#define PIXEL_DIFF_THRESHOLD 20
#define IMAGE_DIFF_THRESHOLD 20

double
Vision::compare(Mat im1, Mat im2){
   
   Mat gray1;
   Mat gray2;
   
   // convert image from RGB to grayscale
   cvtColor(im1, gray1, CV_RGB2GRAY);
   cvtColor(im2, gray2, CV_RGB2GRAY);
   
   Mat diff1;
   absdiff(gray1,gray2,diff1);
   
   typedef float T;
   
   Size size = diff1.size();
   
   int diff_cnt = 0;
   for( int i = 0; i < size.height; i++ )
   {
      const T* ptr1 = diff1.ptr<T>(i);
      for( int j = 0; j < size.width; j += 4 )
      {         
         if (ptr1[j] > PIXEL_DIFF_THRESHOLD)
            diff_cnt++;
      }
   }
   
   
   // ratio of pixels that are different
   double score = 1.0 * diff_cnt / (im1.rows * im1.cols);
   return score;
}


vector<FindResult> 
Vision::find(ScreenImage simg, Pattern ptn) { 

//	SYSTEMTIME begin, end;
    
//    GetSystemTime(&begin);

   Mat screenImage = simg.getMat();
      
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
   
   vector<FindResult> final_results;
   int n = min((int)results.size(), (int)ptn.getLimit());
   for (int i=0; i< n; ++i){
      final_results.push_back(results[i]);
   }
   
//    GetSystemTime(&end);
//	WORD elapsed_ms = (end.wSecond - begin.wSecond)*1000+(end.wMilliseconds - begin.wMilliseconds);
//	cout << elapsed_ms << " ms." << endl;

   return final_results;
}
