import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Image;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Dictionary;
import java.util.Hashtable;
import java.util.Iterator;

import javax.imageio.ImageIO;
import javax.swing.Box;
import javax.swing.BoxLayout;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLayeredPane;
import javax.swing.JPanel;
import javax.swing.JTextField;


import org.apache.lucene.analysis.Analyzer;
import org.apache.lucene.analysis.standard.StandardAnalyzer;
import org.apache.lucene.document.Document;
import org.apache.lucene.index.IndexReader;
import org.apache.lucene.index.IndexWriter;
import org.apache.lucene.queryParser.QueryParser;
import org.apache.lucene.search.Collector;
import org.apache.lucene.search.IndexSearcher;
import org.apache.lucene.search.Query;
import org.apache.lucene.search.ScoreDoc;
import org.apache.lucene.search.Scorer;
import org.apache.lucene.search.Searcher;
import org.apache.lucene.search.TopScoreDocCollector;
import org.apache.lucene.store.Directory;
import org.apache.lucene.store.FSDirectory;
import org.apache.lucene.store.RAMDirectory;
import org.apache.lucene.util.Version;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.Date;

public class ScreenshotViewer extends JPanel {
	
	Image image;
	int current_shot_id;

	static final File INDEX_DIR = new File("index");
	static Hashtable db;
	static {
		   db = new Hashtable();
		   
		   String files[] = {"amazon1.png","amazon2.png","amazon3.png","amazon4.png",
				   "finder1.png","finder2.png",
				   "word1.png","word2.png",
				   "chi1.png","chi2.png","chi3.png","chi4.png"};
		   
		   int i=1;
		   for (String file : files){
			   db.put(new Integer(i), file);
			   i = i + 1;
		   }
		   
//		   final File docDir = new File("ocr");
//		   Date start = new Date();
//		    try {
//		      IndexWriter writer = new IndexWriter(FSDirectory.open(INDEX_DIR), new StandardAnalyzer(Version.LUCENE_CURRENT), true, IndexWriter.MaxFieldLength.LIMITED);
//		      System.out.println("Indexing to directory '" +INDEX_DIR+ "'...");
//		      indexDocs(writer, docDir);
//		      System.out.println("Optimizing...");
//		      writer.optimize();
//		      writer.close();
//
//		      Date end = new Date();
//		      System.out.println(end.getTime() - start.getTime() + " total milliseconds");
//
//		    } catch (IOException e) {
//		      System.out.println(" caught a " + e.getClass() +
//		       "\n with message: " + e.getMessage());
//		    }
		   try{
		   //doSearch();
		   }catch(Exception e){}
	}	        
	
	
	static void doSearch(String line) throws Exception {
	    IndexReader reader = IndexReader.open(FSDirectory.open(new File("index")), true); // only searching, so read-only=true

//	    if (normsField != null)
//	      reader = new OneNormsReader(reader, normsField);

	    String field = "contents";
	    Searcher searcher = new IndexSearcher(reader);
	    Analyzer analyzer = new StandardAnalyzer(Version.LUCENE_CURRENT);
	    
	    QueryParser parser = new QueryParser(Version.LUCENE_CURRENT, field, analyzer);

	    //String line = "reading area";
	    Query query = parser.parse(line);
	    
	    //searcher.search(query, null, 100);
	    
//	    Collector streamingHitCollector = new Collector() {
//	        private Scorer scorer;
//	        private int docBase;
//	        
//	        // simply print docId and score of every matching document
//	        @Override
//	        public void collect(int doc) throws IOException {
//	          System.out.println("doc=" + doc + docBase + " score=" + scorer.score());
//	          
//	          Document d = searcher.doc(doc);
//	          String path = d.get("path");
//	          System.out.println(path);
//	           
//	        }
//
//	        @Override
//	        public boolean acceptsDocsOutOfOrder() {
//	          return true;
//	        }
//
//	        @Override
//	        public void setNextReader(IndexReader reader, int docBase)
//	            throws IOException {
//	          this.docBase = docBase;
//	        }
//
//	        @Override
//	        public void setScorer(Scorer scorer) throws IOException {
//	          this.scorer = scorer;
//	        }
//	        
//	      };
	      
	    TopScoreDocCollector collector = TopScoreDocCollector.create(10, false);
        searcher.search(query, collector);
        
        ScoreDoc[] hits;
        hits = collector.topDocs().scoreDocs;

        for (int i = 0; i < hits.length; i++) {
            

            Document doc = searcher.doc(hits[i].doc);
            String path = doc.get("path");
            System.out.println(path);
             
                      
         }

	    
	      //searcher.search(query, streamingHitCollector);
	      
	     // hits = streamingHitCollector.topDocs().scoreDocs
	}
	
	static void indexDocs(IndexWriter writer, File file)
    throws IOException {
    // do not try to index files that cannot be read
    if (file.canRead()) {
      if (file.isDirectory()) {
        String[] files = file.list();
        // an IO error could occur
        if (files != null) {
          for (int i = 0; i < files.length; i++) {
            indexDocs(writer, new File(file, files[i]));
          }
        }
      } else {
        System.out.println("adding " + file);
        try {
          writer.addDocument(FileDocument.Document(file));
        }
        // at least on windows, some temporary files raise this exception with an "access denied" message
        // checking if the file can be read doesn't help
        catch (FileNotFoundException fnfe) {
          ;
        }
      }
    }
  }
	
	static String getFilename(int id){
		return (String) db.get(new Integer(id));
	}
	
	
	FilmStrip filmStrip;
	
	JLayeredPane layeredPane;
	public ScreenshotViewer(){

		layeredPane = new JLayeredPane();
		layeredPane.setPreferredSize(new Dimension(800, 600));
		add(layeredPane);
		
		JPanel controlPanel = new JPanel();
		controlPanel.setLayout(new BoxLayout(controlPanel, BoxLayout.Y_AXIS));
		controlPanel.setBounds(0,400,800,120);
		
		ArrayList<Shot> shots = new ArrayList<Shot>();
		shots.add(new Shot(1,0,150));
		shots.add(new Shot(2,150,350));		
		shots.add(new Shot(3,351,450));
		shots.add(new Shot(4,451,550));
		shots.add(new Shot(5,551,700));
		shots.add(new Shot(6,701,800));
		shots.add(new Shot(7,801,950));
		shots.add(new Shot(8,951,1000));
		shots.add(new Shot(9,1001,1200));
		shots.add(new Shot(10,1201,1300));
		
		
		JPanel controls = new JPanel(new FlowLayout(FlowLayout.LEFT));
		JButton rewind = new JButton("<");
		rewind.setPreferredSize(new Dimension(20,20));;
		JButton forward = new JButton(">");
		forward.setPreferredSize(new Dimension(20,20));;
		JButton stop = new JButton("[][]");
		stop.setPreferredSize(new Dimension(20,20));
	
		JButton zoomin = new JButton("+");
		zoomin.setPreferredSize(new Dimension(20,20));
		JButton zoomout = new JButton("-");
		zoomout.setPreferredSize(new Dimension(20,20));

		JButton hide = new JButton("X");
		hide.setPreferredSize(new Dimension(20,20));
		hide.addActionListener(new ActionListener(){

			@Override
			public void actionPerformed(ActionEvent e) {
				filmStrip.setVisible(false);
			}			
		});

				
		JTextField searchTerm = new JTextField(10);		
		JButton searchBtn = new JButton("Find");
		searchBtn.addActionListener(new ActionListener(){

			@Override
			public void actionPerformed(ActionEvent e) {
				try {
					doSearch(((JButton)e.getSource()).getText());
				} catch (Exception e1) {
					// TODO Auto-generated catch block
					e1.printStackTrace();
				}
			}			
		});
		
		controls.add(rewind);
		controls.add(stop);
		controls.add(forward);
		controls.add(hide);
		controls.add(zoomin);
		controls.add(zoomout);
		controls.add(searchTerm);
		controls.add(searchBtn);
		controls.setAlignmentX(LEFT_ALIGNMENT);
		controls.setOpaque(false);
		
		filmStrip = new FilmStrip();
		
		Iterator<Shot> i = shots.iterator();
	     while(i.hasNext())  {
	      		
			ShotTile tile = new ShotTile((Shot) i.next());
			tile.addEventListener(new MyShotTileEventListener());
		
			filmStrip.addTile(tile);
		}
		
	    filmStrip.setPreferredSize(new Dimension(800,100));
	    filmStrip.setAlignmentX(LEFT_ALIGNMENT);

	    controlPanel.add(controls);
	    controlPanel.add(filmStrip);
	    
	    layeredPane.add(controlPanel,new Integer(2));
		
		Lense lense = new Lense(new Dimension(800,600-100));
		layeredPane.add(lense,3);
		lense.setBounds(0,66,800,600-100);
		
		
		current_shot_id = -1;
        updateShot(1);
		
		setSize(800,600);
		setVisible(true);
		
		//controlPanel.setVisible(false)
		lense.setVisible(false);
		
		hide.requestFocusInWindow(); 
		hide.addKeyListener(lense);
		lense.addMouseListener(new MouseAdapter(){
			public void mouseEntered(MouseEvent event){
				System.out.println("hello");
			}
		});
		
		lense.setFocusable(true);
		lense.addKeyListener(lense);
		lense.requestFocus();
	}
	
	
	
	class MyShotTileEventListener implements ShotTileEventListener{
		public void shotSelected(ShotTileEvent event){
			ShotTile shotTile = (ShotTile) event.getSource();
			updateShot(shotTile.getShot().id);
			
		}

		@Override
		public void markerMoved(ShotTileEvent event) {
		}
			
	}
	
	public void updateShot(int shot_id){
		if (current_shot_id == shot_id)
			return;
	
		current_shot_id = shot_id;
		
		String filename = ScreenshotViewer.getFilename(current_shot_id);
		
		 try {
			 File sourceimage = new File(filename);
		     image = ImageIO.read(sourceimage);          
        } catch (IOException ioe) {
            ioe.printStackTrace();
        }
        
        repaint();
	}
	
	public void paintComponent(Graphics g) {
		
		super.paintComponent(g);
        Graphics2D g2d = (Graphics2D) g;

        g2d.drawImage(image, 0, 0, 800, 600, null);

    
    }

    public static void main(String[] args) {
    	JFrame frame = new JFrame();
    	ScreenshotViewer main =  new ScreenshotViewer();

        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.add(main);        
        frame.setSize(800,600);
        frame.setLocationRelativeTo(null);
        frame.setVisible(true);
    }
}
