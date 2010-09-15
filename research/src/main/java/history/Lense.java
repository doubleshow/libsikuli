package history;

import java.awt.BasicStroke;
import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Container;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Image;
import java.awt.Rectangle;
import java.awt.Stroke;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;

import javax.imageio.ImageIO;
import javax.swing.BorderFactory;
import javax.swing.BoxLayout;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLayeredPane;
import javax.swing.JPanel;


public class Lense extends JPanel implements KeyListener{
    
	private static final Container overiewFrame = null;

	private BufferedImage image;
	
	int x=0;
	int y=0;
	int w=600;
	int h=480;
	int step=50;
	
	private void paintRectangle(Graphics2D g2d, Rectangle r, float width, Color color){
		Stroke old_pen = g2d.getStroke();
		Stroke Pen = new BasicStroke(width);
		g2d.setStroke (Pen);
		g2d.setColor(color);
		g2d.drawRect(r.x,r.y,r.width,r.height);
		g2d.setStroke(old_pen);
	}
	
	
	class Overview extends JPanel {
		
		public Overview(int preferred_width){
			
			int height = image.getHeight() * preferred_width  / image.getWidth();
			setPreferredSize(new Dimension(preferred_width, height));
			setBorder(BorderFactory.createLineBorder(Color.yellow));
			//setBorder(BorderFactory.createRaisedBevelBorder());
			
		}


		public void paintComponent(Graphics g) {
			
			super.paintComponent(g);
	        
			Graphics2D g2d = (Graphics2D) g;
			
			float scale = (float) getWidth() / (float) image.getWidth();
			
			
	        g2d.drawImage(image, 0, 0, getWidth(), getHeight(), null);
	      
	        int rx = Math.round(x * scale);
	        int ry = Math.round(y * scale);
	        int rh = Math.round(h * scale);
	        int rw = Math.round(w * scale);
	        
	        paintRectangle(g2d, new Rectangle(rx,ry,rw,rh), 2, Color.red);
	        
		}
		
	}
	
	
	private Overview overview;
	
	
	JFrame oveviewFrame;
	
	public Lense(Dimension size, String filename) {
				
		this.setPreferredSize(size);
		w = (int) size.getWidth();
		h = (int) size.getHeight();
		
		try {
			 File sourceimage = new File(filename);
		     image = ImageIO.read(sourceimage);          
       } catch (IOException ioe) {
           ioe.printStackTrace();
       }
       
       
       //setLayout(null);
       
       JLayeredPane layeredPane = new JLayeredPane();
       //layeredPane.setLayout(new BoxLayout(layeredPane, BoxLayout.Y_AXIS));
       
       
       overview = new Overview(100);
       //overview.setLocation(20,20);
       //overview.setL
      // overview.setBounds(20,20,200,200);
       //overview.setAlignmentX(LEFT_ALIGNMENT);
       //layeredPane.add(overview, new Integer(2));
       
       
//       oveviewFrame = new JFrame();
//       oveviewFrame.setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
//       oveviewFrame.add(overview);        
//       oveviewFrame.setSize(overview.getPreferredSize());
//       oveviewFrame.setLocationRelativeTo(this);
//       oveviewFrame.setVisible(true);
		
//       oveviewFrame.addKeyListener(this);

       setLayout(new FlowLayout(FlowLayout.LEFT));
       add(overview);
       

       
	}
	
	public void move(int x, int y){
		this.x = x;
		this.y = y;
		repaint();
		overview.invalidate();
	}
	
	public void paintComponent(Graphics g) {
		
		super.paintComponent(g);
        
		Graphics2D g2d = (Graphics2D) g;
        BufferedImage subImage = image.getSubimage(x, y, w, h);
        g2d.drawImage(subImage, 0, 0, w, h, null);

    }
	
	public static void main(String[] args) {
    	
    	JFrame frame = new JFrame();
    	
        
        Dimension size = new Dimension(640,480);
        Lense main = new Lense(size,"long.png");
     	frame.addKeyListener(main);
        
        
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.add(main);
        
        frame.setSize(size);
        frame.setLocationRelativeTo(null);
        frame.setVisible(true);
    }

	@Override
	public void keyPressed(KeyEvent e) {
		if (e.getKeyCode() == KeyEvent.VK_DOWN){
			y = Math.min(image.getHeight() - h, y + step);
		}
		if (e.getKeyCode() == KeyEvent.VK_UP){
			y = Math.max(0, y - step);
		}
		if (e.getKeyCode() == KeyEvent.VK_RIGHT){
			x = Math.min(image.getWidth() - w, x + step);
		}
		if (e.getKeyCode() == KeyEvent.VK_LEFT){
			x = Math.max(0, x - step);
		}		
		
		repaint();
		overview.invalidate();
	}

	@Override
	public void keyReleased(KeyEvent e) {
	}

	@Override
	public void keyTyped(KeyEvent e) {
		
	}

}
