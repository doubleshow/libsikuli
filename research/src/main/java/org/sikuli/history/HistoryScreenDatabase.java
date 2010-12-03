/**
/**
 * 
 */
/**
/**
 * 
 */
package org.sikuli.history;


import java.awt.AWTException;
import java.awt.Rectangle;
import java.awt.Robot;
import java.awt.Toolkit;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.FilenameFilter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.Comparator;

import javax.imageio.ImageIO;

import org.apache.lucene.analysis.Analyzer;
import org.apache.lucene.analysis.standard.StandardAnalyzer;
import org.apache.lucene.document.Document;
import org.apache.lucene.index.IndexReader;
import org.apache.lucene.index.IndexWriter;
import org.apache.lucene.queryParser.MultiFieldQueryParser;
import org.apache.lucene.search.IndexSearcher;
import org.apache.lucene.search.Query;
import org.apache.lucene.search.ScoreDoc;
import org.apache.lucene.search.Searcher;
import org.apache.lucene.search.TopScoreDocCollector;
import org.apache.lucene.store.FSDirectory;
import org.apache.lucene.util.Version;
import org.grlea.log.SimpleLogger;
import org.sikuli.history.controls.NavigationIterator;
import org.sikuli.history.db.DerbyDB;

public class HistoryScreenDatabase{

	private static final SimpleLogger log = new SimpleLogger(HistoryScreenDatabase.class);
//	private final static Logger LOGGER = Logger.getLogger(HistoryScreenDatabase.class .getName());

	
	private static final File INDEX_DIR =  new File("index");
	static ArrayList<HistoryScreen> _history_screens = new ArrayList<HistoryScreen>();
	
	static String _root = ".";
	static public void setRoot(String root){
		_root = root;
		_current_index_path = _root + "/lucene";
		_image_save_path = _root + "/screens";
		_sqldb_path = _root + "/derby";
		
		new File(_image_save_path).mkdir();
		
		_sqldb = new DerbyDB();
		_sqldb.connect(_sqldb_path);
	}
	
	static ArrayList<String> _filenames = new ArrayList<String>();
	static ArrayList<Integer> _ids = new ArrayList<Integer>();

	
	static private String _current_index_path;
	static private String _current_example_name;
	static private String _image_save_path;
	
	static private DerbyDB _sqldb;
	static private String _sqldb_path;
	
	
	
	//static private int _last_id;
	static private int _current_id = 300;
	
	static private int getNewId(){
		_current_id ++;
		return _current_id;
	}
	
	static public void load0(String name){
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
	
	static public void load(String name){
		setRoot("databases/" + name);
//		_history_screens = _sqldb.getScreens();
//		return;
	}
	
	
	public void create(String new_dir){
		setRoot(new_dir);
		new File(new_dir).mkdir();
		createEmptyIndex(_current_index_path);
		_sqldb.createTables();
	}
	
	public void import_from_old(String src_dir, String dest_dir){	
		
		//create(dest_dir);
		
//		setRoot("dest_dir");
//		createEmptyIndex(_current_index_path);
//		_sqldb.createTables();
		
		//_sqldb.reset();
		
		File[] files = (new File(src_dir)).listFiles(new FilenameFilter(){

			@Override
			public boolean accept(File dir, String name) {
				return name.endsWith("png");
			}
			
		});

		// sort by last modified time
		Arrays.sort(files, new Comparator<File>(){
		    public int compare(File f1, File f2)
		    {
		        return Long.valueOf(f1.lastModified()).compareTo(f2.lastModified());
		    } });
		
		log.info("Found " + files.length + " png images to import");
		
		for (File file : files){
			String filename = file.getPath();
			String ocr_filename = filename + ".ocr.loc";
			String image_filename = filename;

			log.info("importing " + filename);

			if (new File(ocr_filename).exists()){
				import_one_screen_from_old_version(image_filename, ocr_filename);
			}
		}
		
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
			
			_sqldb.insertScreen(hs);
		}
		
		//.disconnect();
		
		
		_current_index_path = "captured/lucene.index/" + name;
		
		File index_file = new File(_current_index_path);
		if (rebuild_index || !index_file.exists()){
			indexOcrFiles(index_file);
		}
		
		System.out.println(_current_index_path +" loaded.");
	}
	
	
	static public void insert(String screen_image_filename, Document ocr_doc){
		
		int new_id = getNewId();
		_ids.add(new_id);
		
		HistoryScreen hs = new HistoryScreen(new_id, screen_image_filename);
		_history_screens.add(hs);
		
		_sqldb.insertScreen(hs);
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
		String file = hs.getFilename() + ".ocr";
		return new OCRDocument(file);
	}

	static public OCRDocument getUIDocument(int id){
		HistoryScreen hs = get(id);
		String file = hs.getFilename() + ".ui";
		return new OCRDocument(file);
	}

	
	static public HistoryScreen get(int id){
		return _sqldb.findScreen(id);
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
			return HistoryScreenDatabase.get(current_id);
		}

		@Override
		public Object getBefore() {
			current_id--;
			return HistoryScreenDatabase.get(current_id);
		}

		@Override
		public boolean hasBefore() {
			return HistoryScreenDatabase.get(current_id-1) != null;
		}

		@Override
		public boolean hasAfter() {
			return HistoryScreenDatabase.get(current_id+1) != null;
		}

		@Override
		public Object getCurrent() {
			return HistoryScreenDatabase.get(current_id);
		}

		@Override
		public Object get(int i) {
			current_id = i;
			return HistoryScreenDatabase.get(current_id);
		}
		
	}
	
	
	static void insert0(String screen_image_filename){
		
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
	
	
	static void createEmptyIndex(String path){
		try{
			IndexWriter writer = new IndexWriter(FSDirectory.open(new File(path)), 
					new StandardAnalyzer(Version.LUCENE_30), true, 
					IndexWriter.MaxFieldLength.LIMITED);
			writer.close();
		} catch (IOException e) {
			log.errorException(e);
		}
	}
	
	
	static void indexOCRDocument(int id, OCRDocument doc, OCRDocument ui){
		
		try {			
			IndexWriter writer = new IndexWriter(FSDirectory.open(new File(_current_index_path)), 
					new StandardAnalyzer(Version.LUCENE_30), false, 
					IndexWriter.MaxFieldLength.LIMITED);

			writer.addDocument(FileDocument.Document(doc.getString(), ui.getString(), id));
			writer.optimize();
			writer.close();
			
		} catch (IOException e) {
			log.errorException(e);
		}
	}

	
	static void indexOCRDocument(int id, OCRDocument doc){
		
		try {			
			IndexWriter writer = new IndexWriter(FSDirectory.open(new File(_current_index_path)), 
					new StandardAnalyzer(Version.LUCENE_30), false, 
					IndexWriter.MaxFieldLength.LIMITED);

			writer.addDocument(FileDocument.Document(doc.getString(), "", id));
			writer.optimize();
			writer.close();
			
		} catch (IOException e) {
			log.errorException(e);
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

	static public ArrayList<HistoryScreen> find(String query_string){
		
		log.infoObject("query_string",query_string);
		
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
				
				HistoryScreen hs = _sqldb.findScreen(id);
				if (hs != null){
					log.info("" + (i+1) +" : [lucene] id = " + id + "\t [derby] filename = " + hs.getFilename());
				}
				ret.add(hs);
			}
			
			Collections.sort(ret);
			
			
		}catch(Exception e){
			e.printStackTrace();
		}

		return ret;
	}

	public void testRecorder(){
		
		Recorder r = new Recorder();
		r.run();

	}
	
	
	String getScreenImageFilename(int id){
		return _image_save_path + "/" + id + ".png";
	}
	
	void import_one_screen_from_old_version(String src_image_filename, String src_ocr_filename){
		int id = getNewId();

		String dest_image_filename = _image_save_path + "/" + id + ".png";
		String dest_ocr_filename = _image_save_path + "/" + id + ".png.ocr";
		String dest_ui_filename = _image_save_path + "/" + id + ".png.ui";
			
		(new File(_image_save_path)).mkdir();
		
		try {
			Process p;
			
			if (!new File(dest_image_filename).exists()){
				p = Runtime.getRuntime().exec("cp " + src_image_filename + " " + dest_image_filename);
				p.waitFor();
			}

			if (!new File(dest_ocr_filename).exists()){
				p = Runtime.getRuntime().exec("cp " + src_ocr_filename + " " + dest_ocr_filename);
				p.waitFor();
			}

		} catch (IOException e) {
			e.printStackTrace();
			
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		
		OCRDocument doc = new OCRDocument(dest_ocr_filename);		
		OCRDocument ui = new OCRDocument(dest_ui_filename);
		
		indexOCRDocument(id, doc, ui);
//		
//		// Add the screenshot's meta data to Derby
		HistoryScreen hs = new HistoryScreen(id, dest_image_filename);
		_sqldb.insertScreen(hs);
	}
	
	void insert(BufferedImage screen_image){
		int id = getNewId();

		// Save the screenshot in the file system
		String output_screen_image_filename = _image_save_path + "/" + id + ".png";
		
		try {
			ImageIO.write(screen_image, "png", new File(output_screen_image_filename));
		} catch (IOException e1) {
			log.errorException(e1);
		}
		log.info("Saved a screenshot at: " + output_screen_image_filename);
						
		// Save the ocr content in the file system
		OCRDocument doc = new OCRDocument(screen_image);
		
		String output_filename = _image_save_path + "/" + id + ".ocr"; 
		doc.save(output_filename);
		
		// Add the screenshot's ocr content to the Lucene index
		indexOCRDocument(id, doc);
			
		// Add the screenshot's meta data to Derby
		HistoryScreen hs = new HistoryScreen(id, output_screen_image_filename);
		_history_screens.add(hs);
		
		_sqldb.insertScreen(hs);
	}
	
	
	
	class Recorder{
		
		Robot _robot;
		
		
		public Recorder(){
			try {
				_robot = new Robot();
			} catch (AWTException e) {
				e.printStackTrace();
			} 
		}
		
	
		
		public void run(){
			log.entry("run");
		
			HistoryScreenDatabase db = new HistoryScreenDatabase();
			db.create("databases/live");
			
//			createEmptyIndex(_current_index_path);
//			_sqldb.reset();
			
			for (int i=0;i<5;++i){
				
				Rectangle screen_rectangle = new Rectangle(Toolkit.getDefaultToolkit().getScreenSize());
				BufferedImage screen_image = _robot.createScreenCapture(screen_rectangle);
				
				
				//BufferedImage screen_image = _robot.createScreenCapture(new Rectangle(0,0,800,600));
				
				insert(screen_image);

			}
			
			_sqldb.list();

			log.exit("run");

		}

		
	}
	
	static void tool_import_to_new(){
		HistoryScreenDatabase db = new HistoryScreenDatabase();
		db.create("databases/chi");
		db.import_from_old("captured/chi","databases/chi");
		db.import_from_old("captured/facebook","databases/chi");
		db.import_from_old("captured/login","databases/chi");
		db.import_from_old("captured/inbox","databases/chi");
		db.import_from_old("captured/video","databases/chi");
		
		_sqldb.list();	
	}
	
	public static void main(String[] args) throws Exception {
		
//		tool_import_to_new();
		
//		HistoryScreenDatabase db = new HistoryScreenDatabase();
//		db.testRecorder();
//		
//		HistoryScreenDatabase.find("deadline");
		
		setRoot("databases/chi");
		HistoryScreenDatabase.find("ui478");
			
				//HistoryScreenDatabase.find("vancouver && conference");
		//HistoryScreenDatabase.find("ui64 AND volunteers");
	    
	}
	public static String getSikuliIndexFilename() {
		return "captured/" + _current_example_name + ".index";
	}
}