package org.sikuli;

import java.awt.AWTException;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Rectangle;

import javax.swing.JFrame;

public class Artist extends JFrame{

	public Artist(){
		setMinimumSize(new Dimension(500,500));

		//		int[]xs = new int[7];
		//		int[]ys = new int[7];
		//
		//		// Make a triangle
		//		xs[0]=100; xs[1]=150; xs[2]=50;
		//		ys[0]=100; ys[1]=150; ys[2]=150;
		//		
		//		int h = 200;
		//		int w = 300;
		//
		//		// attachment point
		//		int px = 250;
		//		int py = 400;
		//		
		//		float hp = 0.2f;
		//
		//		int thickness = 150;
		//		int spacing = 50;
		//
		//		int y = py - spacing - h;
		//		int x = px - Math.round(w*hp);
		//		
		//		
		//		xs[0] = (int) x + w/2 - thickness/2;
		//		xs[1] = (int) px;
		//		xs[2] = (int) x + w/2 + thickness/2;
		//		
		//		ys[0] = (int) y;
		//		ys[1] = (int) py;
		//		ys[2] = (int) y;
		//		
		//		int n = 3;
		//
		//		myTri = new Polygon(xs, ys, n);
		//		r = new Rectangle(x,y,w,h);
		//		
		//		
	}

	public void paint(Graphics g) {

		super.paint(g);
		Graphics2D g2d = (Graphics2D) g;	
		//paintCallout(g2d, new Point(200,250), new Dimension(200,150));


		//		g.setColor(Color.yellow);
		//		g.fillPolygon(myTri); 
		//		
		//		g.fillRect(r.x, r.y, r.width, r.height);
		//	}
	}

	public static void main(String[] args){

		Artist dc = new Artist();
		dc.setVisible(true);
	}

}
