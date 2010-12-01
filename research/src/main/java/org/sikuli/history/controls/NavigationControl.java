/**
 * 
 */
package org.sikuli.history.controls;


import java.awt.Container;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;

import javax.swing.Box;
import javax.swing.BoxLayout;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;

import org.grlea.log.SimpleLogger;
import org.sikuli.history.HistoryScreen;
import org.sikuli.history.HistoryViewer;
import org.sikuli.history.MoveMouseListener;

public class NavigationControl extends Box 
	implements KeyListener{
	
	private static final SimpleLogger log = new SimpleLogger(NavigationControl.class);


	private final HistoryViewer _historyViewer;
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
		
		JFrame f = getFrame(this);
		if (f != null){
			f.pack();
		}
	}
	
	private JFrame getFrame(Container target) {
		if (target == null){
			return null;
		}
		if (target instanceof JFrame) {
			return (JFrame) target;
		}
		return getFrame(target.getParent());
	}
	
	
	JFrame f;
	public void show(){
		
		if (f == null){
			f = new JFrame();

			f.add(this);

			f.setUndecorated(true);
			f.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
			f.getRootPane().putClientProperty("Window.alpha", new Float(0.8f));
			f.pack();
			f.setResizable(false);
			f.setLocationRelativeTo(null);
			MoveMouseListener mml = new MoveMouseListener(this);
			addMouseListener(mml);
			addMouseMotionListener(mml);

		}

		f.setVisible(true);
		requestFocus();
	}

	JButton backwardBtn;
	JButton forwardBtn;		
	JButton playBtn;
	JButton rewindBtn;
	JLabel  timeLabel;
	
	public NavigationControl(HistoryViewer historyViewer){
		super(BoxLayout.X_AXIS);
		this._historyViewer = historyViewer;

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
					stop();
				else
					play();		
				updateButtons();
			}			
		});

		rewindBtn = new JButton("<< Rewind");
		rewindBtn.addActionListener(new ActionListener(){
			@Override
			public void actionPerformed(ActionEvent e) {
				direction = LEFT;
				if (playing)
					stop();
				else{
					play();	
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
		if (iterator.hasAfter()){
			Object item = iterator.getAfter();
			listener.itemSelected(item);
			updateButtons();
		}
	}

	public void jump(int id){
		Object item = iterator.get(id);
		listener.itemSelected(item);
		updateButtons();
	}

	public void stepBackward(){
		if (iterator.hasBefore()){
			Object item = iterator.getBefore();
			listener.itemSelected(item);
			updateButtons();		
		}
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



	@Override
	public void keyPressed(KeyEvent e) {
		int key = e.getKeyCode();
        if (key == KeyEvent.VK_LEFT) {
        	stepBackward();
        }else if (key == KeyEvent.VK_RIGHT){
        	stepForward();
        }
	}

	@Override
	public void keyReleased(KeyEvent e) {
	}

	@Override
	public void keyTyped(KeyEvent e) {
	}

}