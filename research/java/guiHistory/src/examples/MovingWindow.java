package examples;
import java.awt.FlowLayout;
import java.awt.Font;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.ComponentEvent;
import java.awt.event.ComponentListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;

import javax.swing.BoxLayout;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;


public class MovingWindow extends JFrame 
	implements ComponentListener, MouseMotionListener {

    private JLabel labelx;
    private JLabel labely;
    private JLabel labelw;
    private JLabel labelh;
    private JLabel mx;
    private JLabel my;

    public MovingWindow() {

        setTitle("Moving window");

        JPanel panel = new JPanel();
        panel.setLayout(new BoxLayout(panel, BoxLayout.Y_AXIS));
        
        labelx = new JLabel("x: ");
        labelx.setFont(new Font("Serif", Font.BOLD, 14));
 
        labely = new JLabel("y: ");
        labely.setFont(new Font("Serif", Font.BOLD, 14));

        labelh = new JLabel("h: ");
        labelh.setFont(new Font("Serif", Font.BOLD, 14));

        labelw = new JLabel("w: ");
        labelw.setFont(new Font("Serif", Font.BOLD, 14));
        
        panel.add(new JLabel("Mouse location:"));
        mx = new JLabel("x: ");
        my = new JLabel("y: ");
        
        panel.add(labelx);
        panel.add(labely);
        panel.add(labelh);
        panel.add(labelw);
        panel.add(mx);
        panel.add(my);

        add(panel);

        addComponentListener(this);
        
        panel.addMouseMotionListener(this);
         
        setSize(310, 200);
        setLocationRelativeTo(null);
        setDefaultCloseOperation(EXIT_ON_CLOSE);
        setVisible(true);
    }

	public void mouseMoved(MouseEvent e){
		int x = e.getXOnScreen();
		int y = e.getYOnScreen();
		int x0 = e.getX();
		int y0 = e.getY();
		mx.setText("x: " + x + "," + x0);
		my.setText("y: " + y + "," + y0);
	}
	
	public void mouseDragged(MouseEvent e) {
	
	}

    public void componentResized(ComponentEvent e) {
        int h = e.getComponent().getHeight();
        int w = e.getComponent().getWidth();
        labelh.setText("h: " + h);
        labelw.setText("w: " + w);
    }

    public void componentMoved(ComponentEvent e) {
        int x = e.getComponent().getX();
        int y = e.getComponent().getY();
        labelx.setText("x: " + x);
        labely.setText("y: " + y);
    }

    public void componentShown(ComponentEvent e) {
    	
    }

    public void componentHidden(ComponentEvent e) {
    }


    public static void main(String[] args) {
        new MovingWindow();
    }
}