package history;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Point;
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
import javax.swing.BoxLayout;
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

	HistoryScreenImage screen;
	
	//Screen history_screen;
	
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
	
	
	
	int history_screen_index = -1;
	

	JTextField input_query_string;
	Lense virtualPage;
	
	ScreenImage present_screen;
	TriggerEditor trigger_editor;
	
	FindResult find_result;
	
	public HistoryViewer(){

		isBrowsing = false;
		
		layeredPane = new JLayeredPane();
		layeredPane.setPreferredSize(new Dimension(1152, 720));
		add(layeredPane);
	
		present_screen = new ScreenImage("testdata/images/scenario-deadline-word.png"); 

		createControls();
			
		virtualPage = new Lense(new Dimension(963,518), "long.png");
		virtualPage.setBounds(20,96,959,512);
		virtualPage.setVisible(false);
		virtualPage.setFocusable(true);
		virtualPage.addKeyListener(virtualPage);

		
		layeredPane.add(virtualPage, new Integer(3));	
				
		addMouseListener(new MouseAdapter(){
	         public void mousePressed(java.awt.event.MouseEvent e){
	        	 
	        	 current_mode = Mode.SELECT;
	        	 
	            destx = srcx = e.getX();
	            desty = srcy = e.getY();
	            System.out.println("pressed " + srcx + "," + srcy);
	            
	            screen.setHighlightRectangle(new Rectangle(srcx,srcy,2,2));
	            
	            repaint();
	         }

	         public void mouseReleased(java.awt.event.MouseEvent e){
	        	 
	        	 current_mode = Mode.PRESENT;
	        	 
	        	System.out.println("selected " + srcx + "," + srcy
	        			+" - "+desty + "," + destx);
	        	
	        	if (destx > srcx + 2 && desty > srcy + 2){
	        	
	        		Rectangle selected_rectangle = new Rectangle(srcx,srcy,
	        				destx-srcx,desty-srcy);
	        	
	        		selected_image = screen.crop(selected_rectangle);
	            	
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
	             
		            screen.setHighlightRectangle(selected_rectangle);
		            
	             }
		        
	             
	            repaint(); 
	          }
	       });
		
		
		
		//testDiff();
		//testMove();
		//testPilyoung();
		
		current_mode = Mode.FIND;
		input_query_string.setText("deadline");
		
		//find_result = new FindResult("deadline");
		
		find_result = FindResult.createMockResult();
		
		setHistoryScreen(find_result.getMostRecent());
		
		navigator.setList(find_result);
//		earlier.setList(find_result);
//		later.setList(find_result);
		
		
	}
	
	interface NavigationList{
		boolean hasBefore();
		boolean hasAfter();	
		Object getBefore();
		Object getAfter();
	}

	NavigationControl navigator;
	class NavigationControl extends JPanel {
		
		static public final int LEFT = 1;
		static public final int RIGHT = 2;
		
		
		NavigationList list;
		public void setList(NavigationList list){
			this.list = list;
			
			updateButtons();
		}
		
		private void updateButtons(){
			before.setEnabled(list.hasBefore());
			after.setEnabled(list.hasAfter());
		}
		
		JButton before;
		JButton after;		
		public NavigationControl(){
			
			before = new JButton("before");
			before.addActionListener(new NavigationActionListener(LEFT));
			
			after = new JButton("after");
			after.addActionListener(new NavigationActionListener(RIGHT));
			
			add(before);
			add(after);
		}
			
			
		class NavigationActionListener implements ActionListener{
			
			int direction;
			
			public NavigationActionListener(int direction){
				super();
				this.direction = direction;
			}
			
			@Override
			public void actionPerformed(ActionEvent e) {

				HistoryScreen hs;
				if (direction == LEFT){
					hs = (HistoryScreen) list.getBefore();
				}else{
					hs = (HistoryScreen) list.getAfter();
				}
				
				setHistoryScreen(hs);
			
				
				updateButtons();
			}
						
		}
		
		
	}
	

	
	private void createControls(){
		
		// create find control

		//controls.setBorder(new LineBorder(Color.black, 1));		
		//controls.setBounds(20,645,1112,40);
		//controls.setBounds(20,645,500,40);

		input_query_string = new JTextField(10);		

		JButton closeBtn = new JButton("X");
		closeBtn.setPreferredSize(new Dimension(20,20));
		closeBtn.addActionListener(new ActionListener(){
			@Override
			public void actionPerformed(ActionEvent e) {
				//		sikuli.Screen scr = new sikuli.Screen();
				//		sikuli.ScreenImage img = scr.userCapture();
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
				//current_screen = present_screen;				
				current_mode = Mode.PRESENT;
				repaint();
			}


		});

		JButton browseBtn = new JButton("Browse");
		browseBtn.addActionListener(new ActionListener(){
			@Override
			public void actionPerformed(ActionEvent e) {

				current_mode = Mode.BROWSE;

//				if (! (current_screen instanceof HistoryScreen)){
//					current_screen = HistoryScreenDatabase.getMostRecent();
//				}

//				earlier.setEnabled(true);
//				later.setEnabled(true);				

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

					BufferedImage tracked = screen.crop(new Rectangle(370,200,200,100));
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
					screen.setHighlightRectangle(r);
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

		navigator = new NavigationControl();


		time = new JLabel("-15:20");
		time.setOpaque(true);
		//Font font = new Font("sansserif", Font.BOLD, 32);
		//time.setFont(font);
		//time.setForeground(Color.red);

		boolean search_only = true;

		controls = new JPanel();
		controls.setLayout(new BoxLayout(controls, BoxLayout.X_AXIS));

		//controls.add(exitBtn);
		controls.add(input_query_string);
		controls.add(searchBtn);
		controls.add(new JSeparator(SwingConstants.VERTICAL));
		controls.add(navigator);
//		controls.add(new JSeparator(SwingConstants.VERTICAL));		
//		controls.add(scriptBtn);
		//controls.add(readBtn);
		//controls.add(browseBtn);

		if (! search_only){

			controls.add(time);
			controls.add(browseBtn);

			controls.add(scriptBtn);
		}
		controls.setPreferredSize(new Dimension(500,60));

	}



	public void displayControls(){
		JFrame controlsFrame = new JFrame();
		controlsFrame.add(controls);
		controlsFrame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		controlsFrame.setMinimumSize(new Dimension(480,50));
		controlsFrame.setLocationRelativeTo(this);
		controlsFrame.setVisible(true);
		//controlsFrame.setLocation(10,10);
		controlsFrame.toFront();
	}
	
	
	private void showMatchViewer(FindResult find_result){
		if (true){
			return;
		}
		
		MatchViewer mv = new MatchViewer();
		
		ArrayList<HistoryScreen> hss = find_result.getAll();
		int is[] = new int[]{0,1,6,7,8};
		//int is[] = new int[]{0,3,5,6};//,6,7,8};
		
		//for (HistoryScreen hs : hss){
		for (int i : is){
			HistoryScreen hs = hss.get(i);
		
			
			HistoryScreenImage im = hs.createImage();
			BufferedImage screen_image = im.getImage();
			
			im.clearHighlightedRectangles();
			//hs.addHighlightedImage(selected_image);
			im.addHighlightedWord(input_query_string.getText());
			Rectangles rects = im.getHighlightRectangles();
						
			ArrayList<BufferedImage> match_images = new ArrayList<BufferedImage>();
			for (Rectangle rect : rects){

				Rectangle r = new Rectangle(rect);
				int dw = 300;
				int dh = 20;
				r.width += dw;
				r.height += dh;
				r.x -= 4;
				r.y -= 4;
				Rectangle hl = new Rectangle(4,4,rect.width,rect.height);
//				int dh = 20;
//				int dw = 100;
//				r.x -= dw;
//				r.y -= dh;
//				r.width += 2*dw;
//				r.height += 2*dh;
				
				//Rectangle hl = new Rectangle(dw,dh,rect.width,rect.height);
				hl.x -=2;
				hl.y -=2;
				hl.width += 4;
				hl.height += 4;
				
				BufferedImage cim = im.crop(r,hl);
				match_images.add(cim);
			}
			mv.addMatchGroup(screen_image, match_images, hs.getTimeString());
		}
	
		
		JFrame frame = new JFrame();
		frame.setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
		frame.add(mv);        
		frame.setSize(1000,600);
		frame.setLocationRelativeTo(null);
		frame.setVisible(true);
		
		
		
		
	
		//mv.updateUI();
	}
	
	
	private void testPilyoung(){
		current_mode = Mode.BROWSE;
		HistoryScreen h1 = HistoryScreenDatabase.getMostRecent();
	//	this.setHistoryScreen(h1);
		displayControls();
	}
	
	
	private void testMove(){
		current_mode = Mode.READ;
		HistoryScreen h1 = HistoryScreenDatabase.getMostRecent();
		//HistoryScreen h2 = h1.getBefore();
		//HistoryScreen h3 = h2.getBefore();
		
//		h2.setHighlightRectanglePadding(3);
//		h2.setHighlightRectangleBorder(false);
//		//h2.highlightAllWords();
		
		//h1.highlightSameWords(h3,h2);
	//	h2.highlightSameWords(h3,null);//,h2);
		
		
		//this.setHistoryScreen(h2);
	}
	
	private void testDiff(){
		current_mode = Mode.READ;
		HistoryScreen h1 = HistoryScreenDatabase.getMostRecent();
//		HistoryScreen h2 = h1.getBefore();
		
//		h2.setHighlightRectanglePadding(3);
//		h2.setHighlightRectangleBorder(false);
//		//h2.highlightAllWords();
//		
//		h2.highlightNewWords(h1);
//		
		//this.setHistoryScreen(h2);
	}
	
	private void doFind(String query_string){
		current_mode = Mode.FIND;
		
		if (selected_image != null){
			String ui_query_string = Sikuli.find_ui(selected_image); 
			query_string = ui_query_string;
		}
		
		find_result = new FindResult(query_string);	
		//setHistoryScreen(find_result.getMostRecent());
		
		
		
		
		//showMatchViewer(find_result);
		
		
		

		repaint();
	}
	
	BufferedImage selected_image;
	int srcx, srcy, destx, desty;
	
	private void setHistoryScreen(HistoryScreen hs){
		
		screen = hs.createImage();
		
		//if (current_mode == Mode.FIND){
		//hs.clearHighlightedRectangles();
		
		String query_text = input_query_string.getText();
		String words[] = query_text.split(" ");

		for (String word : words){
			screen.addHighlightedWord(word);
		}

		//hs.addHighlightedImage(selected_image);
		//}


		screen.addAnnotationArrow(new Point(10,10), new Point(10,500), Color.red);

		repaint();
	}
		
	public void paintComponent(Graphics g) {
		
		super.paintComponent(g);
        Graphics2D g2d = (Graphics2D) g;

        if (screen != null)
        
        	screen.paintHelper(g, current_mode);
        
        if (selected_image != null){
        	
        //	g2d.drawImage(selected_image, 0, 0, null);
        	
        }
    }
	
    public static void main(String[] args) {
    	JFrame frame = new JFrame();
    	HistoryViewer main =  new HistoryViewer();

        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.add(main);        
        frame.setSize(1152,740);
        frame.setLocationRelativeTo(null);
        frame.setVisible(true);
        main.displayControls();
    }
}
