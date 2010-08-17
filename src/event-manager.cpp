#include <iostream>
#include <vector>
#include <list>
#include <algorithm>

#include "finder.h"
#include "event-manager.h"

using namespace std;
using namespace sikuli;

#define CHANGE_THRESHOLD 0.001


vector<RegionObserver> EventManager::region_observers;
bool EventManager::bObserving = false;

void 
EventManager::addObserver(Region r, Observer observer){
   
   // find the region
   for (vector<RegionObserver>::iterator it = region_observers.begin(); 
        it != region_observers.end(); it++){
      RegionObserver& rob = (*it);
      
      if (rob.getRegion() == r){
         rob.addObserver(observer);
         return;
      }
   }
   
   RegionObserver rob(r);
   region_observers.push_back(rob);
   region_observers.back().addObserver(observer);
}

void
EventManager::observe(int seconds, bool background){
   bObserving = true;
   long clocks_limit = clock() + seconds * CLOCKS_PER_SEC;   
   while (clock() < clocks_limit && bObserving){
      EventManager::update(); 
      Robot::delay(100);
   }
}

void
EventManager::stop(){
   bObserving = false;
}

vector<Event> 
EventManager::update(){
   
   vector<Event> all_events;
   
   for (vector<RegionObserver>::iterator rob = region_observers.begin(); 
        rob != region_observers.end(); rob++){
      
      
      vector<Event> region_events = (*rob).observe();
      
      for (vector<Event>::iterator evt = region_events.begin();
           evt != region_events.end(); evt++)
         all_events.push_back(*evt);
            
   }

   return all_events;
}

void 
RegionObserver::addObserver(Observer observer){
   observers.push_back(observer);
};

RegionObserver::RegionObserver(const Region& r){
   region = r;
}

RegionObserver::~RegionObserver(){
}

#include "vision.h"

vector<Event> 
RegionObserver::observe(){
   
   Match top_match;
   
   vector<Event> events;
   for (vector<Observer>::iterator it = observers.begin(); it != observers.end(); it++){
      
      Observer& ob = (*it);   
      Event e;
      bool triggered = false;
      if (ob.event_type == CHANGE){
         
         ScreenImage simg = region.capture();
         
         Mat current_screen_image = simg.getMat();
         
         if (prev_screen_image.data == NULL)
            prev_screen_image = current_screen_image;
         
         double score = Vision::compare(prev_screen_image, current_screen_image);
         prev_screen_image = current_screen_image;
         
         if (score > CHANGE_THRESHOLD){
            //cout << score << endl;
            
            e.type = ob.event_type;
            e.region = region;
            triggered = true;
            
         }
      }
      else if (ob.event_type == APPEAR){
            
            try{
               
               top_match = region.findNow(ob.pattern);
               
               if (!ob.active){
                  
                  e.type = ob.event_type;
                  e.match = top_match;
                  e.pattern = ob.pattern;
                  e.region = region;
                  triggered = true;
                  
                  ob.active = true;
               }
               
            }catch (FindFailed ff){
               ob.active = false;
            }
            
      }else if (ob.event_type == VANISH){
            
            
            try{
               
               top_match = region.findNow(ob.pattern);
               ob.active = false;
               
            }catch (FindFailed ff){
               
               if (!ob.active){
                  
                  e.type = ob.event_type;
                  e.pattern = ob.pattern;
                  e.region = region;
                  triggered = true;
                  
                  ob.active = true;
               }
               
            }
      }

            
      if (triggered){
         events.push_back(e);
         
         if (ob.callback)
            (*ob.callback)(e);
         
         if (ob.event_handler)
            ob.event_handler->handle(e);
      }
      
   }   
   
   return events;
   
}

Observer::Observer(int event_type_, Pattern ptn, SikuliEventCallback func_){
   
   event_type = event_type_;
   callback = func_;
   event_handler = NULL;
   active = false;  
   pattern = ptn;
}


Observer::Observer(int event_type_, Pattern ptn, SikuliEventHandler* handler){
   
   event_type = event_type_;
   callback = 0;
   event_handler = handler;
   active = false;
   pattern = ptn;
}

Observer::~Observer(){
}
