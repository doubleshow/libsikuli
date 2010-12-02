/**
 * 
 */
package org.sikuli;

import java.awt.BasicStroke;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.FontMetrics;
import java.awt.Graphics2D;
import java.awt.Point;
import java.awt.Polygon;
import java.awt.Rectangle;
import java.awt.Stroke;

import javax.swing.JComponent;

public class CalloutBox extends JComponent{

	// style parameters
	final int spacing = 50;	// spacing between the box and the target
	final float hp = 0.2f;
	final int thickness = 20;

	String _text;
	Point _target;
	
	public CalloutBox(String text, Point target){
		super();
		_text = text;
		_target = target;
	}
	
	void paintContainer(Graphics2D g2d, Point target, Rectangle rect){

		// dimension of the callout box
		int h = rect.height;
		int w = rect.width;

		// target point
		int px = target.x;
		int py = target.y;


		// upper-left corner
		int y = rect.y;
		int x = rect.x;

		// compute the vertices of the triangle pointing to the target
		int[]xs = new int[3];
		int[]ys = new int[3];

		int xc = x + Math.round(0.3f * w);

		xs[0] = (int) xc - thickness/2;
		xs[1] = (int) px;
		xs[2] = (int) xc + thickness/2;

		ys[0] = (int) y + h;
		ys[1] = (int) py;
		ys[2] = (int) y + h;

		Polygon pointing_triangle = new Polygon(xs, ys, xs.length);

		Stroke pen = new BasicStroke(3.0F);
		g2d.setStroke(pen);
		g2d.setColor(Color.black);
		g2d.drawPolygon(pointing_triangle);
		g2d.drawRect(x,y,w,h);

		
		g2d.setColor(Color.yellow);
		g2d.fillPolygon(pointing_triangle);
		g2d.fillRect(x,y,w,h);


	}
	
	public void paint(Graphics2D g2d){
		
		Font f = new Font("sansserif", Font.BOLD, 32);
		g2d.setFont(f);
		
		FontMetrics fm = getFontMetrics(f);
		int width = fm.stringWidth(_text);
		int height = fm.getHeight();
		
		int y = _target.y - spacing - height;
		int x = _target.x - Math.round(0.2f * width);
		
		Rectangle rect = new Rectangle(x,y,width,height);
		
		paintContainer(g2d, _target, rect);
		
		g2d.setColor(Color.black);
		g2d.drawString(_text, x, y+height-4);
	}
}