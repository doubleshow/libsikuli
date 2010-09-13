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
import java.util.ArrayList;

import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JLayeredPane;
import javax.swing.JPanel;
import javax.swing.JTextField;
import javax.swing.border.LineBorder;

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
		SELECT
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
		
		
	};
	
	
	
	
	int history_screen_index = -1;
	
	JButton later;
	JButton earlier;
	JTextField input_query_string;
	
	Screen present_screen;
	
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
		
		controls.setBounds(0,640,1152,80);
		
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
		
		
		controls.add(closeBtn);
		controls.add(input_query_string);
		controls.add(searchBtn);
		controls.add(exitBtn);
		
		navigationControl = new JPanel(new FlowLayout(FlowLayout.CENTER));
		navigationControl.setPreferredSize(new Dimension(350,40));
		navigationControl.setBorder(new LineBorder(Color.black, 1));
		
		earlier = new JButton("Earlier");
		earlier.addActionListener(new ActionListener(){
			@Override
			public void actionPerformed(ActionEvent e) {
				
				
					if (current_mode == Mode.PRESENT) {
						HistoryScreen hs = HistoryScreenDatabase.getMostRecent();
						setHistoryScreen(hs);
						
						current_mode = Mode.BROWSE;					
						later.setEnabled(true);
						
					}else if (current_mode == Mode.BROWSE) {
						HistoryScreen hs = ((HistoryScreen)current_screen).getBefore();
						setHistoryScreen(hs);	
					
					}else if (current_mode == Mode.FIND) {
						
						HistoryScreen hs = find_result.getBefore();
						setHistoryScreen(hs);
						
						later.setEnabled(find_result.hasAfter());
						earlier.setEnabled(find_result.hasBefore());
						
					}
				
			}			
		});
		
		later = new JButton("Later");
		later.addActionListener(new ActionListener(){
			@Override
			public void actionPerformed(ActionEvent e) {
				
				if (current_mode == Mode.BROWSE) {
					HistoryScreen hs = ((HistoryScreen)current_screen).getAfter();
					setHistoryScreen(hs);
				}else if (current_mode == Mode.FIND) {
					
					HistoryScreen hs = find_result.getAfter();				
					setHistoryScreen(hs);
					
					later.setEnabled(find_result.hasAfter());
					earlier.setEnabled(find_result.hasBefore());
				}
				
				
			}			
		});
		later.setEnabled(false);
		
		JButton browseBtn = new JButton("Browse");
		browseBtn.addActionListener(new ActionListener(){
			@Override
			public void actionPerformed(ActionEvent e) {
				
				current_mode = Mode.BROWSE;
				repaint();
				
			}			
		});
		
		
		time = new JLabel("-15:20");
		time.setOpaque(true);
		//Font font = new Font("sansserif", Font.BOLD, 32);
		//time.setFont(font);
		//time.setForeground(Color.red);
		
		navigationControl.add(earlier);
		navigationControl.add(later);
		navigationControl.add(time);
		navigationControl.add(browseBtn);
		
		
		//navigationControl.setVisible(false);
		//navigationControl.setOpaque(false);
		

		controls.add(navigationControl);
		
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
	        	
	        	Rectangle selected_rectangle = new Rectangle(srcx,srcy,
	            		destx-srcx,desty-srcy);
	        	
	            selected_image = current_screen.crop(selected_rectangle);
	        	
	            OCRDocument doc = new OCRDocument(selected_image);
	            
	            String query_string = null;
	            for (String word : doc.getWords()){
	            	System.out.println(word);
	            	
	            	if (query_string == null)
	            		query_string = word;
	            	else
	            		query_string = query_string + " && " + word;
	            }
	            
	            doFind(query_string);
	            
	        	repaint();

	         }
		});
		
		addMouseMotionListener( new MouseMotionAdapter(){
	          public void mouseDragged(java.awt.event.MouseEvent e) {

	        	 destx = e.getX();
	             desty = e.getY();
	             
	             System.out.println("moved " + desty + "," + destx);
	             

	             	Rectangle selected_rectangle = new Rectangle(srcx,srcy,
		            		destx-srcx,desty-srcy);
	             
		            current_screen.setHighlightRectangle(selected_rectangle);
		            
		            
		        
	             
	            repaint(); 
	          }
	       });
	
	}
	
	private void doFind(String query_string){
		current_mode = Mode.FIND;
		
		find_result = new FindResult(query_string);	
		setHistoryScreen(find_result.getMostRecent());
		
		later.setEnabled(true);
		repaint();
	}
	
	BufferedImage selected_image;
	int srcx, srcy, destx, desty;
	
	private void setHistoryScreen(HistoryScreen hs){
		//time.setText(hs.getTimeString());
		
		hs.setHighlightedWord(input_query_string.getText());
				
		if (selected_image != null)
			hs.setHighlightedImage(selected_image);
		
		
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
        	
//        	
//        	Rectangle m;
//        	
//        	if (m != null){
//        		g2d.drawRect(m.x,m.y,m.width,m.height);
//        	}
        		
        	
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
