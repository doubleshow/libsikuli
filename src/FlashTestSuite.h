/*
 *  FlashTestSuite.h
 *  sikuli
 *
 *  Created by Tom Yeh on 7/29/10.
 *  Copyright 2010 sikuli.org. All rights reserved.
 *
 */

#include <cxxtest/TestSuite.h>
#include "sikuli.h"
#include "event-manager.h"

using namespace sikuli;

//
//enum{
//   UNKNOWN = -1,
//   BLUE = 0,
//   ORANGE,
//   YELLOW,
//   RED,
//   PURPLE,
//   GREEN,
//   SILVER
//};
//
//#define JEWEL_HEIGHT 36
//#define JEWEL_WIDTH 36
//
//const char files[][20] = {
//   "blue.png",
//   "orange.png",
//   "yellow.png",
//   "red.png",
//   "purple.png",
//   "green.png",
//   "silver.png"
//};
//
//class Jewel{
// 
//public:
//   
//   Jewel(){};
//   Jewel(int type_, int i_, int j_) : i(i_), j(j_), type(type_) {};
//   Jewel(int type_) : type(type_) {};
//   
//   int type;
//   int i,j;
//   
//   friend ostream& operator<<(ostream& os, const Jewel& jewel){
//      switch (jewel.type){
//         case UNKNOWN:
//            os << "?";
//            break;
//         case BLUE:
//            os << "B";
//            break;
//         case ORANGE:
//            os << "O";
//            break;
//         case YELLOW:
//            os << "Y";
//            break;
//         case RED:
//            os << "R";
//            break;
//         case PURPLE:
//            os << "P";
//            break;
//         case GREEN:
//            os << "G";
//            break;
//         case SILVER:
//            os << "S";
//            break;
//            
//      }
//      return os;
//   }
//   
//};
//
//
//class Move{
//   
//public:
//   
//   Move() {};
//   Move(Jewel a, Jewel b) : from(a), to(b){};
//   
//   Jewel from;
//   Jewel to;
//   
//};
//
//class Board{
//
//public:
//
//   Board(int w, int h){
//      this->w = w;
//      this->h = h;
//      
//      for (int i=0; i<h; i++){
//      
//         vector<Jewel> row;
//         row.clear();
//         for (int j=0; j<w; j++){
//            row.push_back(Jewel(UNKNOWN,i,j));
//         }
//         
//         jewels.push_back(row);
//      }
//   };
//   
//   int h;
//   int w;
//   
//   void place(Jewel jewel){
//      if (within_bounds(jewel))
//         jewels[jewel.i][jewel.j] = jewel;
//   }
//   
//   void display(){
//      cout << h << "x" << w << endl;
//      for (int i=0; i<h; i++){
//         
//         for (int j=0; j<w; j++){
//            cout << jewels[i][j];
//         }
//         cout << endl;
//      }      
//   }
//   
//   void reset(){
//      for (int i=0; i<h; i++)
//         for (int j=0; j<w; j++)
//            jewels[i][j].type = UNKNOWN;
//   }
//   
//   bool has(Jewel jewel){
//      return within_bounds(jewel) && 
//         jewels[jewel.i][jewel.j].type == jewel.type;      
//   }
//   
//   bool within_bounds(Jewel p){
//      return p.i >= 0 && p.i < h && p.j >= 0 && p.j < w;
//   } 
//   
//   bool getMove(Move& move) {
//      
//      for (int p=0; p<h;++p){
//         for (int q=0; q<w;++q){
//            
//            Jewel jewel = jewels[p][q];
//            
//            if (jewel.type == UNKNOWN)
//               continue;
//            
//            Jewel right(jewel.type,p,q+1);
//            Jewel right_right(jewel.type,p,q+2);
//            Jewel left(jewel.type,p,q-1);
//            
//            if (has(right)){
//               
//               Jewel right_right_up(jewel.type,p-1,q+2);
//               Jewel right_right_down(jewel.type,p+1,q+2);
//               Jewel right_right_right(jewel.type,p,q+3);
//               Jewel left_up(jewel.type,p-1,q-1);
//               Jewel left_down(jewel.type,p+1,q-1);
//               Jewel left_left(jewel.type,p,q-2);
//               
//               if (has(left_up)){
//                  move = Move(left_up, left);
//                  return true;
//               }
//               
//               if (has(left_left)){
//                  move = Move(left_left, left);
//                  return true;
//               }
//                              
//               if (has(left_down)){
//                  move = Move(left_down, left);
//                  return true;
//               }
//               
//               if (has(right_right_up)){
//                  move = Move(right_right_up, right_right);
//                  return true;
//               }
//               
//               if (has(right_right_down)){
//                  move = Move(right_right_down, right_right);
//                  return true;
//               }
//               
//               if (has(right_right_right)){
//                  move = Move(right_right_right, right_right);
//                  return true;
//               }
//               
//            }else if (has(right_right)){
//               
//               Jewel right_up(jewel.type,p-1,q+1);
//               Jewel right_down(jewel.type,p+1,q+1);
//               
//               if (has(right_up)){
//                  move = Move(right_up, right);
//                  return true;
//               }
//               if (has(right_down)){
//                  move = Move(right_down, right);
//                  return true;
//               }
//               
//               
//            
//            }
//            
//         }
//      }
//      
//      return false;
//   }
//  
//private:
//   
//   vector< vector<Jewel> > jewels;
//};
//
//
//
//void swapXY(Location& loc){
//   Location newloc(loc.y,loc.x);
//   loc = newloc;
//}
//
//Location jewel2Location(Jewel jewel){
//   Location loc;
//   loc.x = 20 + jewel.j * JEWEL_WIDTH + JEWEL_WIDTH/2;
//   loc.y = 20 + jewel.i * JEWEL_HEIGHT + JEWEL_HEIGHT/2;      
//   return loc;
//}
//
//void locationToJewelIndices(Location loc, int& i, int &j){
//   j = (loc.x - JEWEL_WIDTH/2) / JEWEL_WIDTH;
//   i = (loc.y - JEWEL_HEIGHT/2) / JEWEL_HEIGHT;
//}
//
//
////0013115F10AA
//
//Board board(8,8);
//Board boardT(8,8);
//Region boardRegion;
//
//
//bool move(int type){
//   board.reset();
//   boardT.reset();
//   
//   vector<Match> ms;
//   ms = boardRegion.findAllNow(Pattern(files[type]).similar(0.95));
//   if (ms.empty())
//      return false;
//   
//   for (int p=0; p<ms.size();++p){
//      int i,j;
//      locationToJewelIndices(ms[p].getCenter(), i, j);
//      board.place(Jewel(type,i,j));
//      boardT.place(Jewel(type,j,i));
//   }
//   
//   bool hasValidMove = false;
//   Move move;
//   Location loc1,loc2;
//   if (board.getMove(move)){
//      loc1 = jewel2Location(move.from);
//      loc2 = jewel2Location(move.to);
//      hasValidMove = true;
//   }else if (boardT.getMove(move)){
//      loc1 = jewel2Location(move.from);
//      loc2 = jewel2Location(move.to);  
//      swapXY(loc1);
//      swapXY(loc2);
//      hasValidMove = true;
//   }
//   
// 
//   if (hasValidMove){
////      cout << " has move" << endl;
////         board.display();
//
//      boardRegion.click(loc1);
//      Robot::delay(100);
//      
//      boardRegion.click(loc2);
//      Robot::delay(1000);
//      return true;
//   }else{
//      return false;
//   }
//   
//}

void play_bejeweled();
void play_rome_puzzle();


void appear_callback(Event event){
   cout << event.pattern.getImageURL() << " appeared in " << event.region.toString() << endl;
   event.region.hover(event.match);
}

void vanish_callback(Event event){
   cout << event.pattern.getImageURL() << " vanished from " << event.region.toString() << endl;
}

void change_callback(Event event){
   cout << "change observed in " << event.region.toString() << endl;
}


void appear_and_stop_callback(Event event){
   cout << event.pattern.getImageURL() << " appeared in " << event.region.toString() << endl;
   EventManager::stop();
   cout << "stopping event manager.." << endl;
}

class VanishEventHandler : public SikuliEventHandler{
  
public:
   virtual void handle(Event event){
      cout << event.pattern.getImageURL() << " vanished from " << event.region.toString() << endl;  
   }
};

void wack(Event event){
   cout << event.pattern.getImageURL() << " appeared in " << event.region.toString() << endl;
   event.region.click(event.match);
}


class FlashTestSuite : public CxxTest::TestSuite 
{
public:
   
   Screen s;
   
   void setUp() {
      Robot::openApp("Google Chrome.app");
      Settings::addImagePath("flash/images");
      s = Screen(1);
   }
   
   void tearDown() {      
      // sleep to allow the previous test to complete
      sleep(1);
      Settings::resetImagePaths();
   }
   
   void switchToTest(const char* testTabImage)
   {
      s.click(testTabImage);
      sleep(1);
   }
   
//   void testClick(void)
//   {  
//      switchToTest("TestClick.png");
//      s.click("clickHere.png");
//      TS_ASSERT(s.exists("success.png"));
//   }
//
//   
//   void testDoubleClick(void)
//   {
//      switchToTest("TestDoubleClick.png");
//      s.doubleClick("clickHere.png"); 
//      TS_ASSERT(s.exists("success.png"));
//   }
//   
//   void testRightClick(void)
//   {
//      s.rightClick("clickHere.png");
//      TS_ASSERT(s.exists("about_adobe_flash_player.png"));
//      s.press(ESC);
//   }
//   
//   void testType(void)
//   {
//      switchToTest("TestType.png");
//      s.type("textinput.png",
//      "abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ !@#$%^&*()_+ 1234567890-=`~;':,./<>?\"[]{}\\|");
//      TS_ASSERT(s.exists("success.png"));
//   }
//   
//   void testKeys(void)
//   {
//      switchToTest("TestKeys.png");
//      s.click("textinput.png");
//      s.press(DOWN);
//      s.press(UP);
//      s.press(LEFT);
//      s.press(RIGHT);
//      s.press(F1); s.press(F2); s.press(F3); s.press(F4); s.press(F5); s.press(F6);
//      s.press(F7); s.press(F8); s.press(F9); s.press(F10); s.press(F11); s.press(F12);
//      s.press(BACKSPACE);
//      s.press(DELETE);
//      s.press(END);
//      s.press(ENTER);
//      s.press(ESC);
//      s.press(PAGE_DOWN);
//      s.press(PAGE_UP);
//      s.press(SPACE);
//      s.press(CAPSLOCK);
//      s.press(INSERT);
//      s.press(HOME);
//      s.press(TAB);
//   }
//   
//   void testOCR(void)
//   {
//      switchToTest("TestOCR.png");
//      initOCR("arial.png");
//      s.click("/System Preferences/");
//      s.click("/Configuration/");
//      s.click("/Keyboard/");
//      s.click("/My Favorites/");
//
//   }
//   
//   
//   void testDragDrop(void)
//   {
//      switchToTest("TestDragDrop.png");
//      s.dragDrop("hslider_thumb.png", "dropHere.png");
//      TS_ASSERT(s.exists("success.png"));
//   }
//
//   void testFindAll(void)
//   {
//      switchToTest("TestFindAll.png");
//      vector<Match> ms = s.findAll("flower.png");
//      TS_ASSERT_EQUALS(ms.size(), 6);
//
//      ms = s.findAll("folder.png");
//      TS_ASSERT_EQUALS(ms.size(), 5);
//
//      ms = s.findAll("checkbox.png");
//      TS_ASSERT_EQUALS(ms.size(), 5);      
//
//      for (int i=0; i< ms.size(); ++i)
//         s.click(ms[i]);
//
//      TS_ASSERT(s.exists("success.png"));      
//   }
//   
//   void testVanish(void)
//   {
//      switchToTest("TestVanish.png");
//      s.waitVanish("flower.png");
//      TS_ASSERT(!s.exists("flower.png",0)); 
//   }
//   
//   void testSpatialOp(void)
//   {
//      switchToTest("TestSpatialOp.png");
//      
//      
//      
//      Match computer = s.find("computer.png");
//      s.hover(computer);
//      
//      computer.left().find("clickButton.png").click(Location(10,10));
//      computer.below().find("clickButton.png").click(Location(10,10));
//      computer.right().find("clickButton.png").click(Location(10,10));
//      computer.above().find("clickButton.png").click(Location(10,10));
//      
//      TS_ASSERT(s.exists("success.png"));
//   }
//
// void testOrdering(void)
//   {
//      switchToTest("TestOrdering.png");
//
//      
//      Pattern p; 
//      p = Pattern("button.png").leftMost();
//      s.click(p);
//      p = Pattern("button.png").bottomMost();
//      s.click(p);
//      p = Pattern("button.png").rightMost();
//      s.click(p);
//      p = Pattern("button.png").topMost();
//      s.click(p);
//      
//      p = Pattern("button.png").order(LEFTRIGHT);
//      s.click(s.findAll(p)[2]);
//      p = Pattern("button.png").order(BOTTOMUP);
//      s.click(s.findAll(p)[1]);
//      p = Pattern("button.png").order(RIGHTLEFT);
//      s.click(s.findAll(p)[2]);
//      p = Pattern("button.png").order(TOPDOWN);
//      s.click(s.findAll(p)[1]);
//      
//      TS_ASSERT(s.exists("success.png"));
//      
//   }
//   
//   void testPaste(void)
//   {
//      switchToTest("TestPaste.png");
//      s.paste("paste_target.png", "paSte tHis text here!!!");
//      TS_ASSERT(s.exists("success.png"));
//   }
//   
//   void testEvent(void)
//   {
//      switchToTest("TestEvent.png");
//      
//      Match m = s.find("SikuliTester.png");
//      
//      Region r = s.inner(m.x,m.y,230,250);
//      
//      EventManager::addObserver(r, Observer(APPEAR, Pattern("computer.png"), &appear_callback));
//      
//      VanishEventHandler ve;
//      EventManager::addObserver(r, Observer(VANISH, Pattern("computer.png"), &ve));
//      
//      r.onAppear(Pattern("flower.png"), &appear_callback);
//      r.onVanish(Pattern("flower.png"), &ve);
//      
//      r.onAppear(Pattern("bug.png"), &appear_and_stop_callback);      
//      EventManager::observe(20);
// 
//  
//   }
   
//   void testWakAMole(void)
//   {
//      Match g = s.find("startpage.png");
//      g.click("play.png");
// 
//      while (!g.exists("enter.png",0)){
//         try{
//            
//            vector<Match> ms = g.findAllNow(Pattern("mole.png"));
//            cout << ms.size() << ": ------" << endl;
//            for (int j=0;j<ms.size();++j){
//               g.click(ms[j]);
//               cout << ms[j].toString() << endl; 
//            }
//            
//            if (ms.size()>0)
//               Robot::delay(200);
//            
//         }catch (FindFailed ff){
//            
//         }
//         
//         
//      }
//      
//   }
   
   void testBejeweled(void)
   {
      //play_bejeweled();
      
      // http://www.addictinggames.com/romepuzzle.html
//      play_rome_puzzle();
      
//      for (int i=0; i<ms.size();++i){
//         board.click(ms[i]);
//         
//         ms[i]
//         Robot::delay(100);
//      }
      
      
      
      //board.click("blue.png");
      
      
      
   }
   
//   void testUnicode(void)
//   {
//      switchToTest("TestPaste.png");
//      s.click("paste_target.png");
//      s.paste("paSte tHis text here!!!");
//   }
   
};


   
   
  
