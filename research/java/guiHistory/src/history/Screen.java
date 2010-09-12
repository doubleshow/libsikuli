/**
 * 
 */
package history;

import history.HistoryViewer.Mode;

import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.image.BufferedImage;
import java.awt.image.RescaleOp;
import java.io.File;
import java.io.IOException;

import javax.imageio.ImageIO;

public class Screen {
	
	protected BufferedImage image;
	
	String filename;
	
	public Screen(String filename){
		this.filename = filename;
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
	}
	
	public void paintHelper(Graphics g, Mode current_mode){
		
		loadImage();
		
		Graphics2D g2d = (Graphics2D) g;
	    g2d.drawImage(image, 0, 0, null);
	        
	}
	
}