/**
 * 
 */
package history;

import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;

import javax.imageio.ImageIO;

public class Screen {
	
	protected BufferedImage image;
	
	public Screen(String filename){
		
		 try {
			 File sourceimage = new File(filename);
		     image = ImageIO.read(sourceimage);    
		} catch (IOException ioe) {
            ioe.printStackTrace();
        }
	}
	
	public void paintHelper(Graphics g){
		
		Graphics2D g2d = (Graphics2D) g;
	    g2d.drawImage(image, 0, 0, null);
	        
	}
	
}