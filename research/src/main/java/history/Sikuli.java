package history;

import java.awt.Rectangle;
import java.awt.image.BufferedImage;
import java.io.BufferedReader;
import java.io.File;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;

import javax.imageio.ImageIO;

public class Sikuli {

	
	public static String ocr(BufferedImage image){
		
		try {


			File f;
			File curdir = new File(".");
			f = File.createTempFile("ocrimage", ".png",curdir);
			f.deleteOnExit(); 

			//File  f = new File("tmpocr.png");
			ImageIO.write(image, "png", f);

			String command = "./sikulicmd OCR " + f.getPath();
			System.out.println(command);
			Process child = Runtime.getRuntime().exec(command);


			child.waitFor();
			
			return f.getPath() + ".ocr.loc";

		} catch (InterruptedException e) {
			e.printStackTrace();
		} catch (IOException e){

		}
	
		return null;
	}
	
	public static double compare(String image1, String image2){
		String command = "./sikulicmd COMPARE " +  image1 + " " + image2;
		String result = "";
		System.out.println(command);
		Process p;
		try {
			p = Runtime.getRuntime().exec(command);

			BufferedReader input = new BufferedReader
			(new InputStreamReader(p.getInputStream()));

			String line;
			while ((line = input.readLine()) != null) {
				System.out.println(line);
				result = line;
			}
			input.close();
		} catch (IOException e) {
		}

		return Double.parseDouble(result);
	}
	
	public static double compare(BufferedImage image1, BufferedImage image2){
		
		String result = "";
		try{
			File f1,f2;
			File curdir = new File(".");
			f1 = File.createTempFile("sikuli", ".png",curdir);
			f2 = File.createTempFile("sikuli", ".png",curdir);			
			f1.deleteOnExit(); 
			f2.deleteOnExit(); 
			
			ImageIO.write(image1, "png", f1);
			ImageIO.write(image2, "png", f2);

			return compare(f1.getPath(),f2.getPath());
						
		} catch (IOException e){

		}

		return 0;
	}
	
	public static String find_ui(BufferedImage ui_image){
		String result = "";
		try{
			File f;
			File curdir = new File(".");
			f = File.createTempFile("uiimage", ".png",curdir);
			f.deleteOnExit(); 

			//File  f = new File("tmpocr.png");
			ImageIO.write(ui_image, "png", f);

			String index_filename = HistoryScreenDatabase.getSikuliIndexFilename();
			
			String command = "./sikulicmd QUERY " + index_filename + " " + f.getPath();
			System.out.println(command);
			Process p = Runtime.getRuntime().exec(command);

			
			BufferedReader input = new BufferedReader
			(new InputStreamReader(p.getInputStream()));
		
			int x=-1,y=-1;
			String line;
			while ((line = input.readLine()) != null) {
				System.out.println(line);
				//String[] xy = line.split(" ");
				//x = Integer.parseInt(xy[0]);
				//y = Integer.parseInt(xy[1]);
				result = line;
			}
			input.close();
			
						
		} catch (IOException e){

		}

		
		
		return result;
	}
	
	public static ArrayList<Rectangle> find(BufferedImage screen_image, BufferedImage target_image){
		try{
			File f;
			
			f = File.createTempFile("screen", ".png");
			f.deleteOnExit();
			ImageIO.write(screen_image, "png", f);
			
			return find(f.getPath(), target_image);
		}	
		catch(IOException e){
			e.printStackTrace();
			return null;
		}
		
		
	}

	
	
	public static ArrayList<Rectangle> find(String screen_image, BufferedImage target_image){
	
		ArrayList<Rectangle> rectangles = new ArrayList<Rectangle>();
		
		try{
			File f;
			String line;

			f = File.createTempFile("target", ".png");
			f.deleteOnExit(); 

			ImageIO.write(target_image, "png", f);

			String command = "./sikulicmd MATCH " + screen_image + " " + f.getPath();
			//System.out.println(command);
			Process p = Runtime.getRuntime().exec(command);

			BufferedReader input = new BufferedReader
			(new InputStreamReader(p.getInputStream()));
		
			int x=-1,y=-1;
			while ((line = input.readLine()) != null) {
				//System.out.println(line);
				String[] xy = line.split(" ");
				x = Integer.parseInt(xy[0]);
				y = Integer.parseInt(xy[1]);
				
				if (x>=0 && y>=0)
					rectangles.add(new Rectangle(x,y,target_image.getWidth(),target_image.getHeight()));
				
			}
			input.close();
		
			
			
		}
		catch(IOException e){
			e.printStackTrace();
		}
		
		return rectangles;
	}
	
	public static void main(String[] args) {
		
		BufferedImage image1, image2;
		try {
			//image = ImageIO.read(new File("query.png"));
			//Sikuli.find_ui(image);
			
			image1 = ImageIO.read(new File("screen.png"));
			image2 = ImageIO.read(new File("screen2.png"));
			double ret = Sikuli.compare(image1, image2);
			System.out.println(ret);
			
		} catch (IOException e) {
			e.printStackTrace();
		}

		
	}
	
}
