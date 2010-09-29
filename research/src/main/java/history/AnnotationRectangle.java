/**
 * 
 */
package history;

import java.awt.Color;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Rectangle;
import java.awt.image.BufferedImage;

class AnnotationRectangle extends Annotation {

	Rectangle rectangle;
	public AnnotationRectangle(Rectangle rectangle){
		super();
		this.rectangle = rectangle;
	}

	public void paintAnnotation(Graphics g) {
		Graphics2D g2d = (Graphics2D) g;
		paintRectangle(g2d, rectangle, 3.0F, Color.green);
	}
}