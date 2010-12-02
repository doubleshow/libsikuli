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

public class SimpleBox extends JComponent{

	// style parameters
	final int spacing = 10;	// spacing between the box and the target
	final float hp = 0.2f;
	final int thickness = 20;

	String _text;
	Point _target;
	
	public SimpleBox(String text, Point target){
		super();
		_text = text;
		_target = target;
	}
	
	void paintContainer(Graphics2D g2d, Rectangle rect){

		Stroke pen = new BasicStroke(3.0F);
		g2d.setStroke(pen);
		
		g2d.setColor(new Color(255,250,205));
		g2d.fillRect(rect.x,rect.y,rect.width,rect.height);

		g2d.setColor(new Color(238,221,130));
		g2d.drawRect(rect.x,rect.y,rect.width,rect.height);
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
		
		paintContainer(g2d, rect);
		
		g2d.setColor(Color.black);
		g2d.drawString(_text, x, y+height-4);
	}
}