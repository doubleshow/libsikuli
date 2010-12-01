/**
 * 
 */
package org.sikuli.history;

import javax.swing.JFrame;

class HistoryViewerFrame extends JFrame{
	
	private static HistoryViewerFrame _instance;
	public static HistoryViewerFrame getInstance(){
		if (_instance == null){
			_instance = new HistoryViewerFrame();
		}
		return _instance;
	}
	
	
	HistoryViewer _viewer;
	public HistoryViewerFrame(){
		
		_viewer =  new HistoryViewer();
		add(_viewer);
		
//		add(HistoryViewer.getInstance());

		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		setSize(1152,740);
		setLocationRelativeTo(null);
		setResizable(false);
	}
}