/**
 * 
 */
package history;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Date;

import org.apache.lucene.analysis.Analyzer;
import org.apache.lucene.analysis.standard.StandardAnalyzer;
import org.apache.lucene.document.Document;
import org.apache.lucene.index.IndexReader;
import org.apache.lucene.index.IndexWriter;
import org.apache.lucene.queryParser.ParseException;
import org.apache.lucene.queryParser.QueryParser;
import org.apache.lucene.search.IndexSearcher;
import org.apache.lucene.search.Query;
import org.apache.lucene.search.ScoreDoc;
import org.apache.lucene.search.Searcher;
import org.apache.lucene.search.TopScoreDocCollector;
import org.apache.lucene.store.FSDirectory;
import org.apache.lucene.util.Version;

public class HistoryScreenDatabase{

	private static final File INDEX_DIR =  new File("index");
	static ArrayList<HistoryScreen> history_screens = new ArrayList<HistoryScreen>();

	
	static final String ROOT_DIR = "captured/facebook";
	
	static final int TIME_OFFSET = 5;
	
	static ArrayList<String> filenames = new ArrayList<String>();
	static {

		for (int i=17; i >= 0 ; --i){
			String file = ROOT_DIR + "/screen" + i + ".png";
			filenames.add(file);
		}

		for (int i=0; i < filenames.size() ; ++i){
			HistoryScreen hs = new HistoryScreen(filenames.get(i), i);
			hs.setTimeString("" + (i + TIME_OFFSET) + " minutes ago");
			history_screens.add(hs);
		}

	}

	static public Rectangles findRectangles(int id, String word){
		
		String file = filenames.get(id) + ".ocr.loc";
		OCRDocument doc = new OCRDocument(file);
		
		String ws[] = word.split(" ");
		return doc.find(ws[0]);	
	}

	static public HistoryScreen find(int id){
		return history_screens.get(id);
	}

	static public HistoryScreen getMostRecent(){
		return history_screens.get(0);
	}

	static void indexOcrFiles(){

		try {

			IndexWriter writer = new IndexWriter(FSDirectory.open(INDEX_DIR), 
					new StandardAnalyzer(Version.LUCENE_30), true, 
					IndexWriter.MaxFieldLength.LIMITED);

			System.out.println("Indexing to directory '" +INDEX_DIR+ "'...");

			for (int i = 0; i < filenames.size(); ++i){
				
				String filename = filenames.get(i);

				//String ocr_filename = filename + ".ocr.txt";
				String ocr_filename = filename + ".ui.txt";

				File ocr_file = new File(ocr_filename);
				
				System.out.println("adding " + ocr_file);
				writer.addDocument(FileDocument.Document(ocr_file,i));

			}

			System.out.println("Optimizing...");
			writer.optimize();
			writer.close();
			System.out.println("done.");
			
		} catch (IOException e) {
			System.out.println(" caught a " + e.getClass() +
					"\n with message: " + e.getMessage());
		}


	}

	
	static public ArrayList<HistoryScreen> find(String query_string){
		
		ArrayList<HistoryScreen> ret = new ArrayList<HistoryScreen>();

		try{
			IndexReader reader = IndexReader.open(FSDirectory.open(new File("index")), true); 
			// only searching, so read-only=true

			String field = "contents";
			Searcher searcher = new IndexSearcher(reader);
			Analyzer analyzer = new StandardAnalyzer(Version.LUCENE_30);

			QueryParser parser = new QueryParser(Version.LUCENE_30, field, analyzer);

			Query query = parser.parse(query_string);

			TopScoreDocCollector collector = TopScoreDocCollector.create(10, false);
			searcher.search(query, collector);

			ScoreDoc[] hits;
			hits = collector.topDocs().scoreDocs;

			for (int i = 0; i < hits.length; i++) {


				Document doc = searcher.doc(hits[i].doc);
				int id = Integer.parseInt(doc.get("id"));
				String path = doc.get("path");

				System.out.println("" + id + ":" + path);
				
				ret.add(find(id));
			}
			
			Collections.sort(ret);
			
		}catch(Exception e){

		}

		return ret;
	}



	public static void main(String[] args) throws Exception {

		HistoryScreenDatabase.indexOcrFiles();
		
		//HistoryScreenDatabase.find("attendees && vancouver && conference");
		HistoryScreenDatabase.find("ui64");
	    
	}
}