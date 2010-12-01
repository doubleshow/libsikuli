/**
 * 
 */
package org.sikuli.history.controls;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;

import javax.swing.Box;
import javax.swing.BoxLayout;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JTextField;

import org.sikuli.history.HistoryQuery;
import org.sikuli.history.HistoryViewer;

public class SimpleSearchBox extends JFrame{
		
		JTextField queryTextField;
		JButton searchBtn;
		
		HistoryViewer _viewer;
		public SimpleSearchBox(HistoryViewer viewer){
			_viewer = viewer;
			
			Box box = new Box(BoxLayout.X_AXIS);
			
			queryTextField = new JTextField(20);	
			queryTextField.addKeyListener(new KeyAdapter(){
				
			    public void keyPressed(KeyEvent e) {
			    	int key = e.getKeyCode();
			        if (key == KeyEvent.VK_ENTER) {
			        	submitQuery();
			        }
			    }

			});

			searchBtn = new JButton("Find");
			searchBtn.addActionListener(new ActionListener(){
				@Override
				public void actionPerformed(ActionEvent e) {
					submitQuery();
				}
			});

			box.add(queryTextField);
			box.add(searchBtn);
			
			add(box);
			
			// set Frame properties
			setUndecorated(true);
			setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
			getRootPane().putClientProperty("Window.alpha", new Float(0.8f));
			pack();
			setResizable(false);
			setLocationRelativeTo(this);

			setVisible(true);	
			toFront();

			
//			MoveMouseListener mml = new MoveMouseListener(box);
//			box.addMouseListener(mml);
//			box.addMouseMotionListener(mml);
			
		}
		
		private void submitQuery(){
			if (queryTextField.getText().length()>0){
				HistoryQuery query = new HistoryQuery(queryTextField.getText());
				_viewer.doFind(query);
			}
		}
	}