/**
 * 
 */
package history;

import java.awt.image.BufferedImage;

public class HistoryQuery {
	
	String _text = null;
	BufferedImage _image = null;
	
	public HistoryQuery(String text){
		_text = text;
	}
	
	public HistoryQuery(BufferedImage image){
		_image = image;
	}
	
	public String getText() {
		return _text;
	}
	public void setText(String text) {
		this._text = text;
	}
	public BufferedImage getImage() {
		return _image;
	}
	public void setImage(BufferedImage image) {
		this._image = image;
	}
	
}