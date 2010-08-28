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
#include "ui.h"

#define dout if (0) cout

using namespace sikuli;

void
ScreenImage::show(int seconds){
   cv::namedWindow("sikuli",CV_WINDOW_AUTOSIZE);
   cv::imshow("sikuli", _image);
   cv::waitKey(seconds);
   cvDestroyWindow("sikuli");
}

void
ScreenImage::show(){
   show(0);
}

void
ScreenImage::save(const char* filename){
   cv::imwrite(filename, _image);
}

////////////////////////////////////////////////////////////////////
/// Match Class
////////////////////////////////////////////////////////////////////

Match::Match() : Region(-1,-1,-1,-1) {
   init(0);
   score = -1;
}

Match::~Match(){
}

Match::Match(const Match& m) :
   Region(m), score(m.score)  {
   _target = Location(getCenter());   
}

Match::Match(const Region& r, double _score) :
   Region(r), score(_score){
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
   init(0);
}

Region::Region(int x_, int y_, int w_, int h_, int screen_id_) :
   Rectangle(x_,y_,w_,h_) {
   init(screen_id_);
}

Region::Region(const Rectangle& r, int screen_id) : Rectangle(r) {
   init(screen_id);
}

Region::Region(const Region& r) : Rectangle(r) {
   init(r._screen_id);
   xo = r.xo;
   yo = r.yo;
}

Region::~Region(){
   if (_pLastMatch != NULL)
      delete _pLastMatch;   
   if (_pLastMatches != NULL)
      delete _pLastMatches;
}


Region& 
Region::operator=(const Region& r){
   if (this == &r)
      return *this;
   
   if (r._pLastMatch != NULL)
      setLastMatch(*r._pLastMatch);
   
   if (r._pLastMatches != NULL)
      setLastMatches(*r._pLastMatches);
   
   Rectangle::operator=(r);
   
   xo = r.xo;
   yo = r.yo;
   _screen_id = r._screen_id;
   
   return *this;
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
Region::init(int screen_id){
   _screen_id = screen_id;
   _pLastMatch = NULL;
   _pLastMatches = NULL;   
   xo = 0;
   yo = 0;
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

Location 
Region::getCenter() const{ 
   return Location(x+w/2, y+h/2);
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
Region::click(Pattern target, int modifiers) throw(FindFailed) {
   Match m;
   if (findInteractive(target, m))
      return click(m, modifiers);
   else 
      return false;
}

int 
Region::click(const char* str, int modifiers) throw(FindFailed) {   
   return click(Pattern(str),modifiers);
}

int 
Region::click(Region target, int modifiers){
   return click(target.getCenter(),modifiers);
}

int 
Region::click(Match target, int modifiers){
   return click(target.getCenter(),modifiers);
}

int 
Region::doubleClick(int modifiers){
   return Robot::click(BUTTON1_MASK, modifiers, true);
}

int 
Region::doubleClick(Location target, int modifiers){
   return Robot::click(_screen_id, xo+x+target.x,  yo+y+target.y, BUTTON1_MASK, modifiers, true);   
}

int 
Region::doubleClick(Pattern target, int modifiers) throw(FindFailed){
   Match m;
   if (findInteractive(target, m))
      return doubleClick(m, modifiers);
   else 
      return false;
}

int 
Region::doubleClick(const char* target, int modifiers) throw(FindFailed){
   return doubleClick(Pattern(target), modifiers);
}

int 
Region::doubleClick(Region target, int modifiers){
   return doubleClick(target.getCenter(),modifiers);
}

int 
Region::doubleClick(Match target, int modifiers){
   return doubleClick(target.getCenter(),modifiers);
}

int 
Region::rightClick(int modifiers){
   return Robot::click(BUTTON3_MASK, modifiers, false);
}

int 
Region::rightClick(Location target, int modifiers){
   return Robot::click(_screen_id, xo+x+target.x,  yo+y+target.y, BUTTON3_MASK, modifiers, false);
}

int 
Region::rightClick(Pattern target, int modifiers) throw(FindFailed){
   Match m;
   if (findInteractive(target, m))
      return rightClick(m, modifiers);
   else 
      return false;
}

int 
Region::rightClick(const char* target, int modifiers) throw(FindFailed){
   return rightClick(Pattern(target),modifiers);
}

int 
Region::rightClick(Region target, int modifiers){
   return rightClick(target.getCenter(),modifiers);
}

int 
Region::rightClick(Match target, int modifiers){
   return rightClick(target.getCenter(),modifiers);
}

int 
Region::hover(Location target){
   return Robot::hover(_screen_id,  xo+x+target.x, yo+y+target.y);
}

int 
Region::hover(Pattern target) throw(FindFailed){
   Match m;
   if (findInteractive(target, m))
      return hover(m);
   else 
      return false;
}

int 
Region::hover(const char* target) throw(FindFailed){
   return hover(Pattern(target));
}

int 
Region::hover(Region target){
   return hover(target.getCenter());
}

int 
Region::hover(Match target){
   return hover(target.getCenter());
}

int 
Region::dragDrop(Location t1, Location t2, int modifiers){
   // TODO: support cross-monitor drag-drop
   return Robot::dragDrop(_screen_id, xo+x+t1.x,yo+y+t1.y,_screen_id, 
                          xo+x+t2.x,yo+y+t2.y,modifiers);
}

int 
Region::dragDrop(Pattern t1, Pattern t2, int modifiers) throw(FindFailed){
   Match m1,m2;
   if (findInteractive(t1, m1) && findInteractive(t2,m2))
      return dragDrop(m1,m2,modifiers);
   else 
      return false;
}
   
int 
Region::dragDrop(const char* t1, const char* t2, int modifiers) throw(FindFailed){
   return dragDrop(Pattern(t1),Pattern(t2), modifiers);
}

int 
Region::dragDrop(Region t1, Region t2, int modifiers){
   Location loc1 = t1.getCenter();
   Location loc2 = t2.getCenter();
   return dragDrop(loc1, loc2, modifiers);
};

int 
Region::dragDrop(Match t1, Match t2, int modifiers){
   Location loc1 = t1.getCenter();
   Location loc2 = t2.getCenter();
   return dragDrop(loc1, loc2, modifiers);
}      


int
Region::drag(Location target){   
   return Robot::drag(_screen_id, xo+x+target.x,  yo+y+target.y);
}

int 
Region::drag(Pattern target) throw(FindFailed){
   Match m;
   if (findInteractive(target, m))
      return drag(m);
   else 
      return false;
}

int 
Region::drag(const char* target) throw(FindFailed){
   return drag(Pattern(target));
}

int 
Region::drag(Region target){
   return drag(target.getCenter());
}

int 
Region::drag(Match target){
   return drag(target.getCenter());
}

int
Region::dropAt(Location target, double delay){
   return Robot::dropAt(_screen_id, xo+x+target.x,  yo+y+target.y);
}

int 
Region::dropAt(Pattern target, double delay) throw(FindFailed){
   Match m;
   if (findInteractive(target, m))
      return dropAt(m,delay);
   else 
      return false;
}

int 
Region::dropAt(const char* target, double delay) throw(FindFailed){
   return dropAt(Pattern(target), delay);
}

int 
Region::dropAt(Region target, double delay){
   return dropAt(target.getCenter(),delay);
}

int 
Region::dropAt(Match target, double delay){
   return dropAt(target.getCenter(),delay);
}

int
Region::paste(const char* text){
   return Robot::paste(text);
}

int
Region::paste(const Location target, const char* text){
   return Robot::paste(_screen_id, xo+x+target.x, yo+y+target.y,text);
}

int
Region::paste(const Pattern target, const char* text) throw(FindFailed){
   Match m;
   if (findInteractive(target, m))
      return paste(m, text);
   else 
      return false;
}

int
Region::paste(const char* target, const char* text) throw(FindFailed){
   return paste(Pattern(target), text);
}

int
Region::paste(const Region target, const char* text){
   return paste(target.getCenter(), text);
}

int
Region::paste(const Match target, const char* text){
   return paste(target.getCenter(), text);
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
   return Robot::type(_screen_id, xo+x+target.x, yo+y+target.y, text, modifiers);
}

int
Region::type(Pattern target, const char* text, int modifiers) throw(FindFailed){   
   Match m;
   if (findInteractive(target, m))
      return type(m, text, modifiers);
   else 
      return false;
}

int
Region::type(const char* target, const char* text, int modifiers) throw(FindFailed){
   return type(Pattern(target), text, modifiers);
}

int
Region::type(Region target, const char* text, int modifiers){
   return type(target.getCenter(), text, modifiers);
}

int
Region::type(Match target, const char* text, int modifiers){
   return type(target.getCenter(), text, modifiers);
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
Region::find(Pattern target) throw(FindFailed) {
   Match match;
   findInteractive(target, match);
   return match;
}

Match 
Region::find(const char* target) throw(FindFailed){
   return find(Pattern(target));
}

vector<Match> 
Region::findAll(Pattern target) {
   vector<Match> matches;
   matches = findRepeat(target.all(), 
                        Settings::AutoWaitTimeout,
                        Settings::WaitScanRate);
   return matches;
}

vector<Match> 
Region::findAll(const char* target){
   return findAll(Pattern(target));
}


Match 
Region::findNow(Pattern target) throw(FindFailed){
   Match match;
   findInteractive(target, match, 0, 0);
   return match;
}

Match 
Region::findNow(const char* target) throw(FindFailed){
   return findNow(Pattern(target));
}

vector<Match> 
Region::findAllNow(Pattern ptn){
   return doFind(ptn.all());
}

vector<Match> 
Region::findAllNow(const char* imgURL){
   return findAllNow(Pattern(imgURL));
}
   



vector<Match> 
Region::findRepeat(Pattern target, int seconds, int frequency){
   vector<Match> ms;
   ms = repeat(&Region::doFind_callback, target, seconds, frequency);
   return ms;
}

bool 
Region::doFind_callback(Pattern target, vector<Match>& matches){
   matches = doFind(target);
   return !matches.empty();
}

vector<Match>
Region::doFind(Pattern target) {

 
   dout << "[Region::doFind] Searching in (" << xo+x << "," << yo+y << ")-(" << xo+x+w << "," << yo+y+h << ")" << endl; 
  
   ScreenImage simg = capture();

	//namedWindow("Test");
	//imshow("Test",simg.getMat());
	//waitKey();

   vector<FindResult> results = Vision::find(simg, target);
   
   vector<Match> matches;
   int n = min((int)results.size(), (int)target.getLimit());
   for (int i=0; i< n; ++i){
      FindResult& r = results[i];
      Match match(inner(r.x,r.y,r.w,r.h),r.score); 
      matches.push_back(match);
   }
   
   
   if (!matches.empty()){
      SikuliUI::sikuliUI->handleMatchFound(*this, target, matches);

      setLastMatch(matches[0]);
      setLastMatches(matches);
   }
   
   // TODO: setTargetOffset 
   //match.setTargetOffset(ptn.getTargetOffset());
   return matches;
}


#include <time.h>
vector<Match> 
Region::repeat(callback func, Pattern target, int seconds, int frequency){
   vector<Match> ms;
   seconds = 5;
   //cout << CLOCKS_PER_MSEC << endl;
   long max_clocks_per_scan = CLOCKS_PER_SEC / frequency;
   //long start = clock();
   //long clocks_limit = clock() + seconds * CLOCKS_PER_SEC;   
   time_t time_limit = time(NULL) + seconds;
   time_t start_time;
   start_time = time(NULL);
   do {
      //while (clock() < clocks_limit){
      long before_find = clock();
      
      cout << ".";
      bool ret = (this->*func)(target,ms);
      if (ret)
         return ms;
      
      long actual_clocks_per_scan = clock() - before_find;
      long mseconds_to_delay = 1000*(max_clocks_per_scan - actual_clocks_per_scan)/CLOCKS_PER_SEC;
      
      //if (seconds > 0)
      //Robot::delay(max((long)10, mseconds_to_delay));
      //cout << 1.0*(clock() - start)/ CLOCKS_PER_SEC << " seconds" << endl;
      //cout << (time(NULL) - start_time) << " seconds" << endl;
   } while (time(NULL) < time_limit);
   return ms;
}



Match
Region::wait(Pattern target) throw(FindFailed){
   return wait(target, Settings::AutoWaitTimeout);
} 
      
Match
Region::wait(Pattern target, int seconds) throw(FindFailed){
   Match match;
   findInteractive(target, match, seconds, 
                   Settings::WaitScanRate);
   return match;
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
Region::waitAll(Pattern target, int seconds){
   vector<Match> matches;
   matches = findRepeat(target.all(), seconds, 
              Settings::WaitScanRate);
   return matches;
}   


vector<Match>
Region::waitAll(const char* target, int seconds){
   return waitAll(Pattern(target), seconds);
}

bool
Region::exists(Pattern target, int seconds){
   vector<Match> ms;
   if (seconds > 0)
	ms = findRepeat(target, seconds, Settings::WaitScanRate);
   else
    ms = doFind(target);
   return !ms.empty();
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


bool
Region::waitVanish_callback(Pattern target, vector<Match>& matches) {
   matches = doFind(target);
   return matches.empty();
}

bool
Region::waitVanish(Pattern target, int seconds){
   vector<Match> ms = repeat(&Region::waitVanish_callback, target, seconds,
                             Settings::WaitScanRate);   
   return !ms.empty();
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
   return waitVanish(Pattern(target));
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


bool 
Region::findInteractive(Pattern target, Match& match) throw(FindFailed) {
   return findInteractive(target, match, 
                          Settings::AutoWaitTimeout, 
                          Settings::WaitScanRate);
}

bool 
Region::findInteractive(Pattern target, Match& match,
                        int seconds, int frequency) throw(FindFailed) {
   
   while (true){

      vector<Match> ms;
      if (seconds > 0)
         ms = findRepeat(target, seconds, frequency);
      else 
         ms = doFind(target);

      if (!ms.empty()){
         match = ms[0];         
         return true;
      }
         
      int response = SikuliUI::sikuliUI->handleFindFailedException(*this, target);
          
         if (response == SKIP){
            return false;
         }else if (response == RETRY){
            continue;
         }else if (response == ABORT){
            throw FindFailed(target);
         }
      
   }
}


//Location 
//Region::getLocationFromPSRML(Pattern target){
//   Match m = find(target); 
//   return m.getTarget();
//}
//
//Location 
//Region::getLocationFromPSRML(const char* target){
//   Match m = find(target);
//   return m.getTarget();  
//}
//
//Location 
//Region::getLocationFromPSRML(Region target){
//   return target.getCenter();
//}
//
//Location 
//Region::getLocationFromPSRML(Match target){
//   return target.getTarget();
//}
//
//Location 
//Region::getLocationFromPSRML(Location target){
//   return target;
//}

void 
Region::onEvent(int event_type, Pattern ptn, SikuliEventHandler* handler){
   Observer ob(event_type, ptn, handler);
   EventManager::addObserver(*this, ob);
}


void 
Region::onEvent(int event_type, Pattern ptn,  SikuliEventCallback func){
   Observer ob(event_type, ptn, func);
   EventManager::addObserver(*this, ob);
}

void 
Region::onAppear(Pattern ptn, SikuliEventHandler* handler){
   onEvent(APPEAR, ptn, handler);
}

void 
Region::onVanish(Pattern ptn, SikuliEventHandler* handler){
   onEvent(VANISH, ptn, handler);
}

void 
Region::onChange(SikuliEventHandler* handler){
   onEvent(CHANGE, Pattern(), handler);
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

Location
Region::getMouseLocation(){
   Location loc;
   Robot::getMouseLocation(_screen_id, loc.x,loc.y);
   loc.x -= x;
   loc.y -= y;
   return loc;
}