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
   
   //==================================================================
   // Setting/Getting Attributes
   //==================================================================
   
   static Region getFullScreen(int screenId = 0);

   
   int getX(){ return x; }
   int getY(){ return y; }
   int getW(){ return w; }
   int getH(){ return h; }
   
   void setX(int x_){ x = x_; }
   void setY(int y_){ y = y_; }
   void setW(int w_){ w = w_; }
   void setH(int h_){ h = h_; }   
   
   
   Rectangle getROI();
   void setROI(int X, int Y, int W, int H);
   void setROI(Region roi);
   void setROI(Rectangle roi);
   
   void setScreen(int screenId);
      
   Location getCenter() const;
   
   Match getLastMatch();
   vector<Match> getLastMatches();
   
   void setThrowException(bool flag){ _throwException = flag; } 
   void setAutoWaitTimeout(double sec){ _autoWaitTimeout = sec; }
   
   bool getThrowException(){ return _throwException; }
   double getAutoWaitTimeout(){ return _autoWaitTimeout; }   
   
   //==================================================================
   // Pattern Matching Functions
   //==================================================================
   
public:
   
   Match find(Pattern ptn) throw(FindFailed);
   Match find(const char* imgURL) throw(FindFailed);
   
   vector<Match> findAll(Pattern ptn) throw(FindFailed);
   vector<Match> findAll(const char* imgURL) throw(FindFailed);
   
   Match wait(Pattern target) throw(FindFailed);
   Match wait(Pattern target, double timeout) throw(FindFailed);
   Match wait(const char* target) throw(FindFailed);
   Match wait(const char* target, double timeout) throw(FindFailed); 
   
   bool exists(Pattern target);
   bool exists(Pattern target, double timeout);
   bool exists(const char* target);
   bool exists(const char* target, double timeout);
   
   bool waitVanish(Pattern target); 
   bool waitVanish(Pattern target, double timeout);
   bool waitVanish(const char* target); 
   bool waitVanish(const char* target, double timeout);
   
   //==================================================================
   // Automation Functions
   //==================================================================
   
public:
   
   int click(int modifiers = 0);
   int click(Location target, int modifiers = 0);
   int click(Pattern& target, int modifiers = 0);
   int click(char const* target, int modifiers = 0);
   int click(Region& target, int modifiers = 0);
   int click(Match& target, int modifiers = 0);
   
   int doubleClick(int modifiers = 0);
   int doubleClick(Location target, int modifiers = 0);
   int doubleClick(Pattern& target, int modifiers = 0);
   int doubleClick(char const* target, int modifiers = 0);
   int doubleClick(Region& target, int modifiers = 0);
   int doubleClick(Match& target, int modifiers = 0);
   
   int rightClick(int modifiers = 0);
   int rightClick(Location target, int modifiers = 0);   
   int rightClick(Pattern& target, int modifiers = 0);
   int rightClick(char const* target, int modifiers = 0);
   int rightClick(Region& target, int modifiers = 0);
   int rightClick(Match& target, int modifiers = 0);
   
   int hover(Location target);
   int hover(Pattern& target);
   int hover(char const* target);
   int hover(Region& target);
   int hover(Match& target);
   
   int dragDrop(Location t1, Location t2, int modifiers = 0);
   int dragDrop(Pattern& t1, Pattern& t2, int modifiers = 0);
   int dragDrop(char const* t1, char const* t2, int modifiers = 0);
   int dragDrop(Region& t1, Region& t2, int modifiers = 0);
   int dragDrop(Match& t1, Match& t2, int modifiers = 0);
   
   int drag(Location target);
   int drag(Pattern& target);
   int drag(char const* target);
   int drag(Region& target);
   int drag(Match& target);
   
   int dropAt(Location target, double delay = 0.0);
   int dropAt(Pattern& target, double delay = 0.0);
   int dropAt(char const* target, double delay = 0.0);
   int dropAt(Region& target, double delay = 0.0);
   int dropAt(Match& target, double delay = 0.0);
   
   int type(const char* text, int modifiers = 0);   
   int type(Location target, const char* text, int modifiers = 0);
   int type(Pattern& target, const char* text, int modifiers = 0);
   int type(const char* target, const char* text, int modifiers = 0);
   int type(Region& target, const char* text, int modifiers = 0);
   int type(Match& target, const char* text, int modifiers = 0);
   
   
   
};



