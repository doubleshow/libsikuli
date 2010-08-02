%module sikuli
%{
#define SWIG_FILE_WITH_INI
//#include "finder.h"
#include "region.h"
#include "screen.h"
#include "vision.h"
using namespace sikuli;
%}

%pythoncode
%{
_type = type
def _swig_setattr_nondynamic(self,class_type,name,value,static=1):
   if (name == "thisown"): return self.this.own(value)
   if (name == "this"):
      if _type(value).__name__ == 'SwigPyObject':
         self.__dict__[name] = value
         return
   method = class_type.__swig_setmethods__.get(name,None)
   if method: return method(self,value)
   if (not static) or hasattr(self,name):
      self.__dict__[name] = value
   else:
      raise AttributeError("You cannot add attributes to %s" % self)   
%}

#define KEY_SHIFT 1
#define KEY_CTRL  2
#define KEY_META  4  
#define KEY_ALT   8

enum specialKeys{
   ALT,
   CMD,
   CTRL,
   META,
   SHIFT,
   WIN,
   ENTER,
   TAB,
   ESC,
   BACKSPACE,
   DELETE,
   F1,
   F2,
   F3,
   F4,
   F5,
   F6,
   F7,
   F8,
   F9,
   F10,
   F11,
   F12,
   F13,
   F14,
   F15,
   HOME,
   END,
   LEFT,
   RIGHT,
   DOWN,
   UP,
   PAGE_DOWN,
   PAGE_UP
}; 

class Point {
   Point(int _x, int _y);
};

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
   
   Pattern();
   Pattern(const Pattern& p);
   Pattern(const char* str);
   
   Pattern similar(float similarity_);
   Pattern exact();
   Pattern targetOffset(int dx_, int dy_);
   
   Location getTargetOffset();
   float getSimilarity();
   
   
   string toString();
   
   const char* getText();
   const char* getImageURL();
   
   bool isImageURL();
   bool isText();
      
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
   
   Location getCenter() const;
   
   Match getLastMatch();
   vector<Match> getLastMatches();
   
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
      
   
   //==================================================================
   // Automation Functions
   //==================================================================
   
public:
   
   int click(int modifiers = 0);
   int click(Location target, int modifiers = 0);
   int click(Pattern& target, int modifiers = 0);
   int click(const char* target, int modifiers = 0);
   int click(Region& target, int modifiers = 0);
   int click(Match& target, int modifiers = 0);
   
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
   int hover(Pattern& target);
   int hover(const char* target);
   int hover(Region& target);
   int hover(Match& target);
   
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
   
};

%pythoncode 
%{
   
import inspect
import sys
import __main__

def Region__enter__(self): 
   self._global_funcs = {}
   for name in dir(self):
      if inspect.ismethod(getattr(self,name)) and __main__.__dict__.has_key(name):
         self._global_funcs[name] = __main__.__dict__[name]
         #print "save " + name + " :" + str(__main__.__dict__[name])
         __main__.__dict__[name] = eval("self."+name)
   return self

def Region__exit__(self, type, value, traceback): 
   for name in self._global_funcs.keys():
      __main__.__dict__[name] = self._global_funcs[name]

Region.__enter__ = Region__enter__
Region.__exit__ = Region__exit__

%}


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
   
};

class Screen : public Region{
   
public:
   
   Screen();
   Screen(int id);
   ~Screen();
   
   ScreenImage capture(int x, int y, int w, int h);
   
   Rectangle getBounds();
   
//   static int getNumberScreens();
   
};

%pythoncode 
%{ 

#TODO: figure out how to expose type without overwriting the default type
def Screen_exposeAllMethods(self, mod):
   exclude_list = [ 'class', 'classDictInit', 'clone', 'equals', 'finalize', 
                   'getClass', 'hashCode', 'notify', 'notifyAll', 
                   'toGlobalCoord', 'toString',
                   'capture', 'selectRegion']
   dict = sys.modules[mod].__dict__
   for name in dir(self):
      if inspect.ismethod(getattr(self,name)) \
         and name[0] != '_' and name[:7] != 'super__' and \
         not name in exclude_list:
         #if DEBUG: print "expose " + name
         dict[name] = eval("self."+name)
         #__main__.__dict__[name] = eval("self."+name)
Screen._exposeAllMethods = Screen_exposeAllMethods
%}
   
class Vision{
public:
   
   static Match find(ScreenImage simg, Pattern ptn) throw(FindFailed);   
   static vector<Match> findAll(ScreenImage simg, Pattern ptn) throw(FindFailed);
   
   static void trainOCR(const char* trainingImagePath);
   
};
   
%pythoncode 
%{ 

def initSikuli():
   dict = globals()
   dict['SCREEN'] = Screen()
   dict['SCREEN']._exposeAllMethods(__name__)
   #print "Sikuli is initialized." 
  
   
initSikuli()
   
%}

