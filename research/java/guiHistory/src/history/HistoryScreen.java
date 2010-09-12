/**
 * 
 */
package history;

import history.HistoryViewer.Mode;

import java.awt.BasicStroke;
import java.awt.Color;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Rectangle;
import java.awt.Stroke;
import java.awt.image.BufferedImage;
import java.awt.image.RescaleOp;

public class HistoryScreen extends Screen
	implements Comparable{
			
	
	public HistoryScreen(String filename, int id){
		super(filename);
		     
		
		this.id = id;
		
	
	}
	
	private Rectangles highlightRectangles = new Rectangles();
	private BufferedImage image_darken;
	
	protected void loadImage(){
		
		super.loadImage();
		
		if (image_darken == null){
		
			// Darken the image by 10% 
			float scaleFactor = .6f; 
			RescaleOp op = new RescaleOp(scaleFactor, 0, null); 
			image_darken = op.filter(image, null); 
		}
   
	}
	
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
		//this.highlightRectangle = highlightRectangles
	}
	
	String highlightedWord;
	public void setHighlightedWord(String word){
		highlightedWord = word;
		highlightRectangles = HistoryScreenDatabase.findRectangles(id, highlightedWord);
	}
	
	public Rectangles getHighlightRectangles(){
		return highlightRectangles;
	}
	
	public String getTimeString(){
		return timeString;
	}
	
	public void paintHelper(Graphics g, Mode mode){
		
		loadImage();
		
		Graphics2D g2d = (Graphics2D) g;
		if (mode == Mode.FIND){
		
			g2d.drawImage(image_darken, 0, 0, null);

			if (highlightRectangles == null)
				return;

			for (Rectangle r : highlightRectangles){

				int m = 2;
				r.x = r.x - m;
				r.y = r.y - m;
				r.width = r.width + 2*m;
				r.height = r.height + 2*m;
				
				BufferedImage subimage;
				subimage = image.getSubimage(r.x,r.y,r.width,r.height);
				g2d.drawImage(subimage, r.x, r.y, null);

				Stroke old_pen = g2d.getStroke();
				Stroke Pen = new BasicStroke(3.0F);
				g2d.setStroke (Pen);
				g2d.setColor(Color.yellow);
				g2d.drawRect(r.x,r.y,r.width,r.height);
				g2d.setStroke(old_pen);
			}
		}else if (mode == Mode.BROWSE){
			
			g2d.drawImage(image_darken, 0, 0, null);
			
			BufferedImage subimage;
			int d = 50;
			subimage = image.getSubimage(d,d,image.getWidth()-2*d, image.getHeight()-2*d);
			
			float scaleFactor = .6f; 
			RescaleOp op = new RescaleOp(0.8f, 0, null); 
			BufferedImage image_grayed = op.filter(subimage, null);
			
			g2d.drawImage(image_grayed, d, d, null);
			
			subimage = subimage.getSubimage(d,d,subimage.getWidth()-2*d, subimage.getHeight()-2*d);
			g2d.drawImage(subimage, 2*d, 2*d, null);
			
		}
	    
	}

	@Override
	public int compareTo(Object o) {
		HistoryScreen hs = (HistoryScreen) o;
		return this.id - hs.id;    
	}
	
	

	
}