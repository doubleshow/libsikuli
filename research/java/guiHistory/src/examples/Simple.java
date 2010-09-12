package examples;
import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.GridLayout;
import java.awt.Insets;
import java.awt.LayoutManager;
import java.awt.Toolkit;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.BorderFactory;
import javax.swing.Box;
import javax.swing.BoxLayout;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTextArea;
import javax.swing.JToolBar;
import javax.swing.border.LineBorder;

public class Simple extends JFrame{
	
	public Simple() {

        setSize(300, 200);
        setTitle("Simple");
        setDefaultCloseOperation(EXIT_ON_CLOSE);
        
        
        final Toolkit toolkit = getToolkit();
        Dimension size = toolkit.getScreenSize();
        setLocation(size.width/2 - getWidth()/2, 
        			size.height/2 - getHeight()/2);
        
        JPanel panel = new JPanel();
        getContentPane().add(panel);
        panel.setToolTipText("A Panel container");
        
        
        JButton beep = new JButton("Beep");
        beep.setToolTipText("Beep button");
        beep.setBounds(150, 60, 80, 30);
        beep.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent event) {
                toolkit.beep();
            }
        });
        
        JButton close = new JButton("Close");
        close.setBounds(50, 60, 80, 30);
        close.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent event) {
                System.exit(0);
           }
        });
        
        
        JTextArea area = new JTextArea("text area");
        area.setPreferredSize(new Dimension(100, 100));
        
        panel.add(beep);
        panel.add(close);
        panel.add(area);
        
        JPanel gridpanel = new JPanel();
        panel.setBorder(BorderFactory.createEmptyBorder(5, 5, 5, 5));
        panel.setBackground(new Color(255,0,0));
        gridpanel.setLayout(new GridLayout(2,4,3,3));
        panel.add(gridpanel);
        
        gridpanel.add(new JLabel("1"));
        gridpanel.add(new JLabel("2"));
        gridpanel.add(new JLabel("3"));
        
        ImageIcon exit = new ImageIcon("run.png");
        JButton runButton = new JButton(exit);
              
        JToolBar toolbar = new JToolBar(JToolBar.VERTICAL);
        toolbar.setFloatable(false);
        toolbar.setMargin(new Insets(10, 5, 5, 5));
        toolbar.add(runButton);
        
        add(toolbar, BorderLayout.WEST);


        JLabel statusbar = new JLabel(" Statusbar");
        statusbar.setPreferredSize(new Dimension(-1, 22));
        statusbar.setBorder(LineBorder.createGrayLineBorder());
        add(statusbar, BorderLayout.SOUTH);
	
        
        JPanel box = new JPanel();
        box.setLayout(new BoxLayout(box, BoxLayout.Y_AXIS));
        panel.add(box);
        
        
        box.add(Box.createVerticalGlue());
        
        JPanel bottom = new JPanel();
        bottom.setAlignmentX(1f);
        bottom.setLayout(new BoxLayout(bottom, BoxLayout.X_AXIS));
        
        JButton okButton = new JButton("OK");
        JButton closeButton = new JButton("Close");

        bottom.add(okButton);
        bottom.add(Box.createRigidArea(new Dimension(5, 0)));
        bottom.add(closeButton);
        bottom.add(Box.createRigidArea(new Dimension(15, 0)));

        box.add(bottom);
        box.add(Box.createRigidArea(new Dimension(0, 15)));

	}
	
	public static void main(String[] args) {

		Simple simple = new Simple();
        simple.setVisible(true);

        RigidArea ra = new RigidArea();
        ra.setVisible(true);
    } 
}
