package org.sikuli.ide.extmanager;

import java.awt.AWTException;
import java.awt.BorderLayout;
import java.awt.Container;
import java.awt.Dimension;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.io.BufferedInputStream;
import java.io.BufferedReader;
import java.io.DataInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.URL;
import java.net.URLConnection;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.Map;

import javax.swing.BoxLayout;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JRootPane;
import javax.swing.JTextField;

import org.json.simple.JSONArray;
import org.json.simple.JSONValue;


public class ManagerFrame extends JFrame {

	final static String EXTENSION_LIST_URL = "http://depot.sikuli.org/extensions.json";


	static ManagerFrame _instance = null;
	static ManagerFrame getInstance(){
		if (_instance == null)
			_instance = new ManagerFrame();
		return _instance;
	}


	ArrayList<Extension> _extensions;

	public ManagerFrame(){
		super();

		setTitle("Extensions");
		//setMinimumSize(new Dimension(400,200));
		setResizable(false);
		createComponents();

		addKeyListener(new SelectExtensionKeyListener());

		pack();
	}

	void createComponents(){
		Container pane;

		pane = getContentPane();
		pane.setLayout(new BoxLayout(pane, BoxLayout.Y_AXIS));

		try{

			// Populate the list of extensions
			_extensions = retrieveExtensions();

			for (Extension ext : _extensions){
				pane.add(ext);;
			}

			// Select the first one
			select(0);

		} catch(IOException io){
			//System.out.println("Una " + io.getMessage());
			String msg = "Unable to load extensions from the server: " + io.getMessage();
			JTextField txt = new JTextField(msg);
			txt.setBackground(null);
			txt.setBorder(null);
			txt.setEditable(false);
			pane.add(txt);
		}

		JPanel bottomBar = new JPanel();
		bottomBar.setLayout(new BorderLayout());
		bottomBar.setMinimumSize(new Dimension(400,20));
		JButton closeBtn = new JButton("Close");

		closeBtn.addActionListener(new ActionListener(){

			@Override
			public void actionPerformed(ActionEvent arg0) {
				dispose();
			}
			
		});
		
		
		closeBtn.setFocusable(false);
		bottomBar.add(closeBtn, BorderLayout.LINE_END);
		pane.add(bottomBar);
		
	}

	ArrayList<Extension> retrieveExtensions() throws IOException{

		ArrayList<Extension> extensions = new ArrayList<Extension>();

		System.out.println("Retrieving from " + EXTENSION_LIST_URL);
		
		String json = ManagerFrame.html2txt(EXTENSION_LIST_URL);
		System.out.println(json);

		Object obj=JSONValue.parse(json);
		Map map=(Map)obj;

		Map extension_list = (Map) map.get("extension-list");
		List exts = (List) extension_list.get("extensions");

		for (Object o : exts){
			Map ext = (Map) o;

			String name = (String) ext.get("name");
			System.out.println(name);

			String description = (String) ext.get("description");
			String imgurl = (String) ext.get("imgurl");
			String infourl = (String) ext.get("infourl");
			String jarurl = (String) ext.get("jarurl");

			extensions.add(new Extension(name,description,imgurl,infourl,jarurl));
		}

		return extensions;
	}

	static String html2txt(String urlstring) throws IOException {
		URL url = new URL(urlstring);
		URLConnection yc = url.openConnection();
		yc.setConnectTimeout(5000);
		BufferedReader in = new BufferedReader(
				new InputStreamReader(
						yc.getInputStream()));
		String txt = "";
		String inputLine;

		while ((inputLine = in.readLine()) != null) {
			txt = txt + inputLine;
		}
		in.close();	

		return txt;

	}

	//	static String read_json_test() throws IOException {
	//        URL yahoo = new URL("http://localhost:4567/extensions.json" );
	//        URLConnection yc = yahoo.openConnection();
	//        BufferedReader in = new BufferedReader(
	//                                new InputStreamReader(
	//                                yc.getInputStream()));
	//        String json = "";
	//        String inputLine;
	//
	//        while ((inputLine = in.readLine()) != null) {
	//        	json = json + inputLine;
	//        }
	//        in.close();	
	//        
	//        return json;
	//
	//	}

	//	static String read_json_test1() throws IOException{
	//		File file = new File("extensions.json");
	//		BufferedReader reader = new BufferedReader(new FileReader(file));
	//
	//		String json = "";
	//        //... Loop as long as there are input lines.
	//        String line = null;
	//        while ((line=reader.readLine()) != null) {
	//            System.out.println(line);
	//            json = json + line;
	//        }
	//
	//        //... Close reader and writer.
	//        reader.close();  // Close to unlock.
	//        
	//        return json;
	//	}

	int selected_idx = 0; 
	void select(Extension ext){
		int idx = _extensions.indexOf(ext);
		select(idx);
	}

	void select(int idx){
		_extensions.get(selected_idx).setSelected(false);
		selected_idx = idx;
		_extensions.get(selected_idx).setSelected(true);
	}

	void selectPrevious(){
		if (selected_idx == 0){
			return;
		}

		select(selected_idx - 1);
	}

	void selectNext(){
		if (selected_idx == _extensions.size() - 1){
			return;
		}

		select(selected_idx + 1);
	}


	class SelectExtensionKeyListener implements KeyListener {

		@Override
		public void keyPressed(KeyEvent e) {
			int key = e.getKeyCode();
			if (key == KeyEvent.VK_UP) {
				selectPrevious();
			}else if (key == KeyEvent.VK_DOWN){
				selectNext();
			}

		}

		@Override
		public void keyReleased(KeyEvent arg0) {
		}

		@Override
		public void keyTyped(KeyEvent arg0) {
		}

	}

	public static void main(String[] args) throws IOException{

		//		String json = read_json_test();
		//		System.out.println(json);
		//		
		//		Object obj=JSONValue.parse(json);
		//		Map map=(Map)obj;
		//		
		//		Map extension_list = (Map) map.get("extension-list");
		//		List extensions = (List) extension_list.get("extensions");
		//		
		//		for (Object o : extensions){
		//			Map ext = (Map) o;
		//			System.out.println(ext.get("name"));
		//		}


		ManagerFrame f = ManagerFrame.getInstance();		
		f.setVisible(true);
	}

}
