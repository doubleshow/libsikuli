/**
 * 
 */
package history;

import java.awt.BasicStroke;
import java.awt.Color;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Point;
import java.awt.Stroke;

class AnnotationArrow extends Annotation{
	
	public Point getFrom() {
		return from;
	}

	public Point getTo() {
		return to;
	}

	Point from;
	Point to;
	Color color;
	
	public Color getColor() {
		return color;
	}

	public AnnotationArrow(ScreenImage screen, Point from, Point to, Color color){
		super(screen);
		this.from = from;
		this.to = to;
		this.color = color;
	}
	
	
	public void paintAnnotation(Graphics g) {
		Graphics2D g2d = (Graphics2D) g;
		
		g2d.setColor(color);
		Stroke pen = new BasicStroke(3.0F);
		g2d.setStroke(pen);
		g2d.drawLine(from.x,from.y,to.x,to.y);
	}
}