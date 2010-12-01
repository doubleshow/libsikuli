/**
 * 
 */
package history.controls;

public interface NavigationIterator{
	boolean hasBefore();
	Object getCurrent();
	boolean hasAfter();	
	Object getBefore();
	Object getAfter();
	Object get(int i);
}