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

using namespace sikuli;


int Settings::DelayBeforeDrop = 10;
int Settings::DelayAfterDrag = 10;
int Settings::WaitScanRate = 5;

////////////////////////////////////////////////////////////////////
/// Match Class
////////////////////////////////////////////////////////////////////

Match::Match(){
   init(-1,-1,-1,-1);
   score = -1;
}

Match::~Match(){
}

Match::Match(const Match& m){
   init(m.x, m.y, m.w, m.h);
   score = m.score;
}

Match::Match(int _x, int _y, int _w, int _h, double _score){
   init(_x,_y,_w,_h);
   score = _score;
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
   return getCenter();
}

void
Match::setTargetOffset(Location offset){
   _target = Location(getCenter());
   _target.x += offset.x;
   _target.y += offset.y;
}

////////////////////////////////////////////////////////////////////
/// Pattern Class
////////////////////////////////////////////////////////////////////

Pattern::Pattern(){
   imgURL = "";
   similarity = 0.7f;
   dx = 0;
   dy = 0;
}

Pattern::Pattern(const Pattern& p){
   imgURL = p.imgURL;
   similarity = p.similarity;
   dx = p.dx;
   dy = p.dy;
}

Pattern::Pattern(const char* imgURL_){
   imgURL = string(imgURL_);
   similarity = 0.7f;
   dx = 0;
   dy = 0;   
}

Pattern 
Pattern::similar(float similarity_){
   Pattern ret(*this);
   ret.similarity = similarity_;
   return ret;
}

Pattern 
Pattern::exact(){
   Pattern ret(*this);
   ret.similarity = 1.0f;
   return ret;
}

Pattern 
Pattern::targetOffset(int dx_, int dy_){
   Pattern ret(*this);
   ret.dx = dx_;
   ret.dy = dy_;
   return ret;
}

Location 
Pattern::getTargetOffset(){
   return Location(dx, dy);
}

string 
Pattern::toString(){
   stringstream ret;
   ret << "Pattern(\"" << imgURL + "\")";
   ret << ".similar(" << similarity << ")";
   if(dx!=0 || dy!=0)
      ret << ".targetOffset(" << dx << "," << dy << ")";
   return ret.str();
}

////////////////////////////////////////////////////////////////////
/// Region Class
////////////////////////////////////////////////////////////////////

Region::Region(){
   init(0,0,0,0);
}

Region::Region(int x_, int y_, int w_, int h_){
   init(x_,y_,w_,h_);
}

Region::Region(Rectangle r) {
   init(r.x, r.y, r.width, r.height);
}

Region::Region(const Region& r) {
   init(r.x, r.y, r.w, r.h);
}

Region::~Region(){
   if (_pLastMatch != NULL)
      delete _pLastMatch;   
   if (_pLastMatches != NULL)
      delete _pLastMatches;
}

void
Region::init(int x_, int y_, int w_, int h_){
   x = x_;
   y = y_;
   w = w_;
   h = h_;
   _throwException = true;
   _autoWaitTimeout = 3.0;
   _pLastMatch = NULL;//new Match();
   _pLastMatches = NULL;
  // new vector<Match>();
   //_pLastMatches->push_back(Match());
   //_scr = initScreen();
   //_robot = Screen.getRobot(0); // mouseMove only works on the primary robot
   //_evtMgr = new EventManager(this);   
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
   w = (int)roi.width;
   h = (int)roi.height;
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
   return _click(BUTTON1_MASK, modifiers, false);   
}

int 
Region::click(Location target, int modifiers){
   return _click(target, BUTTON1_MASK, modifiers, false);
}

int 
Region::doubleClick(int modifiers){
   return _click(BUTTON1_MASK, modifiers, true);
}

int 
Region::doubleClick(Location target, int modifiers){
   return _click(target, BUTTON1_MASK, modifiers, true);   
}

int 
Region::rightClick(int modifiers){
   return _click(BUTTON3_MASK, modifiers, false);
}

int 
Region::rightClick(Location target, int modifiers){
   return _click(target, BUTTON3_MASK, modifiers, false);
}

int 
Region::hover(Location target){
   _robot.mouseMove(target.x, target.y);
   _robot.waitForIdle();
   return 1;
}

int
Region::drag(Location target){
   _robot.mouseMove(target.x, target.y);
   _robot.mousePress(BUTTON1_MASK);
   _robot.waitForIdle();
   return 1;  
}

int
Region::dropAt(Location target, double delay){
   _robot.mouseMove(target.x, target.y);
   _robot.delay((int) delay*1000);
   _robot.mouseRelease(BUTTON1_MASK);
   _robot.waitForIdle();
   return 1;
}

template<class PSRML> int 
Region::paste(PSRML target, string text){
   // use clipboard
   click(target, 0);
   if (text.empty()){
      return 0;
   }
   else{
      _robot.paste(text);   
      return 1;
   }
}

int
Region::type(const char* text, int modifiers){
   if (strlen(text) < 0)
      return 0;
   for (int i=0; i < strlen(text); i++){
      pressModifiers(modifiers);
      type_ch(text[i], PRESS_RELEASE);
      releaseModifiers(modifiers);
      _robot.delay(20);
   }
   _robot.waitForIdle();
   return 1;   
}

int
Region::type(Location target, const char* text, int modifiers){
   click(target, 0);
   return type(text, modifiers);
}

void 
Region::type_ch(char character, int mode){
   switch (character) {
      case 'a': doType(mode,VK_A); break;
      case 'b': doType(mode,VK_B); break;
      case 'c': doType(mode,VK_C); break;
      case 'd': doType(mode,VK_D); break;
      case 'e': doType(mode,VK_E); break;
      case 'f': doType(mode,VK_F); break;
      case 'g': doType(mode,VK_G); break;
      case 'h': doType(mode,VK_H); break;
      case 'i': doType(mode,VK_I); break;
      case 'j': doType(mode,VK_J); break;
      case 'k': doType(mode,VK_K); break;
      case 'l': doType(mode,VK_L); break;
      case 'm': doType(mode,VK_M); break;
      case 'n': doType(mode,VK_N); break;
      case 'o': doType(mode,VK_O); break;
      case 'p': doType(mode,VK_P); break;
      case 'q': doType(mode,VK_Q); break;
      case 'r': doType(mode,VK_R); break;
      case 's': doType(mode,VK_S); break;
      case 't': doType(mode,VK_T); break;
      case 'u': doType(mode,VK_U); break;
      case 'v': doType(mode,VK_V); break;
      case 'w': doType(mode,VK_W); break;
      case 'x': doType(mode,VK_X); break;
      case 'y': doType(mode,VK_Y); break;
      case 'z': doType(mode,VK_Z); break;
      case 'A': doType(mode,VK_SHIFT, VK_A); break;
      case 'B': doType(mode,VK_SHIFT, VK_B); break;
      case 'C': doType(mode,VK_SHIFT, VK_C); break;
      case 'D': doType(mode,VK_SHIFT, VK_D); break;
      case 'E': doType(mode,VK_SHIFT, VK_E); break;
      case 'F': doType(mode,VK_SHIFT, VK_F); break;
      case 'G': doType(mode,VK_SHIFT, VK_G); break;
      case 'H': doType(mode,VK_SHIFT, VK_H); break;
      case 'I': doType(mode,VK_SHIFT, VK_I); break;
      case 'J': doType(mode,VK_SHIFT, VK_J); break;
      case 'K': doType(mode,VK_SHIFT, VK_K); break;
      case 'L': doType(mode,VK_SHIFT, VK_L); break;
      case 'M': doType(mode,VK_SHIFT, VK_M); break;
      case 'N': doType(mode,VK_SHIFT, VK_N); break;
      case 'O': doType(mode,VK_SHIFT, VK_O); break;
      case 'P': doType(mode,VK_SHIFT, VK_P); break;
      case 'Q': doType(mode,VK_SHIFT, VK_Q); break;
      case 'R': doType(mode,VK_SHIFT, VK_R); break;
      case 'S': doType(mode,VK_SHIFT, VK_S); break;
      case 'T': doType(mode,VK_SHIFT, VK_T); break;
      case 'U': doType(mode,VK_SHIFT, VK_U); break;
      case 'V': doType(mode,VK_SHIFT, VK_V); break;
      case 'W': doType(mode,VK_SHIFT, VK_W); break;
      case 'X': doType(mode,VK_SHIFT, VK_X); break;
      case 'Y': doType(mode,VK_SHIFT, VK_Y); break;
      case 'Z': doType(mode,VK_SHIFT, VK_Z); break;
      case '`': doType(mode,VK_BACK_QUOTE); break;
      case '0': doType(mode,VK_0); break;
      case '1': doType(mode,VK_1); break;
      case '2': doType(mode,VK_2); break;
      case '3': doType(mode,VK_3); break;
      case '4': doType(mode,VK_4); break;
      case '5': doType(mode,VK_5); break;
      case '6': doType(mode,VK_6); break;
      case '7': doType(mode,VK_7); break;
      case '8': doType(mode,VK_8); break;
      case '9': doType(mode,VK_9); break;
      case '-': doType(mode,VK_MINUS); break;
      case '=': doType(mode,VK_EQUALS); break;
      case '~': doType(mode,VK_SHIFT, VK_BACK_QUOTE); break;
      case '!': doType(mode,VK_SHIFT, VK_1); break;
      case '@': doType(mode,VK_SHIFT, VK_2); break;
      case '#': doType(mode,VK_SHIFT, VK_3); break;
      case '$': doType(mode,VK_SHIFT, VK_4); break;
      case '%': doType(mode,VK_SHIFT, VK_5); break;
      case '^': doType(mode,VK_SHIFT, VK_6); break;
      case '&': doType(mode,VK_SHIFT, VK_7); break;
      case '*': doType(mode,VK_SHIFT, VK_8); break;
      case '(': doType(mode,VK_SHIFT, VK_9); break;
      case ')': doType(mode,VK_SHIFT, VK_0); break;
      case '_': doType(mode,VK_SHIFT, VK_MINUS); break;
      case '+': doType(mode,VK_SHIFT, VK_EQUALS); break;
      case '\b': doType(mode,VK_BACK_SPACE); break;
      case '\t': doType(mode,VK_TAB); break;
      case '\r': doType(mode,VK_ENTER); break;
      case '\n': doType(mode,VK_ENTER); break;
      case '[': doType(mode,VK_OPEN_BRACKET); break;
      case ']': doType(mode,VK_CLOSE_BRACKET); break;
      case '\\': doType(mode,VK_BACK_SLASH); break;
      case '{': doType(mode,VK_SHIFT, VK_OPEN_BRACKET); break;
      case '}': doType(mode,VK_SHIFT, VK_CLOSE_BRACKET); break;
      case '|': doType(mode,VK_SHIFT, VK_BACK_SLASH); break;
      case ';': doType(mode,VK_SEMICOLON); break;
      case ':': doType(mode,VK_SHIFT, VK_SEMICOLON); break;
      case '\'': doType(mode,VK_QUOTE); break;
      case '"': doType(mode,VK_SHIFT, VK_QUOTE); break;
      case ',': doType(mode,VK_COMMA); break;
      case '<': doType(mode,VK_SHIFT, VK_COMMA); break;
      case '.': doType(mode,VK_PERIOD); break;
      case '>': doType(mode,VK_SHIFT, VK_PERIOD); break;
      case '/': doType(mode,VK_SLASH); break;
      case '?': doType(mode,VK_SHIFT, VK_SLASH); break;
      case ' ': doType(mode,VK_SPACE); break;
/*
      case '\u001b': doType(mode,VK_ESCAPE); break;
      case '\ue000': doType(mode,VK_UP); break;
      case '\ue001': doType(mode,VK_RIGHT); break;
      case '\ue002': doType(mode,VK_DOWN); break;
      case '\ue003': doType(mode,VK_LEFT); break;
      case '\ue004': doType(mode,VK_PAGE_UP); break;
      case '\ue005': doType(mode,VK_PAGE_DOWN); break;
      case '\ue006': doType(mode,VK_DELETE); break;
      case '\ue007': doType(mode,VK_END); break;
      case '\ue008': doType(mode,VK_HOME); break;
      case '\ue009': doType(mode,VK_INSERT); break;
      case '\ue011': doType(mode,VK_F1); break;
      case '\ue012': doType(mode,VK_F2); break;
      case '\ue013': doType(mode,VK_F3); break;
      case '\ue014': doType(mode,VK_F4); break;
      case '\ue015': doType(mode,VK_F5); break;
      case '\ue016': doType(mode,VK_F6); break;
      case '\ue017': doType(mode,VK_F7); break;
      case '\ue018': doType(mode,VK_F8); break;
      case '\ue019': doType(mode,VK_F9); break;
      case '\ue01A': doType(mode,VK_F10); break;
      case '\ue01B': doType(mode,VK_F11); break;
      case '\ue01C': doType(mode,VK_F12); break;
      case '\ue01D': doType(mode,VK_F13); break;
      case '\ue01E': doType(mode,VK_F14); break;
      case '\ue01F': doType(mode,VK_F15); break;
      case '\ue020': doType(mode,VK_SHIFT); break;
      case '\ue021': doType(mode,VK_CONTROL); break;
      case '\ue022': doType(mode,VK_ALT); break;
      case '\ue023': doType(mode,VK_META); break;
*/
 }
}

void 
Region::doType(int mode, int keycode){
   if(mode==PRESS_ONLY){
      _robot.keyPress(keycode);
   }
   else if(mode==RELEASE_ONLY){
      _robot.keyRelease(keycode);
   }
   else{
      _robot.keyPress(keycode);
      _robot.keyRelease(keycode);
   }
}

void 
Region::doType(int mode, int keycode1, int keycode2){
   if(mode==PRESS_ONLY){
      _robot.keyPress(keycode1);
      _robot.keyPress(keycode2);      
   }
   else if(mode==RELEASE_ONLY){
      _robot.keyRelease(keycode1);
      _robot.keyRelease(keycode2);      
   }
   else{
      _robot.keyPress(keycode1);
      _robot.keyPress(keycode2);
   }
}

void 
Region::mouseDown(int buttons){
   _hold_buttons = buttons;
   _robot.mousePress(buttons);
   _robot.waitForIdle();
}
   
void 
Region::mouseUp(int buttons){
   if (buttons == 0)
      _robot.mouseRelease(_hold_buttons);
   else
      _robot.mouseRelease(buttons);
   _robot.waitForIdle();
}
         
void 
Region::keyDown(string keys){
   
   if (keys.length() > 0){
      for (int i=0; i < keys.length(); ++i){
         char key = keys[i];
         // TODO: if this key has not already been pressed down
         //if (_hold_keys.indexOf(keys.charAt(i)) == -1)
         if (true){
            type_ch(key, PRESS_ONLY);
            _hold_keys += key;
         }
      }
      _robot.waitForIdle();
   }
}

void 
Region::pressModifiers(int modifiers){
   if((modifiers & K_SHIFT) != 0) _robot.keyPress(VK_SHIFT);
   if((modifiers & K_CTRL) != 0) _robot.keyPress(VK_CONTROL);
   if((modifiers & K_ALT) != 0) _robot.keyPress(VK_ALT);
//   if((modifiers & K_META) != 0){
 //     if( Env.getOS() == OS.WINDOWS )
//         _robot.keyPress(KeyEvent.VK_WINDOWS);
//      else
//         _robot.keyPress(KeyEvent.VK_META);
//   }
}

void 
Region::releaseModifiers(int modifiers){
   if((modifiers & K_SHIFT) != 0) _robot.keyRelease(VK_SHIFT);
   if((modifiers & K_CTRL) != 0) _robot.keyRelease(VK_CONTROL);
   if((modifiers & K_ALT) != 0) _robot.keyRelease(VK_ALT);//
//   if((modifiers & K_META) != 0){ 
//      if( Env.getOS() == OS.WINDOWS )
//         _robot.keyRelease(KeyEvent.VK_WINDOWS);
//      else
//         _robot.keyRelease(KeyEvent.VK_META);
//   }
}

// TODO: Re-implement using vector
void 
Region::keyUp(string keys){
   if (keys.empty())
      keys = _hold_keys;
   for (int i=0; i < keys.length(); ++i){
      char key = keys[i];
      int pos;
      pos = 1;
      // TODO:
      // pos=_hold_keys.indexOf(keys.charAt(i))
      if (pos != -1){
         type_ch(key, RELEASE_ONLY);
         // TODO: remove key from _hold_keys
//         _hold_keys = _hold_keys.substring(0,pos) + 
//         _hold_keys.substring(pos+1);
      }
   }
   _robot.waitForIdle();
}


int 
Region::_click(int buttons, int modifiers, bool dblClick){
   pressModifiers(modifiers);
   //_scr.showClick(loc);
   _robot.mousePress(buttons);
   _robot.mouseRelease(buttons);
   if( dblClick ){
      _robot.mousePress(buttons);
      _robot.mouseRelease(buttons);
   }
   releaseModifiers(modifiers);
   _robot.waitForIdle();
   return 1;
}

int 
Region::_click(Location loc, int buttons, int modifiers, bool dblClick) {
//   Debug.info( getClickMsg(loc, buttons, modifiers, dblClick) );
   _robot.mouseMove(loc.x, loc.y);
   sleep(0.1);
   return _click(buttons, modifiers, dblClick);
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
   if (_autoWaitTimeout > 0)
      m = wait(ptn, _autoWaitTimeout);
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
   if (_autoWaitTimeout > 0){
      ms = waitAll(ptn, _autoWaitTimeout);
   }else{
      ms  = findAllNow(ptn);
      if(ms.empty() && _throwException)
         throw FindFailed(ptn);
   }
   *_pLastMatches = ms;
   return ms;
}

vector<Match> 
Region::findAll(const char* imgURL) throw(FindFailed) {
   return findAll(Pattern(imgURL));
}


Match 
Region::findNow(Pattern ptn) throw(FindFailed){
   //ScreenImage simg( = _scr.capture(x, y, w, h);
   ScreenImage simg = _robot.capture();
   Match m = Vision::find(simg, ptn);
   cout << "Found pattern at: " << m.x << "," << m.y << endl;
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
   ScreenImage simg = _scr.capture(x, y, w, h);
   vector<Match> ms = Vision::findAll(simg, ptn);
   if (ms.empty())
      throw FindFailed(ptn);
   return ms;
}

vector<Match> 
Region::findAllNow(const char* imgURL) throw(FindFailed){
   return findAllNow(Pattern(imgURL));
}
   

#include <time.h>
Match
Region::wait(Pattern target) throw(FindFailed){
   return wait(target, _autoWaitTimeout);
}

Match
Region::wait(Pattern target, double timeout) throw(FindFailed){
   Match m(-1,-1,-1,-1,-1);
   int MaxTimePerScan = (int)(1000.0/Settings::WaitScanRate); 
   long begin_t = time(NULL);
   do{
      long before_find = time(NULL);
      try {
         m = findNow(target);
         if (_pLastMatch == NULL)
            _pLastMatch = new Match();
         *_pLastMatch = m;
         return m;
      }catch (FindFailed e){
      }
      long after_find = time(NULL);
      if(after_find-before_find<MaxTimePerScan)
         _robot.delay((int)(MaxTimePerScan-(after_find-before_find)));
      else
         _robot.delay(10);
   }while( begin_t + timeout*1000 > time(NULL));
   if(_throwException)
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
   int MaxTimePerScan = (int)(1000.0/Settings::WaitScanRate); 
   long begin_t = time(NULL);
   do{
      long before_find = time(NULL);
      try{
         ms = findAllNow(target);
         return ms;
      }catch (FindFailed ff){}
      long after_find = time(NULL);
      if(after_find-before_find<MaxTimePerScan)
         _robot.delay((int)(MaxTimePerScan-(after_find-before_find)));
      else
         _robot.delay(10);
   }while( begin_t + timeout*1000 > time(NULL));
   if(_throwException)
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
   return exists(target, _autoWaitTimeout);
}
bool
Region::exists(const char* target){
   return exists(Pattern(target));
}

bool
Region::waitVanish(Pattern target, double timeout){
   int MaxTimePerScan = (int)(1000.0/Settings::WaitScanRate); 
   long begin_t = time(NULL);
   do{
      long before_find = time(NULL);
      try {
         Match m = findNow(target);
      }catch (FindFailed e){
         return true;
      }      
      long after_find = time(NULL);
      if(after_find-before_find<MaxTimePerScan)
         _robot.delay((int)(MaxTimePerScan-(after_find-before_find)));
      else
         _robot.delay(10);
   }while( begin_t + timeout*1000 > time(NULL));
   return false;
}

bool
Region::waitVanish(Pattern target){
   return waitVanish(target, _autoWaitTimeout);
}

bool
Region::waitVanish(const char* target, double timeout){
   return waitVanish(Pattern(target));
}

bool
Region::waitVanish(const char* target){
   return waitVanish(target, _autoWaitTimeout);
}

//final int PADDING = 50;
#define PADDING 50

Region 
Region::nearby(){
   return nearby(PADDING);
}

Region 
Region::nearby(int range){
   Rectangle bounds;// = getScreen().getBounds();
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
   Rectangle bounds;// = getScreen().getBounds();
   Rectangle rect = Rectangle(x+w,y,range,h);
   rect = rect.intersection(bounds);
   return Region(rect);
}

Region 
Region::left(){   
   return left(9999999);   
}

Region 
Region::left(int range){
   Rectangle bounds;// = getScreen().getBounds();
   Region r(*this);
   r.x = x-range < bounds.x? bounds.x: x-range;
   r.y = y;
   r.w = x - r.x;
   r.h = h;
   return r;
}

Region 
Region::above(){
   return above(9999999);    
}

Region
Region::above(int range){
   Rectangle bounds;// TODO: = getScreen().getBounds();
   Region r(*this);
   r.x = x;
   r.y = y-range < bounds.y? bounds.y : y-range;
   r.w = w;
   r.h = y-r.y;
   return r;
}

Region 
Region::below(){
   return below(999999);   
}

Region 
Region::below(int range){
   Rectangle bounds; // TODO: = getScreen().getBounds();
   Rectangle rect = Rectangle(x,y+h,w,range);
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

////////////////////////////////////////////////////////////////////
/// Vision Class
////////////////////////////////////////////////////////////////////
#include "finder.h"
Match 
Vision::find(ScreenImage simg, Pattern ptn) { 
   Finder f(simg.getMat());
   f.find(ptn.getImgURL().c_str(), 0.5);
   FindResult r = f.next();
   return Match(r.x,r.y,r.w,r.h,r.score);
};

vector<Match> 
Vision::findAll(ScreenImage simg, Pattern ptn){ 
   return vector<Match>();
};


