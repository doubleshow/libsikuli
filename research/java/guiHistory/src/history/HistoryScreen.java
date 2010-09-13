/**
 * 
 */
package history;

import history.HistoryViewer.Mode;

import java.awt.BasicStroke;
import java.awt.Color;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Rectangle;
import java.awt.Stroke;
import java.awt.image.BufferedImage;
import java.awt.image.RescaleOp;

public class HistoryScreen extends Screen
	implements Comparable{
			
	
	public HistoryScreen(String filename, int id){
		super(filename);
		     
		
		this.id = id;
		
	
	}
	
	public HistoryScreen getBefore(){
		return HistoryScreenDatabase.find(id + 1);
	}
	
	public HistoryScreen getAfter(){
		return HistoryScreenDatabase.find(id - 1);
	}
	
	int id;
	public int getId() {
		return id;
	}

	String timeString;
	
	public void setTimeString(String timeString) {
		this.timeString = timeString;
	}

	public String getTimeString(){
		return timeString;
	}

	@Override
	public int compareTo(Object o) {
		HistoryScreen hs = (HistoryScreen) o;
		return this.id - hs.id;    
	}
	
	@Override
	public void setHighlightedWord(String word) {
		highlightedWord = word;
		highlightRectangles = HistoryScreenDatabase.findRectangles(id, highlightedWord);
	}

	
}