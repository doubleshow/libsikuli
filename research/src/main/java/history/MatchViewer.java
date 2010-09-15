package history;

import history.HistoryViewer.Mode;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.Image;
import java.awt.Rectangle;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import java.util.ArrayList;

import javax.imageio.ImageIO;
import javax.swing.BorderFactory;
import javax.swing.Box;
import javax.swing.BoxLayout;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;

public class MatchViewer extends JPanel{
	
	
	class Match extends JPanel {
		
		BufferedImage region_image;
		MatchGroup group;
		public Match(BufferedImage region_image){
			
			//setBackground(new Color(176,196,222));
			
			this.region_image = region_image;
			
			JPanel content = new JPanel();
			content.setLayout(new BoxLayout(content, BoxLayout.Y_AXIS));
			
			JLabel picLabel = new JLabel(new ImageIcon(region_image));
			content.add(picLabel);	
			
			add(content);
			
			content.setBorder(BorderFactory.createLineBorder( Color.black ));


			addMouseListener(new MatchMouseListener());
		}
		
		class MatchMouseListener extends MouseAdapter{
			public void mousePressed(MouseEvent e){
				System.out.println(getParent() + " " + region_image + " clicked");
			}		
		}
		
		
		
	}
	
	class MatchGroup extends JPanel{
		
		BufferedImage screen;
		String time;
		ArrayList<Match> matches = new ArrayList<Match>();
		
		public MatchGroup(BufferedImage screen, String time){

			this.time = time;
			this.screen = screen;
			
			setLayout(new BoxLayout(this, BoxLayout.X_AXIS));
			setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10));
			setBackground(Color.white);
			
			left = new JPanel();
			left.setLayout(new BoxLayout(left, BoxLayout.Y_AXIS));
			left.setAlignmentY(Box.TOP_ALIGNMENT);
			//left.setBackground(new Color(230,230,230));
			//left.setBackground(new Color(176,196,222));
			
			right = new JPanel();
			right.setLayout(new BoxLayout(right, BoxLayout.Y_AXIS));
			right.setAlignmentY(TOP_ALIGNMENT);
			

			
			add(left);
			add(right);
			
		}	
		
		
		JPanel right;
		JPanel left;
		public void addMatch(Match match){

			if (matches.size() == 0){
			

				Image newimg = screen.getScaledInstance(115, 72,  java.awt.Image.SCALE_SMOOTH);  
				ImageIcon newIcon = new ImageIcon(newimg);


				JLabel l = new JLabel(newIcon);
				l.setBorder(BorderFactory.createLineBorder( Color.black ));

				l.setAlignmentX(Box.CENTER_ALIGNMENT);

				left.add(l);
				
				JLabel t = new JLabel(time);
				t.setAlignmentX(Box.CENTER_ALIGNMENT);
				left.add(t);


			}
			
			
			matches.add(match);
			
			
			right.add(match);

		}
		
		public class MatchGroupMouseListener extends MouseAdapter{
			public void mousePressed(MouseEvent e){
				System.out.println("group clicked");
			}		
		}

	}
	
	
	public MatchViewer(){
		
		setLayout(new BoxLayout(this, BoxLayout.Y_AXIS));
	}
	
	public void addMatchGroup(BufferedImage screen, 
			ArrayList<BufferedImage> match_images,
			String time_string){
		
		MatchGroup mg = new MatchGroup(screen, time_string);
		
		for (BufferedImage match_image : match_images){
			mg.addMatch(new Match(match_image));
		}
		
		add(mg);	
	}
	
	public static void main(String[] args) {
		JFrame frame = new JFrame();
		MatchViewer main =  new MatchViewer();

		
		try {
			BufferedImage screen = ImageIO.read(new File("screen.png"));			
			BufferedImage image = ImageIO.read(new File("tile.png"));
			
			ArrayList<BufferedImage> images = new ArrayList<BufferedImage>();
			images.add(image);
			images.add(image);
			
			main.addMatchGroup(screen, images, "5 minutes ago");
			main.addMatchGroup(screen, images, "6 minutes ago");
			
		} catch (IOException e) {
		}
		
		
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.add(main);        
		frame.setSize(1000,600);
		frame.setLocationRelativeTo(null);
		frame.setVisible(true);
	}

}
