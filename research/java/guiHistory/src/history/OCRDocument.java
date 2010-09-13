package history;
import java.awt.Rectangle;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.Hashtable;
import java.util.Map;
import java.util.Scanner;

import javax.imageio.ImageIO;

public class OCRDocument {
	
	public OCRDocument(BufferedImage image){
		
		try{
			//File tmp = File.createTempFile("sikulihistory", "selected_image")
	        File  f = new File("tmpocr.png");
			ImageIO.write(image, "png", f);
	      }
	      catch(IOException e){
	    	  
	      }
		
	      try {
	    	    // Execute a command without arguments
	    	    String command = "./ocr tmpocr.png";
	    	    Process child = Runtime.getRuntime().exec(command);
	    	    
	    	} catch (IOException e) {
	    	}
		
	}
	
	public OCRDocument(String input){
		word_to_rectangles = new Hashtable<String,Rectangles>();
		
		File fFile = new File(input);  

		Scanner scanner;
	    try {
	    	 scanner = new Scanner(fFile);
	    
	    
	    	//first use a Scanner to get each line
		      while (scanner.hasNextLine() ){
		        //processLine( scanner.nextLine() );
		    	  
		    	  String line = scanner.nextLine();
		    	  processLine(line);
		    	 //System.out.println(scanner.nextLine());
		      }
		      
		      scanner.close();
		      
	    } catch (FileNotFoundException e) {
			e.printStackTrace();
		}
		    		
	}
	
	String normalize(String in){
		
		//String out;
		in = in.replace('.',' ');
		in = in.replace(':',' ');
		in = in.toLowerCase();
		in = in.trim();
		return in;
	}
	
	private void processLine(String line){
		 Scanner scanner = new Scanner(line);
		 scanner.useDelimiter(" ");
		 
		 int x = scanner.nextInt();
		 int y = scanner.nextInt();
		 int w = scanner.nextInt();
		 int h = scanner.nextInt();
		 String word = scanner.next();
		 
		 word = normalize(word);
		 //System.out.println("("+word+")");
		 
		 
		 if (word_to_rectangles.containsKey(word)){
			 Rectangles rects = word_to_rectangles.get(word);
			 rects.add(new Rectangle(x,y,w,h));
		 }else{

			 Rectangles rects = new Rectangles();
			 rects.add(new Rectangle(x,y,w,h));
			 word_to_rectangles.put(word, rects);
			 
		 }
	    	
	}

	Map<String,Rectangles> word_to_rectangles;

	public Rectangles find(String word){
		return word_to_rectangles.get(word);
	}
	
	public static void main(String[] args) {
		
		OCRDocument doc = new OCRDocument("test.loc");
		Rectangles rects = doc.find("deadline");
		
		for (Rectangle rect : rects){
			System.out.println(rect);
		}
	}
}
