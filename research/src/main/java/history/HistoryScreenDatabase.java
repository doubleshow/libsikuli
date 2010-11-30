/**
 * 
 */
package history;

import history.HistoryViewer.NavigationIterator;

import java.awt.Rectangle;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FilenameFilter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Date;

import org.apache.lucene.analysis.Analyzer;
import org.apache.lucene.analysis.standard.StandardAnalyzer;
import org.apache.lucene.document.Document;
import org.apache.lucene.index.IndexReader;
import org.apache.lucene.index.IndexWriter;
import org.apache.lucene.queryParser.MultiFieldQueryParser;
import org.apache.lucene.queryParser.ParseException;
import org.apache.lucene.queryParser.QueryParser;
import org.apache.lucene.search.IndexSearcher;
import org.apache.lucene.search.Query;
import org.apache.lucene.search.ScoreDoc;
import org.apache.lucene.search.Searcher;
import org.apache.lucene.search.TopScoreDocCollector;
import org.apache.lucene.store.FSDirectory;
import org.apache.lucene.util.Version;

import db.DerbyDB;

public class HistoryScreenDatabase{

	private static final File INDEX_DIR =  new File("index");
	static ArrayList<HistoryScreen> _history_screens = new ArrayList<HistoryScreen>();

	
	static ArrayList<String> _filenames = new ArrayList<String>();
	static ArrayList<Integer> _ids = new ArrayList<Integer>();

	
	static private String _current_index_path;
	static private String _current_example_name;
	
	
	static private DerbyDB _db;
	static {
		_db = new DerbyDB();
		_db.connect();
	};
	
	
	
	//static private int _last_id;
	static private int _current_id = 152;
	
	static private int getNewId(){
		_current_id ++;
		return _current_id;
	}
	
	static public void load(String name){
		_current_example_name = name;
		String root = "captured/" + name;
		

		File f1=new File(root);
		String files[]=f1.list(new FilenameFilter(){
			@Override
			public boolean accept(File dir, String name) {
				return name.endsWith("png");
			}
		});

		int num_files = files.length;
		
		// skip the last file because it may not finished
		load(name, num_files - 1, true);
//		for (int i=0;i<files.length;i++)
//		{System.out.println(files[i]);}
//		System.out.println(files.length + " files");
	}
	static public void load(String name, int n){
		load(name,n,false);
	}
	
	static public void load(String name, int n, boolean rebuild_index){
		String root = "captured/" + name;
		_current_example_name = name;
		
		_filenames.clear();
		_ids.clear();
		_history_screens.clear();
		
		for (int i=0; i < n ; ++i){
			String file = root + "/screen" + i + ".png";
			_filenames.add(file);
			
		}	
		
		
		DerbyDB t = new DerbyDB();
		
		//t.connect();
		//t.createTables();
		//t.insert();
		//t.list();
		
		for (String filename : _filenames){
			
			int new_id = getNewId();
			_ids.add(new_id);
			
			HistoryScreen hs = new HistoryScreen(new_id, filename);
			_history_screens.add(hs);
			
			_db.insertScreen(hs);
		}
		
		//.disconnect();
		
		
		_current_index_path = "captured/lucene.index/" + name;
		
		File index_file = new File(_current_index_path);
		if (rebuild_index || !index_file.exists()){
			indexOcrFiles(index_file);
		}
		
		System.out.println(_current_index_path +" loaded.");
	}
	
	static public Rectangles findRectangles(int id, String word, Rectangle filter){
		
		OCRDocument doc = getOCRDocument(id);
		
		String ws[] = word.split(" ");
		
		Rectangles result;
		result = doc.find(ws[0]);
		
		if (result == null)
			result = new Rectangles();
		
		Rectangles filtered_result = new Rectangles();
		for (Rectangle rect : result){
		
			if (filter == null || filter.contains(rect)){
				
				filtered_result.add(rect);
				
			}
			
		}
		
		return filtered_result;
	}
	
	static public OCRDocument getOCRDocument(int id){
		HistoryScreen hs = get(id);
		String file = hs.getFilename() + ".ocr.loc";
		return new OCRDocument(file);
	}

	static public OCRDocument getUIDocument(int id){
		HistoryScreen hs = get(id);
		String file = hs.getFilename() + ".ui.loc";
		return new OCRDocument(file);
	}

	
	static public HistoryScreen get(int id){
		return _db.findScreen(id);
	}
	
	static public HistoryScreen find(int id){
		return _history_screens.get(id);
	}

	static public HistoryScreen getMostRecent(){
		return _history_screens.get(_history_screens.size()-1);
	}

	static public FindResult getMostRecent(int n){
		FindResult fr = new FindResult();
		for (int i=0;i<n;++i){
			fr.add(_history_screens.get(_history_screens.size()-i-1));
		}
		return fr;
	}		
	
	
	static public HistoryScreenIterator getIterator(int id){
		return new HistoryScreenIterator(id);		
	}
	
	static public HistoryScreenIterator getIteratorFromEarliest(){
		return new HistoryScreenIterator(0);		
	}
	
	static public HistoryScreenIterator getShotIterator(int id){
		return new HistoryScreenShotIterator(id);		
	}

	static public class HistoryScreenShotIterator extends HistoryScreenIterator{

		public HistoryScreenShotIterator(int current_id) {
			super(current_id);
		}
		
		@Override
		public Object getAfter() {
			
			boolean same = true;
			while (same && hasAfter()){
				
				int next_id = current_id+1;
				String current_image_filename = _filenames.get(current_id);
				String next_image_filename = _filenames.get(next_id);
				
				double difference = Sikuli.compare(current_image_filename,next_image_filename);
				same = difference < 0.001;
				
				current_id = next_id;
			}
				
			return _history_screens.get(current_id);
		}

		@Override
		public Object getBefore() {
			boolean same = true;
			while (same && hasBefore()){
				
				int next_id = current_id-1;
				String current_image_filename = _filenames.get(current_id);
				String next_image_filename = _filenames.get(next_id);
				
				double difference = Sikuli.compare(current_image_filename,next_image_filename);
				same = difference < 0.001;
				
				current_id = next_id;
			}
			return _history_screens.get(current_id);
		}
		
	}
	
	static public class HistoryScreenIterator implements NavigationIterator {
		
		int current_id;
		
		public HistoryScreenIterator(int current_id){
			this.current_id = current_id;
		}

		@Override
		public Object getAfter() {
			current_id++;
			return _history_screens.get(current_id);
		}

		@Override
		public Object getBefore() {
			current_id--;
			return _history_screens.get(current_id);
		}

		@Override
		public boolean hasBefore() {
			return current_id > 0;
		}

		@Override
		public boolean hasAfter() {
			return current_id < _history_screens.size() - 1;
		}

		@Override
		public Object getCurrent() {
			return _history_screens.get(current_id);
		}

		@Override
		public Object get(int i) {
			current_id = i;
			return _history_screens.get(current_id);
		}
		
	}
	
	
//	String _index_dir;
//	static void select(String index_file){
//		
//		
//	}
	
	static void insert(String screen_image_filename){
		
		File index_file = new File(_current_index_path);
		
		try {

			IndexWriter writer = new IndexWriter(FSDirectory.open(index_file), 
					new StandardAnalyzer(Version.LUCENE_30), false, 
					IndexWriter.MaxFieldLength.LIMITED);
			
			int new_id = getNewId();
			_ids.add(new_id);
			HistoryScreen hs = new HistoryScreen(new_id, screen_image_filename);
			_history_screens.add(hs);
			
			writer.addDocument(FileDocument.Document("happy",screen_image_filename,new_id));
			writer.optimize();
			writer.close();
			
		}catch(IOException e){
			
		}
	}
	
	static void indexOcrFiles(File index_file){

		try {

			IndexWriter writer = new IndexWriter(FSDirectory.open(index_file), 
					new StandardAnalyzer(Version.LUCENE_30), true, 
					IndexWriter.MaxFieldLength.LIMITED);

			System.out.println("Indexing to directory '" +INDEX_DIR+ "'...");

			for (int i = 0; i < _filenames.size(); ++i){
				
				String filename = _filenames.get(i);
				Integer id = _ids.get(i);

				String ocr_filename = filename + ".ocr.txt";
				String ui_filename = filename + ".ui.txt";

				File ocr_file = new File(ocr_filename);
				File ui_file = new File(ui_filename);
				
				System.out.println("adding id = " + id + " : " + ocr_file);
				//System.out.println("adding " + ui_file);

				writer.addDocument(FileDocument.Document(ocr_file,ui_file,id));

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

	
	static public String getImageFilename(int id){
		return _filenames.get(id);
	}
	
	static public ArrayList<HistoryScreen> find(String query_string){
		
		ArrayList<HistoryScreen> ret = new ArrayList<HistoryScreen>();

		try{
			IndexReader reader = IndexReader.open(FSDirectory.open(new File(_current_index_path)), true); 
			// only searching, so read-only=true

			Searcher searcher = new IndexSearcher(reader);
			Analyzer analyzer = new StandardAnalyzer(Version.LUCENE_30);

			//QueryParser parser = new QueryParser(Version.LUCENE_30, field, analyzer);
		
			MultiFieldQueryParser parser = new MultiFieldQueryParser(Version.LUCENE_30,
			                                        new String[] {"ocr", "ui"},
			                                        analyzer);

		
			Query query = parser.parse(query_string);

			TopScoreDocCollector collector = TopScoreDocCollector.create(20, false);
			searcher.search(query, collector);

			ScoreDoc[] hits;
			hits = collector.topDocs().scoreDocs;

			for (int i = 0; i < hits.length; i++) {


				Document doc = searcher.doc(hits[i].doc);
				int id = Integer.parseInt(doc.get("id"));
				//String path = doc.get("path");

				String time = doc.get("modified");

				
				
				int j = _ids.indexOf(new Integer(id));
				
				HistoryScreen hs = _db.findScreen(id);
				if (hs != null){
					System.out.println("" + (i+1) +" : [lucene] id = " + id + "\t [derby] filename = " + hs.getFilename());
				}
				ret.add(hs);
			}
			
			Collections.sort(ret);
			
			
		}catch(Exception e){
			e.printStackTrace();
		}

		return ret;
	}



	public static void main(String[] args) throws Exception {

		//HistoryScreenDatabase.load("facebook", 19);
		//HistoryScreenDatabase.load("pilyoung", 1000);

		HistoryScreenDatabase.load("chi", 30, true);
		HistoryScreenDatabase.find("deadline");
		
		
		//HistoryScreenDatabase.insert("screen.png");
		//HistoryScreenDatabase.find("happy");
		
		//HistoryScreenDatabase.load("video", 15);
		
		//HistoryScreenDatabase.indexOcrFiles();
		//HistoryScreenDatabase.find("vancouver && conference");
		//HistoryScreenDatabase.find("ui64 AND volunteers");
	    
	}
	public static String getSikuliIndexFilename() {
		return "captured/" + _current_example_name + ".index";
	}
}