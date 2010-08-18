/*
 *  region.h
 *  sikuli
 *
 *  Created by Tom Yeh on 7/23/10.
 *  Copyright 2010 sikuli.org. All rights reserved.
 *
 */
#ifndef _SIKULI_REGION_
#define _SIKULI_REGION_

#include <vector>
#include <string>
#include <sstream>

#include "robot.h"
#include "pattern.h"
#include "location.h"
#include "exceptions.h"

using namespace std;

namespace sikuli{
   
class Rectangle{

public:
   
   Rectangle(){
      x = 0; y = 0; w = 0; h = 0;
   };
   
   Rectangle(const Rectangle& r){
      x = r.x; y = r.y; w = r.w; h = r.h;
   }
   
   Rectangle(int x_,int y_,int w_,int h_){
      x = x_; y = y_; w = w_; h = h_;
   }
   
   Rectangle intersection(Rectangle r) { 
      int x2 = min(x+w,r.x+r.w);
      int y2 = min(y+h,r.y+r.h);
      int x1 = max(x,r.x);
      int y1 = max(y,r.y);
      return Rectangle(x1,y1,x2-x1,y2-y1);
   };
   
   const char* toString(){
      stringstream ss;
      ss << x << "," << y << "," << w << "," << h;
      repr = ss.str();
      return repr.c_str();
   }
   
   int x;
   int y;
   int w;
   int h;
   
   private:
      string repr;
};    

class ScreenImage{
   
public:
   
   ScreenImage(){};
   ScreenImage(cv::Mat image) : _image(image){};
   ScreenImage(Rectangle roi, 
               cv::Mat image) : _roi(roi), _image(image){};
   
   Rectangle getROI(){
      return _roi;
   }
   
   string getFilename(){
      return _filename;
   }
   
   cv::Mat getMat() { return _image;}
   
private:
   
   string _filename;
   Rectangle _roi;
   cv::Mat _image;

};
  

   //
class Match;
class SikuliEventManager;
class SikuliEventHandler;
struct Event;
typedef void (*SikuliEventCallback)(Event);

//#include "event-manager.h"
#include "keys.h"

#define PADDING 50   
   
   

class Region : public Rectangle {


public:
   
   Region(int x_, int y_, int w_, int h_, int screen_id_ = 0);
   Region(const Region& r);
   Region(const Rectangle& r, int screen_id_ = 0);

   Region();
   ~Region();   
   
protected:
   
   void init(int screen_id);
   void setScreenCoordinateOrigin(int x, int y);
   
   // top-left origin in the screen coordinate
   int xo;
   int yo;
   
   int _screen_id;
   
   
//==================================================================
// Setting/Getting Attributes
//==================================================================

public:
   
   int getX(){ return x; }
   int getY(){ return y; }
   int getW(){ return w; }
   int getH(){ return h; }
   
   void setX(int x_){ x = x_; }
   void setY(int y_){ y = y_; }
   void setW(int w_){ w = w_; }
   void setH(int h_){ h = h_; }   
   
   
   
   // Deprecated
//   Rectangle getROI();
//   void setROI(int X, int Y, int W, int H);
//   void setROI(Region roi);
//   void setROI(Rectangle roi);
   
  
   // Compare whether two regions are spatially equivalent. Two
   // regions are considered spatially equivalent if they have the
   // same upper-left corner in the screen coordinate, same width,
   // same height, and on the same screen. Note that since regions
   // can have different local coordinate systems, having the same
   // (x,y,w,h) does not necessarily mean they refer to the same
   // region on the screen. On the other hand, two regions with 
   // different (x,y,w,h)'s may refer to the same screen region after
   // being converted to the screen coordinate.
   bool operator==(const Region& r);
   
   
   Location getCenter() const;
   
   Match getLastMatch();
   vector<Match> getLastMatches();
   void setLastMatch(Match match);
   void setLastMatches(vector<Match> matches);

protected:
   
   Match* _pLastMatch;
   vector<Match>* _pLastMatches;
   
//==================================================================
// Pattern Matching Functions
//==================================================================
   
public:
   
   Match find(Pattern ptn) throw(FindFailed);
   Match find(const char* target) throw(FindFailed);
   
   vector<Match> findAll(Pattern ptn) throw(FindFailed);
   vector<Match> findAll(const char* imgURL) throw(FindFailed);
      
   Match wait(Pattern target) throw(FindFailed);
   Match wait(Pattern target, int seconds) throw(FindFailed);
   Match wait(const char* target) throw(FindFailed);
   Match wait(const char* target, int seconds) throw(FindFailed); 
   
   
   bool exists(Pattern target);
   bool exists(Pattern target, int seconds);
   bool exists(const char* target);
   bool exists(const char* target, int seconds);

   bool waitVanish(Pattern target); 
   bool waitVanish(Pattern target, int seconds);
   bool waitVanish(const char* target); 
   bool waitVanish(const char* target, int seconds);

   
   Match findNow(Pattern ptn) throw(FindFailed);
   Match findNow(const char* imgURL) throw(FindFailed);

   vector<Match> findAllNow(Pattern ptn) throw(FindFailed);
   vector<Match> findAllNow(const char* imgURL) throw(FindFailed);
      
   vector<Match> waitAll(Pattern target,  int seconds) throw(FindFailed);
   vector<Match> waitAll(const char* target,  int seconds) throw(FindFailed);

private:
   
   vector<Match> wait_callback(Pattern target);
   vector<Match> waitAll_callback(Pattern target);
   vector<Match> waitVanish_callback(Pattern target);
   
   typedef vector<Match> (Region::*callback)(Pattern target); 
   vector<Match> try_for_n_seconds(callback func, Pattern target, int seconds);   
   
   
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
   int click(Pattern target, int modifiers = 0);
   int click(const char* target, int modifiers = 0);
   int click(Region target, int modifiers = 0);
   int click(Match target, int modifiers = 0);
   
   int doubleClick(int modifiers = 0);
   int doubleClick(Location target, int modifiers = 0);
   int doubleClick(Pattern& target, int modifiers = 0);
   int doubleClick(const char* target, int modifiers = 0);
   int doubleClick(Region& target, int modifiers = 0);
   int doubleClick(Match& target, int modifiers = 0);
      
   int rightClick(int modifiers = 0);
   int rightClick(Location target, int modifiers = 0);   
   int rightClick(Pattern& target, int modifiers = 0);
   int rightClick(const char* target, int modifiers = 0);
   int rightClick(Region& target, int modifiers = 0);
   int rightClick(Match& target, int modifiers = 0);
   
   int hover(Location target);
   int hover(Pattern target);
   int hover(const char* target);
   int hover(Region target);
   int hover(Match target);

   int dragDrop(Location t1, Location t2, int modifiers = 0);
   int dragDrop(Pattern& t1, Pattern& t2, int modifiers = 0);
   int dragDrop(const char* t1, const char* t2, int modifiers = 0);
   int dragDrop(Region& t1, Region& t2, int modifiers = 0);
   int dragDrop(Match& t1, Match& t2, int modifiers = 0);
   
   int drag(Location target);
   int drag(Pattern& target);
   int drag(const char* target);
   int drag(Region& target);
   int drag(Match& target);
   
   int dropAt(Location target, double delay = 0.0);
   int dropAt(Pattern& target, double delay = 0.0);
   int dropAt(const char* target, double delay = 0.0);
   int dropAt(Region& target, double delay = 0.0);
   int dropAt(Match& target, double delay = 0.0);
   
   int type(const char* text, int modifiers = 0);   
   int type(Location target, const char* text, int modifiers = 0);
   int type(Pattern& target, const char* text, int modifiers = 0);
   int type(const char* target, const char* text, int modifiers = 0);
   int type(Region& target, const char* text, int modifiers = 0);
   int type(Match& target, const char* text, int modifiers = 0);

   int press(int key, int modifiers = 0);   
//   int press(Location target, int key, int modifiers = 0);
//   int press(Pattern& target, int key, int modifiers = 0);
//   int press(const char* target, int key, int modifiers = 0);
//   int press(Region& target, int key, int modifiers = 0);
//   int press(Match& target, int key, int modifiers = 0);
   
   int paste(const char* text);   
   int paste(const Location& target, const char* text);
   int paste(const Pattern& target, const char* text);
   int paste(const char* target, const char* text);
   int paste(const Region& target, const char* text);
   int paste(const Match& target, const char* text);
   
   void mouseDown(int buttons);
   void mouseUp(int buttons);

   void keyDown(int key);
   void keyUp(int key);
   
   
   ScreenImage capture();
   
//==================================================================
// Spatial Operators
//==================================================================
   
public:
   
   
   // All derivative regions using spatial operators will use
   // the local coordinate system defined by the source region.
   // The origin (0,0) corresponds to the upper-left corner
   // of the source region. A derivative region often goes beyond
   // the boundary of the source region. For example, all the
   // "neighborhood" operators (i.e., left, right, below, above)
   // return regions outside but adjacent to the source region.
   // An inner region obtained by the "inner" operator will be
   // strictly inside and no larger than the source region. 
   
   // (xo,yo) stores the screen location of the origin of the
   // coordinate system used by the region. Internally, it is
   // used to convert a local coordinate to the corresponding
   // screen coordinate where mouse/key events can be delivered
   // to. 
   
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
   
   // Obtain an inner region
   Region inner(int x, int y, int w, int h);   
   
private:
   
   // Take a Rectangle in the screen coordinate system and return
   // a valid Region w.r.t. the screen of this region
   Region spatialOpHelper(Rectangle rect);


//==================================================================
// Events
//==================================================================
public:
   
   void onAppear(Pattern ptn, SikuliEventHandler* handler);
   void onVanish(Pattern ptn, SikuliEventHandler* handler);
   void onChange(SikuliEventHandler* handler);
   
   void onAppear(Pattern ptn, SikuliEventCallback func);
   void onVanish(Pattern ptn, SikuliEventCallback func);
   void onChange(SikuliEventCallback func);   
   
private:
   
   void onEvent(int event_type, Pattern ptn, SikuliEventHandler* handler);
   void onEvent(int event_type, Pattern ptn, SikuliEventCallback func);
         
};
   


class Match : public Region {
   
public:
   
   Match();
   ~Match();
   
   Match(const Region& r, double _score);
   Match(const Match& m);
   
   int compareTo(const Match& m);
   
   Location getTarget() const;

   double getScore() const;
   
   void setTargetOffset(Location offset);
   
private:
 
   double score;
   Location _target;
};

   

   

};

#endif // _SIKULI_REGION_
