package history;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Rectangle;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseMotionAdapter;
import java.awt.image.BufferedImage;
import java.awt.image.RasterFormatException;
import java.io.File;
import java.io.IOException;
import java.util.ArrayList;

import javax.imageio.ImageIO;
import javax.swing.BorderFactory;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JLayeredPane;
import javax.swing.JPanel;
import javax.swing.JSeparator;
import javax.swing.JTextField;
import javax.swing.SwingConstants;
import javax.swing.border.LineBorder;

import sikuli.SikuliPane;
import sikuli.ImageButton;

public class HistoryViewer extends JPanel {
	
	private static final long serialVersionUID = 1L;

	Screen current_screen;
	
	JLayeredPane layeredPane;
	
	JPanel navigationControl;
	JPanel controls;
	JLabel time;
	
	boolean isBrowsing; 
	
	Mode current_mode = Mode.PRESENT;
	enum Mode{
		PRESENT,
		BROWSE,
		FIND,
		SELECT,
		READ
	};
	
	
	
	class FindResult {
		
		// a reversed chronologically ordered list of history screens
		ArrayList<HistoryScreen> history_screens = new ArrayList<HistoryScreen>();
		int current_index;
		
		public FindResult(String query_string){
			history_screens = HistoryScreenDatabase.find(query_string);
			current_index = 0;
		}
		
		public boolean hasBefore(){
			return current_index < history_screens.size() - 1;
		}
		
		public boolean hasAfter(){
			return current_index > 0;
		}
		
	
		public HistoryScreen getBefore(){
			current_index = current_index + 1;
			return history_screens.get(current_index);				
		}
		
		public HistoryScreen getAfter(){
			current_index = current_index - 1;
			return history_screens.get(current_index);	
		}
		
		public HistoryScreen getMostRecent(){
			return history_screens.get(0);
		}
		
		public ArrayList<HistoryScreen> getAll(){
			return history_screens;
		}
		
		
	};
	
	
	
	
	int history_screen_index = -1;
	
	JButton later;
	JButton earlier;
	JTextField input_query_string;
	Lense virtualPage;
	
	Screen present_screen;
	TriggerEditor trigger_editor;
	
	FindResult find_result;
	
	public HistoryViewer(){

		isBrowsing = false;
		
		layeredPane = new JLayeredPane();
		layeredPane.setPreferredSize(new Dimension(1152, 720));
		add(layeredPane);
	
		present_screen = new Screen("testdata/images/scenario-deadline-word.png"); 
		current_screen = present_screen;
		
		// create find control
		controls = new JPanel(new FlowLayout(FlowLayout.LEFT));
		controls.setBorder(new LineBorder(Color.black, 1));		
		controls.setBounds(20,645,1112,40);
		
		input_query_string = new JTextField(10);		
		
		JButton closeBtn = new JButton("X");
		closeBtn.setPreferredSize(new Dimension(20,20));
		closeBtn.addActionListener(new ActionListener(){
			@Override
			public void actionPerformed(ActionEvent e) {
//				sikuli.Screen scr = new sikuli.Screen();
//				sikuli.ScreenImage img = scr.userCapture();
			}
		});	
		
		JButton searchBtn = new JButton("Find");
		searchBtn.addActionListener(new ActionListener(){
			@Override
			public void actionPerformed(ActionEvent e) {
				String query_string = input_query_string.getText();
				doFind(query_string);
			}
		});
		
		JButton exitBtn = new JButton("Exit");
		exitBtn.addActionListener(new ActionListener(){
			@Override
			public void actionPerformed(ActionEvent e) {
				current_screen = present_screen;				
				current_mode = Mode.PRESENT;
				repaint();
			}

		
		});
		
		JButton browseBtn = new JButton("Browse");
		browseBtn.addActionListener(new ActionListener(){
			@Override
			public void actionPerformed(ActionEvent e) {
				
				current_mode = Mode.BROWSE;
				repaint();
				
			}			
		});
		
		JButton readBtn = new JButton("Read");
		readBtn.addActionListener(new ActionListener(){
			@Override
			public void actionPerformed(ActionEvent e) {
								
				if (virtualPage.isVisible()){				
					virtualPage.setVisible(false);
				}else{
					current_mode = Mode.READ;
					
					BufferedImage tracked = current_screen.crop(new Rectangle(370,200,200,100));
					ArrayList<Rectangle> rects = Sikuli.find("long.png", tracked);
					if (!rects.isEmpty()){
						
						Rectangle top = rects.get(0);
						virtualPage.move(top.x-370+20,top.y-200+96+5);
					}
				
					virtualPage.setVisible(true);
					virtualPage.requestFocus();
					
					Rectangle r = virtualPage.getBounds();
					r.grow(2,2);
					r.translate(0,4);
					current_screen.setHighlightRectangle(r);
				//updateUI();
				}
				
				repaint();
			}			
		});
		
		
		JButton scriptBtn = new JButton("Script");
		scriptBtn.addActionListener(new ActionListener(){
			@Override
			public void actionPerformed(ActionEvent e) {
				
				if (selected_image != null)
					trigger_editor = new TriggerEditor(selected_image);
				
			}			
		});
		
		
		earlier = new NavigationButton("Earlier", NavigationButton.LEFT);
		later   = new NavigationButton("Later", NavigationButton.RIGHT);

		

		
		
		time = new JLabel("-15:20");
		time.setOpaque(true);
		//Font font = new Font("sansserif", Font.BOLD, 32);
		//time.setFont(font);
		//time.setForeground(Color.red);
				
		controls.add(exitBtn);
		controls.add(input_query_string);
		controls.add(searchBtn);
		controls.add(new JSeparator(SwingConstants.HORIZONTAL));
		controls.add(earlier);
		controls.add(later);
		controls.add(time);
		controls.add(browseBtn);
		controls.add(readBtn);
		controls.add(scriptBtn);
		

		//controls.add(navigationControl);
		
		virtualPage = new Lense(new Dimension(963,518), "long.png");
		virtualPage.setBounds(20,96,963,518);
		virtualPage.setVisible(false);
		virtualPage.setFocusable(true);
		virtualPage.addKeyListener(virtualPage);
		//virtualPage.setBorder(BorderFactory.createEtchedBorder());
		//virtualPage.setBorder(BorderFactory.createEtchedBorder());
		
		
		
		layeredPane.add(virtualPage, new Integer(3));	
		layeredPane.add(controls,new Integer(2));
				
		addMouseListener(new MouseAdapter(){
	         public void mousePressed(java.awt.event.MouseEvent e){
	        	 
	        	 current_mode = Mode.SELECT;
	        	 
	            destx = srcx = e.getX();
	            desty = srcy = e.getY();
	            System.out.println("pressed " + srcx + "," + srcy);
	            
	            current_screen.setHighlightRectangle(new Rectangle(srcx,srcy,2,2));
	            
	            repaint();
	         }

	         public void mouseReleased(java.awt.event.MouseEvent e){
	        	 
	        	 current_mode = Mode.PRESENT;
	        	 
	        	System.out.println("selected " + srcx + "," + srcy
	        			+" - "+desty + "," + destx);
	        	
	        	if (destx > srcx + 2 && desty > srcy + 2){
	        	
	        		Rectangle selected_rectangle = new Rectangle(srcx,srcy,
	        				destx-srcx,desty-srcy);
	        	
	        		selected_image = current_screen.crop(selected_rectangle);
	            	
	            	if (trigger_editor != null && selected_image != null){
	            		trigger_editor.insertImage(selected_image);	   
	            	}
	            	
	        	}
//	            OCRDocument doc = new OCRDocument(selected_image);
//	            
//	            String query_string = null;
//	            for (String word : doc.getWords()){
//	            	System.out.println(word);
//	            	
//	            	if (query_string == null)
//	            		query_string = word;
//	            	else
//	            		query_string = query_string + " && " + word;
//	            }
	           
	            
	        	repaint();

	         }
		});
		
		addMouseMotionListener( new MouseMotionAdapter(){
	          public void mouseDragged(java.awt.event.MouseEvent e) {

	        	 destx = e.getX();
	             desty = e.getY();
	             
	             System.out.println("moved " + desty + "," + destx);
	             
	             if (destx > srcx + 2 && desty > srcy + 2){
	             	Rectangle selected_rectangle = new Rectangle(srcx,srcy,
		            		destx-srcx,desty-srcy);
	             
		            current_screen.setHighlightRectangle(selected_rectangle);
		            
	             }
		        
	             
	            repaint(); 
	          }
	       });
		
		


	
	}
	

	
	class NavigationButton extends JButton implements ActionListener {
		
		static public final int LEFT = 1;
		static public final int RIGHT = 2;
		
		int direction;
		
		public NavigationButton(String s, int direction){
			super(s);
			this.direction = direction;
			addActionListener(this);
		}

		
		public void actionPerformed(ActionEvent e) {


			if (current_mode == Mode.PRESENT) {
				HistoryScreen hs = HistoryScreenDatabase.getMostRecent();
				setHistoryScreen(hs);

				current_mode = Mode.BROWSE;					
				later.setEnabled(true);

			}else if (current_mode == Mode.BROWSE) {
				
				HistoryScreen hs;
				if (direction == LEFT){
					hs = ((HistoryScreen)current_screen).getBefore();
				}else{
					hs = ((HistoryScreen)current_screen).getAfter();
				}
				
				setHistoryScreen(hs);	
				

			}else if (current_mode == Mode.FIND) {
				
				HistoryScreen hs;
				if (direction == LEFT){
					hs = find_result.getBefore();
				}else{
					hs = find_result.getAfter();
				}
				
				setHistoryScreen(hs);

				later.setEnabled(find_result.hasAfter());
				earlier.setEnabled(find_result.hasBefore());

			}

		}	
		
		
	}

	private void showMatchViewer(FindResult find_result){
		
		MatchViewer mv = new MatchViewer();
		
		ArrayList<HistoryScreen> hss = find_result.getAll();
		int is[] = new int[]{0,1,6};
		
		//for (HistoryScreen hs : hss){
		for (int i : is){
			HistoryScreen hs = hss.get(i);
		
			BufferedImage screen = hs.getImage();
			
			hs.clearHighlightedRectangles();
			hs.addHighlightedImage(selected_image);
			hs.addHighlightedWord(input_query_string.getText());
			Rectangles rects = hs.getHighlightRectangles();
						
			ArrayList<BufferedImage> match_images = new ArrayList<BufferedImage>();
			for (Rectangle rect : rects){

				Rectangle r = new Rectangle(rect);
				//r.width += 300;
				//r.height += 20;
				
				int dh = 20;
				int dw = 100;
				r.x -= dw;
				r.y -= dh;
				r.width += 2*dw;
				r.height += 2*dh;
				
				Rectangle hl = new Rectangle(dw,dh,rect.width,rect.height);
				hl.x -=2;
				hl.y -=2;
				hl.width += 4;
				hl.height += 4;
				
				BufferedImage im = hs.crop(r,hl);
				match_images.add(im);
			}
			mv.addMatchGroup(screen, match_images, hs.getTimeString());
		}
	
		
		JFrame frame = new JFrame();
		frame.setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
		frame.add(mv);        
		frame.setSize(1000,600);
		frame.setLocationRelativeTo(null);
		frame.setVisible(true);
		
		//mv.updateUI();
	}
	
	private void doFind(String query_string){
		current_mode = Mode.FIND;
		
		if (selected_image != null){
			String ui_query_string = Sikuli.find_ui(selected_image); 
			query_string = query_string + " " + ui_query_string;
		}
		
		find_result = new FindResult(query_string);	
		setHistoryScreen(find_result.getMostRecent());
		
		
		
		
		//showMatchViewer(find_result);
		
		
		
		
		later.setEnabled(true);
		repaint();
	}
	
	BufferedImage selected_image;
	int srcx, srcy, destx, desty;
	
	private void setHistoryScreen(HistoryScreen hs){
		//time.setText(hs.getTimeString());
		
		if (current_mode == Mode.FIND){
			hs.clearHighlightedRectangles();
			hs.addHighlightedWord(input_query_string.getText());
			hs.addHighlightedImage(selected_image);
		}
						
		time.setText("" + hs.getId());
		current_screen = hs;
		repaint();
	}
		
	public void paintComponent(Graphics g) {
		
		super.paintComponent(g);
        Graphics2D g2d = (Graphics2D) g;

        
        current_screen.paintHelper(g, current_mode);
        
        if (selected_image != null){
        	
        	g2d.drawImage(selected_image, 0, 0, null);
        	
        }
    }
	
    public static void main(String[] args) {
    	JFrame frame = new JFrame();
    	HistoryViewer main =  new HistoryViewer();

        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.add(main);        
        frame.setSize(1152,720);
        frame.setLocationRelativeTo(null);
        frame.setVisible(true);
    }
}
