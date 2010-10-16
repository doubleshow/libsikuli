#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdlib.h>
#include "tessocr.h"

using namespace std;

#include "tesseract/baseapi.h"
#define COMPUTE_IMAGE_XDIM(xsize,bpp) ((bpp)>8 ? ((xsize)*(bpp)+7)/8 :((xsize)+8/(bpp)-1)/(8/(bpp)))
static char* mytesseract(const unsigned char* imagedata,
                         int width, int height, int bpp){
   
   int bytes_per_pixel = bpp / 8;        
   int bytes_per_line = COMPUTE_IMAGE_XDIM(width,bpp);
   char* text = TessBaseAPI::TesseractRectBoxes(imagedata,
                                                bytes_per_pixel,
                                                bytes_per_line, 0, 0,
                                                width,
                                                height,
                                                height);
   return text;
}


void
OCRWord::add(OCRChar& ocr_char){
   ocr_chars_.push_back(ocr_char);
}

string
OCRWord::str(){
   string ret;
   for (vector<OCRChar>::iterator it = ocr_chars_.begin(); it != ocr_chars_.end(); ++it){
      ret = ret + it->ch;    
   }
   return ret;
};

bool
OCRWord::isValidWord(){
   return TessBaseAPI::IsValidWord(str().c_str());
}

void
OCRLine::addWord(OCRWord& ocr_word){
   ocr_words_.push_back(ocr_word);
}


void
OCRParagraph::addLine(OCRLine& ocr_line){
   ocr_lines_.push_back(ocr_line);
}

void
OCRText::add(OCRWord& ocr_word){
   ocr_words_.push_back(ocr_word);
}

void
OCRText::addLine(OCRLine& ocr_line){
   ocr_lines_.push_back(ocr_line);
}


void
OCRText::save(const char* filename){
   
   ofstream of(filename);
   
   for (iterator it = begin();
        it != end(); ++it){
      
      of << it->str() << " ";
   }
   
   of.close();
}

void
OCRText::save_with_location(const char* filename){
   
   ofstream of(filename);
   
   for (iterator it = begin();
        it != end(); ++it){
      
      of << it->x << " " << it->y << " " << it->width << " " << it->height << " ";
      of << it->str() << " ";
      of << endl;
   }
   
   of.close();
}

void 
OCRText::addParagraph(OCRParagraph& ocr_paragraph){
   ocr_paragraphs_.push_back(ocr_paragraph);
}

char  
encode(char ch){
   char code;
   if (ch >= '0' && ch <= '9')
      code = ch - '0' + 2;
   else if (ch >= 'a' && ch <= 'z')
      code = ch - 'a' + 12;
   else if (ch >= 'A' && ch <= 'Z')
      code = ch - 'A' + 12;
   else 
      code = 0;
   return code;
}


// produce a new image 200% the size of the given image
unsigned char* x2(const unsigned char* imagedata,
                  int width, int height, int bpp){
   
   int bytes_per_pixel = bpp / 8;
   
   unsigned char* newimage = new unsigned char[width*height*4];
   
   const unsigned char* p = imagedata;
   unsigned char* q = newimage;
   
   for (int k=0;k<height;++k){
      
      const unsigned char* p1 = p;
      
      for (int i=0;i<2;++i){
         for (int j=0;j<width;++j){
            *q = *p1;
            q++;            
            *q = *p1;
            q++;p1++;
         }
      }
      
      p += width * bytes_per_pixel;
   }
   
   return newimage;
   
}

bool 
OCR::isInitialized = false;

void
OCR::init(){
   init("tessdata");
}

void
OCR::init(const char* datapath){
   if (isInitialized)
      return;
   
   char outputbase[] = "output";
   char lang[] = "eng";
   bool numeric_mode = false;
   TessBaseAPI::InitWithLanguage(datapath,outputbase,lang,NULL,numeric_mode,0,0);
   isInitialized = true;   
}


#include "cvgui.h"
using namespace cv;

#define MAXLEN 80

static int findMin(int d1, int d2, int d3) {
   /*
    * return min of d1, d2 and d3.
    */
   if(d1 < d2 && d1 < d3)
      return d1;
   else if(d1 < d3)
      return d2;
   else if(d2 < d3)
      return d2;
   else
      return d3;
}

static int findEditDistanceLessThanK(const char *s1, const char *s2, 
                                    int k){
   /*
    * returns edit distance between s1 and s2.
    */
   int d1, d2, d3;
   
   if(*s1 == 0)
      return strlen(s2);
   if(*s2 == 0)
      return strlen(s1);
   if (k == 0)
      return 0;
   if(*s1 == *s2)
      d1 = findEditDistanceLessThanK(s1+1, s2+1, k);
   else
      d1 = 1 + findEditDistanceLessThanK(s1+1, s2+1, k-1);    // update.
   d2 = 1+findEditDistanceLessThanK(s1, s2+1, k-1);                   // insert.
   d3 = 1+findEditDistanceLessThanK(s1+1, s2, k-1);                   // delete.
   
   return findMin(d1, d2, d3);
}

static int findEditDistance(const char *s1, const char *s2) {
   /*
    * returns edit distance between s1 and s2.
    */
   int d1, d2, d3;
   
   if(*s1 == 0)
      return strlen(s2);
   if(*s2 == 0)
      return strlen(s1);
   if(*s1 == *s2)
      d1 = findEditDistance(s1+1, s2+1);
   else
      d1 = 1 + findEditDistance(s1+1, s2+1);    // update.
   d2 = 1+findEditDistance(s1, s2+1);                   // insert.
   d3 = 1+findEditDistance(s1+1, s2);                   // delete.
   
   return findMin(d1, d2, d3);
}

vector<OCRChar> run_ocr(const Mat& screen, Blob& blob){
 
   
   Mat blobImage(screen,blob);
   
   Mat ocrImage;  // the image passed to tesseract      
   bool upsampled = false;
   if (blobImage.rows < 20){
      upsampled = true;
      resize(blobImage, ocrImage, Size(blobImage.cols*2,blobImage.rows*2));
   }else {
      ocrImage = blobImage.clone(); 
   }  
   
   vector<OCRChar> ocr_chars;
   ocr_chars = OCR::recognize((unsigned char*)ocrImage.data,
                              ocrImage.cols,
                              ocrImage.rows,
                              8); 
   
   for (vector<OCRChar>::iterator iter = ocr_chars.begin(); 
        iter != ocr_chars.end(); iter++){
      
      
      OCRChar& ocrchar = *iter;
      
      
      if (upsampled){
         // scale back the coordinates in the OCR result
         
         ocrchar.x = ocrchar.x/2;
         ocrchar.y = ocrchar.y/2;
         ocrchar.width = ocrchar.width/2;
         ocrchar.height = ocrchar.height/2;
      }
      
      
      
      ocrchar.x += blob.x;
      ocrchar.y += blob.y;
      
      
      char ch = ocrchar.ch;
      if (ch < 0 || ch > '~') 
         ch = '?';
      ocrchar.ch = ch;
   
   }
   
   return ocr_chars;
}

ostream& dout(const char* name){
   return cout;
}

ostream& dhead(const char* name){
   return cout << "[" << name << "] ";
}

void
find_phrase_helper(const Mat& screen_gray, vector<string> words, vector<LineBlob> lineblobs,
                   LineBlob resultblob, vector<FindResult>& results){
   
   string word = words[0];
   
   vector<string> rest;
   for (vector<string>::iterator it2 = words.begin()+1;
        it2 != words.end(); ++ it2)
      rest.push_back(*it2);
   
   dhead("find_phrase") << "<" << word << ">" << endl;
   
   
   for (vector<LineBlob>::iterator it = lineblobs.begin();
        it != lineblobs.end(); ++it){
      
      LineBlob& lineblob = *it;

      
      if (abs((int)lineblob.blobs.size() - (int)word.size()) > 2)
         continue;
      
      vector<OCRChar> ocr_chars = run_ocr(screen_gray, lineblob);
      
      
      dhead("find_phrase") << word << "<->";
      
      string ocrword = "";
      for (vector<OCRChar>::iterator iter = ocr_chars.begin(); 
           iter != ocr_chars.end(); iter++){
         
         OCRChar& ocrchar = *iter;
         cout << ocrchar.ch;
         
         ocrword = ocrword + ocrchar.ch;
      }
      
      if (ocr_chars.size() < 1){
         dout("find_phrase") << endl;
         continue;
      }
      
      
      //int d = findEditDistance(word.c_str(), ocrword.c_str());
      int d = findEditDistanceLessThanK(word.c_str(), ocrword.c_str(),3);
   
      
      dout("find_phrase") << '[' << d << ']';      
            
      if (d > 2){
         dout("find_phrase") << endl;
         continue;
      }
         
         
      if (rest.empty()){
         dout("find_phrase") << " ... match!" << endl;
         
         Blob b = resultblob;
         cout << b.x << "," << b.y << endl;
         b = lineblob;
         cout << b.x << "," << b.y << endl;
         
         
         
         resultblob.merge(lineblob);
         
         FindResult result(resultblob.x,resultblob.y,
                           resultblob.width,resultblob.height, 1.0);

         results.push_back(result);
         return;
         
      }
      else 
         dout("find_phrase") << endl;
         
      

      
      vector<LineBlob> nextblobs;
      for (vector<LineBlob>::iterator it2 = lineblobs.begin();
           it2 != lineblobs.end(); ++it2){
         
         LineBlob& b1 = lineblob;
         LineBlob& b2 = *it2;
         
         bool similar_baseline = abs((b1.y + b1.height) - (b2.y + b2.height)) < 5;
         //            int horizontal_spacing = b2.x - (b1.x + b1.width);
         //            bool small_horizontal_spacing = horizontal_spacing < 10 && horizontal_spacing > -2;
         
         bool close_right = (b2.x > b1.x) && (b2.x - (b1.x+b1.width)) < 20;
         bool close_below = (b2.y > b1.y) && (b2.y - b1.y) < 20;
         
         if (close_right && similar_baseline)
            nextblobs.push_back(b2);
         
      }
      

      
      if (!rest.empty() && !nextblobs.empty()){
         
         LineBlob next_resultblob = resultblob;
         next_resultblob.merge(lineblob);
         
         find_phrase_helper(screen_gray, rest, nextblobs, next_resultblob, results);
      }
         
         
         
      dout("find_phrase") << endl;
      
   }
}

vector<FindResult>
OCR::find_phrase(const Mat& screen, vector<string> words){
   
   vector<LineBlob> lineblobs;
   cvgui::getLineBlobsAsIndividualWords(screen, lineblobs);
   
   Mat screen_gray;
   cvtColor(screen,screen_gray,CV_RGB2GRAY);
   
   vector<FindResult> results;
   
   LineBlob empty;
   find_phrase_helper(screen_gray, words, lineblobs, empty, results);
      

   return results;   
}

vector<FindResult>
OCR::find_word(const Mat& screenshot, string word){
   
   vector<string> words;
   words.push_back(word);
   
   return find_phrase(screenshot, words);
}

OCRText
OCR::recognize_screenshot(const char* screenshot_filename){
   
   Mat screenshot = imread(screenshot_filename, 1);
   return recognize(screenshot);
}


OCRText 
OCR::recognize(cv::Mat screenshot){
   
   OCRText text;

   return text; 
}


vector<OCRChar>
OCR::recognize(const unsigned char* imagedata,
               int width, int height, int bpp){
   
   OCR::init();
   
   vector<OCRChar> ret;
   
   char* text = mytesseract(imagedata,width,height,bpp);
   
   //Result ocr_result;   
   
   if (text){
      
      stringstream str(text);
      string ch;
      int x0,y0,x1,y1;
      while (str >> ch >> x0 >> y0 >> x1 >> y1){    
         //cout << ch << " " << x0 << " " << y0 << " " << x1 << " " << y1 << endl;  
         
         //convert back to the screen coordinate (0,0) - (left,top)
         int h = y1 - y0;
         int w = x1 - x0;
         OCRChar ocr_char(ch[0],x0,height-y1,w,h);
         
         ret.push_back(ocr_char);
      };
      
      
      delete text;
   }
   
   
   
   return ret;
}



