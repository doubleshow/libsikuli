package history;

public class HistoryScreenImage extends ScreenImage {

	int id;
	
	public HistoryScreenImage(int id, String filename){
		super(filename);
		this.id = id;
	}
	
	public void addHighlightedWord(String word) {
		Rectangles rects = HistoryScreenDatabase.findRectangles(id, word);
		addHighlightedRectangles(rects);		
	}

}
