package org.sikuli.guide;
import java.awt.AWTException;
import java.awt.AlphaComposite;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Point;
import java.awt.Rectangle;
import java.awt.RenderingHints;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.ArrayList;

import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.JPanel;

import org.sikuli.script.FindFailed;
import org.sikuli.script.Location;
import org.sikuli.script.Pattern;
import org.sikuli.script.Region;
import org.sikuli.script.Screen;
import org.sikuli.script.ScreenHighlighter;



public class ScreenAnnotator extends ScreenHighlighter {

	public ScreenAnnotator(){
		super(null);
	}

	JPanel content = new JPanel(null);
	public ScreenAnnotator(Screen scr) {
		super(scr);
		content.setPreferredSize(new Dimension(500,500));
		add(content);
	}
	
	ArrayList<Annotation> _annotations = new ArrayList<Annotation>();

	public void paint(Graphics g){
		Graphics2D g2d = (Graphics2D)g;
		
		// clear the screen
		g2d.setComposite(AlphaComposite.getInstance(AlphaComposite.CLEAR, 0.0f));
		g2d.fillRect(0,0,getWidth(),getHeight());	
		


		for (Annotation an : _annotations){
			g2d.setComposite(AlphaComposite.getInstance(AlphaComposite.SRC_OVER, 1.0f));
			g2d.setRenderingHint(RenderingHints.KEY_ANTIALIASING, 
					RenderingHints.VALUE_ANTIALIAS_ON);			
			an.paintAnnotation(g2d);
		}

		g2d.setComposite(AlphaComposite.getInstance(AlphaComposite.SRC_OVER, 0.9f));
		g2d.setRenderingHint(RenderingHints.KEY_ANTIALIASING, 
					RenderingHints.VALUE_ANTIALIAS_ON);
		super.paint(g);
	}

	public void setFullScreen(){
		Screen s = new Screen();
		setLocation(0,0);
		setSize(s.w,s.h);
		setVisible(true);
		toFront();
	}

	public void clear(){
		_annotations.clear();
		content.removeAll();
	}
	
	public void addAnnotation(Annotation annotation){
		_annotations.add(annotation);
	}
	
	public void addArrow(Point from, Point to){
		addAnnotation(new AnnotationArrow(from, to, Color.black));
	}
	
	public void addHighlight(Region region){		
		addAnnotation(new AnnotationHighlight(region.getRect()));
	}
	
	public void addToolTip(String message, Point location){
		addAnnotation(new AnnotationToolTip(message, location));
	}
	
	public void addText(String message, Point location){
		String tooltipStyle = "font-size:16px;background-color:#FFFFDD;padding:3px;";
		String bwStyle = "font-size:16px;color:white;background-color:#333333;padding:3px";
		
		String htmltxt = 
			"<html><div style='" + bwStyle + "'>"
			+ message + "</div></html>";

		JLabel l = new JLabel(htmltxt);
		Dimension size = l.getPreferredSize();
		l.setBounds(location.x,location.y,size.width,size.height);
		content.add(l);
		repaint();
	}
	
	public void show(float secs){
		// must be at least 3.0 seconds
		float secs1 = (float) Math.max(3.0, secs);
		
		setFullScreen();
		repaint();
		closeAfter(secs1);
	}

	private void closeAfter(float secs){
		try{
			Thread.sleep((int)secs*1000);
		}
		catch(InterruptedException e){
			close();
			e.printStackTrace();
		}
		clear();
		close();
	}

	public static void main(String[] args) throws AWTException, FindFailed {

//		Screen screen = new Screen();
//
//		ScreenAnnotator sa = new ScreenAnnotator(screen);
//		
//		Screen s = new Screen();
//		Region r = null;
//		
		
		//r = s.find(new Pattern("http://sikuli.org/images/puzzle.png"));
		// = s.find(new Pattern("http://udn.com/2010MAIN/photonews/6048009-2482714_small.jpg"));

		//r = s.find(new Pattern("puzzle.png"));
//		sh.addToolTip("Text recog", new Point(r.x,r.y+r.h+5));
//		sh.addHighlight(r);
//		sh.drawNow(3.0f);
		
//		sh.clear();
		
		ScreenAnnotator sa = new ScreenAnnotator(new Screen());
		
		Screen s = new Screen();
		Region r = null;

		r = s.find("play.png");
		sa.addHighlight(r);
		sa.addToolTip("Run", new Point(r.x,r.y+r.h+5));
		

	
		r = s.find("addjava.png");
		sa.addHighlight(r);
		Point x = new Point(r.x,r.y);
		Point x1 = new Point(x);
		x1.translate(0, r.h+5);
		
		Point c =  r.getCenter();
	
		
		//sa.addToolTip("Add a Java class", x);
		sa.addText("Click this to create a Java class", x1);
		sa.addArrow(x1,c);
	
		sa.show(3.0f);


		
		r = s.find("new.png");
		sa.addHighlight(r);
		sa.addToolTip("Create a new project", new Point(r.x,r.y+r.h+5));
		//sa.addText("Click this to create <br>a new project", new Point(r.x,r.y+r.h+20));
		
		
		sa.show(3.0f);
		
			
		

	}



}
