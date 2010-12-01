/**
 * 
 */
package org.sikuli.history;


import java.util.ArrayList;

import org.sikuli.history.controls.NavigationIterator;

class FindResult implements NavigationIterator {
	
	// a reversed chronologically ordered list of history screens
	ArrayList<HistoryScreen> history_screens = new ArrayList<HistoryScreen>();
	int current_index;
	
	public FindResult(String query_string){
		history_screens = HistoryScreenDatabase.find(query_string);
		current_index = 0;
	}
	
	public FindResult(){	
	}
	
	public static FindResult createMockResult(){
		
		FindResult fr = new FindResult();
		fr.history_screens.add(HistoryScreenDatabase.find(0));
		fr.history_screens.add(HistoryScreenDatabase.find(2));
		fr.history_screens.add(HistoryScreenDatabase.find(3));
		fr.current_index = 0;
		
		return fr;
	}
	
	public boolean hasBefore(){
		return current_index < history_screens.size() - 1;
	}
	
	public boolean hasAfter(){
		return current_index > 0;
	}
	

	public HistoryScreen getBefore(){
		current_index = current_index + 1;
		return history_screens.get(current_index);				
	}
	
	public HistoryScreen getAfter(){
		current_index = current_index - 1;
		return history_screens.get(current_index);	
	}
	
	public HistoryScreen getMostRecent(){
		return history_screens.get(0);
	}
	
	public ArrayList<HistoryScreen> getAll(){
		return history_screens;
	}

	public void add(HistoryScreen historyScreen) {
		history_screens.add(historyScreen);
	}

	public boolean isEmpty() {
		return history_screens.isEmpty();
	}

	@Override
	public Object getCurrent() {
		return history_screens.get(current_index);
	}
	
	@Override
	public Object get(int i){
		current_index = i;
		return history_screens.get(current_index);
	}
	
	
}