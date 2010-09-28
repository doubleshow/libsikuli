/**
 * 
 */
package history;

import java.awt.Color;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Rectangle;
import java.awt.image.BufferedImage;

class AnnotationHighlight extends Annotation {

	Rectangle rectangle;
	public AnnotationHighlight(ScreenImage screen, Rectangle rectangle){
		super(screen);
		this.rectangle = rectangle;
	}


	public void paintAnnotation(Graphics g) {
		Graphics2D g2d = (Graphics2D) g;

		BufferedImage image = screen.getImage();
		BufferedImage subimage;


		subimage = image.getSubimage(rectangle.x,rectangle.y,rectangle.width,rectangle.height);
		g2d.drawImage(subimage, rectangle.x, rectangle.y, null);

		paintRectangle(g2d, rectangle, 3.0F, Color.yellow);
		//			g2d.setColor(Color.black);	
		//			g2d.fillRect(r.x, r.y, r.width, r.height);

	}
}