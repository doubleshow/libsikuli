/*
 *  region.h
 *  sikuli
 *
 *  Created by Tom Yeh on 7/23/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef _SIKULI_REGION_
#define _SIKULI_REGION_

#include <vector>
#include <string>
#include <exception>

#include "robot.h"


using namespace std;

namespace sikuli{
   
   
class Settings{
public:
   static int DelayAfterDrag;
   static int DelayBeforeDrop;
   static int WaitScanRate;
   static bool ThrowException;
   static double AutoWaitTimeout;
};
   
struct Rectangle{
   
   Rectangle(){};
   
   Rectangle(int x_,int y_,int width_,int height_){
      x = x_; y = y_; width = width_; height = height_;
   }
   
   Rectangle intersection(Rectangle r) { 
      int x2 = min(x+width,r.x+r.width);
      int y2 = min(y+height,r.y+r.height);
      int x1 = max(x,r.x);
      int y1 = max(y,r.y);
      return Rectangle(x1,y1,x2-x1,y2-y1);
   };
   
   int x;
   int y;
   int width;
   int height;
};    


//#include <robot.h>
class ScreenImage{
   
public:
   
   ScreenImage(){};
   ScreenImage(cv::Mat image) : _image(image){};
   ScreenImage(Rectangle roi, 
               cv::Mat image) : _roi(roi), _image(image){};
   
   Rectangle getROI(){
      return _roi;
   }
   
   string getFilename();
   
   cv::Mat getMat() { return _image;};
   
private:
   
   string _filename;
   Rectangle _roi;
   cv::Mat _image;

};






struct Point {

   Point(int _x, int _y) : x(_x), y(_y) {};   
   int x;
   int y;
};

class Location : public Point{
   
public: 
   
   Location() : Point(-1,-1){};
   Location(int _x, int _y) : Point(_x,_y){}      
   Location(const Location& loc) : Point(loc.x, loc.y){};
   
   Location offset(int dx, int dy){
      return Location(x+dx, y+dy);
   }
      
   Location left(int dx){
      return Location(x-dx, y);
   }
      
   Location right(int dx){
      return Location(x+dx, y);
   }
      
   Location above(int dy){
      return Location(x, y-dy);
   }
      
   Location below(int dy){
      return Location(x, y+dy);
   }
      
//      public String toString(){
//         return "(" + x + "," + y + ")";
//      }
};
   
   
   
   
class Pattern{
   
public:
   
   Pattern();
   Pattern(const Pattern& p);
   Pattern(const char* imgURL_);
   
   Pattern similar(float similarity_);
   Pattern exact();
   
   Pattern targetOffset(int dx_, int dy_);
   
   Location getTargetOffset();
   float getSimilarity();
   const char* getImgURL();
   
   string toString();
   
private:
   
   string imgURL;
   float similarity;
   int dx;
   int dy;

};   
  
   class FindFailed : public exception {
      
   public:
      FindFailed(string _msg) : msg(_msg) {};
      FindFailed(Pattern _ptn) : ptn(_ptn) {};
      
      virtual const char* what() const throw(){
         return msg.c_str();
      }
      
      virtual ~FindFailed() throw() {};
      
   private:
      
      string msg;
      Pattern ptn;
      
   };
   
class Match;



// TYPE MODE
#define PRESS_RELEASE 0
#define PRESS_ONLY 1
#define RELEASE_ONLY 2
   
//
#define K_SHIFT 1
#define K_CTRL  2
#define K_META  4  
#define K_ALT   8
   

#define PADDING 50   
   
class Vision;
   
class Region{


public:
   
   Region(int x_, int y_, int w_, int h_);
   Region(const Region& r);
   Region(Rectangle r);
   Region();
   ~Region();
   
//==================================================================
// Setting/Getting Attributes
//==================================================================
   
   
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
   
   static Region getFullScreen(int screenId = 0);
   
   Location getCenter() const;
   
   Match getLastMatch();
   vector<Match> getLastMatches();
  
//   void setThrowException(bool flag){ _throwException = flag; } 
//   void setAutoWaitTimeout(double sec){ _autoWaitTimeout = sec; }
//   
//   bool getThrowException(){ return _throwException; }
//   double getAutoWaitTimeout(){ return _autoWaitTimeout; }   
   
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

private:
   
   Match findNow(Pattern ptn) throw(FindFailed);
   Match findNow(const char* imgURL) throw(FindFailed);

   vector<Match> findAllNow(Pattern ptn) throw(FindFailed);
   vector<Match> findAllNow(const char* imgURL) throw(FindFailed);
      
   vector<Match> waitAll(Pattern target, double timeout) throw(FindFailed);
   vector<Match> waitAll(const char* target, double timeout) throw(FindFailed);
   
   Location getLocationFromPSRML(Pattern target);
   Location getLocationFromPSRML(const char* target);
   Location getLocationFromPSRML(Region target);
   Location getLocationFromPSRML(Match target);
   Location getLocationFromPSRML(Location target);   
 
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

   
   template<class PSRML> int paste(PSRML target, string text);
   
   void mouseDown(int buttons);
   void mouseUp(int buttons);
   void keyDown(string keys);
   void keyUp(string keys);
   
private:
   
   int _click(Location loc, int buttons, int modifers, bool dblClick);
   int _click(int buttons, int modifers, bool dblClick);
   
   void pressModifiers(int modifiers);
   void releaseModifiers(int modifiers);
   
   void doType(int mode, int keyCode);
   void doType(int mode, int keyCode1, int keycode2);
   void type_ch(char character, int mode );  
     
//==================================================================
// Spatial Operators
//==================================================================
   
public:
   
   Region nearby();
   Region nearby(int range);
   
   Region right();
   Region right(int range);
   
   Region left();
   Region left(int range);
   
   Region above();
   Region above(int range);
   
   Region below();
   Region below(int range);
   
   Region wider(int range = 9999999);
   Region taller(int range = 9999999);
   
   Region inside();
   
protected:
   
   void init(int x_, int y_, int w_, int h_);
   
   Match* _pLastMatch;
   vector<Match>* _pLastMatches;
   
   int x;
   int y;
   int w;
   int h; 
   
//   bool _throwException;
//   double _autoWaitTimeout;
   
   
//   int _screenId;
   
private:
      
   Location toRobotCoord(Location l);
   Match toGlobalCoord(Match m);
  
   int _hold_buttons;// = 0;
   string _hold_keys;
   
   Robot _robot;
   //Screen* _scr;
   
 
};
   
//class Screen : public Region {
//
//public: 
//   
//   Rectangle getBounds();
//   int getNumberScreens();
//   void capture();
//   
//   
//};
   
class Match : public Region {
   
public:
   
   Match();
   ~Match();
   Match(int _x, int _y, int _w, int _h, double _score);
   Match(const Match& m);
   
   int compareTo(const Match& m);
   
   Location getTarget() const;

   double getScore() const;
   
   void setTargetOffset(Location offset);
   
private:
 
   double score;
   Location _target;
};

   
class Vision{
   public:
      
      //   static Match find(ScreenImage& simg);
   static Match find(ScreenImage simg, Pattern ptn) throw(FindFailed);
   static vector<Match> findAll(ScreenImage simg, Pattern ptn) throw(FindFailed);
      
};
   

};

#endif // _SIKULI_REGION_
