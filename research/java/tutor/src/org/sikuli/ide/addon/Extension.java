/**
 * 
 */
package org.sikuli.ide.addon;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.BoxLayout;
import javax.swing.JButton;
import javax.swing.JComponent;
import javax.swing.JLabel;
import javax.swing.JPanel;

class Extension extends JPanel {
	
	static public boolean isInstalled(String name){
		// TODO: Check whether this extension has been already installed
		if (name == "Sikuli Copier")
			return true;
		return false;
	}
	
	JLabel description;
	JButton _installCtrl;
	
	String _name;
	boolean _installed;
	
	public Extension(String name, String descript_html){
		this._name = name;
		this._installed = isInstalled(name);

		setLayout(new BoxLayout(this, BoxLayout.X_AXIS));
		
		description = new JLabel(descript_html);
		
		JButton btn = new JButton("Install");
		btn.addActionListener(new InstallActionListener());
		_installCtrl = btn;
		
		
		add(description);
		add(_installCtrl);
		
		//updateUI();
	}
	
	
	
	public void updateInstallCtrl(){
		
		if (_installed){
			_installCtrl.setEnabled(false);
			_installCtrl.setText("Installed");
		}else{
			_installCtrl.setEnabled(true);
		}
	}
	
	class InstallActionListener implements ActionListener{

		@Override
		public void actionPerformed(ActionEvent e) {
			// TODO: Do the installation
			System.out.println("Installing " + _name);
			
			
			_installed = true;
			updateInstallCtrl();
		}
		
	}
}