/**
 * 
 */
package history;

import history.HistoryViewer.Mode;
import history.OCRDocument.OCRWord;

import java.awt.BasicStroke;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Image;
import java.awt.Point;
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
import java.util.Arrays;
import java.util.HashSet;
import java.util.Set;

import javax.imageio.ImageIO;

public class ScreenImage {
	
	
	
	
	
	
	private static final Set<String> STOPLIST = new HashSet<String>(Arrays.asList(
				new String[] {"iyiote,","iâlilâihâiâ","it'li'=ii|","[]","ihhjm1","ytiotb",
						"whq","37l","bamelson","windows","http //hiqi vâ","fulâif\\âf",
						"doubie show,","mil","'ej","lm","eq"}
		));

	ArrayList<AnnotationImage> annotation_images = new ArrayList<AnnotationImage>();
	
	ArrayList<Annotation> annotations = new ArrayList<Annotation>();
	
	class AnnotationImage {
		
		public int getX() {
			return x;
		}

		public int getY() {
			return y;
		}

		public BufferedImage getImage() {
			return image;
		}

		int x;
		int y;
		BufferedImage image;
		
		public AnnotationImage(BufferedImage image, int x, int y){
			this.image = image;
			this.x = x;
			this.y = y;
		}
				
	};
	
	protected BufferedImage image;
	
	String filename;

	private Rectangles highlightRectangles = new Rectangles();

	protected BufferedImage image_darken;

	String highlightedWord;
	Image highlightedImage;
	
	public ScreenImage(String filename){
		this.filename = filename;
		setImage(filename);
	}
	
	public ScreenImage(BufferedImage image){
		setImage(image);
	}
	
	public ArrayList<Rectangle> find(BufferedImage target_image){
		return Sikuli.find(filename, target_image);
	}
	
	public BufferedImage getImage(){
		return image;
	}
	
	public void setImage(BufferedImage image){
		
		this.image = image;
		if (image_darken == null){
			
			// Darken the image by 10% 
			float scaleFactor = .6f; 
			RescaleOp op = new RescaleOp(scaleFactor, 0, null); 
			image_darken = op.filter(image, null); 
		}
		
	}
	
	public void setImage(String filename){
		if (image == null){			
			try {
				File sourceimage = new File(filename);				
				setImage(ImageIO.read(sourceimage));  
			} catch (IOException ioe) {
				ioe.printStackTrace();
			}
		}
	}
	
//	protected void loadImage(){
//		
//		if (image == null){
//		
//			try {
//				File sourceimage = new File(filename);
//				image = ImageIO.read(sourceimage);    
//			} catch (IOException ioe) {
//				ioe.printStackTrace();
//			}
//		}
//		
//		if (image_darken == null){
//			
//			// Darken the image by 10% 
//			float scaleFactor = .6f; 
//			RescaleOp op = new RescaleOp(scaleFactor, 0, null); 
//			image_darken = op.filter(image, null); 
//		}
//	}
	
	public void setHighlightRectangle(Rectangle highlightRectangle) {
		//highlightRectangles.clear();
		//highlightRectangles.add(highlightRectangle);
		//
	}

	int padding = 0;
	public void setHighlightRectanglePadding(int p){
		padding = p;
	}
	
	boolean hightlightRectangleBorder = true;
	public void setHighlightRectangleBorder(boolean b){
		hightlightRectangleBorder = b;
	}
	
	public void paintHelper(Graphics g, Mode mode){
		
		//loadImage();
		
		Graphics2D g2d = (Graphics2D) g;
		if (mode == Mode.PRESENT){
		
			g2d.drawImage(image, 0, 0, null);
			
		}else if (mode == Mode.READ){
		
			g2d.drawImage(image, 0, 0, null);			
			
		}else if (mode == Mode.FIND){
		
			g2d.drawImage(image_darken, 0, 0, null);
			//g2d.drawImage(image, 0, 0, null);

//			if (highlightRectangles == null)
//				return;

//			for (Annotation an : annotation_highlights){		
//				an.paintAnnotation(g2d);
//			}
			
//			Rectangle selected_region = new Rectangle(200,107,920,520);
//			selected_region = null;
			
//			for (Rectangle r : highlightRectangles){
//
//				if (selected_region != null && !selected_region.contains(r))
//					continue;
				
				
//				int m = 2;
//				r.x = r.x - m;
//				r.y = r.y - m;
//				r.width = r.width + 2*m;
//				r.height = r.height + 2*m;
				//r.grow(padding, padding);
				//r.grow(2, 2);
				
//				BufferedImage subimage;
//				subimage = image.getSubimage(r.x,r.y,r.width,r.height);
//				g2d.drawImage(subimage, r.x, r.y, null);
				

				
//				if (hightlightRectangleBorder){
////					paintRectangle(g2d, r, 3.0F, Color.yellow);
//
////					g2d.setColor(Color.black);	
////					g2d.fillRect(r.x, r.y, r.width, r.height);
//				}
//				
//				if (selected_region != null)
//					paintRectangle(g2d, selected_region, 3.0f, Color.green);
				
//			}
		}else if (mode == Mode.BROWSE){
			
			g2d.drawImage(image_darken, 0, 0, null);
			
			BufferedImage subimage;
			int d = 50;
			subimage = image.getSubimage(d,d,image.getWidth()-2*d, image.getHeight()-2*d);
			
			RescaleOp op = new RescaleOp(0.8f, 0, null); 
			BufferedImage image_grayed = op.filter(subimage, null);
			
			g2d.drawImage(image_grayed, d, d, null);
			
			subimage = subimage.getSubimage(d,d,subimage.getWidth()-2*d, subimage.getHeight()-2*d);
			op = new RescaleOp(0.9f, 0, null); 
			image_grayed = op.filter(subimage, null);
	
			g2d.drawImage(image_grayed, 2*d, 2*d, null);
			
			subimage = subimage.getSubimage(d,d,subimage.getWidth()-2*d, subimage.getHeight()-2*d);
			g2d.drawImage(subimage, 3*d, 3*d, null);
			
		}else if (mode == Mode.SELECT){
			
			g2d.drawImage(image_darken, 0, 0, null);
			
			//if (highlightRectangles == null)
//				return;
//			
////			Rectangle r = highlightRectangles.get(0);
//			
//			BufferedImage subimage;
//			subimage = image.getSubimage(r.x,r.y,r.width,r.height);
//			g2d.drawImage(subimage, r.x, r.y, null);
//
//			Stroke old_pen = g2d.getStroke();
//			Stroke Pen = new BasicStroke(3.0F);
//			g2d.setStroke (Pen);
//			g2d.setColor(Color.yellow);
//			g2d.drawRect(r.x,r.y,r.width,r.height);
//			g2d.setStroke(old_pen);
		}
		
		
		for (AnnotationImage ai : annotation_images){
			
			g2d.drawImage(ai.getImage(), ai.getX(), ai.getY(), null);
			
		}
		
		for (Annotation a : annotations){
			a.paintAnnotation(g2d);
		}
	        
	}

	public void addAnnotationImage(BufferedImage image, int x, int y){
		AnnotationImage ai = new AnnotationImage(image,x,y);
		annotation_images.add(ai);
	}
	
	public void addAnnotationArrow(Point from, Point to, Color color){
		AnnotationArrow ai = new AnnotationArrow(this, from,to,color);
		//annotation_arrows.add(ai);
		annotations.add(ai);
	}
	
	public void clearHighlightedRectangles() {
		//highlightRectangles.clear();
		//annotation_highlights.clear();
	}
	
	public void clear(){
		annotations.clear();
	}
	
	public void addHighlightedRectangles(Rectangles rects){
		for (Rectangle rect : rects){
			annotations.add(new AnnotationHighlight(this,rect));
		}
	}
	
	public void addHighlight(Rectangle rect){
		annotations.add(new AnnotationHighlight(this,rect));
	}
	
	public Rectangles getHighlightRectangles() {
		return highlightRectangles;
	}
	

	
//	protected void paintRectangle(Graphics2D g2d, Rectangle r, float width, Color color){
//		Stroke old_pen = g2d.getStroke();
//		Stroke Pen = new BasicStroke(width);
//		g2d.setStroke (Pen);
//		g2d.setColor(color);
//		g2d.drawRect(r.x,r.y,r.width,r.height);
//		g2d.setStroke(old_pen);
//	}
	
//	protected void paintSolidRectangle(Graphics2D g2d, Rectangle r, float width, Color color){
//		Stroke old_pen = g2d.getStroke();
//		Stroke Pen = new BasicStroke(width);
//		g2d.setStroke (Pen);
//		g2d.setColor(color);
//		g2d.drawRect(r.x,r.y,r.width,r.height);
//		g2d.setStroke(old_pen);
//	}
//	
	
	public BufferedImage crop(Rectangle rectSelection){
		return crop(rectSelection, null);
	}
	
	public BufferedImage createRenderedImage(Mode mode){
		int w = image.getWidth();
		int h = image.getHeight();
		
		BufferedImage rendered  = new BufferedImage(w, h, BufferedImage.TYPE_INT_RGB);
		Graphics2D g2d = rendered.createGraphics();
		paintHelper(g2d, mode);
		g2d.dispose();
		return rendered;
	}
	
	public ScreenImage crop0(Rectangle rect){		
		BufferedImage cropped_image = ImageUtil.getSubImageBounded(image, rect);
		return new ScreenImage(cropped_image);
	}
	
	public BufferedImage crop(Rectangle rectSelection, Rectangle highlight){
		//loadImage();
		
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
		//		paintRectangle(g2d, highlight, 2.0f, Color.red);
			}
			
		}
		catch (RasterFormatException e) {
			e.printStackTrace();
		}
		g2d.dispose();
		return crop;
	}
	
	public Rectangle getGrownRectangle(Rectangle in, int h, int v,
			Rectangle bounds) {
			
				int x1 = Math.max(bounds.x, in.x - h);
				int y1 = Math.max(bounds.y, in.y - v);
				int x2 = Math.min(in.x + in.width + h, bounds.width);
				int y2 = Math.min(in.y + in.height + v, bounds.height);
			
				if (x2 < x1 || y2 < y1)
					return null;
				else
					return new Rectangle(x1,y1,x2-x1,y2-y1);
			}

	public void highlightSameWords(HistoryScreen another, HistoryScreen another1) {
	
//		OCRDocument doc = HistoryScreenDatabase.getOCRDocument(id);
//		OCRDocument doca = HistoryScreenDatabase.getOCRDocument(another.getId());
//	
//		OCRDocument docb;
//		if (another1 != null)
//			docb = HistoryScreenDatabase.getOCRDocument(another1.getId());
//		else
//			docb = null;
//		
//		
//		//Rectangle newcontent = new Rectangle()
//	
//		int i=0;
//		ArrayList<OCRWord> words = doc.getWords();
//		for (OCRWord word : words){
//	
//			//			if ((i++) % 3 != 0)
//			//				continue;
//			//				
//	
//			String word_string = word.getWord();
//			Rectangle word_rect = word.getRectangle();
//			//System.out.println(word_string);
//	
//	
//			// don't track short words
//			if (word_string.length() < 5)
//				continue;
//	
//	
//			if (STOPLIST.contains(word_string)){
//				continue;
//			}
//	
//			Rectangles matches = doca.find(word_string);
//	
//			//addHighlightedRectangles(word_rect);
//			
//			if (matches == null){// && matches.size() == 1){
//	
//	
//				//addHighlightedRectangles(word_rect);
//	
//			}
//			else{
//	
//				// don't track short words
//				if (word_string.length() < 9)
//					continue;
//	
//				for (Rectangle match : matches){
//					//match = matches.get(0);
//					int dx = match.x - word_rect.x;
//					int dy = match.y - word_rect.y;
//					//				
//					if (Math.abs(dx) > 5 || dy < 0)
//						continue;
//	
//					//if (dx != 0 && dy != 0){
//					//System.out.println("[" + word_string + "]" + dx + " " + dy);
//	
//					Point from = new Point(word_rect.x, word_rect.y);
//					Point to = new Point(match.x, match.y);
//					//to.translate(dx, dy);
//	
//					int h = word_rect.height;
//					int w = word_rect.width;
//					from.translate(w/2,h/2);
//					to.translate(w/2,h/2);
//	
//					addAnnotationArrow(from, to, Color.red);
//	
//					Rectangle larger_word_rect = new Rectangle(word_rect);
//					larger_word_rect.grow(5,5);
//					addHighlightedRectangles(larger_word_rect);
//	
//					//Rectangle recta = new Rectangle(word_rect);
//					//recta.translate(dx, dy);
//	
//					match.grow(5,5);
//					BufferedImage imagea = another.crop(match);
//					//addAnnotationImage(imagea, match.x, match.y);
//	
//	
//	
//					if (docb == null)
//						continue;
//	
//					Rectangles matches1 = docb.find(word_string);
//					if (matches1 != null && matches1.size() == 1){
//						Rectangle first_match1 = matches1.get(0);
//						int dx1 = first_match1.x - to.x;
//						int dy1 = first_match1.y - to.y;
//	
//						if (dx1 != 0 && dy1 != 0){
//							System.out.println("[" + word_string + "]" + dx + " " + dy);
//	
//							Point from1 = to;//new Point(word_rect.x, word_rect.y);
//							Point to1 = new Point(from1);
//							to.translate(dx1, dy1);
//	
//							int h1 = word_rect.height;
//							int w1 = word_rect.width;
//							from1.translate(w1/2,h1/2);
//							to1.translate(w1/2,h1/2);
//	
//							addAnnotationArrow(from1, to1, Color.red);
//							//addHighlightedRectangles(word_rect);
//						}
//					}
//	
//				}
//			}
//		}
	
		//image = another.image;//id = another.id;
	}

	public void highlightNewWords(HistoryScreen another) {
	
//			OCRDocument doc = HistoryScreenDatabase.getOCRDocument(id);
//			OCRDocument doca = HistoryScreenDatabase.getOCRDocument(another.getId());
//	
//	
//	
//	
//			ArrayList<OCRWord> words = doc.getWords();
//			for (OCRWord word : words){
//	
//	
//				//			if (word.getWord().length() < 2)
//				//				continue;
//				//			
//				String w = word.getWord();
//	
//				Rectangle word_rect = word.getRectangle();
//				Rectangle word_rect2 = new Rectangle(word_rect);
//				word_rect2.translate(179, 15);
//	
//				OCRWord word2 = doca.find(word_rect2);
//	
//				// if there is a word at the corresponding location and
//				// the word is the same word, continue to the next word
//				if (word2 != null && word2.getWord().compareTo(w) == 0){
//					continue; 
//				}
//	
//				if (w.length() <= 2){
//					continue;
//				}
//	
//				if (STOPLIST.contains(w)){
//					continue;
//				}
//	
//				BufferedImage word_image = crop(word_rect);
//	
//				BufferedImage another_image = another.getImage();
//				Rectangle another_image_bounds = new Rectangle(another_image.getWidth(),another_image.getHeight());			
//				Rectangle region2 = getGrownRectangle(word_rect2, 20, 20, another_image_bounds);
//				if (region2 == null)
//					continue;
//	
//				BufferedImage region_image2 = 
//					another_image.getSubimage(region2.x,region2.y,
//							region2.width,region2.height);
//	
//	
//	
//				//ArrayList<Rectangle> matches = Sikuli.find(another.getImage(), word_image);
//				ArrayList<Rectangle> matches = Sikuli.find(region_image2, word_image);
//				if (matches.size() > 0){
//					Rectangle first_match = matches.get(0);
//					int dx = first_match.x - word_rect.x;
//					int dy = first_match.y - word_rect.y;
//					System.out.println(dx + " " + dy);
//					continue;
//				}
//	
//	
//				if (word2 != null){
//					System.out.println(w);
//					Rectangle word_rect2_g = new Rectangle(word2.getRectangle());
//					word_rect2_g.grow(2, 3);
//					BufferedImage imagea = another.crop(word_rect2_g);
//	
//					Graphics2D g2d = imagea.createGraphics();
//					BufferedImage im = imagea;
//					try {
//						g2d.setStroke(new BasicStroke(2.0f));
//						g2d.setColor(Color.red);
//						g2d.drawLine(0, im.getHeight()/2, im.getWidth(), im.getHeight()/2);
//	//					paintRectangle(g2d,new Rectangle(1,1,im.getWidth()-2,im.getHeight()-2),
//	//							2.0f,Color.black);
//					}
//					catch (RasterFormatException e) {
//						e.printStackTrace();
//					}
//	
//					addAnnotationImage(imagea, word_rect.x, word_rect.y + word_rect.height + 1);
//	
//					addHighlightedRectangles(word.getRectangle());	
//	
//	
//				}
//	
//	
//	
//				//System.out.println(word.getWord().length());
//	
//			}		
		}

	public void highlightAllWords() {
//		OCRDocument doc = HistoryScreenDatabase.getOCRDocument(id);
//	
//		ArrayList<OCRWord> words = doc.getWords();
//		for (OCRWord word : words){
//			System.out.println(word.getWord());
//			addHighlightedRectangles(word.getRectangle());		
//		}		
//	
		//Rectangles rects = HistoryScreenDatabase.findRectangles(id, word);
	}



	public void addHighlightedImage(BufferedImage image) {
		if (image == null)
			return;
	
		Rectangles rects = new Rectangles(find(image));
		addHighlightedRectangles(rects);
	}

	public static void main(String[] args) throws Exception {

		ScreenImage s = new ScreenImage("screen.png");
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