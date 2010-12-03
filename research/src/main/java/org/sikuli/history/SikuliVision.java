package org.sikuli.history;

import java.awt.Graphics2D;
import java.awt.Point;
import java.awt.Rectangle;
import java.awt.Transparency;
import java.awt.color.ColorSpace;
import java.awt.image.BufferedImage;
import java.awt.image.ColorModel;
import java.awt.image.ComponentColorModel;
import java.awt.image.DataBuffer;
import java.awt.image.DataBufferByte;
import java.awt.image.SampleModel;
import java.awt.image.WritableRaster;
import java.io.File;
import java.io.IOException;
import java.util.ArrayList;

import javax.imageio.ImageIO;

import vision.FindInput;
import vision.FindResults;
import vision.Mat;
import vision.OCRText;
import vision.Vision;

public class SikuliVision {

	static {
		System.out.println(System.getProperty("java.library.path"));
		System.loadLibrary("sikulicv");
	}


	public static ArrayList<Rectangle> find(BufferedImage screen_image, BufferedImage target_image){

		ArrayList<Rectangle> rectangles = new ArrayList<Rectangle>();

		Mat screen_mat = convertBufferedImageToMat(screen_image);
		Mat target_mat = convertBufferedImageToMat(target_image);

		FindInput findInput = new FindInput();
		findInput.setTarget(target_mat);
		findInput.setSource(screen_mat);
		findInput.setFindAll(true);


		FindResults results = null;
		results = Vision.find(findInput);

		for(int i=0;i<results.size();i++){

			vision.FindResult r = (vision.FindResult) results.get(i);
			System.out.println("x:" + r.getX());
			System.out.println("y:" + r.getY());
			System.out.println("Vector Element "+i+" :"+results.get(i));

			Rectangle rect = new Rectangle(r.getX(),r.getY(),r.getW(),r.getH());
			rectangles.add(rect);
		}

		return rectangles;
	}

	public static ArrayList<Rectangle> find(String screen_image_filename, BufferedImage target_image) throws IOException{
		BufferedImage screen_image = ImageIO.read(new File(screen_image_filename));
		return find(screen_image, target_image);
	}
	
	public static String find_ui(BufferedImage ui_image){		
		Mat ui_mat = convertBufferedImageToMat(ui_image);
		return Vision.query("img.db", ui_mat);
	}


	public static BufferedImage createBufferedImage(int w, int h)
	{
		ColorSpace cs = ColorSpace.getInstance(ColorSpace.CS_sRGB);
		int[] nBits = {8, 8, 8, 8};
		ColorModel cm = new ComponentColorModel(cs, nBits, 
				true, false,
				Transparency.TRANSLUCENT, 
				DataBuffer.TYPE_BYTE);

		SampleModel sm = cm.createCompatibleSampleModel(w, h);
		DataBufferByte db = new DataBufferByte(w*h*4); //4 channels buffer
		WritableRaster r = WritableRaster.createWritableRaster(sm, db, new Point(0,0));
		BufferedImage bm = new BufferedImage(cm,r,false,null);
		return bm;
	}

	public static byte[] convertBufferedImageToByteArray(BufferedImage img){
		BufferedImage cvImg = createBufferedImage(img.getWidth(), img.getHeight());
		Graphics2D g = cvImg.createGraphics();
		g.drawImage(img, 0, 0, null);
		g.dispose();
		return ((DataBufferByte)cvImg.getRaster().getDataBuffer()).getData();
	}
	public static Mat convertBufferedImageToMat(BufferedImage img){
		byte[] data = convertBufferedImageToByteArray(img);
		return Vision.createMat(img.getHeight(), img.getWidth(), data);
	}
	
	public static OCRText ocr(BufferedImage image){
		Mat mat = convertBufferedImageToMat(image);
		OCRText text = Vision.recognize_as_ocrtext(mat);
		return text;
	}

	public static void main(String[] args) throws IOException {

		BufferedImage screen = ImageIO.read(new File("screen.png"));		
		BufferedImage target = ImageIO.read(new File("chilogo.png"));

		//find(screen, target);
		Mat screen_mat = convertBufferedImageToMat(screen);
		OCRText text = Vision.recognize_as_ocrtext(screen_mat);
		
		System.out.println(text.getString());

	}
	
}
