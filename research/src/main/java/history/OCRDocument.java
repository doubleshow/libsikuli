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

import vision.OCRText;
import vision.OCRWords;

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


	Map<String,Rectangles> _word_to_rectangles;
	ArrayList<OCRWord> _words = new ArrayList<OCRWord>();

	
	
	
	public OCRDocument(BufferedImage image){
		
		_word_to_rectangles = new Hashtable<String,Rectangles>();
		
		OCRText ocrtext = SikuliVision.ocr(image);

		OCRWords ws = ocrtext.getWords();
		
		for (int i=0; i < ws.size(); ++i){
			vision.OCRWord w = (vision.OCRWord) ws.get(i);
			String wstr = w.getString();
			
			String nstr = normalize(wstr);
			
			Rectangle rect = new Rectangle(w.getX(),w.getY(),w.getWidth(),w.getHeight());
			OCRWord word = new OCRWord(w.getString(), rect);
			
			if (_word_to_rectangles.containsKey(nstr)){
				Rectangles rects = _word_to_rectangles.get(nstr);
				rects.add(rect);

			}else{

				Rectangles rects = new Rectangles();
				rects.add(rect);
				_word_to_rectangles.put(nstr, rects);		 
			}

			_words.add(word);
			
		}
	}

	private void parseFile(String filename){

		_word_to_rectangles = new Hashtable<String,Rectangles>();

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


		if (_word_to_rectangles.containsKey(word)){
			Rectangles rects = _word_to_rectangles.get(word);
			rects.add(rect);

		}else{

			Rectangles rects = new Rectangles();
			rects.add(rect);
			_word_to_rectangles.put(word, rects);		 
		}

		_words.add(new OCRWord(word, rect));

	}


	public ArrayList<OCRWord> getWords() {
		return _words;
	}

	public boolean hasWord(String word){
		return _word_to_rectangles.containsKey(word);
	}
	
	public Rectangles find(String word){
		return _word_to_rectangles.get(word);
	}

	public OCRWord find(Rectangle rect){

		for (OCRWord word : _words){

			if (word.getRectangle().intersects(rect))
				return word;
		}

		return null;
	}


	public static void main(String[] args) throws IOException {

//		OCRDocument doc = new OCRDocument("test.loc");
//		Rectangles rects = doc.find("deadline");
//
//		for (Rectangle rect : rects){
//			System.out.println(rect);
//		}
		
		BufferedImage screen_image = ImageIO.read(new File("screen.png"));
		OCRDocument doc = new OCRDocument(screen_image);
		Rectangles rects = doc.find("happy");

		for (Rectangle rect : rects){
			System.out.println(rect);
		}
	}
}
