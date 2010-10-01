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
import java.awt.Point;
import java.awt.Rectangle;
import java.awt.Stroke;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;

import javax.imageio.ImageIO;
import javax.swing.BorderFactory;
import javax.swing.Box;
import javax.swing.BoxLayout;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLayeredPane;
import javax.swing.JPanel;


public class Lens extends JPanel implements KeyListener{

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

		public Overview(BufferedImage image, int preferred_width){
			//setBorder(BorderFactory.createLineBorder(Color.yellow));
			
			int height = image.getHeight() * preferred_width  / image.getWidth();
			setPreferredSize(new Dimension(preferred_width, height));
			//setBorder(BorderFactory.createLineBorder(Color.yellow));
			
		    MoveMouseListener mml = new MoveMouseListener(this);
		    addMouseListener(mml);
		    addMouseMotionListener(mml);

		}

		public void setWidth(int preferred_width){
			
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

	public Lens(){
	}
	
	public void show(Dimension size, String filename){
		setDimension(size);
		setImage(filename);
		createOverview();
	}

	public void setDimension(Dimension size){
		this.setPreferredSize(size);
		w = (int) size.getWidth();
		h = (int) size.getHeight();
	}

	public void setImage(String filename){
		try {
			File sourceimage = new File(filename);
			image = ImageIO.read(sourceimage);          
		} catch (IOException ioe) {
			ioe.printStackTrace();
		}
	}

	JFrame overviewFrame;
	public void createOverview(){
		overview = new Overview(image, 100);
		
		overviewFrame = new JFrame();
		overviewFrame.add(overview);
		overviewFrame.setUndecorated(true);
		overviewFrame.setVisible(true);
		overviewFrame.setAlwaysOnTop(true);
		overviewFrame.setLocationRelativeTo(this);		
		overviewFrame.pack();
		
		overviewFrame.addKeyListener(this);
	}
	
	public Lens(Dimension size, String filename) {
		setDimension(size);
		setImage(filename);
	}

	public void move(int x, int y){
		this.x = x;
		this.y = y;
		repaint();
		
		if (overview != null)
			overview.repaint();
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
		
		Lens lens = new Lens();
		lens.show(size, "long.png");
		
		frame.addKeyListener(lens);	
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.add(lens);
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
		if (e.getKeyCode() == KeyEvent.VK_ESCAPE){
			overviewFrame.dispose();
			setVisible(false);
		}

		repaint();
		overview.repaint();
	}

	@Override
	public void keyReleased(KeyEvent e) {
	}

	@Override
	public void keyTyped(KeyEvent e) {

	}

}
