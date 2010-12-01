package org.sikuli.history;

import java.awt.Color;
import java.awt.Rectangle;
import java.util.ArrayList;

import org.sikuli.history.OCRDocument.OCRWord;

public class HistoryScreenImage extends ScreenImage {

	int id;
	
	public HistoryScreenImage(int id, String filename){
		super(filename);
		this.id = id;
	}
	
	public void highlightWord(String word, Rectangle filter) {
		Rectangles rects = HistoryScreenDatabase.findRectangles(id, word, filter);
		
		for (Rectangle rect : rects){				
				addHighlight(rect);	
		}
		
	}
	
	public void highlightAllIcons() {
		
		OCRDocument doc = HistoryScreenDatabase.getUIDocument(id);			
		ArrayList<OCRWord> words = doc.getWords();
		for (OCRWord word : words){
			Rectangle rect = word.getRectangle();
			
			// ignore really large icons (something is wrong)
			if (rect.width>500 || rect.height>500){
				continue;
			}
			
			AnnotationHighlight highlight = new AnnotationHighlight(this,rect);
			highlight.setBorder(false);

			addAnnotation(highlight);
			
			addYellowText(word.getString(),rect.x,rect.y);
		}			

	}
	
	public void highlightAllWords() {
		OCRDocument doc = HistoryScreenDatabase.getOCRDocument(id);			
		ArrayList<OCRWord> words = doc.getWords();
		for (OCRWord word : words){
			Rectangle rect = word.getRectangle();
			
			
			AnnotationHighlight highlight = new AnnotationHighlight(this,rect);
			highlight.setBorder(false);

			addAnnotation(highlight);
			
			addText(word.getString(),rect.x,rect.y);
		}			
	}

	private void addText(String string, int x, int y) {
		annotations.add(new AnnotationText(string,x,y));
	}
	
	private void addYellowText(String string, int x, int y) {
		AnnotationText at = new AnnotationText(string,x,y);
		at.setColor(Color.yellow);
		annotations.add(at);
	}

}
