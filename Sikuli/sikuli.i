%module sikuli
%{
#define SWIG_FILE_WITH_INI
//#include "finder.h"
#include "region.h"
using namespace sikuli;
%}
/*
class Finder {
public:

	//Finder(Mat mat);	
   Finder(const char* source);
	~Finder();
   
	void find(const char *target, double min_similarity);
   	
	bool hasNext();
	FindResult next();
};
*/

class Location : public Point{
   
public: 
   
   Location();
   Location(int _x, int _y);  
   Location(const Location& loc);   
   Location offset(int dx, int dy);
   Location left(int dx);
   Location right(int dx);
   Location above(int dy);
   Location below(int dy);
   
};

class Robot{
   
public:
   
   void delay(int time);
   void mouseMove(int x, int y);
   void mousePress(int buttons);
   void mouseRelease(int buttons);
   void waitForIdle();
   void keyPress(int key);
   void keyRelease(int key);
   
//   void paste(string text);
   
//   cv::Mat capture();
   
};


class Pattern{
   
public:
   
   Pattern(const char* imgURL_);
   
   Pattern similar(float similarity_);
   Pattern exact();
   
   Pattern targetOffset(int dx_, int dy_);
   
   Location getTargetOffset();
   
   string getImgURL() { return imgURL;} ;
   
   string toString();
   
};  

class Region{
   
public:
   
   Region();

   template<class PSRML> int click(PSRML target, int modifiers);

   template<class PSRML> int type(PSRML target, const char* text, int modifiers);
   
   
   %template(click) click<Location>;
   %template(click) click<Pattern>;
   
   %template(type) type<Pattern>;
   
};



