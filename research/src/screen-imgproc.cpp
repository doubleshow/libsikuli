/*
 *  screen-imgproc.cpp
 *  sikuli
 *
 *  Created by Tom Yeh on 9/8/10.
 *  Copyright 2010 sikuli.org. All rights reserved.
 *
 */

#include "screen-imgproc.h"
#include "cv-util.h"

#include <iostream>
using namespace std;

#define SHOW(x) namedWindow(#x,1); imshow(#x,x);
#define SHOW0(x) namedWindow(#x,1); imshow(#x,x); waitKey(0);


void drawRects(Mat& image, vector<Rect> rects, Scalar color){
   for (vector<Rect>::iterator r = rects.begin();
        r != rects.end(); ++r){

      rectangle(image, 
                Point(r->x, r->y),
                Point(r->x + r->width, r->y + r->height),
                color);
   }
}

void drawRects(Mat& image, vector<Rect> rects){
   drawRects(image, rects, Scalar(0,0,255));
}


void extractFeatures(Mat src){
   
   
   // dimensions
   cout << src.rows << endl;
   cout << src.cols << endl;
   
   
   Mat srcgray;
   cvtColor(src,srcgray, CV_RGB2GRAY);
   
   adaptiveThreshold(srcgray, srcgray, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY_INV, 3, 1);

   SHOW(srcgray);

   cout << countNonZero(srcgray) << endl;
   
   Moments mnts = moments(srcgray);
   
   cout << mnts.m00 << "," << mnts.m11 << endl;
   
   Mat hsv;
   cvtColor(src, hsv, CV_BGR2HSV);
   
   // let's quantize the hue to 30 levels
   // and the saturation to 32 levels
   int hbins = 30, sbins = 32;
   int histSize[] = {hbins, sbins};
   // hue varies from 0 to 179, see cvtColor
   float hranges[] = { 0, 180 };
   // saturation varies from 0 (black-gray-white) to
   // 255 (pure spectrum color)
   float sranges[] = { 0, 256 };
   const float* ranges[] = { hranges, sranges };
   MatND hist;
   // we compute the histogram from the 0-th and 1-st channels
   int channels[] = {0, 1};
   
   calcHist( &hsv, 1, channels, Mat(), // do not use mask
            hist, 2, histSize, ranges,
            true, // the histogram is uniform
            false );
   
   double maxVal=0;
   minMaxLoc(hist, 0, &maxVal, 0, 0);
   
   int scale = 10;
   Mat histImg = Mat::zeros(sbins*scale, hbins*10, CV_8UC3);
   
   for( int h = 0; h < hbins; h++ )
      for( int s = 0; s < sbins; s++ )
      {
         float binVal = hist.at<float>(h, s);
         int intensity = cvRound(binVal*255/maxVal);
         rectangle( histImg, Point(h*scale, s*scale),
                     Point( (h+1)*scale - 1, (s+1)*scale - 1),
                     Scalar::all(intensity),
                     CV_FILLED );
      }
   
   namedWindow( "Source", 1 );
   imshow( "Source", src );
   
   namedWindow( "H-S Histogram", 1 );
   imshow( "H-S Histogram", histImg );
   
   //flann::IndexParams );
   
   flann::Index idx(src, flann::KDTreeIndexParams(4));
   
   //cvflann::Index idx;
   
   waitKey(0);

}


#include "blob.h"
#include "BlobResult.h"
void bwareaopen(Mat& src, int p){
   
   
   //////////////////////////////////////////////////////////////
   // get blobs and filter them using its area
   /////////////////////////////////////////////////////////////
   CBlobResult blobs;
   int i;
   CBlob *currentBlob;
   //IplImage *original, *originalThr;
   
   // load an image and threshold it
   //original = cvLoadImage("pic1.png", 0);
   //cvThreshold( original, originalThr, 100, 0, 255, CV_THRESH_BINARY );
   
   IplImage originalThr = src;
   
   
   // find non-white blobs in thresholded image
   blobs = CBlobResult( &originalThr, NULL, 255 );
   // exclude the ones smaller than param2 value
   blobs.Filter( blobs, B_EXCLUDE, CBlobGetArea(), B_LESS, 1);
   blobs.Filter( blobs, B_EXCLUDE, CBlobGetArea(), B_GREATER, 50000);
   
   
   // get mean gray color of biggest blob
  // CBlob biggestBlob;
//   CBlobGetMean getMeanColor( original );
//   double meanGray;
//   
//   blobs.GetNth( CBlobGetArea(), 0, biggestBlob );
//   meanGray = getMeanColor( biggestBlob );
//   
//   // display filtered blobs
//   cvMerge( originalThr, originalThr, originalThr, NULL, displayedImage );
   
   Mat result = Mat::zeros(src.size(), CV_8UC3);
   IplImage ipl = result;
   
   for (i = 0; i < blobs.GetNumBlobs(); i++ )
   {
      currentBlob = blobs.GetBlob(i);
      currentBlob->FillBlob( &ipl, CV_RGB(0,0,255));
   }
   
   imwrite("pBlobs.png", result);
}


void denoise(Mat& src){
   
   //src = src * 0.1;
   
   Mat kernel = Mat::ones(3,3,CV_32FC1);
   kernel.at<float>(2,2) = 0;
   
   Mat srcF, destF;
   src.convertTo(srcF, CV_32FC1, 0.1);
   
   filter2D(srcF, destF, -1, kernel);
   
   Mat destU;
   destF.convertTo(destU, CV_8UC1);
   
   threshold(destU, destU, 60, 255, THRESH_BINARY);
   
   bitwise_and(src, destU, src);
}

void VLOG(const char* name, const Mat& image){

   static int i = 0;
   
   char buf[100];
   sprintf(buf, "results/vlog-%d-%s.png", i, name);           
   imwrite(buf, image);
   
   i = i + 1;
   
}


class ImageRecord{
   

public:
   
   int id;
   int screen_id;
   int x;
   int y;
   int height;
   int width;
   
   int area;
   
};


class Database{
   
public:   
   Database();
   
   void insert(const ImageRecord& b);
   vector<ImageRecord> find(const ImageRecord& q);
  
private:
   
   vector<ImageRecord> _image_records;
   
};

Database::Database(){
}

void
Database::insert(const ImageRecord& b){
   _image_records.push_back(b);
}

vector<ImageRecord>
Database::find(const ImageRecord& q){
   
   vector<ImageRecord> ret;
   
   // find by area
   vector<ImageRecord>::iterator it = _image_records.begin();
   for (; it != _image_records.end(); ++it){
    
      if (abs(q.area - it->area) < 10)
         ret.push_back(*it);
   }
   
   return ret;
}



void segment_screenshot(const Mat& screen, vector<Rect>& text_rects, vector<Rect>& image_rects){
   
//   Mat image = imread("pp0.png");
//   extractFeatures(image);
//   
//   
//   return 1;
   

   
   //Mat screen = imread("test/ocr/chi2011.png",1);
   
   Mat gray;
   cvtColor(screen,gray,CV_RGB2GRAY);
   
   adaptiveThreshold(gray, gray, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY_INV, 5, 1);
   VLOG("AdaptiveThresholded", gray);
  

   Mat lines;
   ImageProc::findLongLines(gray, lines);    
   VLOG("LongLinesFound", lines);
   
   Mat not_lines;
   bitwise_not(lines, not_lines);
   
   Mat foreground;
   bitwise_and(gray, not_lines, foreground);
   
   gray.setTo(0, lines);
   VLOG("LongLinesRemoved",gray);

   
   denoise(gray);   
   VLOG("NoiseRemoved", gray);  
   
   dilate(gray,gray,Mat::ones(1,3,CV_8UC1));
   VLOG("Dilated",gray);
   
   vector<Rect> rects;
   ImageProc::extractRects(gray, rects);
   
   
   // visualization
   Mat result;
   Mat g;
   cvtColor(screen, g, CV_RGB2GRAY);
   
   Mat ss[3] = {g,g,g};
   merge(ss,3,result);   
   result = result * 0.5;
   
   Mat bg = result.clone();

   
   text_rects.clear();
   
   for (vector<Rect>::iterator r = rects.begin();
        r != rects.end(); ++r){
           
      Mat part(screen, *r);
      //paste(part, result, r->x, r->y);
      
      
      Mat p = part.clone();
      Mat g;
      cvtColor(p, g, CV_RGB2GRAY);
      adaptiveThreshold(g, g, 255, 
                        ADAPTIVE_THRESH_MEAN_C, 
                        THRESH_BINARY_INV, 3, 1);
      vector<Rect> rs;
      ImageProc::extractSmallRects(g, rs);
      for (vector<Rect>::iterator x = rs.begin();
           x != rs.end(); ++x){
         x->x += r->x;
         x->y += r->y;
      }

      vector<Rect> rs0;
      for (vector<Rect>::iterator q = rs.begin();
           q != rs.end(); ++q){
         
         // exclude overly short element (e.g., dot of i)
         if (q->height < 5)
            continue;         
         
         rs0.push_back(*q);
      }
      
      rs = rs0;
      
      if (ImageProc::areHorizontallyAligned(rs) && rs.size()>1)
         drawRects(result, rs, Scalar(0,255,0));
      else
         drawRects(result, rs, Scalar(0,0,255));

      if (ImageProc::areHorizontallyAligned(rs) && rs.size()>1)
         text_rects.push_back(*r);

   }
   
   drawRects(result, rects, Scalar(255,0,0));
   
   VLOG("TextBlocksExtracted",result);
   
   
   
   Mat text_mask = Mat::zeros(screen.size(), CV_8UC1);
   
   
   for (vector<Rect>::iterator tr = text_rects.begin();
        tr != text_rects.end(); ++tr){
      
     Mat m(text_mask, 
           Range(tr->y,tr->y+tr->height),
           Range(tr->x,tr->x+tr->width));
      
      m = 255;
           
   }
   
   VLOG("TextBinaryMaskComputed", text_mask);
   
   
   gray.setTo(0, text_mask);
   VLOG("AfterTextRemoved",gray);
   
   dilate(gray,gray,Mat());
   VLOG("DilatedAgain",gray);
   
   vector<Rect> temp;
   ImageProc::extractRects(gray, temp);
   
   // only keep image rects larger than n pixels
   
   const int MIN_IMAGE_RECT_AREA = 150;
   
   image_rects.clear();
   for (vector<Rect>::iterator tr = temp.begin();
        tr != temp.end(); ++tr){
   
      if (tr->width * tr->height > MIN_IMAGE_RECT_AREA){
         image_rects.push_back(*tr);
      }
      
   }
   
   Mat image_result = bg.clone();
   
   drawRects(image_result, image_rects);
   VLOG("ImageRecordsExtracted", image_result);
   
}


vector<ImageRecord>
create_image_records(const Mat& src, const vector<Rect> image_rects){
   
   static int image_record_id = 0;
   vector<ImageRecord> ret;
   
   
   for (vector<Rect>::const_iterator tr = image_rects.begin();
        tr != image_rects.end(); ++tr){
      
      Mat part(src, *tr);
      
      char buf[80];
      sprintf(buf, "ir-%d.png",image_record_id);
      imwrite(buf, part);
      
      
      ImageRecord ib;
      ib.x = tr->x;
      ib.y = tr->y;
      ib.width = tr->width;
      ib.height = tr->height;
      ib.area = tr->width * tr->height;
      ib.id = image_record_id;
      
      ret.push_back(ib);
      
      image_record_id++;
   }
   
   return ret;
}


int main(){

   
   //   char file[] = "test/ocr/word.png";
   char file[] = "research/test/amazon.png";
   Mat screen = imread(file, 1);
   
   vector<Rect> text_rects;
   vector<Rect> image_rects;
   segment_screenshot(screen, text_rects, image_rects);
   
   
   
   vector<ImageRecord> records;
   records = create_image_records(screen, image_rects);
      
   
   Database db;
   for (vector<ImageRecord>::iterator r = records.begin();
        r != records.end(); ++r){
      
      db.insert(*r);
   }

   //char query_file[] = "test/ocr/kindle.png";
   char query_file[] = "research/test/amazon_logo.png";
   
   
   Mat query = imread(query_file, 1);
   
   segment_screenshot(query, text_rects, image_rects);
  
   records = create_image_records(query, image_rects);   
   
   for (vector<ImageRecord>::iterator r = records.begin();
        r != records.end(); ++r){
      
      cout << endl << r->area << " : ";
      vector<ImageRecord> matches = db.find(*r);
      for (vector<ImageRecord>::iterator m = matches.begin();
           m != matches.end(); ++m){
       
         cout << m->id << " ";
      }
   }
   
   
   
   // save all extracted image blocks
   
   
   
   
   
//   screen.setTo(Scalar(0,0,255), lines);
//   SHOW(result);
   
//   waitKey(0);   
}   


int L1dist(Vec3b p1, Vec3b p2){
	return max(p1[0],p2[0])-min(p1[0],p2[0])+	
   max(p1[1],p2[1])-min(p1[1],p2[1])+
   max(p1[2],p2[2])-min(p1[2],p2[2]);	
}

bool sort_by_x (Rect a, Rect b){ 
	return (a.x < b.x); 
}

bool 
ImageProc::areHorizontallyAligned(const vector<Rect>& rects){
   
   if (rects.size() <= 1)
      return true;
   
   vector<Rect> sorted_rects = rects;
   sort(sorted_rects, sort_by_x);
   
   int ymin = 10000;
   int ymax = 0;
   
   
   bool no_overlap = true;
   for (vector<Rect>::const_iterator r = sorted_rects.begin() + 1;
        r != sorted_rects.end(); ++r){
     
 
      no_overlap = no_overlap && (r->x >= (r-1)->x + (r-1)->width - 2);
      
      
      int baseline = r->y + r->height;
      ymin = min(baseline, ymin);
      ymax = max(baseline, ymax);
      
   }

   // if the difference between the highest and lowest baseline
   // is too large, it means the rects are not alginged horizontally
   
   return (ymax - ymin < 10) && no_overlap;
}


bool
ImageProc::hasMoreThanNUniqueColors(const Mat& src, int n){
   
                                       
   Mat_<Vec3b>::const_iterator it = src.begin<Vec3b>(),
   itEnd = src.end<Vec3b>();
   
   
   vector< pair<Vec3b,int> > colors; 
   //vector<int> counts;
   colors.push_back( pair<Vec3b,int>(*it,1) );
   ++it;
   
   for(; it != itEnd; ++it){
      
      bool matched;
      matched = false;
      for (vector< pair<Vec3b,int> >::iterator c = colors.begin();
           c != colors.end();
           ++c){
         
         Vec3b& color = (*c).first;
         int& count = (*c).second;
         
         int d = L1dist((*it), (color));
         //cout << d << endl;
         if (d < 150){
            matched = true;
            count++;
            break;
         }
      }
      
      if (!matched){
         
     //    if (colors.size() == 4)
       //     return true;
        // else
            colors.push_back(pair<Vec3b,int>(*it,1));
         
      }
      
   }
   
   cout << endl << endl;
   for (vector< pair<Vec3b,int> >::iterator c = colors.begin();
        c != colors.end();
        ++c){
      
      Vec3b& color = (*c).first;
      int& count = (*c).second;
      //cout << count << endl;
   }
   
   return (colors.size() > 5);
}


void 
ImageProc::extractSmallRects(const Mat& src, 
                              vector<Rect>& rects){
   
   Mat copy = src.clone();
   
   vector<vector<Point> > contours;
   findContours(copy, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
   
   for (vector<vector<Point> >::iterator contour = contours.begin();
        contour != contours.end(); ++contour){
      
      Rect bound = boundingRect(Mat(*contour));
      rects.push_back(bound);
   }
   
}


void 
ImageProc::extractRects(const Mat& src, 
                        vector<Rect>& rects){
   
   Mat copy = src.clone();
   rects.clear();

   
   vector<vector<Point> > contours;
   findContours(copy, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
   
   
   for (vector<vector<Point> >::iterator contour = contours.begin();
        contour != contours.end(); ++contour){
      
      
      double area = contourArea(Mat(*contour));
      
      Rect bound = boundingRect(Mat(*contour));
      
      
      // too small
      if (bound.width < 6 || bound.height < 6)
         continue;
      
      // too empty
      if (bound.height > 100 && bound.width * bound.height * 0.50 > area)
          continue;
      
      rects.push_back(bound);

   }
   
}


void
ImageProc::findLongLines(const Mat& src, Mat& dest){
   
   dest = src.clone();
//   adaptiveThreshold(src, dest, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY_INV, 3, 1);
//   Canny(dest, dest, 10, 0);  
   
   Mat mask;
   ImageProc::findLongLines_Horizontal(dest,mask);
   
   Mat maskT, destT;
   transpose(dest,destT);
   ImageProc::findLongLines_Horizontal(destT,maskT);
   
   Mat maskTT;
   transpose(maskT,maskTT);
   
   bitwise_or(mask,maskTT,dest);
   
}


#define LONGLINE_THRESHOLD 50
void
ImageProc::findLongLines_Horizontal(const Mat& binary, Mat& dest){
 
   typedef uchar T;
   
  	dest = Mat::zeros(binary.rows,binary.cols,CV_8UC1);
   
	Size size = binary.size();
	for( int i = 0; i < size.height; i +=1 ){
      
		const T* ptr1 = binary.ptr<T>(i);
		T* ptr2 = dest.ptr<T>(i);	
		
		bool has_previous_baseline = false;
		int previous_baseline_endpoint = 0;
		int current_baseline_startpoint = 0;
      
     
         
				
      for( int j = 1; j < size.width; j += 1 ){			
			
         
         if (ptr1[j] > 0 && ptr1[j-1] == 0){
            current_baseline_startpoint = j;
         }
         
         if (ptr1[j-1] > 0 &&  (ptr1[j] == 0 || j == size.width - 1)){
			
            
				// check for the condition of a baseline hypothesis
				// the length of the baseline must be > 15
				if ((j - current_baseline_startpoint) > LONGLINE_THRESHOLD || j == size.width - 1){
					
//					int closeness_threshold = 1; 					
//					if (has_previous_baseline && 
//                   (current_baseline_startpoint - previous_baseline_endpoint) 
//						 <= closeness_threshold){
//						
//						
//						// merge the current baseline with the previously baseline
//						for (int k=previous_baseline_endpoint; 
//                       k < current_baseline_startpoint; k += 1){
//	                     ptr2[k] = ptr1[j];
//						}
//					}
					
				//	has_previous_baseline = true;
					//previous_baseline_endpoint = j;
					
               
					for (int k=current_baseline_startpoint; k < j; ++k){
                  ptr2[k] = 255;
					}	
					
				}
				
				// forming a new baseline hypothesis
				//current_baseline_startpoint = j+1;
			}
      }
   }	 
} 


Mat
ImageProc::obtainGrayBackgroundMask(const Mat& input){
   
   Mat copy = input.clone();
   
   Mat_<Vec3b>::iterator it = copy.begin<Vec3b>(),
   itEnd = copy.end<Vec3b>();
   for(; it != itEnd; ++it){
      
      uchar& b = (*it)[0];
      uchar& g = (*it)[1];
      uchar& r = (*it)[2];
      
      if (r == 0 && b == 0 && g == 0){
         r = 1; b = 1; g = 1;
      }
      
      if (b > 50 &&
          abs(b-g) < 10 && abs(g-b) < 10 && abs(b-r) < 10){
         r = 0;
         g = 0;
         b = 0;
      } 
      
      
      
   }
   
   
   Mat ms[3];
   split(copy, ms);
   
   Mat mask;
   threshold(ms[0],mask,0,255,THRESH_BINARY);
   
   dilate(mask,mask,Mat());	

   return mask;
}


Mat
ImageProc::removeGrayBackground(const Mat& input){
   
   Mat copy = input.clone();
   Mat mask;
   //  (Size(copy.rows,copy.cols), CV_8UC1);
   
   Mat_<Vec3b>::iterator it = copy.begin<Vec3b>(),
   itEnd = copy.end<Vec3b>();
   for(; it != itEnd; ++it){
      
      uchar& b = (*it)[0];
      uchar& g = (*it)[1];
      uchar& r = (*it)[2];
      
      if (r == 0 && b == 0 && g == 0){
         r = 1; b = 1; g = 1;
      }
      
      if (b > 50 &&
          abs(b-g) < 10 && abs(g-b) < 10 && abs(b-r) < 10){
         r = 0;
         g = 0;
         b = 0;
      } 
      
      
      
   }
   
   
   Mat ms[3];
   split(copy, ms);
   
   //threshold(ms[0],mask,0,255,THRESH_BINARY_INV);
   threshold(ms[0],mask,0,255,THRESH_BINARY);
   
   dilate(mask,mask,Mat());	
   
   
   vector<vector<Point> > contours;
   findContours(ms[0], contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
   
   Scalar red(0,0,255);
   //drawContours(copy, contours, -1, red);
   
   
   vector<Rect> boundingRects;
   
   for (vector<vector<Point> >::iterator contour = contours.begin();
        contour != contours.end(); ++contour){
      
      
      double area = contourArea(Mat(*contour));
      
      Rect bound = boundingRect(Mat(*contour));
      
      
      if (bound.width * bound.height * 0.5 > area)
         continue;
      
      boundingRects.push_back(bound);
      
      rectangle(copy, 
                Point(bound.x, bound.y),
                Point(bound.x + bound.width, bound.y + bound.height),
                red);
      
      cout << bound.x << "," <<  bound.y << endl;
      
   }
   
   
   
   Mat result(Size(input.cols,input.rows), CV_8UC3);   
   for (vector<Rect>::iterator bound = boundingRects.begin();
        bound != boundingRects.end(); ++bound){
      
      if (bound->width < 10 || bound->height < 8)
         continue;
      
      Mat r(input, *bound);
      paste(r, result, bound->x, bound->y);
      
      
   }
   
   
   namedWindow("result",1);
   imshow("result",result);
   
   
   namedWindow("copy",1);
   imshow("copy",copy);
   
   namedWindow("mask",1);
   imshow("mask",mask);
   waitKey(0);
   return Mat();
   
}