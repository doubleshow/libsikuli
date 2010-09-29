package history;

import history.OCRDocument.OCRWord;

import java.awt.Rectangle;
import java.util.ArrayList;

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

}
