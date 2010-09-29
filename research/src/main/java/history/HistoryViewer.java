package history;
import history.HistoryScreenDatabase.HistoryScreenIterator;
import history.OCRDocument.OCRWord;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Image;
import java.awt.Point;
import java.awt.Rectangle;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionAdapter;
import java.awt.event.MouseMotionListener;
import java.awt.image.BufferedImage;
import java.awt.image.RasterFormatException;
import java.io.File;
import java.io.IOException;
import java.util.ArrayList;

import javax.imageio.ImageIO;
import javax.swing.BorderFactory;
import javax.swing.BoxLayout;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JLayeredPane;
import javax.swing.JPanel;
import javax.swing.JSeparator;
import javax.swing.JTextField;
import javax.swing.SwingConstants;
import javax.swing.border.LineBorder;

import sikuli.Clipboard;
import sikuli.SikuliPane;
import sikuli.ImageButton;

public class HistoryViewer extends JPanel {
	
	private static final long serialVersionUID = 1L;

	HistoryScreenImage screen;
	
	//Screen history_screen;
	
	JLayeredPane layeredPane;
	

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
	

	Lense virtualPage;
	
	ScreenImage present_screen;
	ScriptEditor trigger_editor;
	
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
				
	
		//testDiff();
		//testMove();
		//testPilyoung();
		
		
		current_mode = Mode.BROWSE;
		setHistoryScreen(HistoryScreenDatabase.getMostRecent());
		//NavigationIterator iter = HistoryScreenDatabase.getIterator(0);
		NavigationIterator iter = HistoryScreenDatabase.getIterator(10);
		navigator.setIterator(iter);
		navigator.setListener(new HistoryScreenNavigationListener());
		
		
		navigator.play();
		
		
		
//		
//		
//		if (false){
		
//		current_mode = Mode.FIND;
//		//input_query_string.setText("deadline");
//		//find_result = new FindResult("deadline");		
//		find_result = FindResult.createMockResult();
//		setHistoryScreen(find_result.getMostRecent());	
//		navigator.setIterator(find_result);
		
		//}
		
		
	
		selector = new RegionSelector();
		
		
	}
	
	RegionSelector selector;
	
	
	interface RegionSelectorListener{
		public void rectangleSelected(Rectangle rectangle);
	}

	class RegionSelector implements MouseListener, MouseMotionListener{

		
		int srcx, srcy, destx, desty;

		Rectangle selected_rectangle;
		
		RegionSelectorListener listener;
		
		public void setListener(RegionSelectorListener listener) {
			this.listener = listener;
		}

		public Rectangle getSelectedRectangle(){
			return selected_rectangle;
		}
		
		public void mousePressed(java.awt.event.MouseEvent e){

			current_mode = Mode.SELECT;

			destx = srcx = e.getX();
			desty = srcy = e.getY();
	//		System.out.println("pressed " + srcx + "," + srcy);

			screen.addHighlight(new Rectangle(srcx,srcy,2,2));
			repaint();
		}

		public void mouseReleased(java.awt.event.MouseEvent e){

			current_mode = Mode.PRESENT;

			System.out.println("selected " + srcx + "," + srcy
					+" - "+desty + "," + destx);

			if (destx > srcx + 2 && desty > srcy + 2){

				selected_rectangle = new Rectangle(srcx,srcy,
						destx-srcx,desty-srcy);

				removeMouseListener(selector);
				removeMouseMotionListener(selector);
				controlsFrame.setVisible(true);
				repaint();
				listener.rectangleSelected(selected_rectangle);
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


			//repaint();

		}

		public void mouseDragged(java.awt.event.MouseEvent e) {

			destx = e.getX();
			desty = e.getY();

			System.out.println("moved " + desty + "," + destx);

			if (destx > srcx + 2 && desty > srcy + 2){
				Rectangle selected_rectangle = new Rectangle(srcx,srcy,
						destx-srcx,desty-srcy);

				screen.clear();
				screen.addHighlight(selected_rectangle);

			}


			repaint(); 
		}

		@Override
		public void mouseClicked(MouseEvent e) {
		}

		@Override
		public void mouseEntered(MouseEvent e) {
		}

		@Override
		public void mouseExited(MouseEvent e) {
		}

		@Override
		public void mouseMoved(MouseEvent e) {
		}

		public void start() {
			current_mode = Mode.SELECT;
			addMouseListener(selector);
			addMouseMotionListener(selector);
			controlsFrame.setVisible(false);
			screen.clear();
			repaint();
		}
	}
	
	class HistoryScreenNavigationListener implements NavigationListener{
		@Override
		public void itemSelected(Object item) {
			HistoryScreen hs = (HistoryScreen) item;
			setHistoryScreen(hs);	
		}
	}
	
	
	interface NavigationIterator{
		boolean hasBefore();
		boolean hasAfter();	
		Object getBefore();
		Object getAfter();
	}
	
	interface NavigationListener{
		public void itemSelected(Object item);
	}

	NavigationControl navigator;
	class NavigationControl extends JPanel {
		
		static public final int LEFT = 1;
		static public final int RIGHT = 2;
		
		
		NavigationIterator iterator;
		NavigationListener listener;
		
		public void setIterator(NavigationIterator list){
			this.iterator = list;
			
			updateButtons();
		}
		
		public void setListener(NavigationListener listener){
			this.listener = listener;
		}
		
		private void updateButtons(){
			before.setEnabled(iterator.hasBefore());
			after.setEnabled(iterator.hasAfter());
			
			if (playing){
				play.setText("Stop");
			}else{
				play.setText("Play");
				play.setEnabled(iterator.hasAfter());
			}
		}
		
		JButton before;
		JButton after;		
		JButton play;
		public NavigationControl(){
			
			before = new JButton("before");
			before.addActionListener(new NavigationActionListener(LEFT));
			
			after = new JButton("after");
			after.addActionListener(new NavigationActionListener(RIGHT));
			
			play = new JButton("play");
			play.addActionListener(new ActionListener(){
				@Override
				public void actionPerformed(ActionEvent e) {
					if (playing)
						navigator.stop();
					else
						navigator.play();
					updateButtons();		
				}			
			});
			
			add(before);
			add(after);
			add(play);
		}
		
		
		boolean playing = false;
		public void play(){
			
			Thread autoplayThread = new Thread(){
				public void run() {
					
					playing = true;
					while (playing){
						try {

							if (iterator.hasAfter()){
								stepForward();
								Thread.sleep(100);
							}else{
								playing = false;
							}							
					
						} catch (InterruptedException e) {
						}
					}
					
					updateButtons();
				}
			};
			
			autoplayThread.start();
		}
		
		
		public void stop() {
			playing = false;
		}
		
		public void stepForward(){
			Object item = iterator.getAfter();
			listener.itemSelected(item);
			updateButtons();
		}
		
		public void stepBackward(){
			Object item = iterator.getBefore();
			listener.itemSelected(item);
			updateButtons();			
		}
			
		class NavigationActionListener implements ActionListener{
			
			int direction;			
			public NavigationActionListener(int direction){
				super();
				this.direction = direction;
			}
			
			@Override
			public void actionPerformed(ActionEvent e) {
				if (direction == LEFT){
					stepBackward();
				}else{
					stepForward();
				}
			}	
		}


	}
	
	public void doCaptureScriptPatternImage(){
		
		selector.start();
		selector.setListener(new RegionSelectorListener(){

			@Override
			public void rectangleSelected(Rectangle rectangle) {
				
				BufferedImage selected_image = screen.crop(rectangle);
				trigger_editor.insertImage(selected_image);
				trigger_editor.setVisible(true);
			}
			
		});	
		
		
	}
	
	private void doCaptureScriptTargetImage(){
		final HistoryViewer viewer = this;
		
		selector.start();
		selector.setListener(new RegionSelectorListener(){

			@Override
			public void rectangleSelected(Rectangle rectangle) {
				
				BufferedImage selected_image = screen.crop(rectangle);
				trigger_editor = new ScriptEditor(viewer,selected_image);
				
			}
			
		});	
	}
	
	private void doCaptureQueryImage(){

		selector.start();
		selector.setListener(new RegionSelectorListener(){
			
			@Override
			public void rectangleSelected(Rectangle rectangle) {
				BufferedImage newimg = screen.crop(rectangle);
				findBox.setImage(newimg);			
				current_mode = Mode.BROWSE;
			}			
			
		});
	}
	
	private void doCaptureCopyText(){
		
		selector.start();
		selector.setListener(new RegionSelectorListener(){

			@Override
			public void rectangleSelected(Rectangle rectangle) {
				BufferedImage image = screen.crop(rectangle);
				String ocr_string = Sikuli.ocr(image);
				
				OCRDocument ocr = new OCRDocument(image);
				
				
				String ocr_text = "";
				for (OCRWord ocrword : ocr.getWords()){			
					ocr_text = ocr_text + ocrword.getString() + " ";
				}
				
				 Clipboard.putText(Clipboard.PLAIN, Clipboard.UTF8, 
                         Clipboard.BYTE_BUFFER, ocr_text);

			}
			
		});
	}

//	private void endRegionSelection(){
//
//		Rectangle selected_rectangle = selector.getSelectedRectangle();
//		BufferedImage newimg = screen.crop(selected_rectangle);
//		
//		findBox.setImage(newimg);
//		
//		current_mode = Mode.BROWSE;
//		
//	}
	
	
	
	class FindBox extends JPanel{
		
		JTextField queryTextField;
		JButton searchBtn;
		JButton captureBtn;
		
		AutoResize imagePanel;
		
		public void setImage(BufferedImage image){
			imagePanel.setImage(image);
			updateUI();
		}
		
		public FindBox(){
			super();
			
			queryTextField = new JTextField(10);		

			captureBtn = new JButton("Capture");
			captureBtn.addActionListener(new ActionListener(){
				@Override
				public void actionPerformed(ActionEvent e) {					
					doCaptureQueryImage();
				}	
			});
			
			searchBtn = new JButton("Find");
			searchBtn.addActionListener(new ActionListener(){
				@Override
				public void actionPerformed(ActionEvent e) {
					doFind();
				}
			});
			
			add(queryTextField);
			add(searchBtn);
			add(captureBtn);
			
			imagePanel = new AutoResize(null);
			imagePanel.setMinimumSize(new Dimension(200,50));
			imagePanel.setPreferredSize(new Dimension(200,50));
			
			add(imagePanel);
			
			
			//queryTextField.setText("chi");
			queryTextField.setText("facebook");
			
		}
		
		
		public String getQueryString(){
			return queryTextField.getText();
		}
		
		public BufferedImage getQueryImage(){
			return imagePanel.getImage();
		}
		
	}

	FindBox findBox;
	
	private void createControls(){
		
		findBox = new FindBox();

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
				int id = history_screen.getId();
				HistoryScreenIterator iter = HistoryScreenDatabase.getIterator(id);
				navigator.setIterator(iter);
				repaint();
			}			
		});



		
		JButton readBtn = new JButton("Read");
		readBtn.addActionListener(new ActionListener(){
			@Override
			public void actionPerformed(ActionEvent e) {
				current_mode = Mode.READ;
				repaint();
			}			
		});
		
		JButton vreadBtn = new JButton("Read");
		vreadBtn.addActionListener(new ActionListener(){
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

				doCaptureScriptTargetImage();
			}			
		});
		
		
		JButton copyBtn = new JButton("Copy");
		copyBtn.addActionListener(new ActionListener(){
			@Override
			public void actionPerformed(ActionEvent e) {
				doCaptureCopyText();
			}			
		});
		


		navigator = new NavigationControl();


		time = new JLabel("-15:20");
		time.setOpaque(true);
		//Font font = new Font("sansserif", Font.BOLD, 32);
		//time.setFont(font);
		//time.setForeground(Color.red);

		controls = new JPanel();
		controls.setLayout(new GridBagLayout());
		GridBagConstraints c = new GridBagConstraints();
		
		c.fill = GridBagConstraints.HORIZONTAL;
		c.gridx = 0;
		c.gridy = 0;
		c.gridwidth = 4;
		controls.add(findBox,c);
		
		c.fill = GridBagConstraints.HORIZONTAL;
		c.gridx = 6;
		c.gridy = 0;
		c.gridwidth = 1;
		controls.add(navigator,c);
		
		
		c.gridx = 0;
		c.gridy = 1;
		controls.add(scriptBtn,c);
		c.gridx = 1;
		c.gridy = 1;
		controls.add(readBtn,c);
		c.gridx = 2;
		c.gridy = 1;
		controls.add(browseBtn,c);
		c.gridx = 3;
		c.gridy = 1;
		controls.add(copyBtn,c);	
		c.gridx = 4;
		c.gridy = 1;
		controls.add(copyBtn,c);
	}


	JFrame controlsFrame;
	public void displayControls(){
		controlsFrame = new JFrame();
		controlsFrame.add(controls);
		controlsFrame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		controlsFrame.setMinimumSize(new Dimension(900,120));
		controlsFrame.setLocationRelativeTo(this);
		controlsFrame.setVisible(true);
		controlsFrame.toFront();
	}
	
	
	private void showMatchViewer(FindResult find_result){

		MatchViewer mv = new MatchViewer();
		mv.setListener(new NavigationListener(){

			@Override
			public void itemSelected(Object item) {
				HistoryScreen hs = (HistoryScreen) item;
				setHistoryScreen(hs);		
			}
			
		});
		
		// define the context around each match to display
		
		Dimension context_dimension = new Dimension(300,100);
		Point context_offset = new Point(0,0);
		
		ArrayList<HistoryScreen> hss = find_result.getAll();
		int is[] = new int[]{0,1,2,3};
		
		
		int match_count = 0;
		for (HistoryScreen hs : hss){
			HistoryScreenImage hs_image = hs.createImage();
					
			String word = findBox.getQueryString();			
			Rectangles rects = HistoryScreenDatabase.findRectangles(hs.getId(), word);
			
			BufferedImage query_image = findBox.getQueryImage();
			if (query_image != null){
				ArrayList<Rectangle> image_rects = hs_image.find(query_image);
				rects.addAll(image_rects);
			}
			
			// extract a context image for each matched word
			ArrayList<BufferedImage> match_images = new ArrayList<BufferedImage>();
			for (Rectangle rect : rects){

				// calculate the rectangle to crop from the screen image
				// by default, centered on the matched rectangle
				
				int ch = (context_dimension.height - rect.height)/2;
				int cw = (context_dimension.width - rect.width)/2;
				Rectangle context_rectangle = new Rectangle(context_dimension);
				context_rectangle.setLocation(rect.getLocation());
				context_rectangle.translate(-cw,-ch);
				
				// calculate the location of the highlight within the context image
				Rectangle hl = new Rectangle(rect.getSize());
				hl.setLocation(cw,ch);
				
				ScreenImage context_image = hs_image.crop0(context_rectangle);
				context_image.addHighlight(hl);
				
				BufferedImage match_image = context_image.createRenderedImage(Mode.FIND);
	
				match_images.add(match_image);
				
				match_count++;
				if (match_count > 4)
					break;		
			}
			
			mv.addMatchGroup(hs, match_images, hs.getTimeString());
			

			if (match_count > 4)
				break;		
		}
	
		
		JFrame frame = new JFrame();
		frame.setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
		frame.add(mv);        
		frame.setSize(1000,600);
		frame.setLocationRelativeTo(null);
		frame.setVisible(true);
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
	
	private void doFind(){
		current_mode = Mode.FIND;
		
		String query_string = findBox.getQueryString();
		BufferedImage query_image = findBox.getQueryImage();

		if (query_image != null){
			String ui_query_string = Sikuli.find_ui(query_image); 
			
			if (query_string.length()>0)
				query_string += " AND " + ui_query_string;
			else
				query_string = ui_query_string;
		}

		if (query_string.length() == 0){
			return;
		}
		
		find_result = new FindResult(query_string);

		if (find_result.isEmpty()){
			return;
		}
		
		
		setHistoryScreen(find_result.getMostRecent());			
		navigator.setIterator(find_result);
		showMatchViewer(find_result);
		
		repaint();
	}
	


	private HistoryScreen history_screen;
	
	private void setHistoryScreen(HistoryScreen hs){
		
		history_screen = hs;
		screen = hs.createImage();
		
		if (current_mode == Mode.FIND){
		
			String query_text = findBox.getQueryString();
			String words[] = query_text.split(" ");

			for (String word : words){
				screen.addHighlightedWord(word);
			}

			BufferedImage query_image = findBox.getQueryImage();
			if (query_image != null)
				screen.addHighlightedImage(query_image);
		}

		repaint();
	}
		
	public void paintComponent(Graphics g) {
		
		super.paintComponent(g);
        Graphics2D g2d = (Graphics2D) g;
        screen.paintHelper(g, current_mode);
       
    }
	
    public static void main(String[] args) {
    	JFrame frame = new JFrame();
    	HistoryViewer main =  new HistoryViewer();

        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.add(main);        
        frame.setSize(1152,740);
        frame.setLocationRelativeTo(null);
        frame.setVisible(true);
        frame.setResizable(false);
        main.displayControls();
    }
}
