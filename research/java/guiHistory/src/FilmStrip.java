import java.awt.BasicStroke;
import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Image;
import java.awt.Insets;
import java.awt.Point;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.event.MouseMotionAdapter;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import javax.imageio.ImageIO;
import javax.swing.BorderFactory;
import javax.swing.BoxLayout;
import javax.swing.JButton;
import javax.swing.JComponent;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JLayeredPane;
import javax.swing.JPanel;

public class FilmStrip extends JPanel {
	
	
	private ArrayList filmTiles;
	
	JPanel film;
	JComponent marker;
	
	class Ruler extends JPanel{
		
		public Ruler(){
			setBorder(BorderFactory.createLineBorder(new Color(0,0,0),1));
			setPreferredSize(new Dimension(800,15));
			
		}
		
		public void paintComponent(Graphics g) {			
			super.paintComponent(g);
	        Graphics2D g2d = (Graphics2D) g;
			
	        int h = getHeight();
			int w = getWidth();
			
			for (int xpos = 0; xpos < w; xpos = xpos + 20){
				g2d.drawLine(xpos, 0, xpos, 2);
			}
			
	    }
	}
	
	public FilmStrip(){
	
		
		filmTiles = new ArrayList();        
		
		
		setLayout(new BorderLayout());
		
		JLayeredPane layerPane = new JLayeredPane();
		//layerPane.setPreferredSize(new Dimension(300,300));
		
		
		add(layerPane, BorderLayout.CENTER);
		
		
		JPanel p = new JPanel();
		p.setLayout(new BorderLayout());
		
		Ruler ruler = new Ruler();
		p.add(ruler, BorderLayout.NORTH);
		
		film = new JPanel();
		film.setLayout(new BoxLayout(film, BoxLayout.X_AXIS));
		p.add(film);
		
		p.setBounds(0,0,800,300);
		
		layerPane.add(p,new Integer(1));
		
		
		marker = new JPanel();
		marker.setBounds(0,0,4,100);
		marker.setBackground(Color.red);
		
		layerPane.add(marker,new Integer(2));
		
		setBackground(new Color(90,90,90));
		setBorder(BorderFactory.createEmptyBorder(2,0,2,0));
		
		
		layerPane.addMouseMotionListener(new MouseMotionAdapter(){
			public void mouseMoved(MouseEvent e){
				System.out.println("x1 = " + e.getX());
				Point p = marker.getLocation();
	  	 		marker.setLocation(e.getX(),(int) p.getY());
			}
		});
		
		
		

	

	
		
	}
	
	public void moveMarker(int xpos){
		Point p = marker.getLocation();
	 	marker.setLocation(xpos,(int) p.getY());		
	}
	

	private int total_duration = 0;
	public void addTile(ShotTile tile){
		film.add(tile);
		tile.addEventListener(new MyShotTileEventListener());
		total_duration = total_duration + tile.getShot().getDuration();
	}
	
	
	class MyShotTileEventListener implements ShotTileEventListener{
		public void shotSelected(ShotTileEvent event){
		}

		public void markerMoved(ShotTileEvent event) {
			int xpos = event.timestamp * film.getWidth() / total_duration;
			moveMarker(xpos);
		}
			
	}
	
	public void paintComponent(Graphics g) {
		super.paintComponent(g);
    }

}

