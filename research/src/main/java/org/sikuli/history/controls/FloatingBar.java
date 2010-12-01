package org.sikuli.history.controls;

import javax.swing.JComponent;
import javax.swing.JFrame;

import org.sikuli.history.MoveMouseListener;

public class FloatingBar extends JFrame {

	public FloatingBar(JComponent comp){		
		add(comp);

		setUndecorated(true);
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		getRootPane().putClientProperty("Window.alpha", new Float(0.8f));
		pack();
		setResizable(false);
		setLocationRelativeTo(null);

		MoveMouseListener mml = new MoveMouseListener(comp);
		addMouseListener(mml);
		addMouseMotionListener(mml);

	}

}
