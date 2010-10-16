#ifndef _FINDER_H_
#define _FINDER_H_

#include "pyramid-template-matcher.h"

class BaseFinder{
	
public:
	
	BaseFinder(IplImage* screen_image);
	BaseFinder(Mat source);
	BaseFinder(const char* source_image_filename);
	virtual ~BaseFinder();
	
	void setROI(int x, int y, int w, int h);
	
	int get_screen_height() const { return source.rows;};
	int get_screen_width()  const {return source.cols;};
   
	
   void find();
   
 //  virtual void find(const char* str, double min_similarity) = 0;   
   
   virtual bool hasNext() = 0;
   virtual FindResult next() = 0;
   
	
protected:
	
	Rect roi;
	Mat source;
	Mat roiSource;
   
   
   double min_similarity;
};

class TextFinder : public BaseFinder {
	
public:
	TextFinder(Mat source);
   static void train(Mat& trainingImage);
	
   void find(const char* word, double min_similarity);   
   void find(vector<string> words, double min_similarity);
   
   bool hasNext();
   FindResult next();
   
   static vector<string> recognize(const Mat& inputImage);
   
private:
   vector<FindResult> matches;
   vector<FindResult>::iterator matches_iterator;
   
	void test_find(const Mat& inputImage, const vector<string>& testWords);
};

class TemplateFinder : public BaseFinder{
	
public:
	
	TemplateFinder(Mat source);
	TemplateFinder(IplImage* source);
	TemplateFinder(const char* source_image_filename);
	~TemplateFinder();
	
   
	void find(Mat target, double min_similarity);
	void find(IplImage* target, double min_similarity);
	void find(const char *target_image_filename, double min_similarity);
   
	
	void find_all(Mat target, double min_similarity);
	void find_all(IplImage*  target, double min_similarity);
	void find_all(const char *target_image_filename, double min_similarity);  
	
	bool hasNext();
	FindResult next();
	
private:
	
   void create_matcher(Mat& source, Mat& target, int level, float ratio);
	PyramidTemplateMatcher* matcher;
	
	FindResult current_match;
	int current_rank;	
	
	// buffer matches and return top score
   void add_matches_to_buffer(int num_matches_to_add);
   float top_score_in_buffer();	
   
	vector<FindResult> buffered_matches;   
};

class Finder {
public:
   
   
	Finder(Mat mat);	
	Finder(IplImage* source);
   Finder(const char* source);
	~Finder();
   
	void find(IplImage* target, double min_similarity);
	void find(const char *target, double min_similarity);
   
	void find_all(IplImage*  target, double min_similarity);
	void find_all(const char *target, double min_similarity);  
	
	bool hasNext();
	FindResult next();
	
private:
	
   Mat _source;
   BaseFinder* _finder;
};

class FaceFinder : public BaseFinder {
   
public:
   
   FaceFinder(const char* screen_image_filename);
   ~FaceFinder();
   
   void find();
   bool hasNext();
   FindResult next();
   
private:
   
   CvMemStorage* storage;
   
   static CvHaarClassifierCascade* cascade;
   
   CvSeq* faces;
   int face_i;
   
};


class ChangeFinder : public BaseFinder {
   
public:
   
   ChangeFinder(const IplImage* screen_image);
   ChangeFinder(const Mat screen_image);
   ChangeFinder(const char* screen_image_filename);
   
   ~ChangeFinder();
   
   void find(IplImage* new_img);
   void find(Mat new_img);
   void find(const char* new_screen_image_filename);
   
   bool hasNext();
   FindResult next(); 
   
private:
   
   bool is_identical;
   
   IplImage *prev_img;
   CvSeq* c;
   CvMemStorage* storage;
   
};




#endif // _FINDER_H_