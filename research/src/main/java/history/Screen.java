/**
 * 
 */
package history;

import history.HistoryViewer.Mode;

import java.awt.BasicStroke;
import java.awt.Color;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Image;
import java.awt.Rectangle;
import java.awt.Stroke;
import java.awt.event.MouseAdapter;
import java.awt.image.BufferedImage;
import java.awt.image.RasterFormatException;
import java.awt.image.RescaleOp;
import java.io.BufferedReader;
import java.io.File;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;

import javax.imageio.ImageIO;

public class Screen {
	
	protected BufferedImage image;
	
	String filename;

	private Rectangles highlightRectangles = new Rectangles();

	protected BufferedImage image_darken;

	String highlightedWord;
	Image highlightedImage;
	
	public Screen(String filename){
		this.filename = filename;
	}
	
	public ArrayList<Rectangle> find(BufferedImage target_image){
		return Sikuli.find(filename, target_image);
	}
	
	public BufferedImage getImage(){
		loadImage();
		return image;
	}
	
	protected void loadImage(){
		
		if (image == null){
		
			try {
				File sourceimage = new File(filename);
				image = ImageIO.read(sourceimage);    
			} catch (IOException ioe) {
				ioe.printStackTrace();
			}
		}
		
		if (image_darken == null){
			
			// Darken the image by 10% 
			float scaleFactor = .6f; 
			RescaleOp op = new RescaleOp(scaleFactor, 0, null); 
			image_darken = op.filter(image, null); 
		}
	}
	
	public void setHighlightRectangle(Rectangle highlightRectangle) {
		highlightRectangles.clear();
		highlightRectangles.add(highlightRectangle);
	}

	public void paintHelper(Graphics g, Mode mode){
		
		loadImage();
		
		Graphics2D g2d = (Graphics2D) g;
		if (mode == Mode.PRESENT){
		
			g2d.drawImage(image, 0, 0, null);
			
		}else if (mode == Mode.FIND || mode == Mode.READ){
		
			g2d.drawImage(image_darken, 0, 0, null);

			if (highlightRectangles == null)
				return;

			for (Rectangle r : highlightRectangles){

//				int m = 2;
//				r.x = r.x - m;
//				r.y = r.y - m;
//				r.width = r.width + 2*m;
//				r.height = r.height + 2*m;
				
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
			
			RescaleOp op = new RescaleOp(0.8f, 0, null); 
			BufferedImage image_grayed = op.filter(subimage, null);
			
			g2d.drawImage(image_grayed, d, d, null);
			
			subimage = subimage.getSubimage(d,d,subimage.getWidth()-2*d, subimage.getHeight()-2*d);
			g2d.drawImage(subimage, 2*d, 2*d, null);
			
		}else if (mode == Mode.SELECT){
			
			g2d.drawImage(image_darken, 0, 0, null);
			
			if (highlightRectangles == null)
				return;
			
			Rectangle r = highlightRectangles.get(0);
			
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
	        
	}

	
	public void clearHighlightedRectangles() {
		highlightRectangles.clear();
	}
	
	public void addHighlightedRectangles(Rectangles rects){
		if (rects != null)
			highlightRectangles.addAll(rects);
	}
	
	public Rectangles getHighlightRectangles() {
		return highlightRectangles;
	}
	
	
	
	private void paintRectangle(Graphics2D g2d, Rectangle r, float width, Color color){
		Stroke old_pen = g2d.getStroke();
		Stroke Pen = new BasicStroke(width);
		g2d.setStroke (Pen);
		g2d.setColor(color);
		g2d.drawRect(r.x,r.y,r.width,r.height);
		g2d.setStroke(old_pen);
	}
	
	
	public BufferedImage crop(Rectangle rectSelection){
		return crop(rectSelection, null);
	}
	
	public BufferedImage crop(Rectangle rectSelection, Rectangle highlight){
		int w = rectSelection.width, h = rectSelection.height;
		if(w<=0 || h<=0)
			return null;
		BufferedImage crop  = new BufferedImage(w, h, BufferedImage.TYPE_INT_RGB);
		Graphics2D g2d = crop.createGraphics();
		try {
			g2d.drawImage(
					image.getSubimage(rectSelection.x, rectSelection.y, w, h),
					null, 0, 0
			);
			
			if (highlight != null){
				paintRectangle(g2d, highlight, 2.0f, Color.red);
			}
			
		}
		catch (RasterFormatException e) {
			e.printStackTrace();
		}
		g2d.dispose();
		return crop;
	}
	
	public static void main(String[] args) throws Exception {

		Screen s = new Screen("screen.png");
		BufferedImage image = null;
		try {
			File sourceimage = new File("apple.png");
			image = ImageIO.read(sourceimage);    
			Rectangle r = s.find(image).get(0);
			System.out.println(r);
			
		} catch (IOException ioe) {
			ioe.printStackTrace();
		}
		
		
		
	}
	
}