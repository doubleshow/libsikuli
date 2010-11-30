/**
 * 
 */
package history;

public class HistoryScreen 
implements Comparable{

	String filename;
	

	public HistoryScreen(int id, String filename){
		this.id = id;
		this.filename = filename;
	}
	
	public HistoryScreen(){
	}

//	public HistoryScreen getBefore(){
//		return HistoryScreenDatabase.find(id + 1);
//	}
//
//	public HistoryScreen getAfter(){
//		return HistoryScreenDatabase.find(id - 1);
//	}
	
	
	public HistoryScreenImage createImage(){
		
		return new HistoryScreenImage(id,filename);
		
	}

	int id;
	public int getId() {
		return id;
	}

	public String getFilename() {
		return filename;
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

	public String toString(){
		return "[HistoryScreen " + id + "]";
	}

	public OCRDocument getOCRDocument() {
		return HistoryScreenDatabase.getOCRDocument(id);
	}
}