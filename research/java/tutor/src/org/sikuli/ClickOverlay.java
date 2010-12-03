package org.sikuli;
import java.awt.AWTException;
import java.awt.BasicStroke;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.FontMetrics;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.GraphicsDevice;
import java.awt.GraphicsEnvironment;
import java.awt.Point;
import java.awt.Rectangle;
import java.awt.Robot;
import java.awt.Stroke;
import java.awt.Toolkit;
import java.awt.event.ComponentAdapter;
import java.awt.event.ComponentEvent;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.image.BufferedImage;
import java.awt.image.RescaleOp;
import java.io.File;
import java.io.IOException;
import java.lang.reflect.Array;

import javax.sound.sampled.AudioInputStream;
import javax.sound.sampled.AudioSystem;
import javax.sound.sampled.Clip;
import javax.sound.sampled.DataLine;
import javax.sound.sampled.LineUnavailableException;
import javax.sound.sampled.UnsupportedAudioFileException;
import javax.swing.JWindow;



class ClickOverlay extends JWindow{

	Rectangle target;
	BufferedImage screen = null;
	Robot robot;
	String message;

	GraphicsDevice gdev;
	Object o;

	public ClickOverlay(Object o_, Rectangle target_){
		this(o_, target_, "Click Here");
	}

	public ClickOverlay(Object o_, Rectangle target_, String message_){
		this.target = target_;
		this.o = o_;
		this.message = message_;

		try {
			robot = new Robot();
		} catch (AWTException e1) {
			e1.printStackTrace();
		} 

		addMouseListener(new MouseAdapter(){

			public void mouseClicked(MouseEvent e){
				System.out.println(e.getX() + "," + e.getY());

				if (target.contains(e.getPoint())){
					System.out.println("inside");
					gdev.setFullScreenWindow(null);
					setVisible(false);
					screen = null;
					//						toBack();
				}				
			}

		});

		addComponentListener(new ComponentAdapter() {
			public void componentHidden(ComponentEvent e) 
			{
				System.out.println("I'm hidden");
				dispose();
				synchronized(o){
					o.notify();
				}
			}
		});

		start();
	}



	
	public void start(){
		screen = robot.createScreenCapture(new Rectangle(Toolkit.getDefaultToolkit().getScreenSize()));

		gdev = GraphicsEnvironment.getLocalGraphicsEnvironment().getDefaultScreenDevice();

		setVisible(true);
		if(gdev.isFullScreenSupported() ){
			gdev.setFullScreenWindow(this);
		}

	}


	public void paint(Graphics g) {

		if (screen != null){
			super.paint(g);
			Graphics2D g2d = (Graphics2D) g;


			RescaleOp op = new RescaleOp(0.8f, 0, null); 
			BufferedImage image_really_darken = op.filter(screen, null);
			g2d.drawImage(image_really_darken, 0, 0, null);

			BufferedImage subimage = screen.getSubimage(target.x,target.y,target.width,target.height);

			// original size
			g2d.drawImage(subimage, target.x, target.y, null);

			//g2d.drawImage(subimage, target.x-target.width/2, target.y-target.height/2, (int)2*target.width, (int)2*target.height, null);
			//g2d.drawImage(subimage, target.x, target.y-target.height/2, (int)2*target.width, (int)2*target.height, null);


			//Point p = new Point(target.x-target.width/2,target.y-target.height/2);
			//Point p = new Point(target.x,target.y-target.height/2);
			//
			//p.move(target.x-target.width/2, -target.height/2);



//			Stroke pen = new BasicStroke(3.0F);
//			g2d.setStroke(pen);
//			g2d.setColor(Color.red);
//			//g2d.drawRect(p.x, p.y, 2*target.width, 2*target.height);


			//Point pt = new Point(10, target.y-10);

			Point pt = new Point(target.x+target.width/2, target.y);
			
			if (true){			
				CalloutBox o = new CalloutBox(message,pt);
				o.paint(g2d);
			}else{
				
				SimpleBox o = new SimpleBox(message,pt);
				o.paint(g2d);
				
			}
			
	
			setVisible(true);
		}else{
			setVisible(false);
		}

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

	}//end drawPolylineArrow
}