package history.controls;

import history.MoveMouseListener;

import javax.swing.JComponent;
import javax.swing.JFrame;

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
