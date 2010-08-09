/*
 *  region.c
 *  sikuli
 *
 *  Created by Tom Yeh on 7/23/10.
 *  Copyright 2010. All rights reserved.
 *
 */

#include <iostream>
#include <sstream>
#include "region.h"
#include "vision.h"
#include "settings.h"

using namespace sikuli;

////////////////////////////////////////////////////////////////////
/// Match Class
////////////////////////////////////////////////////////////////////

Match::Match() : Region(-1,-1,-1,-1) {
   init();
   score = -1;
}

Match::~Match(){
}

Match::Match(const Match& m) :
   Region(m)  {
   score = m.score;
   _target = Location(getCenter());   
}

Match::Match(int _x, int _y, int _w, int _h, double _score) :
   Region(_x,_y,_w,_h) {
   score = _score;
   _target = Location(getCenter());
}

int
Match::compareTo(const Match& m){
   return getScore() < m.getScore() ? -1 :
   getScore() > m.getScore() ? 1 : 0;
}

double
Match::getScore() const{
   return score;
}

Location
Match::getTarget() const{
   return _target;
}

void
Match::setTargetOffset(Location offset){
   _target = Location(getCenter());
   _target.x += offset.x;
   _target.y += offset.y;
}


////////////////////////////////////////////////////////////////////
/// Region Class
////////////////////////////////////////////////////////////////////

Region::Region() : Rectangle(0,0,0,0){
   init();
}

Region::Region(int x_, int y_, int w_, int h_) :
   Rectangle(x_,y_,w_,h_) {
   init();
}

Region::Region(const Rectangle& r) :
   Rectangle(r) {
   init();
}

Region::Region(const Region& r) :
   Rectangle(r) {
   init();
}

Region::~Region(){
   if (_pLastMatch != NULL)
      delete _pLastMatch;   
   if (_pLastMatches != NULL)
      delete _pLastMatches;
}

void
Region::init(){
   _pLastMatch = NULL;
   _pLastMatches = NULL;   
   //_evtMgr = new EventManager(this);   
}

Region
Region::getFullScreen(int screenId){
   int x,y,w,h;
   Robot::getDisplayBounds(screenId,x,y,w,h);
   return Region(x,y,w,h);
}   

Rectangle 
Region::getROI(){ 
   return Rectangle(x,y,w,h); 
}
   
void 
Region::setROI(int X, int Y, int W, int H){
   x = X;   y = Y;   w = W;   h = H;
}

void 
Region::setROI(Region roi){
   x = roi.x;
   y = roi.y;
   w = roi.w;
   h = roi.h;
}

void 
Region::setROI(Rectangle roi){
   x = (int)roi.x;
   y = (int)roi.y;
   w = (int)roi.w;
   h = (int)roi.h;
} 

Location 
Region::getCenter() const{ 
   return Location(x+w/2, y+h/2);
}


Location 
Region::toRobotCoord(Location l){
   return Location(l.x - x, l.y - y);
}

Match 
Region::toGlobalCoord(Match m){
   m.x += x;
   m.y += y;
   return m;
}

int 
Region::click(int modifiers){
   return Robot::click(BUTTON1_MASK, modifiers, false);   
}

int 
Region::click(Location target, int modifiers){
   return Robot::click(target.x,target.y, BUTTON1_MASK, modifiers, false);
}

int 
Region::click(Pattern target, int modifiers){
   return click(getLocationFromPSRML(target),modifiers);
}

int 
Region::click(const char* target, int modifiers){
   return click(getLocationFromPSRML(target),modifiers);
}

int 
Region::click(Region target, int modifiers){
   return click(getLocationFromPSRML(target),modifiers);
}

int 
Region::click(Match target, int modifiers){
   return click(getLocationFromPSRML(target),modifiers);
}


int 
Region::doubleClick(int modifiers){
   return Robot::click(BUTTON1_MASK, modifiers, true);
}

int 
Region::doubleClick(Location target, int modifiers){
   return Robot::click(target.x,target.y, BUTTON1_MASK, modifiers, true);   
}

int 
Region::doubleClick(Pattern& target, int modifiers){
   return doubleClick(getLocationFromPSRML(target),modifiers);
}

int 
Region::doubleClick(const char* target, int modifiers){
   return doubleClick(getLocationFromPSRML(target),modifiers);
}

int 
Region::doubleClick(Region& target, int modifiers){
   return doubleClick(getLocationFromPSRML(target),modifiers);
}

int 
Region::doubleClick(Match& target, int modifiers){
   return doubleClick(getLocationFromPSRML(target),modifiers);
}

int 
Region::rightClick(int modifiers){
   return Robot::click(BUTTON3_MASK, modifiers, false);
}

int 
Region::rightClick(Location target, int modifiers){
   return Robot::click(target.x, target.y, BUTTON3_MASK, modifiers, false);
}

int 
Region::rightClick(Pattern& target, int modifiers){
   return rightClick(getLocationFromPSRML(target),modifiers);
}

int 
Region::rightClick(const char* target, int modifiers){
   return rightClick(getLocationFromPSRML(target),modifiers);
}

int 
Region::rightClick(Region& target, int modifiers){
   return rightClick(getLocationFromPSRML(target),modifiers);
}

int 
Region::rightClick(Match& target, int modifiers){
   return rightClick(getLocationFromPSRML(target),modifiers);
}

int 
Region::hover(Location target){
   return Robot::hover(target.x, target.y);
}

int 
Region::hover(Pattern& target){
   return hover(getLocationFromPSRML(target));
}

int 
Region::hover(const char* target){
   return hover(getLocationFromPSRML(target));
}

int 
Region::hover(Region& target){
   return hover(getLocationFromPSRML(target));
}

int 
Region::hover(Match& target){
   return hover(getLocationFromPSRML(target));
}

int 
Region::dragDrop(Location t1, Location t2, int modifiers){
   return Robot::dragDrop(t1.x,t1.y,t2.x,t2.y,modifiers);
}

int 
Region::dragDrop(Pattern& t1, Pattern& t2, int modifiers ){
   Location loc1 = getLocationFromPSRML(t1);
   Location loc2 = getLocationFromPSRML(t2);
   return dragDrop(loc1, loc2, modifiers);
}
   
int 
Region::dragDrop(const char* t1, const char* t2, int modifiers){
   Location loc1 = getLocationFromPSRML(t1);
   Location loc2 = getLocationFromPSRML(t2);
   return dragDrop(loc1, loc2, modifiers);
}

int 
Region::dragDrop(Region& t1, Region& t2, int modifiers){
   Location loc1 = getLocationFromPSRML(t1);
   Location loc2 = getLocationFromPSRML(t2);
   return dragDrop(loc1, loc2, modifiers);
};

int 
Region::dragDrop(Match& t1, Match& t2, int modifiers){
   Location loc1 = getLocationFromPSRML(t1);
   Location loc2 = getLocationFromPSRML(t2);
   return dragDrop(loc1, loc2, modifiers);
}      


int
Region::drag(Location target){
   return Robot::drag(target.x, target.y);
}

int 
Region::drag(Pattern& target){
   return drag(getLocationFromPSRML(target));
}

int 
Region::drag(const char* target){
   return drag(getLocationFromPSRML(target));
}

int 
Region::drag(Region& target){
   return drag(getLocationFromPSRML(target));
}

int 
Region::drag(Match& target){
   return drag(getLocationFromPSRML(target));
}

int
Region::dropAt(Location target, double delay){
   return Robot::dropAt(target.x, target.y);
}

int 
Region::dropAt(Pattern& target, double delay){
   return dropAt(getLocationFromPSRML(target),delay);
}

int 
Region::dropAt(const char* target, double delay){
   return dropAt(getLocationFromPSRML(target),delay);
}

int 
Region::dropAt(Region& target, double delay){
   return dropAt(getLocationFromPSRML(target),delay);
}

int 
Region::dropAt(Match& target, double delay){
   return dropAt(getLocationFromPSRML(target),delay);
}

int
Region::paste(const char* text){
   return Robot::paste(text);
}

int
Region::paste(const Location& target, const char* text){
   return Robot::paste(target.x,target.y,text);
}

int
Region::paste(const Pattern& target, const char* text){
   return paste(getLocationFromPSRML(target), text);
}

int
Region::paste(const char* target, const char* text){
   return paste(getLocationFromPSRML(target), text);
}

int
Region::paste(const Region& target, const char* text){
   return paste(getLocationFromPSRML(target), text);
}

int
Region::paste(const Match& target, const char* text){
   return paste(getLocationFromPSRML(target), text);
}

int
Region::press(int key, int modifiers){   
   return 1;   
}

int
Region::type(const char* text, int modifiers){
   return Robot::type(text, modifiers);  
}

int
Region::type(Location target, const char* text, int modifiers){
   return Robot::type(target.x, target.y, text, modifiers);
}

int
Region::type(Pattern& target, const char* text, int modifiers){
   return type(getLocationFromPSRML(target), text, modifiers);
}

int
Region::type(const char* target, const char* text, int modifiers){
   return type(getLocationFromPSRML(target), text, modifiers);
}

int
Region::type(Region& target, const char* text, int modifiers){
   return type(getLocationFromPSRML(target), text, modifiers);
}

int
Region::type(Match& target, const char* text, int modifiers){
   return type(getLocationFromPSRML(target), text, modifiers);
}


void 
Region::mouseDown(int button){
   Robot::mouseDown(button);
}
   
void 
Region::mouseUp(int button){
   Robot::mouseUp(button);
}
         
void 
Region::keyDown(int key){
   Robot::keyDown(key);
}

void 
Region::keyUp(int key){
   Robot::keyUp(key);
}

Match
Region::getLastMatch(){
   if (_pLastMatch){
      return *_pLastMatch;
   }else {
      return Match();
   }
}

vector<Match>
Region::getLastMatches(){
   return *_pLastMatches;
}

Match
Region::find(Pattern ptn) throw(FindFailed) {
   Match m;
   if (Settings::AutoWaitTimeout > 0)
      m = wait(ptn, Settings::AutoWaitTimeout);
   else {
      m = findNow(ptn);
   }
   if (_pLastMatch == NULL)
      _pLastMatch = new Match();
   
   *_pLastMatch = m;
   return m;
}

Match 
Region::find(const char* imgURL) throw(FindFailed){
   return find(Pattern(imgURL));
}

vector<Match> 
Region::findAll(Pattern ptn) throw(FindFailed) {
   vector<Match> ms;
   if (Settings::AutoWaitTimeout > 0){
      ms = waitAll(ptn, Settings::AutoWaitTimeout);
   }else{
      ms  = findAllNow(ptn);
      if (Settings::AutoWaitTimeout && ms.empty())
         throw FindFailed(ptn);
   }
   if (_pLastMatches == NULL)
      _pLastMatches = new vector<Match>();
   *_pLastMatches = ms;
   return ms;
}

vector<Match> 
Region::findAll(const char* imgURL) throw(FindFailed) {
   return findAll(Pattern(imgURL));
}


Match 
Region::findNow(Pattern ptn) throw(FindFailed){
   cout << "[Region::findNow] Searching in (" << x << "," << y << ")-(" << x+w << "," << y+h << ")" << endl;
   ScreenImage simg = Robot::capture(0, x, y, w, h);
   Match m = Vision::find(simg, ptn)[0];
   
   m.x += x;
   m.y += y;
   
   m.setTargetOffset(ptn.getTargetOffset());
 
   cout << "[Region::findNow] Found at (" << m.x << "," << m.y << ") score = " << m.getScore()  << endl;
   
   if (m.getScore() <= 0)
      throw FindFailed(ptn);
   return m;
}

Match 
Region::findNow(const char* imgURL) throw(FindFailed){
   return findNow(Pattern(imgURL));
}

vector<Match> 
Region::findAllNow(Pattern ptn) throw(FindFailed){
   // ToDo: adjust capturing region for multi-monitor
   cout << "[Region::findAll] Searching in (" << x << "," << y << ")-(" << x+w << "," << y+h << ")" << endl;   
   ScreenImage simg = Robot::capture(0, x, y, w, h);
   
   vector<Match> ms = Vision::find(simg, ptn.all());
   
   for (int i=0;i<ms.size();++i)
      ms[i].setTargetOffset(ptn.getTargetOffset());
   cout << "[Region::findAll] Found " << ms.size() << " matches" << endl;
   if (ms.empty())
      throw FindFailed(ptn);
   return ms;
}

vector<Match> 
Region::findAllNow(const char* imgURL) throw(FindFailed){
   return findAllNow(Pattern(imgURL));
}
   

#include <time.h>
#define CLOCKS_PER_MSEC (CLOCKS_PER_SEC/1000)

Match
Region::wait(Pattern target) throw(FindFailed){
   return wait(target, Settings::AutoWaitTimeout );
} 

Match
Region::wait(Pattern target, double timeout) throw(FindFailed){
   Match m;

   long max_clocks_per_scan = CLOCKS_PER_MSEC * Settings::WaitScanRate;
   long clocks_limit = clock() + timeout * CLOCKS_PER_SEC;   
   while (clock() < clocks_limit){
      long before_find = clock();
      
      // [begin] findNow
      try {
         m = findNow(target);
         if (_pLastMatch == NULL)
            _pLastMatch = new Match();
         *_pLastMatch = m;
         return m;
      }catch (FindFailed e){
      }
      // [end]
      
      long actual_clocks_per_scan = clock() - before_find;
      long mseconds_to_delay = (max_clocks_per_scan - actual_clocks_per_scan)/CLOCKS_PER_MSEC;
      Robot::delay(max((long)10, mseconds_to_delay));
   }  

   if(Settings::ThrowException)
      throw FindFailed(target);
   return m;
}

Match
Region::wait(const char* target) throw(FindFailed){
   return wait(Pattern(target));
}

Match
Region::wait(const char* target, double timeout) throw(FindFailed){
   return wait(Pattern(target), timeout);
}

vector<Match>
Region::waitAll(Pattern target, double timeout) throw(FindFailed){
   vector<Match> ms;
   
   long max_clocks_per_scan = CLOCKS_PER_MSEC * Settings::WaitScanRate;
   long clocks_limit = clock() + timeout * CLOCKS_PER_SEC;   
   while (clock() < clocks_limit){
      long before_find = clock();
      
      // [begin] findAllNow
      try{
         ms = findAllNow(target);
         return ms;
      }catch (FindFailed ff){
      }
      // [end]
      
      long actual_clocks_per_scan = clock() - before_find;
      long mseconds_to_delay = (max_clocks_per_scan - actual_clocks_per_scan)/CLOCKS_PER_MSEC;
      Robot::delay(max((long)10, mseconds_to_delay));
   }  
   
   if(Settings::ThrowException)
      throw FindFailed(target);
   return ms;
}

vector<Match>
Region::waitAll(const char* target, double timeout) throw(FindFailed){
   return waitAll(Pattern(target), timeout);
}

bool
Region::exists(Pattern target, double timeout){
   try{
      wait(target, timeout);
      return true;
   }catch (FindFailed ff){
      return false;
   }
}
bool
Region::exists(const char* target, double timeout){
   return exists(Pattern(target), timeout);
}

bool
Region::exists(Pattern target){
   return exists(target, Settings::AutoWaitTimeout);
}
bool
Region::exists(const char* target){
   return exists(Pattern(target));
}

bool
Region::waitVanish(Pattern target, double timeout){
   
   long max_clocks_per_scan = CLOCKS_PER_MSEC * Settings::WaitScanRate;
   long clocks_limit = clock() + timeout * CLOCKS_PER_SEC;   
   while (clock() < clocks_limit){
      long before_find = clock();
      
      // [begin] findNow
      try {
         Match m = findNow(target);
      }catch (FindFailed e){
         return true;
      }      
      // [end]
      
      long actual_clocks_per_scan = clock() - before_find;
      long mseconds_to_delay = (max_clocks_per_scan - actual_clocks_per_scan)/CLOCKS_PER_MSEC;
      Robot::delay(max((long)10, mseconds_to_delay));
   }  
   
   return false;
}

bool
Region::waitVanish(Pattern target){
   return waitVanish(target, Settings::AutoWaitTimeout);
}

bool
Region::waitVanish(const char* target, double timeout){
   return waitVanish(Pattern(target));
}

bool
Region::waitVanish(const char* target){
   return waitVanish(target, Settings::AutoWaitTimeout);
}

#include "screen.h"

Region 
Region::nearby(){
   return nearby(PADDING);
}

Region 
Region::nearby(int range){
   Rectangle bounds = Screen(0).getBounds();
   Rectangle rect = Rectangle(x-range,y-range,w+range*2,h+range*2);
   rect = rect.intersection(bounds);
   return Region(rect);
}

Region 
Region::right(){  
   return right(9999999);
}

Region 
Region::right(int range){
   Rectangle bounds = Screen(0).getBounds();
   Rectangle rect = Rectangle(x+w,y,range,h);
   rect = rect.intersection(bounds);
   return Region(rect).taller(10);      
}

Region 
Region::left(){   
   return left(9999999);   
}

Region 
Region::left(int range){//
   Rectangle bounds = Screen(0).getBounds();
   Rectangle rect = Rectangle(x-range,y,range,h);
   rect = rect.intersection(bounds);
   return Region(rect).taller(10);   
}

Region 
Region::above(){
   return above(9999999);    
}

Region
Region::above(int range){
   Rectangle bounds = Screen(0).getBounds();
   Rectangle rect = Rectangle(x,y-range,w,range);
   rect = rect.intersection(bounds);
   return Region(rect).wider(10);
}

Region 
Region::below(){
   return below(9999999);   
}

Region 
Region::below(int range){
   Rectangle bounds = Screen(0).getBounds();
   Rectangle rect = Rectangle(x,y+h/2,w,range);
   rect = rect.intersection(bounds);
   return Region(rect).wider(10);
}

Region
Region::wider(int range){
   Rectangle bounds = Screen(0).getBounds();
   Rectangle rect = Rectangle(x-range,y,w+2*range,h);
   rect = rect.intersection(bounds);
   return Region(rect);   
}

Region
Region::taller(int range){
   Rectangle bounds = Screen(0).getBounds();
   Rectangle rect = Rectangle(x,y-range,w,h+2*range);
   rect = rect.intersection(bounds);
   return Region(rect);   
}

Region 
Region::inside(){ 
   return *this;
}

Location 
Region::getLocationFromPSRML(Pattern target){
   Match m = find(target);
   return m.getTarget();
}

Location 
Region::getLocationFromPSRML(const char* target){
   Match m = find(target);
   return m.getTarget();  
}

Location 
Region::getLocationFromPSRML(Region target){
   return target.getCenter();
}

Location 
Region::getLocationFromPSRML(Match target){
   return target.getTarget();
}

Location 
Region::getLocationFromPSRML(Location target){
   return target;
}



