#ifndef _OCR_H_
#define _OCR_H_

#include "opencv2/opencv.hpp"
#include "result.h"
using namespace cv;

struct WordRect : public Rect{	
	WordRect(Rect& r) : Rect(r) {};
	vector<Rect> charRects;
};

void train_by_image(const Mat& trainingImage);
vector<FindResult> find_word_by_image(const Mat& inputImage, const char word[]);
vector<FindResult> find_phrase(const Mat& inputImage, vector<string> targetWords);

vector<string> recognize_words(const Mat& inputImageColor);   

void test_segment(const Mat& inputImage, const char word[]);

#define DISPLAY_SEGMENT_IMAGE 0
#define DISPLAY_SEGMENT_LINEIMAGE_STEP 0
#define DISPLAY_SEGMENT_LINEIMAGE_RESULT 0
#define DISPLAY_RECOGNIZE 0
#define DISPLAY_WORD_FIND_RESULT 1

#define DISPLAY_TEST_SEGMENT 1

#define DISPLAY_MATCH_WORD 0
#define DISPLAY_MATCH_CHAR 0

#define DISPLAY_FIND_WORD_STEP 0

#define DISPLAY_NUM_TOP_MATCHES 1

#define MIN_CHAR_MATCH_THRESHOLD 0.4

#define OUTPUT_RESULT_IMAGES 1

//#define DEBUG

#endif