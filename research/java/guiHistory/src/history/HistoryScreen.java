/**
 * 
 */
package history;

import java.awt.BasicStroke;
import java.awt.Color;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Rectangle;
import java.awt.Stroke;
import java.awt.image.BufferedImage;
import java.awt.image.RescaleOp;

public class HistoryScreen extends Screen{
			
	
	public HistoryScreen(String filename, int id){
		super(filename);
		     
		
		this.id = id;
		
		// Darken the image by 10% 
		float scaleFactor = .6f; 
		RescaleOp op = new RescaleOp(scaleFactor, 0, null); 
		image_darken = op.filter(image, null); 
        
	}
	
	private Rectangle highlightRectangle;
	private BufferedImage image_darken;
	
	
	
	public HistoryScreen getBefore(){
		return HistoryScreenDatabase.find(id + 1);
	}
	
	public HistoryScreen getAfter(){
		return HistoryScreenDatabase.find(id - 1);
	}
	
	int id;
	public int getId() {
		return id;
	}

	String timeString;
	
	public void setTimeString(String timeString) {
		this.timeString = timeString;
	}

	public void setHighlightRectangle(Rectangle highlightRectangle) {
		this.highlightRectangle = highlightRectangle;
	}
	
	public String getTimeString(){
		return timeString;
	}
	
	public void paintHelper(Graphics g){
		
		Graphics2D g2d = (Graphics2D) g;
	        
	    if (highlightRectangle != null){
	    	
	    	g2d.drawImage(image_darken, 0, 0, null);
	    	
	        Rectangle r = highlightRectangle;
	        BufferedImage subimage;
	        subimage = image.getSubimage(r.x,r.y,r.width,r.height);
	        g2d.drawImage(subimage, r.x, r.y, null);
	        
	        Stroke old_pen = g2d.getStroke();
	        Stroke Pen = new BasicStroke (3.0F);
	        g2d.setStroke (Pen);
	        g2d.setColor(Color.yellow);
	        g2d.drawRect(r.x,r.y,r.width,r.height);
	        g2d.setStroke(old_pen);
	        
	    }else{
	    	
	    	g2d.drawImage(image, 0, 0, null);
	    }
	}
	
	

	
}