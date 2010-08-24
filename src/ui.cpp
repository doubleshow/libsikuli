/*
 *  ui.cpp
 *  sikuli
 *
 *  Created by Tom Yeh on 8/20/10.
 *  Copyright 2010 sikuli.org. All rights reserved.
 *
 */

#include "ui.h"
#include <iostream>

using namespace sikuli;
using namespace std;

//SikuliUI* 
//sikuliUI = 0;

SikuliUI* SikuliUI::sikuliUI = 0;

class SikuliInitCleanup{
   
public:
   SikuliInitCleanup(){
      SikuliUI::sikuliUI  = new SikuliUI();
      ptr = SikuliUI::sikuliUI;
   }
   
   ~SikuliInitCleanup(){
      delete ptr;
   }
private:
   SikuliUI* ptr;
};
SikuliInitCleanup sic;

void 
SikuliUI::set(SikuliUI* ui){
   SikuliUI::sikuliUI  = ui;
}

int
SikuliUI::handleMatchFound(Region region, Pattern target, vector<Match> matches){
   
   int n = matches.size();
   cout << endl;
   cout << target.toString() << ": " << n << " match(es)" << endl;
   for (int i=0;i <n; ++i){ 
      Match& match = matches[i];
      if (i<5)
         cout << '\t' << i+1 << ": (" << match.x << "," << match.y << "), score = " << match.getScore()  << endl;
      else if (i==5) 
         cout << '\t' << "... more" << endl;
   }
   
   return 1;
}

int 
SikuliUI::handleFindFailedException(Region region, 
                                    Pattern target){
   cout << target.toString() << " can not be found!!" << endl;
   
   while (true){
      char ret = 0;
      cout << "(S)kip, (R)etry, (A)bort?";
      cin >> ret;
      if (ret == 'S' || ret == 's'){
         return SKIP;
      }else if (ret == 'R' || ret == 'r' ){
         return RETRY;
      }else if (ret == 'A' || ret == 'a'){
         return ABORT;
      }
   }
}
