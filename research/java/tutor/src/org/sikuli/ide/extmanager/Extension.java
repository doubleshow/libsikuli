/**
 * 
 */
package org.sikuli.ide.extmanager;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Component;
import java.awt.Dimension;
import java.awt.Image;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.io.IOException;
import java.net.URL;

import javax.imageio.ImageIO;
import javax.swing.BorderFactory;
import javax.swing.BoxLayout;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JComponent;
import javax.swing.JEditorPane;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.border.Border;
import javax.swing.border.EtchedBorder;
import javax.swing.event.MouseInputListener;

class Extension extends JPanel implements ActionListener {
	
	static public boolean isInstalled(String name){
		// TODO: Check whether this extension has been already installed
		if (name.compareTo("Sikuli Copier") == 0)
			return true;
		return false;
	}
	
	JLabel _description;
	
	JButton _installCtrl;
	JButton _infoCtrl;
	
	String _name;
	String _infourl;
	String _jarurl;
	boolean _installed;
	
	JPanel _controls;
	JPanel _content;
	public Extension(String name, String description, 
			String imgurl, String infourl, String jarurl){
		this._name = name;
		this._infourl = infourl;
		this._jarurl = jarurl;
		this._installed = isInstalled(name);

		setLayout(new BoxLayout(this, BoxLayout.Y_AXIS));
		
		Border loweredetched = BorderFactory.createEtchedBorder(EtchedBorder.LOWERED);
		setBorder(loweredetched);

				
		_content = new JPanel();
		
		
		Image image = null;
		try {
		    // Read from a URL
		    URL url = new URL(imgurl);
		    image = ImageIO.read(url);
		} catch (IOException e) {
		}

		JLabel iconLabel = new JLabel();
		iconLabel.setIcon(new ImageIcon(image));

		_content.setLayout(new BorderLayout());
		_content.add(iconLabel, BorderLayout.LINE_START);
		JLabel htmlLabel = new JLabel(renderHTML(name,description));
		_content.add(htmlLabel);
		
		add(_content);
		
		JButton btn = new JButton("Install");
		btn.addActionListener(this);
		btn.setActionCommand("Install"); 
		_installCtrl = btn;
		_installCtrl.setFocusable(false);
		
		btn = new JButton("More info");
		btn.addActionListener(this);
		btn.setActionCommand("Info");
		_infoCtrl = btn;
		_infoCtrl.setFocusable(false);

		
		_controls = new JPanel();
		_controls.setLayout(new BorderLayout());

		_controls.add(_infoCtrl,BorderLayout.LINE_START);
		_controls.add(_installCtrl,BorderLayout.LINE_END);
		
		
		add(_controls);
		
		_controls.setVisible(false);
		updateControlls();
		
		
		addMouseListener(new MouseAdapter(){
			
			public void mousePressed(MouseEvent e){
				
				System.out.println("clicked");
				ManagerFrame.getInstance().select((Extension)e.getSource());
			}
		});
	}
	
	
	public void setSelected(boolean selected){
		_controls.setVisible(selected);
		
		
		Color darkRed = new Color(0.5f,0.0f,0.0f);
		
		Color bg,fg;
		if (selected){
			bg = darkRed;//Color.red;
			fg = Color.white;
		}else{
			bg = null;
			fg = Color.black;
		}
		_controls.setBackground(bg);
		_content.setBackground(bg);
		
		for (Component comp : _content.getComponents()){
			comp.setForeground(fg);
		}
		
	}
	
	
	public String renderHTML(String name, String description){
		return "<html><b>" + name + "</b><br>"
			+ description + "</html>";
	}
	
	public void updateControlls(){
		
		if (_installed){
			_installCtrl.setEnabled(false);
			_installCtrl.setText("Already installed");
		}else{
			_installCtrl.setEnabled(true);
			_installCtrl.setText("Install");
		}
	}
	

	
	@Override
	public void actionPerformed(ActionEvent e) {
		// TODO Implement actions
		String cmd = e.getActionCommand();
		if (cmd.equals("Install")){
			System.out.println("Installing " + _name + " at " + _jarurl);
			
			// DO INSTALLATION
			
			// upon completion
			_installed = true;
			updateControlls();
			
		}else if (cmd.equals("Info")){
			System.out.println("Openning URL " + _infourl);	
		}
		
	}
}