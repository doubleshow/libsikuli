package sikuli;

import java.util.*;
import java.io.*;
import java.awt.*;
import java.awt.event.*;
import java.awt.image.*;
import javax.imageio.ImageIO;


public class Region {
 
   public int x, y, w, h;
   public Region(int x_, int y_, int w_, int h_) {
      init(x_,y_,w_,h_);
   }

   public Region(Rectangle r) {
      init(r.x, r.y, r.width, r.height);
   }

   public Region(Region r) {
      init(r.x, r.y, r.w, r.h);
   }

   protected Region(){}

   void init(int x_, int y_, int w_, int h_) {
      x = x_;
      y = y_;
      w = w_;
      h = h_;
   }
}


