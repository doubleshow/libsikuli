#ifndef _EVENT_MANAGER_
#define _EVENT_MANAGER_

#include <opencv2/opencv.hpp>

//#include <cv.h>
//#include <cxcore.h>
//#include <highgui.h>

#include <vector>

#include "pattern.h"
#include "region.h"

using namespace std;

namespace sikuli{

struct Event{
   int type;
   Pattern pattern;
   Match match; 
   Region region;
};

enum SikuliEventType{
   APPEAR = 1,
   VANISH,
   CHANGE
};
   
   
typedef void  (*SikuliEventCallback)(Event event);

   
class SikuliEventHandler {
public:
   SikuliEventHandler(){};
   virtual ~SikuliEventHandler(){};
   virtual void handle(Event event) {};
};
   
class Observer{
   
public:
   
   // handler_id allows higher-level scripting language wrapper
   // to implement its own callback mechanism
   Observer(int event_type, Pattern ptn, SikuliEventHandler* handler);
   Observer(int event_type, Pattern ptn, SikuliEventCallback func);
   Observer(const Observer& ob);

   ~Observer();
   
   int event_type;   
   Pattern pattern;

   SikuliEventCallback callback;
   SikuliEventHandler* event_handler;
   
   bool active;
};
   
class RegionObserver{

public:
   RegionObserver(const Region& r);
   ~RegionObserver();
   
   void addObserver(Observer observer);   
   
   vector<Event> observe();
   
   
   Region getRegion() { return region;}
   
private:
   
   Region region;
   
   // The screen image captured of the region in the previous
   // time step. It is compared to the current frame to detect
   // changes.
   cv::Mat prev_screen_image;
   vector<Observer> observers;
};


class EventManager{
   
public:
   
   static void addObserver(Region r, Observer observer);  
   static void observe(int seconds, bool background = false);
   static void stop();
   
private:
   
   static bool bObserving;
   static vector<Event> update();
   static vector<RegionObserver> region_observers;
   
   
};
   
}

#endif // _EVENT_MANAGER_
