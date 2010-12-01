package history;
import history.HistoryScreenDatabase.HistoryScreenIterator;
import history.OCRDocument.OCRWord;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.Font;
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
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.awt.image.BufferedImage;
import java.awt.image.RasterFormatException;
import java.io.File;
import java.io.IOException;
import java.util.ArrayList;

import javax.imageio.ImageIO;
import javax.swing.BorderFactory;
import javax.swing.Box;
import javax.swing.BoxLayout;
import javax.swing.ButtonGroup;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JLayeredPane;
import javax.swing.JPanel;
import javax.swing.JRadioButton;
import javax.swing.JSeparator;
import javax.swing.JTextField;
import javax.swing.JToggleButton;
import javax.swing.SwingConstants;
import javax.swing.UIManager;
import javax.swing.border.LineBorder;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;
import javax.swing.plaf.FontUIResource;

import sikuli.Clipboard;
import sikuli.SikuliPane;
import sikuli.ImageButton;

public class HistoryViewer extends JPanel {
	
	private static final long serialVersionUID = 1L;

	HistoryScreenImage screen;
	
	//Screen history_screen;
	
	JLayeredPane layeredPane;
	

	JPanel controls;

	
	boolean isBrowsing; 
	
	Mode current_mode = Mode.PRESENT;
	enum Mode{
		PRESENT,
		BROWSE,
		FIND,
		SELECT,
		READ,
		ABSTRACT,
		OCR,
		DIFF
	};
	
	
	
	int history_screen_index = -1;
	

	Lens virtualPage;
	
	ScreenImage present_screen;
	ScriptEditor trigger_editor;
	
	FindResult find_result;
	
	public HistoryViewer(){
		
//		UIManager.put("Label.font", new FontUIResource("Lucida Grande", Font.PLAIN, 10));
//		UIManager.put("Button.font", new FontUIResource("Lucida Grande", Font.PLAIN, 10));
//		UIManager.put("ToggleButton.font", new FontUIResource("Lucida Grande", Font.PLAIN, 10));

		isBrowsing = false;
		
		layeredPane = new JLayeredPane();
		layeredPane.setPreferredSize(new Dimension(1152, 720));
		add(layeredPane);
	
		present_screen = new ScreenImage("testdata/images/scenario-deadline-word.png"); 

		createControls();
			


		virtualPage = new Lens();
		virtualPage.setVisible(false);
		virtualPage.setFocusable(true);		
		layeredPane.add(virtualPage, new Integer(3));	
				
		selector = new RegionSelector();
	
		current_mode = Mode.BROWSE;
		navigator.setListener(new HistoryScreenNavigationListener());

		//setExample("facebook");		
		//setExample("inbox");	
		setExample("chi");	
		
		
		
	}
	
	RegionSelector selector;
	
	
	interface RegionSelectorListener{
		public void rectangleSelected(Rectangle rectangle);
	}

	class RegionSelector implements MouseListener, MouseMotionListener{

		
		int srcx, srcy, destx, desty;

		Rectangle selected_rectangle;
		
		RegionSelectorListener listener;
		
		Mode saved_mode;
		
		public void setListener(RegionSelectorListener listener) {
			this.listener = listener;
		}

		public Rectangle getSelectedRectangle(){
			return selected_rectangle;
		}
		
		public void mousePressed(java.awt.event.MouseEvent e){



			destx = srcx = e.getX();
			desty = srcy = e.getY();
	//		System.out.println("pressed " + srcx + "," + srcy);

			screen.addHighlight(new Rectangle(srcx,srcy,2,2));
			repaint();
		}

		public void mouseReleased(java.awt.event.MouseEvent e){

			current_mode = saved_mode;

//			System.out.println("selected " + srcx + "," + srcy
//					+" - "+desty + "," + destx);

			if (destx > srcx + 2 && desty > srcy + 2){

				selected_rectangle = new Rectangle(srcx,srcy,
						destx-srcx,desty-srcy);

				removeMouseListener(selector);
				removeMouseMotionListener(selector);
				controlsFrame.setVisible(true);
				repaint();
				listener.rectangleSelected(selected_rectangle);
			}
		}

		public void mouseDragged(java.awt.event.MouseEvent e) {

			destx = e.getX();
			desty = e.getY();

			//System.out.println("moved " + desty + "," + destx);

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
			saved_mode = current_mode;
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
		Object getCurrent();
		boolean hasAfter();	
		Object getBefore();
		Object getAfter();
		Object get(int i);
	}
	
	interface NavigationListener{
		public void itemSelected(Object item);
	}

	NavigationControl navigator;
	class NavigationControl extends Box {
		
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
			
			
			if (playing){

				backwardBtn.setEnabled(false);
				forwardBtn.setEnabled(false);
				
				if (direction == RIGHT){
					playBtn.setText("Stop");
					rewindBtn.setEnabled(false);
				
				}else{
					rewindBtn.setText("Stop");
					playBtn.setEnabled(false);
				}
			
			}else{
				
				backwardBtn.setEnabled(iterator.hasBefore());
				forwardBtn.setEnabled(iterator.hasAfter());
				
				playBtn.setText("Play >>");
				playBtn.setEnabled(iterator.hasAfter());
				rewindBtn.setText("<< Rewind");
				rewindBtn.setEnabled(iterator.hasBefore());
			}
			
			
			HistoryScreen hs = (HistoryScreen) iterator.getCurrent();
			timeLabel.setText("" + hs.getId());
		}
		
		JButton backwardBtn;
		JButton forwardBtn;		
		JButton playBtn;
		JButton rewindBtn;
		JLabel  timeLabel;
		public NavigationControl(){
			super(BoxLayout.X_AXIS);
			
			backwardBtn = new JButton("< Back");
			backwardBtn.addActionListener(new NavigationActionListener(LEFT));
			
			forwardBtn = new JButton("Forward >");
			forwardBtn.addActionListener(new NavigationActionListener(RIGHT));
			
			playBtn = new JButton("Play >>");
			playBtn.addActionListener(new ActionListener(){
				@Override
				public void actionPerformed(ActionEvent e) {				
					direction = RIGHT;
					if (playing)
						navigator.stop();
					else
						navigator.play();		
					updateButtons();
				}			
			});
			
			rewindBtn = new JButton("<< Rewind");
			rewindBtn.addActionListener(new ActionListener(){
				@Override
				public void actionPerformed(ActionEvent e) {
					direction = LEFT;
					if (playing)
						navigator.stop();
					else{
						navigator.play();	
					}
					updateButtons();
					
				}			
			});

			timeLabel = new JLabel();
			
			add(rewindBtn);	
			add(backwardBtn);
			
			add(timeLabel);
			
			add(forwardBtn);
			add(playBtn);
			
		}
		
		
		boolean playing = false;
		int direction = RIGHT;
		public void play(){
			
			Thread autoplayThread = new Thread(){
				public void run() {
					
					playing = true;
					while (playing){
						try {

							if (direction == RIGHT && iterator.hasAfter()){
								stepForward();
							}else if (direction == LEFT && iterator.hasBefore()){
								stepBackward();
							}else{
								playing = false;
							}

							Thread.sleep(100);
							
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
		
		public void jump(int id){
			Object item = iterator.get(id);
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
	
	public HistoryScreen getHistoryScreen() {
		return history_screen;
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
	
	
	class QueryImageFrame extends JFrame{
		
		public QueryImageFrame(BufferedImage newimg){
			ImageIcon imgicn = new ImageIcon(newimg);
			JLabel imglbl = new JLabel(imgicn);
			add(imglbl);
			setVisible(true);
			setLocationRelativeTo(controls);
			setTitle("Image query");
			pack();
			
			
	        addWindowListener(new WindowAdapter() {
	            public void windowClosing(WindowEvent ev) {
	                findBox.removeQueryImage();
	            }
	        });
		}
		
	}
	
	QueryImageFrame queryImageFrame = null;
	private void doCaptureQueryImage(){

		if (queryImageFrame != null)
			queryImageFrame.dispose();
		
		selector.start();
		selector.setListener(new RegionSelectorListener(){
			
			@Override
			public void rectangleSelected(Rectangle rectangle) {
				screen.clear();
				
				BufferedImage newimg = screen.crop(rectangle);
				
				queryImageFrame = new QueryImageFrame(newimg);		
				findBox.setQueryImage(newimg);			
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
	
	
	
	class FindBox extends Box{
		
		JTextField queryTextField;
		JButton searchBtn;
		JButton captureBtn;
		
		BufferedImage queryImage;
		
		public void setQueryImage(BufferedImage image){
			queryImage = image;
		}
		
		public void removeQueryImage() {
			queryImage = null;
		}

		public FindBox(){
			super(BoxLayout.X_AXIS);
			
			queryTextField = new JTextField(15);	
			queryTextField.setMaximumSize(new Dimension(1000,25));

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
			
			
//			queryImage = new AutoResize(null);
//			queryImage.setMaximumSize(new Dimension(200,20));
//			queryImage.setPreferredSize(new Dimension(200,20));
//			
			//add(imagePanel);
			add(Box.createHorizontalGlue());
			
			//queryTextField.setText("chi");
			queryTextField.setText("facebook");
			
		}
		
		
		public String getQueryString(){
			return queryTextField.getText();
		}
		
		public BufferedImage getQueryImage(){
			return queryImage;
		}
		
	}

	FindBox findBox;
	JButton recordBtn;
	static String[] exampleStrings = {"facebook", "chi", "pilyoung","video","live","inbox","login","order"};
	private void createControls(){
		
		findBox = new FindBox();

		JComboBox exampleList = new JComboBox(exampleStrings);
        exampleList.addActionListener(new ActionListener(){ 
            public void actionPerformed(ActionEvent e) {
                JComboBox cb = (JComboBox)e.getSource();
                String exampleName = (String)cb.getSelectedItem();
                setExample(exampleName);
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
		
//		JButton readBtn = new JButton("Read");
//		readBtn.addActionListener(new ActionListener(){
//			@Override
//			public void actionPerformed(ActionEvent e) {
//				current_mode = Mode.READ;
//				repaint();
//			}			
//		});
		
		JButton readBtn = new JButton("Virtual Read");
		readBtn.addActionListener(new ActionListener(){
			@Override
			public void actionPerformed(ActionEvent e) {
				doVirtualRead();

				
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
		
		JButton filterBtn = new JButton("Filter");
		filterBtn.addActionListener(new ActionListener(){
			@Override
			public void actionPerformed(ActionEvent e) {
				doSelectFilter();
			}			
		});

		JToggleButton ocrBtn = new JToggleButton("View OCR");
		ocrBtn.addChangeListener(new ChangeListener(){
			@Override
			public void stateChanged(ChangeEvent e) {
				JToggleButton btn = (JToggleButton) e.getSource();
				
				if (btn.isSelected()){
					current_mode = Mode.OCR;
				}else{
					current_mode = Mode.BROWSE;
				}
				
				refreshHistoryScreen();
				
			}			
		});	
		
		JButton compareBtn = new JButton("Compare");
		compareBtn.addActionListener(new ActionListener(){
			@Override
			public void actionPerformed(ActionEvent e) {
				doCompareToNow();
			}			
		});
		
		recordBtn = new JButton("Record");
		recordBtn.addActionListener(new ActionListener(){
			@Override
			public void actionPerformed(ActionEvent e) {
				doRecord();
			}			
		});
		
		
		
		
		JRadioButton frameButton = new JRadioButton("Frame");
	    frameButton.setActionCommand("Frame");
	    frameButton.setSelected(true);

	    JRadioButton shotButton = new JRadioButton("Shot");
	    shotButton.setActionCommand("Shot");

	    //Group the radio buttons.
	    ButtonGroup group = new ButtonGroup();
	    group.add(frameButton);
	    group.add(shotButton);
	    
	    NavigationModeSelectionActionListener al = new NavigationModeSelectionActionListener();
	    frameButton.addActionListener(al);
	    shotButton.addActionListener(al);
	    
		navigator = new NavigationControl();

		
		controls = new JPanel();
		controls.setLayout(new BoxLayout(controls, BoxLayout.Y_AXIS));
		
		
		Box row1 = new Box(BoxLayout.X_AXIS);
		row1.add(findBox);
		row1.add(new JSeparator(JSeparator.VERTICAL));
		row1.add(navigator);	
		controls.add(row1);

		controls.add(new JSeparator());
		
		JPanel row2 = new JPanel();
		row2.add(exampleList);
		row2.add(scriptBtn);
		row2.add(readBtn);
		row2.add(compareBtn);
		row2.add(browseBtn);
		row2.add(copyBtn);
		row2.add(filterBtn);
		row2.add(ocrBtn);
		row2.add(recordBtn);
		row2.add(new JSeparator(JSeparator.VERTICAL));
		row2.add(frameButton);
		row2.add(shotButton);
		controls.add(row2);
		
	}

	boolean is_recording = false;
	Process recording_process;
	protected void doRecord() {
		
		if (is_recording){
			
			
			recording_process.destroy();
			is_recording = false;
			recordBtn.setText("Record");
			
		}else{


			
			String output_path = "captured/live";
			
			
			// clear directory
			File directory = new File(output_path);

			// Get all files in directory
			File[] files = directory.listFiles();
			for (File file : files){
			   // Delete each file
			   if (!file.delete()){
			       // Failed to delete file
			       System.out.println("Failed to delete "+file);
			   }
			}
			
			String command = "./sikulicmd CAPTURE " + output_path;;
			try {
				recording_process = Runtime.getRuntime().exec(command);
			} catch (IOException e) {
				
				
			}

			is_recording = true;
			recordBtn.setText("Stop");
			
			
		}
		
	}

	private void doVirtualRead() {
		
		if (virtualPage.isVisible()){				
			virtualPage.setVisible(false);
			return;
		}
		
		selector.start();
		selector.setListener(new RegionSelectorListener(){

			@Override
			public void rectangleSelected(Rectangle rectangle) {
				
				
				virtualPage.show(rectangle.getSize(),"long.png");
				virtualPage.setBounds(rectangle.x, rectangle.y-5, rectangle.width, rectangle.height);
				
				Rectangle tracked_rectangle = new Rectangle(150,150);
				tracked_rectangle.translate(rectangle.x+rectangle.width/2,
						rectangle.y+rectangle.height/2);
				
				current_mode = Mode.READ;

				//BufferedImage tracked = screen.crop(new Rectangle(370,200,200,100));
				BufferedImage tracked = screen.crop(tracked_rectangle);
				
				ArrayList<Rectangle> rects;
				try {
					rects = SikuliVision.find("long.png", tracked);
				
					if (!rects.isEmpty()){

						Rectangle top = rects.get(0);
						
						int vx = top.x-tracked_rectangle.x+rectangle.x;
						int vy = top.y-tracked_rectangle.y+rectangle.y;
						
						//virtualPage.move(top.x-370+20,top.y-200+96+5);
						//virtualPage.move(0,0);//vx,vy);
						//virtualPage.move(top.x,top.y);
						virtualPage.move(vx,vy);
					}
					
					virtualPage.setVisible(true);
					virtualPage.requestFocus();
					virtualPage.addKeyListener(virtualPage);

					//Rectangle r = virtualPage.getBounds();
					//r.grow(2,2);
					//r.translate(0,4);
					//screen.setHighlightRectangle(r);
					//updateUI();


					repaint();	
					
				} catch (IOException e) {
					
				}
				
			}
			
		});
		
		
		
		
		
	}

	protected void doCompareToNow() {
		HistoryScreen current_screen = HistoryScreenDatabase.getMostRecent();
		current_mode = Mode.DIFF;
		
		
		screen = current_screen.createImage();
		//screen.highlightAllWords();
		
		screen.drawWordMovements(current_screen, history_screen);
		screen.highlightNewWords(current_screen, history_screen);
		
		repaint();
		//setHistoryScreen(current_screen);
	}

	protected void setExample(String exampleName) {
		if (exampleName == "live"){
			HistoryScreenDatabase.load(exampleName);
			NavigationIterator iter = HistoryScreenDatabase.getIterator(0);
			navigator.setIterator(iter);
			navigator.jump(0);
		}
		else if (exampleName == "facebook"){
			HistoryScreenDatabase.load(exampleName,19);
			NavigationIterator iter = HistoryScreenDatabase.getIterator(0);
			navigator.setIterator(iter);
			navigator.jump(0);
		}
		else if (exampleName == "pilyoung"){
			HistoryScreenDatabase.load(exampleName,1000);
			NavigationIterator iter = HistoryScreenDatabase.getIterator(0);
			navigator.setIterator(iter);
			navigator.jump(800);
		}	
		else if (exampleName == "chi"){
			HistoryScreenDatabase.load(exampleName,30);
			NavigationIterator iter = HistoryScreenDatabase.getIterator(301);
			navigator.setIterator(iter);
			//navigator.jump(10);
		}	
		else if (exampleName == "video"){
			HistoryScreenDatabase.load(exampleName,15);
			NavigationIterator iter = HistoryScreenDatabase.getIterator(0);
			navigator.setIterator(iter);
			navigator.jump(13);
		}	
		else if (exampleName == "inbox"){
			HistoryScreenDatabase.load(exampleName,46);
			NavigationIterator iter = HistoryScreenDatabase.getIterator(0);
			navigator.setIterator(iter);
			navigator.jump(40);
		}
		else if (exampleName == "login"){
			HistoryScreenDatabase.load(exampleName,10);
			NavigationIterator iter = HistoryScreenDatabase.getIterator(0);
			navigator.setIterator(iter);
			navigator.jump(2);
		}
		else if (exampleName == "order"){
			HistoryScreenDatabase.load(exampleName,1800);
			NavigationIterator iter = HistoryScreenDatabase.getIterator(0);
			navigator.setIterator(iter);
			navigator.jump(100);
		}
	}

	private void refreshHistoryScreen() {
		setHistoryScreen(getHistoryScreen());
	}

	class NavigationModeSelectionActionListener implements ActionListener{

		@Override
		public void actionPerformed(ActionEvent e) {

			
			int current_id = getHistoryScreen().getId();
			HistoryScreenIterator iter;	
			
			if (e.getActionCommand() == "Frame"){
				iter = HistoryScreenDatabase.getIterator(current_id);
				navigator.setIterator(iter);
			}
			else if (e.getActionCommand() == "Shot"){
				iter = HistoryScreenDatabase.getShotIterator(current_id);
				navigator.setIterator(iter);
			}
			
			
		}
	}

	Rectangle filter_rectangle;
	private void doSelectFilter() {

		selector.start();
		selector.setListener(new RegionSelectorListener(){

			@Override
			public void rectangleSelected(Rectangle rectangle) {
				filter_rectangle = rectangle;
				ScreenImage.clearGlobalAnnotations();
				ScreenImage.addGlobalAnnotation(new AnnotationRectangle(rectangle));
			}
			
		});	
	}

	JFrame controlsFrame;
	public void showControls(){
		controlsFrame = new JFrame();
		controlsFrame.setUndecorated(true);
		controlsFrame.add(controls);
		controlsFrame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		//controlsFrame.setLocation(new Point(10,30));
		controlsFrame.setVisible(true);
		controlsFrame.toFront();
		controlsFrame.getRootPane().putClientProperty("Window.alpha", new Float(0.8f));
		controlsFrame.pack();
		controlsFrame.setResizable(false);
		controlsFrame.setLocationRelativeTo(this);

	    MoveMouseListener mml = new MoveMouseListener(controls);
	    controls.addMouseListener(mml);
	    controls.addMouseMotionListener(mml);
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
					
			Rectangles rects = new Rectangles();
			
			String query_string = findBox.getQueryString();
			if (query_string.length()>0){
				rects.addAll(HistoryScreenDatabase.findRectangles(hs.getId(), query_string, filter_rectangle));			
			}
			
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
				
				BufferedImage match_image = context_image.createRenderedImage(Mode.ABSTRACT);
	
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
		frame.setSize(450,600);
		frame.setLocationRelativeTo(null);
		frame.setVisible(true);
	}
	
	
	private void testPilyoung(){
		current_mode = Mode.BROWSE;
		HistoryScreen h1 = HistoryScreenDatabase.getMostRecent();
	//	this.setHistoryScreen(h1);
		showControls();
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
		
			String query_string = findBox.getQueryString();
			if (query_string.length()>0){
				String words[] = query_string.split(" ");

				for (String word : words){
					screen.highlightWord(word, filter_rectangle);
				}
			}

			BufferedImage query_image = findBox.getQueryImage();
			if (query_image != null)
				screen.addHighlightedImage(query_image);
		}
		
		if (current_mode == Mode.OCR){
			
			screen.highlightAllWords();
			screen.highlightAllIcons();
			
		}
		
		repaint();
	}
		
	public void paintComponent(Graphics g) {
		
		super.paintComponent(g);
        Graphics2D g2d = (Graphics2D) g;
        
        if (screen != null)
        	screen.paintHelper(g, current_mode);

        if (controlsFrame != null)
        	controlsFrame.pack();
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
        main.showControls();
    }
}
