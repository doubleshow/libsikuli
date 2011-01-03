package org.sikuli.ide.addon;

import java.awt.AWTException;
import java.awt.Dimension;
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
import java.util.Iterator;
import java.util.List;
import java.util.Map;

import javax.swing.BoxLayout;
import javax.swing.JFrame;
import javax.swing.JPanel;

import org.json.simple.JSONArray;
import org.json.simple.JSONValue;


public class InstallerFrame extends JFrame {
	
	JPanel panel;
	public InstallerFrame(){
		super();
	
		setTitle("Sikuli Extensions Installer");
		setMinimumSize(new Dimension(600,200));
		createPanel();
	}
	
	void createPanel(){
		panel = new JPanel();
		panel.setLayout(new BoxLayout(panel, BoxLayout.Y_AXIS));
		add(panel);
	}
	
	void addExtension(Extension ext){
		panel.add(ext);
	}
	
	void test(){
		
		String json = null;
		try {
			json = InstallerFrame.read_json_test();
		} catch (IOException e) {
			e.printStackTrace();
		}
		System.out.println(json);
		
		Object obj=JSONValue.parse(json);
		Map map=(Map)obj;
		
		Map extension_list = (Map) map.get("extension-list");
		List extensions = (List) extension_list.get("extensions");
		
		for (Object o : extensions){
			Map ext = (Map) o;
			
			String name = (String) ext.get("name");
			System.out.println(name);
			
			String html = "<html><h2>" + name + "</h2></html>";
			addExtension(new Extension(name,html));
		}
		
		
	}
	
	static String read_json_test() throws IOException {
        URL yahoo = new URL("http://localhost:4567/extensions.json" );
        URLConnection yc = yahoo.openConnection();
        BufferedReader in = new BufferedReader(
                                new InputStreamReader(
                                yc.getInputStream()));
        String json = "";
        String inputLine;

        while ((inputLine = in.readLine()) != null) {
        	json = json + inputLine;
        }
        in.close();	
        
        return json;

	}
	
	static String read_json_test1() throws IOException{
		File file = new File("extensions.json");
		BufferedReader reader = new BufferedReader(new FileReader(file));

		String json = "";
        //... Loop as long as there are input lines.
        String line = null;
        while ((line=reader.readLine()) != null) {
            System.out.println(line);
            json = json + line;
        }

        //... Close reader and writer.
        reader.close();  // Close to unlock.
        
        return json;
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
		
		
		InstallerFrame f = new InstallerFrame();
		
		
		f.test();
		
		
		f.pack();
		f.setVisible(true);
	}
	
}
