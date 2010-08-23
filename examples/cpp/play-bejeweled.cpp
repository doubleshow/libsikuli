#include "sikuli.h"

using namespace sikuli;

enum{
   UNKNOWN = -1,
   BLUE = 0,
   ORANGE,
   YELLOW,
   RED,
   PURPLE,
   GREEN,
   SILVER
};

#define JEWEL_HEIGHT 36
#define JEWEL_WIDTH 36

const char files[][20] = {
   "blue.png",
   "orange.png",
   "yellow.png",
   "red.png",
   "purple.png",
   "green.png",
   "silver.png"
};

class Jewel{
   
public:
   
   Jewel(){};
   Jewel(int type_, int i_, int j_) : i(i_), j(j_), type(type_) {};
   Jewel(int type_) : type(type_) {};
   
   int type;
   int i,j;
   
   friend ostream& operator<<(ostream& os, const Jewel& jewel){
      switch (jewel.type){
         case UNKNOWN:
            os << "?";
            break;
         case BLUE:
            os << "B";
            break;
         case ORANGE:
            os << "O";
            break;
         case YELLOW:
            os << "Y";
            break;
         case RED:
            os << "R";
            break;
         case PURPLE:
            os << "P";
            break;
         case GREEN:
            os << "G";
            break;
         case SILVER:
            os << "S";
            break;
            
      }
      return os;
   }
   
};





class Move{
   
public:
   
   Move() {};
   Move(Jewel a, Jewel b) : from(a), to(b){};
   
   Jewel from;
   Jewel to;
   
};

class Board{
   
public:
   
   Board(int w, int h){
      this->w = w;
      this->h = h;
      
      for (int i=0; i<h; i++){
         
         vector<Jewel> row;
         row.clear();
         for (int j=0; j<w; j++){
            row.push_back(Jewel(UNKNOWN,i,j));
         }
         
         jewels.push_back(row);
      }
   };
   
   int h;
   int w;
   
   void place(Jewel jewel){
      if (within_bounds(jewel))
         jewels[jewel.i][jewel.j] = jewel;
   }
   
   void display(){
      cout << h << "x" << w << endl;
      for (int i=0; i<h; i++){
         
         for (int j=0; j<w; j++){
            cout << jewels[i][j];
         }
         cout << endl;
      }      
   }
   
   void reset(){
      for (int i=0; i<h; i++)
         for (int j=0; j<w; j++)
            jewels[i][j].type = UNKNOWN;
   }
   
   bool has(Jewel jewel){
      return within_bounds(jewel) && 
      jewels[jewel.i][jewel.j].type == jewel.type;      
   }
   
   bool within_bounds(Jewel p){
      return p.i >= 0 && p.i < h && p.j >= 0 && p.j < w;
   } 
   
   bool getMove(Move& move) {
      
      for (int p=0; p<h;++p){
         for (int q=0; q<w;++q){
            
            Jewel jewel = jewels[p][q];
            
            if (jewel.type == UNKNOWN)
               continue;
            
            Jewel right(jewel.type,p,q+1);
            Jewel right_right(jewel.type,p,q+2);
            Jewel left(jewel.type,p,q-1);
            
            if (has(right)){
               
               Jewel right_right_up(jewel.type,p-1,q+2);
               Jewel right_right_down(jewel.type,p+1,q+2);
               Jewel right_right_right(jewel.type,p,q+3);
               Jewel left_up(jewel.type,p-1,q-1);
               Jewel left_down(jewel.type,p+1,q-1);
               Jewel left_left(jewel.type,p,q-2);
               
               if (has(left_up)){
                  move = Move(left_up, left);
                  return true;
               }
               
               if (has(left_left)){
                  move = Move(left_left, left);
                  return true;
               }
               
               if (has(left_down)){
                  move = Move(left_down, left);
                  return true;
               }
               
               if (has(right_right_up)){
                  move = Move(right_right_up, right_right);
                  return true;
               }
               
               if (has(right_right_down)){
                  move = Move(right_right_down, right_right);
                  return true;
               }
               
               if (has(right_right_right)){
                  move = Move(right_right_right, right_right);
                  return true;
               }
               
            }else if (has(right_right)){
               
               Jewel right_up(jewel.type,p-1,q+1);
               Jewel right_down(jewel.type,p+1,q+1);
               
               if (has(right_up)){
                  move = Move(right_up, right);
                  return true;
               }
               if (has(right_down)){
                  move = Move(right_down, right);
                  return true;
               }
               
               
               
            }
            
         }
      }
      
      return false;
   }
   
private:
   
   vector< vector<Jewel> > jewels;
};



void swapXY(Location& loc){
   Location newloc(loc.y,loc.x);
   loc = newloc;
}

Location jewel2Location(Jewel jewel){
   Location loc;
   loc.x = 20 + jewel.j * JEWEL_WIDTH + JEWEL_WIDTH/2;
   loc.y = 20 + jewel.i * JEWEL_HEIGHT + JEWEL_HEIGHT/2;      
   return loc;
}

void locationToJewelIndices(Location loc, int& i, int &j){
   j = (loc.x - JEWEL_WIDTH/2) / JEWEL_WIDTH;
   i = (loc.y - JEWEL_HEIGHT/2) / JEWEL_HEIGHT;
}

Board board(8,8);
Board boardT(8,8);
Region boardRegion;

bool move(int type){
   board.reset();
   boardT.reset();
   
   vector<Match> ms;
   ms = boardRegion.findAllNow(Pattern(files[type]).similar(0.95));
   if (ms.empty())
      return false;
   
   for (int p=0; p<ms.size();++p){
      int i,j;
      locationToJewelIndices(ms[p].getCenter(), i, j);
      board.place(Jewel(type,i,j));
      boardT.place(Jewel(type,j,i));
   }
   
   bool hasValidMove = false;
   Move move;
   Location loc1,loc2;
   if (board.getMove(move)){
      loc1 = jewel2Location(move.from);
      loc2 = jewel2Location(move.to);
      hasValidMove = true;
   }else if (boardT.getMove(move)){
      loc1 = jewel2Location(move.from);
      loc2 = jewel2Location(move.to);  
      swapXY(loc1);
      swapXY(loc2);
      hasValidMove = true;
   }
   
   
   if (hasValidMove){
      boardRegion.click(loc1);
      Robot::delay(100);
      
      boardRegion.click(loc2);
      Robot::delay(1000);
      return true;
   }else{
      return false;
   }
   
}



int main(void){
   Screen s;
   addImagePath("examples/images");
   
   
   // Load the game in the web browser, in this
   // example, Google's Chrome
   switchApp("Google Chrome.app");
   
   Match menu;
   if (s.exists("bejeweled_menu.png")){
      menu = s.find("bejeweled_menu.png");
   }else{
      s.type("l", CMD);
      s.type("http://www.digbejeweled.com/\n");            
      menu = s.wait("bejeweled_menu.png",5);
   }
   
   s.click(menu);   
   try{
      s.click("bejeweled_play_button.png");
   }catch(FindFailed ff){
   }
   
   // Derive the Region corresponding to the game board,
   // which is to the right of the menu
   boardRegion = menu.right(320);
        
   while(true){
      
      if (move(BLUE))
         continue;
      else if (move(ORANGE))
         continue;
      else if (move(YELLOW))
         continue;
      else if (move(RED))
         continue;
      else if (move(PURPLE))
         continue;
      else if (move(GREEN))
         continue;
      else if (move(SILVER))
         continue;
      
      boardRegion.hover(Location(0,0));
      
   }  
   
   return 1;
}

