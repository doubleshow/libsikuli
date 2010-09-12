/**
 * 
 */
package history;


import java.util.ArrayList;

public class HistoryScreenDatabase{
		
		static ArrayList<HistoryScreen> history_screens = new ArrayList<HistoryScreen>();
		
		static {//public HistoryScreenDatabase(){
			String path = "testdata/images";
			String files[] = {"scenario-deadline-mail.png",
					"scenario-chi4.png",
					"scenario-chi3.png",
					"scenario-chi1.png",
					"scenario-chi2.png"
			};
			
			for (int i=0; i < files.length; ++i){
				HistoryScreen hs = new HistoryScreen(path + "/" + files[i], i);
				history_screens.add(hs);
			}
			
		}
		
		
		static public HistoryScreen find(int id){
			return history_screens.get(id);
		}
		
		static public HistoryScreen getMostRecent(){
			return history_screens.get(0);
		}
		

	}