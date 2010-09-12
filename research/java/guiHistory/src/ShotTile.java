import java.awt.Color;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Image;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.event.MouseMotionAdapter;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import java.net.URL;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

import javax.imageio.ImageIO;
import javax.swing.ImageIcon;
import javax.swing.JFrame;
import javax.swing.JPanel;

public class ShotTile extends JPanel {

	int IMG_WIDTH = 50;
	int IMG_HEIGHT = 50;
	
	int height = 250;
	int width = 250;
	BufferedImage image;
	
	public String filename;

	private Shot shot;
	
	public Shot getShot(){
		return shot;
	}
	
	public ShotTile(Shot shot){
		this.shot = shot;
		
		height = 150;
		width = shot.end_timestamp - shot.start_timestamp;
		
		setBackground(new Color(255,0,0));
		setPreferredSize(new Dimension(width, height));
		
		this.filename = ScreenshotViewer.getFilename(shot.id);
		
		 try {
			 File sourceimage = new File(filename);
		     image = ImageIO.read(sourceimage);          
         } catch (IOException ioe) {
             ioe.printStackTrace();
         }
         
         addMouseMotionListener(new MouseMoveAdapter());
         addMouseListener(new ClickAdapter());
         
         
         
	}

	  class MouseMoveAdapter extends MouseMotionAdapter{
     	 public void mouseMoved(MouseEvent e){
  	 			fireMarkerMovedEvent(e.getX());
  	 	}
	  }
	  
	  class ClickAdapter extends MouseAdapter{
	    	public void mouseClicked(MouseEvent e){
	    		int btn = e.getButton();
	    		if (btn == MouseEvent.BUTTON1)
	    			fireShotSelectedEvent(e.getX());
	    	}
	    	
	    	public void mouseEntered(MouseEvent e){
	    		fireShotSelectedEvent(e.getX());
	    	}
	    }
	
	public void paintComponent(Graphics g) {
        super.paintComponent(g); 

        Graphics2D g2d = (Graphics2D) g;
        
        float width = (float) image.getWidth() * (float) height / (float) image.getHeight();       
        g2d.drawImage(image, 0, 0, (int) width, height, null);
        
        g2d.setColor(new Color(20, 20, 20));
        g2d.drawRect(0, 0, (int) width-1, height-1);
    }

	
	private ArrayList _listeners = new ArrayList();
	public synchronized void addEventListener(ShotTileEventListener listener){
		_listeners.add(listener);	
	}
	
	public synchronized void removeEventListener(ShotTileEventListener listener){
		_listeners.remove(listener);
	}
	
	private synchronized void fireShotSelectedEvent(int x){

		int marker_timestamp = x;//shot.start_timestamp + x;
		
		ShotTileEvent event = new ShotTileEvent(this, marker_timestamp);
		 Iterator i = _listeners.iterator();
		     while(i.hasNext())  {
		       ((ShotTileEventListener) i.next()).shotSelected(event);
		     }
	}
	
	private synchronized void fireMarkerMovedEvent(int x){
	
		System.out.println(this.getWidth());
		int duration = shot.end_timestamp - shot.start_timestamp;
		int w = this.getWidth();
		int marker_timestamp = shot.start_timestamp + duration *  x / w;
	
		ShotTileEvent event = new ShotTileEvent(this, marker_timestamp);
		 Iterator i = _listeners.iterator();
		     while(i.hasNext())  {
		       ((ShotTileEventListener) i.next()).markerMoved(event);
		     }
	}
	
	
    public static void main(String[] args) {
    	
    	int lengths [] = {100, 150, 200};

    	JFrame frame = new JFrame();
        JPanel main = new JPanel(new FlowLayout(FlowLayout.CENTER));

        
        int offset = 0;
        for (int length : lengths) {	
        	Shot shot = new Shot(1, offset, offset + length);
    		main.add(new ShotTile(shot));
    		offset = offset + length;
    	}
        
        
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.add(main);
        
        frame.setSize(500,500);
        frame.setLocationRelativeTo(null);
        frame.setVisible(true);
    }
};