package org.sikuli.history;

import java.awt.Dimension;
import java.awt.Graphics2D;
import java.awt.Rectangle;
import java.awt.image.BufferedImage;

public class ImageUtil {

	static public Rectangle boundRectangle(Rectangle in, Dimension bounds){
		return boundRectangle(in, new Rectangle(bounds));
	}

	static public Rectangle boundRectangle(Rectangle in, Rectangle bounds){
		int x1 = Math.max(bounds.x, in.x);
		int y1 = Math.max(bounds.y, in.y);
		int x2 = Math.min(in.x + in.width, bounds.width);
		int y2 = Math.min(in.y + in.height, bounds.height);
	
		if (x2 < x1 || y2 < y1)
			return null;
		else
			return new Rectangle(x1,y1,x2-x1,y2-y1);
	}
	
	static public BufferedImage getSubImageBounded(BufferedImage src, Rectangle region){
		Rectangle bounds = new Rectangle(src.getWidth(),src.getHeight());

				
		if (bounds.contains(region))
			return src.getSubimage(region.x,region.y,region.width,region.height);
		
		
		BufferedImage ret = new BufferedImage(region.width,region.height, BufferedImage.TYPE_INT_RGB);
		Rectangle br = boundRectangle(region, bounds);
		
		//System.out.println(region + " " + bounds + "->" + br);
		
		BufferedImage subimage = src.getSubimage(br.x,br.y,br.width,br.height);

		Graphics2D g2d = ret.createGraphics();
		g2d.drawImage(subimage, br.x-region.x, br.y-region.y, null);
		g2d.dispose();
		
		return ret;
		
	}

}
