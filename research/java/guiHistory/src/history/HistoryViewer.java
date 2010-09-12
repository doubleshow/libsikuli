package history;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Rectangle;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
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
		FIND
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
		
		
		JButton searchBtn = new JButton("Find");
		searchBtn.addActionListener(new ActionListener(){
			@Override
			public void actionPerformed(ActionEvent e) {
				
				current_mode = Mode.FIND;
				
				String query_string = input_query_string.getText();
				
				find_result = new FindResult(query_string);
				HistoryScreen hs = find_result.getMostRecent();
				hs.setHighlightedWord(query_string);		
				setHistoryScreen(find_result.getMostRecent());
				
				later.setEnabled(true);
				repaint();
			}

		
		});
		
		JButton exitBtn = new JButton("Exit");
		exitBtn.addActionListener(new ActionListener(){
			@Override
			public void actionPerformed(ActionEvent e) {
				current_screen = present_screen;
				repaint();
			}

		
		});
		
		
		controls.add(closeBtn);
		controls.add(input_query_string);
		controls.add(searchBtn);
		controls.add(exitBtn);
		
		navigationControl = new JPanel(new FlowLayout(FlowLayout.CENTER));
		//navigationControl.setBounds(200,400,250,50);
		navigationControl.setPreferredSize(new Dimension(350,40));
		navigationControl.setBorder(new LineBorder(Color.black, 1));
		
		earlier = new JButton("Earlier");
		//earlier.setPreferredSize(new Dimension(20,20));
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
						hs.setHighlightedWord(input_query_string.getText());
						setHistoryScreen(hs);
						
					}
				
			}			
		});
		
		later = new JButton("Later");
		//later.setPreferredSize(new Dimension(20,20));
		later.addActionListener(new ActionListener(){
			@Override
			public void actionPerformed(ActionEvent e) {
				
				if (current_mode == Mode.BROWSE) {
					HistoryScreen hs = ((HistoryScreen)current_screen).getAfter();
					setHistoryScreen(hs);
				}else if (current_mode == Mode.FIND) {
					
					HistoryScreen hs = find_result.getAfter();
					hs.setHighlightedWord(input_query_string.getText());
					setHistoryScreen(hs);
					
				}
				
				
			}			
		});
		later.setEnabled(false);
		
		JButton browseBtn = new JButton("Browse");
		browseBtn.addActionListener(new ActionListener(){
			@Override
			public void actionPerformed(ActionEvent e) {
				//loadHistoryScreensMostRecent();
				//selectHistoryScreen(0);
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
	
	}
	
	private void setHistoryScreen(HistoryScreen hs){
		time.setText(hs.getTimeString());
		current_screen = hs;
		repaint();
	}
	
	public void paintComponent(Graphics g) {
		
		super.paintComponent(g);
        Graphics2D g2d = (Graphics2D) g;

        
        current_screen.paintHelper(g);
        
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
