package history;
import history.HistoryScreenDatabase.HistoryScreenIterator;
import history.OCRDocument.OCRWord;
import history.controls.NavigationControl;
import history.controls.NavigationIterator;
import history.controls.NavigationListener;
import history.controls.SimpleSearchBox;
import history.controls.Toolbar;

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
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
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

import org.grlea.log.SimpleLogger;

import sikuli.Clipboard;
import sikuli.SikuliPane;
import sikuli.ImageButton;
import vision.OCRText;

public class HistoryViewer extends JPanel implements KeyListener {
	
	private static final SimpleLogger log = new SimpleLogger(HistoryViewer.class);

	private static final long serialVersionUID = 1L;

	HistoryScreenImage screen;

	//Screen history_screen;

	JLayeredPane layeredPane;


	JPanel controls;

	HistoryQuery _query;

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

	NavigationControl _navigator;
	Toolbar _toolbar;
	
	public HistoryViewer() {

		//		UIManager.put("Label.font", new FontUIResource("Lucida Grande", Font.PLAIN, 10));
		//		UIManager.put("Button.font", new FontUIResource("Lucida Grande", Font.PLAIN, 10));
		//		UIManager.put("ToggleButton.font", new FontUIResource("Lucida Grande", Font.PLAIN, 10));

		isBrowsing = false;

		layeredPane = new JLayeredPane();
		layeredPane.setPreferredSize(new Dimension(1152, 720));
		add(layeredPane);

		present_screen = new ScreenImage("testdata/images/scenario-deadline-word.png"); 

		//createControls();
		
		

		virtualPage = new Lens();
		virtualPage.setVisible(false);
		virtualPage.setFocusable(true);		
		layeredPane.add(virtualPage, new Integer(3));	

		selector = new RegionSelector();

		current_mode = Mode.BROWSE;

		//setExample("facebook");		
		//setExample("inbox");	



		SimpleSearchBox ssb = new SimpleSearchBox(this);
		
		_navigator = new NavigationControl(this);		
		_navigator.setListener(new HistoryScreenNavigationListener());
		
		
		_toolbar = new Toolbar(this);
		
		setExample("chi");	

		addKeyListener(_navigator);
		addKeyListener(this);
		
		setFocusable(true);		
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

			screen.addHighlight(new Rectangle(srcx,srcy,2,2));
			repaint();
		}

		public void mouseReleased(java.awt.event.MouseEvent e){

			if (destx > srcx + 2 && desty > srcy + 2){
				
				log.info("selected " + srcx + "," + srcy
									+" - "+desty + "," + destx);

				selected_rectangle = new Rectangle(srcx,srcy,
						destx-srcx,desty-srcy);

				removeMouseListener(selector);
				removeMouseMotionListener(selector);
				listener.rectangleSelected(selected_rectangle);
				
				current_mode = saved_mode;
				setHistoryScreen(history_screen);
			}
		}

		public void mouseDragged(java.awt.event.MouseEvent e) {

			destx = e.getX();
			desty = e.getY();

			if (destx > srcx + 2 && desty > srcy + 2){
				Rectangle selected_rectangle = new Rectangle(srcx,srcy,
						destx-srcx,desty-srcy);

				
				AnnotationHighlight highlight = 
					new AnnotationHighlight(screen,selected_rectangle);
				highlight.setBorderColor(Color.gray);
				
				screen.clear();
				screen.addAnnotation(highlight);
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

	public void doCaptureCopyText(){
		_toolbar.hideFrame();
		
		selector.start();
		selector.setListener(new RegionSelectorListener(){

			@Override
			public void rectangleSelected(Rectangle rectangle) {
				BufferedImage image = screen.crop(rectangle);

				OCRDocument ocr = new OCRDocument(image);
				String ocr_text = ocr.getString();
				
				log.info("capture text: " + ocr_text);

				Clipboard.putText(Clipboard.PLAIN, Clipboard.UTF8, 
						Clipboard.BYTE_BUFFER, ocr_text);

			}

		});
	}

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
				doBrowse();
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

		_navigator = new NavigationControl(this);


		controls = new JPanel();
		controls.setLayout(new BoxLayout(controls, BoxLayout.Y_AXIS));


		Box row1 = new Box(BoxLayout.X_AXIS);
		row1.add(findBox);
		row1.add(new JSeparator(JSeparator.VERTICAL));
		row1.add(_navigator);	
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

	public void doVirtualRead() {

		if (virtualPage.isVisible()){				
			virtualPage.setVisible(false);
			return;
		}
		_toolbar.hideFrame();
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

	public void doCompare(){
		_toolbar.hideFrame();
		
		if (bookmarked_screens.size()!=2)
			return;
		
		
		current_mode = Mode.DIFF;
		
		HistoryScreen a = bookmarked_screens.get(0);
		HistoryScreen b = bookmarked_screens.get(1);
		
		screen = a.createImage();

		screen.drawWordMovements(a, b);
		screen.highlightNewWords(a, b);

		repaint();
	}
	
	public void doCompareToNow() {
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
			NavigationIterator iter = HistoryScreenDatabase.getIterator(301);
			_navigator.setIterator(iter);
			_navigator.jump(301);
		}
		else if (exampleName == "facebook"){
			HistoryScreenDatabase.load(exampleName);
			NavigationIterator iter = HistoryScreenDatabase.getIterator(0);
			_navigator.setIterator(iter);
			_navigator.jump(0);
		}
		else if (exampleName == "pilyoung"){
			HistoryScreenDatabase.load(exampleName);
			NavigationIterator iter = HistoryScreenDatabase.getIterator(0);
			_navigator.setIterator(iter);
			_navigator.jump(800);
		}	
		else if (exampleName == "chi"){
			HistoryScreenDatabase.load(exampleName);
			NavigationIterator iter = HistoryScreenDatabase.getIterator(301);
			_navigator.setIterator(iter);
			_navigator.jump(301);
		}	
		else if (exampleName == "video"){
			HistoryScreenDatabase.load(exampleName);
			NavigationIterator iter = HistoryScreenDatabase.getIterator(0);
			_navigator.setIterator(iter);
			_navigator.jump(13);
		}	
		else if (exampleName == "inbox"){
			HistoryScreenDatabase.load(exampleName);
			NavigationIterator iter = HistoryScreenDatabase.getIterator(0);
			_navigator.setIterator(iter);
			_navigator.jump(40);
		}
		else if (exampleName == "login"){
			HistoryScreenDatabase.load(exampleName);
			NavigationIterator iter = HistoryScreenDatabase.getIterator(0);
			_navigator.setIterator(iter);
			_navigator.jump(2);
		}
		else if (exampleName == "order"){
			HistoryScreenDatabase.load(exampleName);
			NavigationIterator iter = HistoryScreenDatabase.getIterator(0);
			_navigator.setIterator(iter);
			_navigator.jump(100);
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
				_navigator.setIterator(iter);
			}
			else if (e.getActionCommand() == "Shot"){
				iter = HistoryScreenDatabase.getShotIterator(current_id);
				_navigator.setIterator(iter);
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

			String query_string = _query.getText();
			if (query_string.length()>0){
				rects.addAll(HistoryScreenDatabase.findRectangles(hs.getId(), query_string, filter_rectangle));			
			}

			BufferedImage query_image = _query.getImage();
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

	public void doFind(HistoryQuery query){
		_query = query;
		
		current_mode = Mode.FIND;

		String query_string = query.getText();
		BufferedImage query_image = query.getImage();

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

		HistoryViewerFrame.getInstance().setVisible(true);
		
		
		setHistoryScreen(find_result.getMostRecent());			
		_navigator.setIterator(find_result);
		
		//_navigator.show();
		//_toolbar.showFrame();
		
		//showMatchViewer(find_result);

		
		requestFocus();
		
		repaint();
		
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
		_navigator.setIterator(find_result);
		showMatchViewer(find_result);

		repaint();
	}



	private HistoryScreen history_screen;

	private void setHistoryScreen(HistoryScreen hs){

		history_screen = hs;
		screen = hs.createImage();

		if (current_mode == Mode.FIND){

			String query_string = _query.getText();
			if (query_string.length()>0){
				String words[] = query_string.split(" ");

				for (String word : words){
					screen.highlightWord(word, filter_rectangle);
				}
			}

			BufferedImage query_image = _query.getImage();
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
	
	public void doBrowse(){
		current_mode = Mode.BROWSE;
		int id = history_screen.getId();
		HistoryScreenIterator iter = HistoryScreenDatabase.getIterator(id);
		_navigator.setIterator(iter);
		repaint();
	}	
	
	ArrayList<HistoryScreen> bookmarked_screens = new ArrayList<HistoryScreen>();
	public void addBookmark(){
		log.info("adding a bookmark:" + history_screen);
		bookmarked_screens.add(history_screen);
	}
	
	public static void main(String[] args) {
		HistoryViewerFrame viewer =  HistoryViewerFrame.getInstance();
		viewer.setVisible(false);		
	}

	@Override
	public void keyPressed(KeyEvent e) {
		int key = e.getKeyCode();
        if (key == KeyEvent.VK_SPACE) {
        	_toolbar.showFrame();
        }else if (key == KeyEvent.VK_ESCAPE){
        	doBrowse();
        }else if (key == KeyEvent.VK_B){
        	addBookmark();
        }
	}

	@Override
	public void keyReleased(KeyEvent e) {
	}

	@Override
	public void keyTyped(KeyEvent e) {
	}

}
