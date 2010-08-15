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

#include "screen.h"
#include "region.h"
#include "vision.h"
#include "settings.h"
#include "event-manager.h"


#define dout if (1) cout

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

Match::Match(const Region& r, double _score) :
Region(r), score(_score){
   _target = Location(getCenter());
}

Match::Match(int _screen_id, int _x, int _y, int _w, int _h, double _score) :
   Region(_x,_y,_w,_h) {
   score = _score;
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

Region::Region(int screen_id_, int x_, int y_, int w_, int h_) :
Rectangle(x_,y_,w_,h_) {
   init();
   _screen_id = screen_id_;
}


Region::Region(const Rectangle& r) : Rectangle(r) {
   init();
}

Region::Region(int screen_id, const Rectangle& r) : Rectangle(r) {
   init();
   _screen_id = screen_id;
   xo = 0;
   yo = 0;
}

Region::Region(const Region& r) : Rectangle(r) {
   init();
   _screen_id = r._screen_id;
   xo = r.xo;
   yo = r.yo;
}

Region::~Region(){
   if (_pLastMatch != NULL)
      delete _pLastMatch;   
   if (_pLastMatches != NULL)
      delete _pLastMatches;
}


Region 
Region::inner(int x_, int y_, int w_, int h_){
   Region r(x_,y_,w_,h_);
   
   // copy the screen id
   r._screen_id = _screen_id;
   
   // calculate the screen coordinate origin
   r.xo = xo + x;
   r.yo = yo + y;
   return r;
}

void
Region::init(){
   _screen_id = 0;
   _pLastMatch = NULL;
   _pLastMatches = NULL;   
}

void
Region::setScreenCoordinateOrigin(int x0, int y0){
   xo = x;
   yo = y;
}

bool
Region::operator==(const Region& r){
   return true;
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


ScreenImage
Region::capture(){
   // when capturing, we need to use the screen coordinate system
   return Robot::capture(_screen_id, xo+x, yo+y, w, h);
}

int 
Region::click(int modifiers){
   return Robot::click(BUTTON1_MASK, modifiers, false);   
}

int 
Region::click(Location target, int modifiers){
   return Robot::click(_screen_id, xo+x+target.x, yo+y+target.y, BUTTON1_MASK, modifiers, false);
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
   return Robot::click(_screen_id, target.x,target.y, BUTTON1_MASK, modifiers, true);   
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
   return Robot::click(_screen_id, target.x, target.y, BUTTON3_MASK, modifiers, false);
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
   return Robot::hover(_screen_id, target.x, target.y);
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
   // TODO: support cross-monitor drag-drop
   return Robot::dragDrop(_screen_id, t1.x,t1.y,_screen_id, t2.x,t2.y,modifiers);
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
   return Robot::drag(_screen_id, target.x, target.y);
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
   return Robot::dropAt(_screen_id, target.x, target.y);
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
   return Robot::paste(_screen_id, target.x,target.y,text);
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
   return Robot::press(key, modifiers);   
}

int
Region::type(const char* text, int modifiers){
   return Robot::type(text, modifiers);  
}

int
Region::type(Location target, const char* text, int modifiers){
   return Robot::type(_screen_id, target.x, target.y, text, modifiers);
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

void
Region::setLastMatch(Match match){
   if (_pLastMatch == NULL)
      _pLastMatch = new Match();
   *_pLastMatch = match;
}

void
Region::setLastMatches(vector<Match> matches){
   if (_pLastMatches == NULL)
      _pLastMatches = new vector<Match>();
   *_pLastMatches = matches;
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
   dout << "[Region::findNow] Searching in (" << xo+x << "," << yo+y << ")-(" << xo+x+w << "," << yo+y+h << ")" << endl;
   ScreenImage simg = capture();//Robot::capture(0, x, y, w, h);
   
   FindResult r = Vision::find(simg, ptn)[0];
   Match match(inner(r.x,r.y,r.w,r.h),r.score); 

   
   // TODO: setTargetOffset 
   //match.setTargetOffset(ptn.getTargetOffset());
 
   dout << "[Region::findNow] Found at (" << match.x << "," << match.y << "), score = " << match.getScore()  << endl;
   
   if (match.getScore() <= 0)
      throw FindFailed(ptn);
   return match;
}

Match 
Region::findNow(const char* imgURL) throw(FindFailed){
   return findNow(Pattern(imgURL));
}

vector<Match> 
Region::findAllNow(Pattern ptn) throw(FindFailed){
   // ToDo: adjust capturing region for multi-monitor
   cout << "[Region::findAll] Searching in (" << x << "," << y << ")-(" << x+w << "," << y+h << ")" << endl;   
   ScreenImage simg = capture();
   
   vector<FindResult> results = Vision::find(simg, ptn.all());
   vector<Match> matches;
   int n = min((int)results.size(), (int)ptn.getLimit());
   for (int i=0; i< n; ++i){
      FindResult& r = results[i];
      Match match(inner(r.x,r.y,r.w,r.h),r.score); 
      matches.push_back(match);
   }
   
   
   for (int i=0;i<matches.size();++i)
      matches[i].setTargetOffset(ptn.getTargetOffset());
 
   cout << "[Region::findAll] Found " << matches.size() << " matches" << endl;
   if (matches.empty())
      throw FindFailed(ptn);
   return matches;
}

vector<Match> 
Region::findAllNow(const char* imgURL) throw(FindFailed){
   return findAllNow(Pattern(imgURL));
}
   

#include <time.h>
#define CLOCKS_PER_MSEC ((long)CLOCKS_PER_SEC/(long)1000)

Match
Region::wait(Pattern target) throw(FindFailed){
   return wait(target, Settings::AutoWaitTimeout);
} 


vector<Match> 
Region::try_for_n_seconds(callback func, Pattern target, int seconds){
   vector<Match> ms;
   
   
   //cout << CLOCKS_PER_MSEC << endl;
   long max_clocks_per_scan = CLOCKS_PER_SEC / Settings::WaitScanRate;
   //long start = clock();
   long clocks_limit = clock() + seconds * CLOCKS_PER_SEC;   
   time_t time_limit = time(NULL) + seconds;
   time_t start_time;
   start_time = time(NULL);
   while (time(NULL) < time_limit){
      //while (clock() < clocks_limit){
      long before_find = clock();
      
      ms = (this->*func)(target);
      if (!ms.empty())
         return ms;
      
      long actual_clocks_per_scan = clock() - before_find;
      long mseconds_to_delay = (max_clocks_per_scan - actual_clocks_per_scan)/CLOCKS_PER_MSEC;
      Robot::delay(max((long)10, mseconds_to_delay));
      //cout << 1.0*(clock() - start)/ CLOCKS_PER_SEC << " seconds" << endl;
      cout << (time(NULL) - start_time) << " seconds" << endl;
   }        
   return ms;
}

vector<Match>
Region::wait_callback(Pattern target) {
   vector<Match> ms;
   try{
      Match m = findNow(target);
      ms.push_back(m);
   }catch(FindFailed ff){
   }
   return ms;
}
      
Match
Region::wait(Pattern target, int seconds) throw(FindFailed){
   vector<Match> ms = try_for_n_seconds(&Region::wait_callback, target, seconds);   
   if (!ms.empty()){
      setLastMatch(ms[0]);
      return getLastMatch();
   }else {
   
  // if(Settings::ThrowException)
      throw FindFailed(target);
   }
}

Match
Region::wait(const char* target) throw(FindFailed){
   return wait(Pattern(target));
}

Match
Region::wait(const char* target, int seconds) throw(FindFailed){
   return wait(Pattern(target), seconds);
}

vector<Match>
Region::waitAll_callback(Pattern target) {
   vector<Match> ms;
   try{
      ms = findAllNow(target);
   }catch(FindFailed ff){
   }
   return ms;
}


vector<Match>
Region::waitAll(Pattern target, int seconds) throw(FindFailed){
   vector<Match> ms = try_for_n_seconds(&Region::waitAll_callback, target, seconds);
   
   if (!ms.empty()){
      return ms;
   }else{
      setLastMatches(ms);
      return getLastMatches();
   
   //if(Settings::ThrowException)
      throw FindFailed(target);
   }
}   


vector<Match>
Region::waitAll(const char* target, int seconds) throw(FindFailed){
   return waitAll(Pattern(target), seconds);
}

bool
Region::exists(Pattern target, int seconds){
   try{
      wait(target, seconds);
      return true;
   }catch (FindFailed ff){
      return false;
   }
}
bool
Region::exists(const char* target, int seconds){
   return exists(Pattern(target), seconds);
}

bool
Region::exists(Pattern target){
   return exists(target, Settings::AutoWaitTimeout);
}
bool
Region::exists(const char* target){
   return exists(Pattern(target));
}


vector<Match>
Region::waitVanish_callback(Pattern target) {
   vector<Match> ms;
   try{
      Match m = findNow(target);
      
      // purposely return an empty vector to
      // indicate the target is still there
      return ms; 
   }catch(FindFailed ff){
      // purposely return a non-empty vector
      // to indicate the target has vanished
      ms.push_back(Match()); 
      return ms;
   }
}

bool
Region::waitVanish(Pattern target, int seconds){
   vector<Match> ms = try_for_n_seconds(&Region::waitVanish_callback, target, seconds);   
   if (!ms.empty()){
      return true;
   }else {
      return false;
   }   
}
   
bool
Region::waitVanish(Pattern target){
   return waitVanish(target, Settings::AutoWaitTimeout);
}

bool
Region::waitVanish(const char* target, int seconds){
   return waitVanish(Pattern(target));
}

bool
Region::waitVanish(const char* target){
   return waitVanish(target, Settings::AutoWaitTimeout);
}

Region 
Region::nearby(){
   return nearby(PADDING);
}

Region 
Region::nearby(int range){
   return spatialOpHelper(Rectangle(xo+x-range,yo+y-range,w+range*2,h+range*2));
}

Region 
Region::right(){  
   return right(9999999);
}

Region 
Region::right(int range){
   return spatialOpHelper(Rectangle(xo+x+w, yo+y, range, h));
}

Region 
Region::left(){   
   return left(9999999);   
}

Region 
Region::left(int range){
   return spatialOpHelper(Rectangle(xo+x-range, yo+y, range, h));
}

Region 
Region::above(){
   return above(9999999);    
}

Region
Region::above(int range){
   return spatialOpHelper(Rectangle(xo+x, yo+y-range, w, range));
}

Region 
Region::below(){
   return below(9999999);   
}

Region 
Region::below(int range){
   return spatialOpHelper(Rectangle(xo+x, yo+y+h/2, w, range));
}

Region
Region::wider(int range){
   return spatialOpHelper(Rectangle(xo+x-range,yo+y,w+2*range,h));
}

Region
Region::taller(int range){
   return spatialOpHelper(Rectangle(xo+x,yo+y-range,w,h+2*range));
}

Region
Region::spatialOpHelper(Rectangle rect){
   int sh,sw;
   Robot::getScreenSize(_screen_id, sw, sh);
   Rectangle bounds(0,0,sw,sh);
   rect = rect.intersection(bounds);   
   Region region = Screen(_screen_id).inner(rect.x,rect.y,rect.w,rect.h);
   region.xo -= xo;
   region.yo -= yo;
   return region;
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

void 
Region::onEvent(int event_type, Pattern ptn, int handler_id){
   Observer ob(event_type, ptn, handler_id);
   EventManager::addObserver(*this, ob);
}


void 
Region::onEvent(int event_type, Pattern ptn,  SikuliEventCallback func){
   Observer ob(event_type, ptn, func);
   EventManager::addObserver(*this, ob);
}

void 
Region::onAppear(Pattern ptn, int handler_id){
   onEvent(APPEAR, ptn, handler_id);
}

void 
Region::onVanish(Pattern ptn, int handler_id){
   onEvent(VANISH, ptn, handler_id);
}

void 
Region::onChange(int handler_id){
   onEvent(CHANGE, Pattern(), handler_id);
}

void 
Region::onAppear(Pattern ptn, SikuliEventCallback func){
   onEvent(APPEAR, ptn, func);
}

void 
Region::onVanish(Pattern ptn, SikuliEventCallback func){
   onEvent(VANISH, ptn, func);
}

void 
Region::onChange(SikuliEventCallback func){   
   onEvent(CHANGE, Pattern(), func);
}


