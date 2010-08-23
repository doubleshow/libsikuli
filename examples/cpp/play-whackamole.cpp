#include "sikuli.h"

using namespace sikuli;

int main(void){
   
   Screen s;
   addImagePath("examples/images");
   
   cout << "Welcome! This is a sample program that uses libsikuli to automatically " << endl;
   cout << "play a whack-a-mole game. First, please load the game by openning the \n";
   cout << "following URL in a web browser:"; 
   cout << endl;
   cout << "  http://www.gamebrew.com/game/whack-mole/play" << endl;
   cout << endl << "After the game finishes loading, press [ENTER] to start!"<< endl;
   
//getch();
   cin.get();
   
   Match g = s.find("startpage.png");
   g.click("play.png");
   wait(0.1);
   g.click("play.png");   
   

   while (!g.exists("enter.png",0)){
         
      vector<Match> ms = g.findAllNow(Pattern("mole.png"));
      for (int j=0;j<ms.size();++j){
            g.click(ms[j]);
      }
         
      if (ms.size()>0)
         wait(0.05);
      
   }
   
   return 1;
}

