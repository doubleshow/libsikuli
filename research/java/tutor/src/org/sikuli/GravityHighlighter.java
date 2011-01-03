package org.sikuli;
import org.sikuli.script.Env;
import org.sikuli.script.FindFailed;
import org.sikuli.script.Location;
import org.sikuli.script.Region;
import org.sikuli.script.ScreenHighlighter;
import org.sikuli.script.Screen;

import java.awt.AWTException;
import java.awt.AlphaComposite;
import java.awt.BasicStroke;
import java.awt.Color;
import java.awt.Cursor;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Point;
import java.awt.Polygon;
import java.awt.Rectangle;
import java.awt.RenderingHints;
import java.awt.Stroke;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.lang.reflect.Array;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.Comparator;



public class GravityHighlighter extends ScreenHighlighter {

	public GravityHighlighter(){
		super(null);
	}

	public GravityHighlighter(Screen scr) {
		super(scr);
	}

	public Point current = null;
	public Point to = null;
	public Rectangle target = null;
	String message;

	public void paint(Graphics g){
		Graphics2D g2d = (Graphics2D)g;
		g2d.setComposite(AlphaComposite.getInstance(AlphaComposite.CLEAR, 0.0f));
		g2d.fillRect(0,0,getWidth(),getHeight());
		g2d.setComposite(AlphaComposite.getInstance(AlphaComposite.SRC_OVER, 0.6f));
		g2d.setRenderingHint(RenderingHints.KEY_ANTIALIASING, 
				RenderingHints.VALUE_ANTIALIAS_ON);

		//drawPolylineArrow(g, new int[]{from.x,to.x}, new int[]{from.y,to.y}, 3, 3);
		drawRayPolygon(g, current, target);


		g2d.setComposite(AlphaComposite.getInstance(AlphaComposite.SRC_OVER, 1.0f));
		g2d.setRenderingHint(RenderingHints.KEY_ANTIALIASING, 
				RenderingHints.VALUE_ANTIALIAS_ON);


		if (target != null && message != null){
			Point pt = new Point(target.x, target.y+target.height+4);
			ToolTip o = new ToolTip(message,pt);
			o.paint(g2d);
		}
	}

	public void drawRayPolygon(Graphics g, Point p, Rectangle rect){
		if (p == null || rect == null)
			return;

		Graphics2D g2d = (Graphics2D)g;

		Rectangle r = rect;
		// corners of the target rectangle
		int cxs[] = {r.x,r.x,r.x+r.width,r.x+r.width};
		int cys[] = {r.y,r.y+r.height,r.y+r.height,r.height};


		ArrayList<Point> corners = new ArrayList<Point>();
		corners.add(new Point(r.x,r.y));
		corners.add(new Point(r.x+r.width,r.y+r.height));
		corners.add(new Point(r.x+r.width,r.y));
		corners.add(new Point(r.x,r.y+r.height));

		Collections.sort(corners, new Comparator(){
			@Override
			public int compare(Object arg0, Object arg1) {
				return (int) (current.distance((Point) arg0) - current.distance((Point) arg1));
			}});

		int[]xs;
		int[]ys;

		int d = 5;
		if (p.x > rect.getMinX()-5 && p.x < rect.getMaxX()+5 ||
				p.y > rect.getMinY()-5 && p.y < rect.getMaxY()+5){

			xs = new int[3];
			ys = new int[3];


			xs[0] = (int) p.x;
			xs[1] = (int) corners.get(0).x;
			xs[2] = (int) corners.get(1).x;

			ys[0] = (int) p.y;
			ys[1] = (int) corners.get(0).y;
			ys[2] = (int) corners.get(1).y;


		}else{

			xs = new int[4];
			ys = new int[4];


			xs[0] = (int) p.x;
			xs[1] = (int) corners.get(2).x;
			xs[2] = (int) corners.get(0).x;
			xs[3] = (int) corners.get(1).x;

			ys[0] = (int) p.y;
			ys[1] = (int) corners.get(2).y;
			ys[2] = (int) corners.get(0).y;
			ys[3] = (int) corners.get(1).y;
		}

		Polygon shape = new Polygon(xs, ys, xs.length);

		Stroke pen = new BasicStroke(3.0F);
		g2d.setStroke(pen);
		g2d.setColor(Color.black);
		//g2d.drawPolygon(pointing_triangle);
		//g2d.drawRect(x,y,w,h);


		g2d.setColor(Color.red);
		g2d.fillPolygon(shape);		
		g2d.drawRect(rect.x,rect.y,rect.width,rect.height);



	}


	private void drawPolylineArrow(Graphics g, int[] xPoints, int[] yPoints, int headLength, int headwidth){

		double theta1;

		//calculate the length of the line - convert from Object to Integer to int value
		Object tempX1 = ((Array.get(xPoints, ((xPoints.length)-2))) );
		Object tempX2 = ((Array.get(xPoints, ((xPoints.length)-1))) );
		Integer fooX1 = (Integer)tempX1;
		int x1 = fooX1.intValue();
		Integer fooX2 = (Integer)tempX2;
		int x2 = fooX2.intValue();

		Object tempY1 = ((Array.get(yPoints, ((yPoints.length)-2))) );
		Object tempY2 = ((Array.get(yPoints, ((yPoints.length)-1))) );

		Integer fooY1 = (Integer)tempY1;
		int y1 = fooY1.intValue();
		Integer fooY2 = (Integer)tempY2;
		int y2 = fooY2.intValue();
		int deltaX = (x2-x1);
		int deltaY = (y2-y1);
		double theta = Math.atan((double)(deltaY)/(double)(deltaX));
		if (deltaX < 0.0){
			theta1 = theta+Math.PI; //If theta is negative make it positive
		}
		else{
			theta1 = theta; //else leave it alone
		}

		int lengthdeltaX =- (int)(Math.cos(theta1)*headLength);
		int lengthdeltaY =- (int)(Math.sin(theta1)*headLength);
		int widthdeltaX = (int)(Math.sin(theta1)*headwidth);
		int widthdeltaY = (int)(Math.cos(theta1)*headwidth);
		g.drawPolyline(xPoints, yPoints, xPoints.length);
		g.drawLine(x2,y2,x2+lengthdeltaX+widthdeltaX,y2+lengthdeltaY-widthdeltaY);
		g.drawLine(x2,y2,x2+lengthdeltaX-widthdeltaX,y2+lengthdeltaY+widthdeltaY);

	}

	public void setFullScreen(){
		Screen s = new Screen();
		setLocation(0,0);
		setSize(s.w,s.h);
		setVisible(true);
		toFront();
	}

	public void run(String ptn){
		run(ptn,"Click here");
	}
	
	public void run(String ptn, String message){

		Region r,s;
		r = null;
		Region previous_match = null;
		this.message = message;
		try {
			s = new Screen();	
			r = s.find(ptn);
		} catch (FindFailed e) {
			e.printStackTrace();
		}

		setFullScreen();


		Cursor handCursor = new Cursor(Cursor.HAND_CURSOR);
		Cursor defaultCursor = new Cursor(Cursor.DEFAULT_CURSOR);
		Cursor currentCursor = null;

		boolean running = true;
		while (running){

			target = new Rectangle(r.x,r.y,r.w,r.h);
			Location m = Env.getMouseLocation();

			current = new Point(m.x,m.y);

			Cursor cursor = null;
			if (target.contains(current)){
				running = false;
				cursor = handCursor;
				closeAfter(1.0f);
			}else{
				cursor = defaultCursor;
				repaint();
			}


			if (cursor != currentCursor){
				setCursor(cursor);
				currentCursor = cursor;
			}

		}

	}

	private void closeAfter(float secs){
		try{
			Thread.sleep((int)secs*1000);
		}
		catch(InterruptedException e){
			close();
			e.printStackTrace();
		}
		close();
	}

	public static void main(String[] args) throws AWTException, FindFailed {

		Screen screen = new Screen();

		GravityHighlighter sh = new GravityHighlighter(screen);

		Region r = new Region(20,20,100,100);
		Location loc = new Location(100,100);



		sh.run("play.png","click here to run");

	}



}
