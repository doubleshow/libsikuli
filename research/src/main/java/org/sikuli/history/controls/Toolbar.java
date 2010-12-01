package org.sikuli.history.controls;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;

import javax.swing.Box;
import javax.swing.BoxLayout;
import javax.swing.JButton;
import javax.swing.JFrame;

import org.sikuli.history.HistoryViewer;

public class Toolbar extends Box{

	HistoryViewer _viewer;
	public Toolbar(HistoryViewer viewer){		
		super(BoxLayout.X_AXIS);
		
		_viewer = viewer;
		
		JButton copyBtn = new JButton("Copy");
		copyBtn.addActionListener(new ActionListener(){
			@Override
			public void actionPerformed(ActionEvent e) {
				_viewer.doCaptureCopyText();				
			}			
		});
		
		JButton readBtn = new JButton("Read");
		readBtn.addActionListener(new ActionListener(){
			@Override
			public void actionPerformed(ActionEvent e) {
				_viewer.doVirtualRead();
			}			
		});
		
		JButton compareBtn = new JButton("Compare");
		compareBtn.addActionListener(new ActionListener(){
			@Override
			public void actionPerformed(ActionEvent e) {
				_viewer.doCompare();
			}			
		});
		
		add(copyBtn);
		add(readBtn);
		add(compareBtn);
		
		
		
		addKeyListener(new KeyAdapter(){
			@Override
			public void keyPressed(KeyEvent e) {
				int key = e.getKeyCode();
		        if (key == KeyEvent.VK_ESCAPE){
		        	hideFrame();
		        }
			}
		});
	}
	
	JFrame f;
	public void showFrame(){
		
		if (f == null){
			f = new FloatingBar(this);
		}

		f.setVisible(true);
		f.toFront();
		requestFocus();
	}
	
	public void hideFrame(){
		
		if (f == null){
			return;
		}else{
			f.setVisible(false);
		}
	}
	

	//public void 
	
}
