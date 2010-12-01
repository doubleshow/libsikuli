package org.sikuli.history;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.Image;
import java.awt.RenderingHints;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.geom.AffineTransform;
import java.awt.image.AffineTransformOp;
import java.awt.image.BufferedImage;
import java.awt.image.BufferedImageOp;
import java.io.File;
import java.io.IOException;

import javax.imageio.ImageIO;
import javax.swing.BorderFactory;
import javax.swing.Box;
import javax.swing.BoxLayout;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JSeparator;
import javax.swing.JSplitPane;

import sikuli.ImageButton;
import sikuli.SikuliPane;

public class ScriptEditor extends JFrame{
	
	SikuliPane sikuli_pane;
	
	HistoryViewer viewer;
	ScriptEditor editor;
	public ScriptEditor(final HistoryViewer viewer, BufferedImage target_image){
		
		editor = this;
		this.viewer = viewer;
		
		JPanel left = new JPanel();
		left.setLayout(new BoxLayout(left, BoxLayout.Y_AXIS));
		left.add(new JLabel("Target"));
		
		
		
//		double scale = 0.5;
//	      BufferedImageOp op = new AffineTransformOp(
//	              AffineTransform.getScaleInstance(scale, scale),
//	              new RenderingHints(RenderingHints.KEY_INTERPOLATION,
//	                                 RenderingHints.VALUE_INTERPOLATION_BICUBIC));
//	      BufferedImage scaled_target_image = op.filter(target_image, null);
//
//	    

		AutoResize ar = new AutoResize(target_image);
		ar.setPreferredSize(new Dimension(200,200));
		ar.setAlignmentX(LEFT_ALIGNMENT);		
		left.add(ar);
		left.setAlignmentY(TOP_ALIGNMENT);
		
		File target_image_file = new File("sikulitest.sikuli/target.png");
		try {
			ImageIO.write(target_image, "png", target_image_file);
		} catch (IOException e2) {
			e2.printStackTrace();
		}
		
		
		JPanel right = new JPanel();
		right.setLayout(new BoxLayout(right, BoxLayout.Y_AXIS));
		JLabel l2 = new JLabel("Script");
		l2.setAlignmentX(LEFT_ALIGNMENT);
		right.add(l2);
		right.setAlignmentY(TOP_ALIGNMENT);
		
		sikuli_pane = new SikuliPane();		
		JScrollPane editorScrollPane = new JScrollPane(sikuli_pane);
		editorScrollPane.setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_AS_NEEDED);
		editorScrollPane.setPreferredSize(new Dimension(350, 250));
		editorScrollPane.setMinimumSize(new Dimension(10, 10));
		editorScrollPane.setAlignmentX(LEFT_ALIGNMENT);
		

		right.add(editorScrollPane);
		
		JPanel buttons = new JPanel(new FlowLayout(FlowLayout.RIGHT));
		
		JButton cancelBtn = new JButton("Cancel");
		cancelBtn.addActionListener(new ActionListener(){
			@Override
			public void actionPerformed(ActionEvent e) {
				dispose();
			}
		});
		
		JButton saveBtn = new JButton("Save");
		saveBtn.addActionListener(new ActionListener(){
			@Override
			public void actionPerformed(ActionEvent e) {
				doSave();
			}
		});
		
		JButton captureBtn = new JButton("Capture");
		captureBtn.	addActionListener(new ActionListener(){
			@Override
			public void actionPerformed(ActionEvent e) {
				//dispose();
				editor.setVisible(false);
				viewer.doCaptureScriptPatternImage();
			}
		});
		
		buttons.add(captureBtn);
		buttons.add(cancelBtn);
		buttons.add(saveBtn);

		buttons.setAlignmentX(LEFT_ALIGNMENT);
		right.add(buttons);
		
		right.add(Box.createVerticalGlue());
		
		
		JSplitPane panel = new JSplitPane(JSplitPane.HORIZONTAL_SPLIT,
                left, right);
//		panel.add(left);
//		panel.add(Box.createRigidArea(new Dimension(10,0)));
//		panel.add(right);
//		
		panel.setBorder(BorderFactory.createEmptyBorder(10,10,10,10));
		
		add(panel);
		
		setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
		setSize(800,300);
		setLocationRelativeTo(null);
		setVisible(true);
	}
	
	
	protected void doSave() {
		try {
			sikuli_pane.saveAsBundle("sikulitest.sikuli");
			
			String command = "python monitor.py";
			//System.out.println(command);
			File dir = new File("sikulitest.sikuli");
			Process p = Runtime.getRuntime().exec(command,null,dir);
			
		} catch (IOException e1) {
			e1.printStackTrace();
		}
		dispose();
	}


	public void insertImage(BufferedImage image){
		
		try {

        	File f;
        	File curdir = new File("sikulitest.sikuli");
			f = File.createTempFile("target", ".png",curdir);
			//f.deleteOnExit(); 
			ImageIO.write(image, "png", f);
			sikuli_pane.insertComponent(new ImageButton(sikuli_pane, f.getPath()));
			sikuli_pane.requestFocus();
			//requestFocus();
			toFront();
		} catch (IOException e1) {
			
			e1.printStackTrace();
		}
		
	}
	
	 public static void main(String[] args) {
		   	   	
		 try {
			ScriptEditor te = new ScriptEditor(null,ImageIO.read(new File("screen.png")));
		} catch (IOException e) {
			e.printStackTrace();
		}
	 }
	

}
