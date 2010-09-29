package history;
import java.awt.Rectangle;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Hashtable;
import java.util.Map;
import java.util.Scanner;

import javax.imageio.ImageIO;

public class OCRDocument {

	class OCRWord {

		OCRWord(String string, Rectangle rectangle){
			this.string = string;
			this.rectangle = rectangle;
		}

		private String string;
		private Rectangle rectangle;
		public void setWord(String string) {
			this.string = string;
		}
		public String getString() {
			return string;
		}
		public void setRectangle(Rectangle rectangle) {
			this.rectangle = rectangle;
		}
		public Rectangle getRectangle() {
			return rectangle;
		}
	}


	Map<String,Rectangles> word_to_rectangles;
	ArrayList<OCRWord> words = new ArrayList<OCRWord>();

	public OCRDocument(BufferedImage image){

		String outputname = Sikuli.ocr(image);
		parseFile(outputname);

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

			Collections.reverse(words);


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
		in = in.replace('?',' ');	
		in = in.replace(')',' ');	
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


		Rectangle rect = new Rectangle(x,y,w,h);


		if (word_to_rectangles.containsKey(word)){
			Rectangles rects = word_to_rectangles.get(word);
			rects.add(rect);

		}else{

			Rectangles rects = new Rectangles();
			rects.add(rect);
			word_to_rectangles.put(word, rects);		 
		}

		words.add(new OCRWord(word, rect));

	}


	public ArrayList<OCRWord> getWords() {
		return words;
	}

	public Rectangles find(String word){
		return word_to_rectangles.get(word);
	}

	public OCRWord find(Rectangle rect){

		for (OCRWord word : words){

			if (word.getRectangle().intersects(rect))
				return word;
		}

		return null;
	}


	public static void main(String[] args) {

		OCRDocument doc = new OCRDocument("test.loc");
		Rectangles rects = doc.find("deadline");

		for (Rectangle rect : rects){
			System.out.println(rect);
		}
	}
}
