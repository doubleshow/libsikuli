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

using namespace sikuli;


int Settings::DelayBeforeDrop = 10;
int Settings::DelayAfterDrag = 10;
int Settings::WaitScanRate = 3;
bool Settings::ThrowException = true;
double Settings::AutoWaitTimeout = 3.0;
vector<string> Settings::_image_paths = vector<string>();

void
Settings::addImagePath(const char* image_path){
   _image_paths.push_back(string(image_path));
}

vector<const char*> 
Settings::getImagePaths(){
   vector<const char*> ret;
   for (int i=0; i<_image_paths.size(); ++i)
      ret.push_back(_image_paths[i].c_str());
   return ret;
}

void 
Settings::resetImagePaths(){
   _image_paths.clear();
}

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
   _target = Location(getCenter());   
}

Match::Match(int _x, int _y, int _w, int _h, double _score){
   init(_x,_y,_w,_h);
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
/// Pattern Class
////////////////////////////////////////////////////////////////////

Pattern::Pattern(){
   str = "";
   similarity = 0.8f;
   dx = 0;
   dy = 0;
}

Pattern::Pattern(const Pattern& p){
   str = p.str;
   bText = p.bText;
   similarity = p.similarity;
   dx = p.dx;
   dy = p.dy;
}

Pattern::Pattern(const char* str_){
   str = string(str_);
   
   bText = str.length()>3 && str[0] == '/' && str[str.length()-1] == '/';
   if (bText)
      str = str.substr(1, str.length()-2);
   
   
   similarity = 0.8f;
   dx = 0;
   dy = 0;   
}

bool
Pattern::isText(){
   return bText;
}

bool
Pattern::isImageURL(){
   return !isText();
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

float
Pattern::getSimilarity(){
   return similarity;
}

const char* 
Pattern::getImageURL() { 
   return str.c_str();
}

const char* 
Pattern::getText() { 
   return str.c_str();
}


string 
Pattern::toString(){
   stringstream ret;
   ret << "Pattern(\"" << str + "\")";
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
//   _throwException = true;
//   _autoWaitTimeout = 3.0;
   _pLastMatch = NULL;//new Match();
   _pLastMatches = NULL;
   
  // setScreen(0);
   
  // new vector<Match>();
   //_pLastMatches->push_back(Match());
   //_scr = initScreen();
   //_robot = Screen.getRobot(0); // mouseMove only works on the primary robot
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
Region::click(Pattern& target, int modifiers){
   return click(getLocationFromPSRML(target),modifiers);
}

int 
Region::click(const char* target, int modifiers){
   return click(getLocationFromPSRML(target),modifiers);
}

int 
Region::click(Region& target, int modifiers){
   return click(getLocationFromPSRML(target),modifiers);
}

int 
Region::click(Match& target, int modifiers){
   return click(getLocationFromPSRML(target),modifiers);
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
   return _click(BUTTON3_MASK, modifiers, false);
}

int 
Region::rightClick(Location target, int modifiers){
   return _click(target, BUTTON3_MASK, modifiers, false);
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
   Robot::mouseMove(target.x, target.y);
   Robot::delay(100);
   return 1;
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
   int ret = 0;
   pressModifiers(modifiers);
   if (drag(t1) != 0){
      Robot::delay((int) Settings::DelayAfterDrag*1000);
      ret = dropAt(t2, Settings::DelayBeforeDrop);
   }
   releaseModifiers(modifiers);
   return ret;
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
   Robot::mouseMove(target.x, target.y);
   Robot::delay(100);
   Robot::drag();
   Robot::delay(100);
   return 1;  
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
   Robot::mouseMove(target.x, target.y);
   Robot::delay((int) delay*1000);
   Robot::drop();
   return 1;
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

template<class PSRML> int 
Region::paste(PSRML target, string text){
   // use clipboard
   click(target, 0);
   if (text.empty()){
      return 0;
   }
   else{
      Robot::paste(text);   
      return 1;
   }
}

int
Region::press(int key, int modifiers){   
   pressModifiers(modifiers);  
   type_key(key, PRESS_RELEASE);
   releaseModifiers(modifiers);
   return 1;   
}

int
Region::type(const char* text, int modifiers){
   if (strlen(text) < 0)
      return 0;
 
   for (int i=0; i < strlen(text); i++){
      pressModifiers(modifiers);  
      type_ch(text[i], PRESS_RELEASE);
      releaseModifiers(modifiers);
      Robot::delay(20);
   }   
   //Robot::waitForIdle();
   return 1;   
}

int
Region::type(Location target, const char* text, int modifiers){
   click(target, 0);
   return type(text, modifiers);
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
 }
}

void 
Region::type_key(int key, int mode){
   switch (key) {
       case ESC: doType(mode,VK_ESCAPE); break;
       case UP: doType(mode,VK_UP); break;
       case RIGHT: doType(mode,VK_RIGHT); break;
       case DOWN: doType(mode,VK_DOWN); break;
       case LEFT: doType(mode,VK_LEFT); break;
       case PAGE_UP: doType(mode,VK_PAGE_UP); break;
       case PAGE_DOWN: doType(mode,VK_PAGE_DOWN); break;
       case DELETE: doType(mode,VK_DELETE); break;
       case END: doType(mode,VK_END); break;
       case HOME: doType(mode,VK_HOME); break;
       //case INSERT: doType(mode,VK_INSERT); break;
       case F1: doType(mode,VK_F1); break;
       case F2: doType(mode,VK_F2); break;
       case F3: doType(mode,VK_F3); break;
       case F4: doType(mode,VK_F4); break;
       case F5: doType(mode,VK_F5); break;
       case F6: doType(mode,VK_F6); break;
       case F7: doType(mode,VK_F7); break;
       case F8: doType(mode,VK_F8); break;
       case F9: doType(mode,VK_F9); break;
       case F10: doType(mode,VK_F10); break;
       case F11: doType(mode,VK_F11); break;
       case F12: doType(mode,VK_F12); break;
       case F13: doType(mode,VK_F13); break;
       case F14: doType(mode,VK_F14); break;
       case F15: doType(mode,VK_F15); break;
   }
}

void 
Region::doType(int mode, int keycode){
//   if(mode==PRESS_ONLY){
//      Robot::keyPress(keycode);
//   }
//   else if(mode==RELEASE_ONLY){
//      Robot::keyRelease(keycode);
//   }
//   else{
     // Robot::keyPressRelease(keycode);
    Robot::keyPress(keycode);
    Robot::keyRelease(keycode);
//   }
}

void 
Region::doType(int mode, int modifier, int keycode){
//   if(mode==PRESS_ONLY){
////      Robot::keyPress(keycode1);
////      Robot::keyPress(keycode2);      
//   }
//   else if(mode==RELEASE_ONLY){
////      Robot::keyRelease(keycode1);
////      Robot::keyRelease(keycode2);      
//   }
//   else{
   //Robot::keyPressRelease(keycode, modifier);
      Robot::keyPress(modifier);
      Robot::keyPress(keycode);      
      Robot::keyRelease(keycode);
      Robot::keyRelease(modifier);      

}

void 
Region::mouseDown(int buttons){
   _hold_buttons = buttons;
   Robot::mousePress(buttons);
   Robot::waitForIdle();
}
   
void 
Region::mouseUp(int buttons){
   if (buttons == 0)
      Robot::mouseRelease(_hold_buttons);
   else
      Robot::mouseRelease(buttons);
   Robot::waitForIdle();
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
      Robot::waitForIdle();
   }
}

void 
Region::pressModifiers(int modifiers){
   if(modifiers & SHIFT) Robot::keyPress(VK_SHIFT);
   if(modifiers & CTRL) Robot::keyPress(VK_CONTROL);
   if(modifiers & ALT) Robot::keyPress(VK_ALT);
   if(modifiers & CMD) Robot::keyPress(VK_META);
   if(modifiers & META) {
 //     if( Env.getOS() == OS.WINDOWS )
//         Robot::keyPress(KeyEvent.VK_WINDOWS);
//      else
      Robot::keyPress(VK_META);
  }
}

void 
Region::releaseModifiers(int modifiers){
   if(modifiers & SHIFT) Robot::keyRelease(VK_SHIFT);
   if(modifiers & CTRL) Robot::keyRelease(VK_CONTROL);
   if(modifiers & ALT) Robot::keyRelease(VK_ALT);//
   if(modifiers & CMD) Robot::keyRelease(VK_META);   
   if(modifiers & META){ 
//      if( Env.getOS() == OS.WINDOWS )
//         Robot::keyRelease(KeyEvent.VK_WINDOWS);
//      else
         Robot::keyRelease(VK_META);
   }
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
   Robot::waitForIdle();
}


int 
Region::_click(int buttons, int modifiers, bool dblClick){
   pressModifiers(modifiers);
   //_scr.showClick(loc);
   if( dblClick ){
      Robot::doubleClick(buttons);
   }else{
      Robot::singleClick(buttons);
   }
   releaseModifiers(modifiers);
   Robot::waitForIdle();
   return 1;
}

int 
Region::_click(Location loc, int buttons, int modifiers, bool dblClick) {
//   Debug.info( getClickMsg(loc, buttons, modifiers, dblClick) );
   Robot::mouseMove(loc.x, loc.y);
   sleep(1);
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
   Match m = Vision::find(simg, ptn);//
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
   vector<Match> ms = Vision::findAll(simg, ptn);
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



