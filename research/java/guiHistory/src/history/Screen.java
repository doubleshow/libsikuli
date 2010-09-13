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

import javax.imageio.ImageIO;

import sikuli.UnionScreen;

public class Screen {
	
	protected BufferedImage image;
	
	String filename;

	protected Rectangles highlightRectangles = new Rectangles();

	protected BufferedImage image_darken;

	String highlightedWord;
	Image highlightedImage;
	
	public Screen(String filename){
		this.filename = filename;
	}
	
	public Rectangle find(BufferedImage target_image){
		
		//loadImage();
		
		try{
			File f;
			String line;

			f = File.createTempFile("target", ".png");
			f.deleteOnExit(); 

			ImageIO.write(target_image, "png", f);

			String command = "./ocr MATCH " + filename + " " + f.getPath();
			//System.out.println(command);
			Process p = Runtime.getRuntime().exec(command);

			BufferedReader input = new BufferedReader
			(new InputStreamReader(p.getInputStream()));
		
			int x=-1,y=-1;
			while ((line = input.readLine()) != null) {
				System.out.println(line);
				String[] xy = line.split(" ");
				x = Integer.parseInt(xy[0]);
				y = Integer.parseInt(xy[1]);
			}
			input.close();
		
			if (x>=0 && y>=0)
				return new Rectangle(x,y,target_image.getWidth(),target_image.getHeight());
			
			
		}
		catch(IOException e){
			e.printStackTrace();
		}
		
		return null;
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
			
		}else if (mode == Mode.FIND){
		
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

	public void setHighlightedWord(String word) {
		highlightedWord = word;
	}

	public void setHighlightedImage(BufferedImage image) {
		Rectangle m = find(image);
		// TODO: handle multiple matches
		highlightRectangles.clear();
		if (m != null)
			highlightRectangles.add(m);
	}
	
	public Rectangles getHighlightRectangles() {
		return highlightRectangles;
	}
	
	public BufferedImage crop(Rectangle rectSelection){
		int w = rectSelection.width, h = rectSelection.height;
		if(w<=0 || h<=0)
			return null;
		BufferedImage crop  = new BufferedImage(w, h, BufferedImage.TYPE_INT_RGB);
		Graphics2D crop_g2d = crop.createGraphics();
		try {
			crop_g2d.drawImage(
					image.getSubimage(rectSelection.x, rectSelection.y, w, h),
					null, 0, 0
			);
		}
		catch (RasterFormatException e) {
			e.printStackTrace();
		}
		crop_g2d.dispose();
		return crop;
	}
	
	public static void main(String[] args) throws Exception {

		Screen s = new Screen("screen.png");
		BufferedImage image = null;
		try {
			File sourceimage = new File("apple.png");
			image = ImageIO.read(sourceimage);    
			Rectangle r = s.find(image);
			System.out.println(r);
			
		} catch (IOException ioe) {
			ioe.printStackTrace();
		}
		
		
		
	}
	
}