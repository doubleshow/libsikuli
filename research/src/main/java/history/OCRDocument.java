package history;
import java.awt.Rectangle;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Hashtable;
import java.util.Map;
import java.util.Scanner;

import javax.imageio.ImageIO;

public class OCRDocument {
	
	public OCRDocument(BufferedImage image){
		
		try{
			File f;
			
			f = File.createTempFile("ocrimage", ".png");
			f.deleteOnExit(); 
	        
			//File  f = new File("tmpocr.png");
			ImageIO.write(image, "png", f);
			
			 String command = "./ocr OCR " + f.getPath();
	    	 System.out.println(command);
			 Process child = Runtime.getRuntime().exec(command);
			
			 
			 try {
				 child.waitFor();
		//		Thread.sleep(1);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
	    	 parseFile(f.getPath() + ".ocr.loc");
	    	 
			
	      }
	      catch(IOException e){
	    	  
	      }
		
	     
	}
	
	private void parseFile(String filename){
		
		word_to_rectangles = new Hashtable<String,Rectangles>();
		
		File fFile = new File(filename);  
		
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
	
	public OCRDocument(String filename){
		
		parseFile(filename);
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
			 words.add(word);
		 }
	    	
	}

	Map<String,Rectangles> word_to_rectangles;
	ArrayList<String> words = new ArrayList<String>();

	public ArrayList<String> getWords() {
		return words;
	}

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
