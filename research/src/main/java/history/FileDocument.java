package history;

/**
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

import java.io.File;
import java.io.FileReader;
import java.io.StringReader;

import org.apache.lucene.document.DateTools;
import org.apache.lucene.document.Document;
import org.apache.lucene.document.Field;

/** A utility for making Lucene Documents from a File. */

public class FileDocument {
  /** Makes a document for a File.
    <p>
    The document has three fields:
    <ul>
    <li><code>path</code>--containing the pathname of the file, as a stored,
    untokenized field;
    <li><code>modified</code>--containing the last modified date of the file as
    a field as created by <a
    href="lucene.document.DateTools.html">DateTools</a>; and
    <li><code>contents</code>--containing the full contents of the file, as a
    Reader field;
    */
  public static Document Document(File f1, File f2, int id)
       throws java.io.FileNotFoundException {
	 
    // make a new, empty document
    Document doc = new Document();
    
    doc.add(new Field("id", ""+id, Field.Store.YES, Field.Index.NOT_ANALYZED));

    // Add the path of the file as a field named "path".  Use a field that is 
    // indexed (i.e. searchable), but don't tokenize the field into words.
    doc.add(new Field("path", f1.getPath(), Field.Store.YES, Field.Index.NOT_ANALYZED));

    // Add the last modified date of the file a field named "modified".  Use 
    // a field that is indexed (i.e. searchable), but don't tokenize the field
    // into words.
    doc.add(new Field("modified",
        DateTools.timeToString(f1.lastModified(), DateTools.Resolution.SECOND),
        Field.Store.YES, Field.Index.NOT_ANALYZED));

    // Add the contents of the file to a field named "contents".  Specify a Reader,
    // so that the text of the file is tokenized and indexed, but not stored.
    // Note that FileReader expects the file to be in the system's default encoding.
    // If that's not the case searching for special characters will fail.
    doc.add(new Field("ocr", new FileReader(f1)));

    doc.add(new Field("ui", new FileReader(f2)));

    
    // return the document
    return doc;
  }
  
  public static Document Document(String text, String path, int id){
	    Document doc = new Document();
	    
	    doc.add(new Field("id", ""+id, Field.Store.YES, Field.Index.NOT_ANALYZED));

	    // Add the path of the file as a field named "path".  Use a field that is 
	    // indexed (i.e. searchable), but don't tokenize the field into words.
	    doc.add(new Field("path", path, Field.Store.YES, Field.Index.NOT_ANALYZED));

	    // Add the last modified date of the file a field named "modified".  Use 
	    // a field that is indexed (i.e. searchable), but don't tokenize the field
	    // into words.
//	    doc.add(new Field("modified",
//	        DateTools.timeToString(f1.lastModified(), DateTools.Resolution.MINUTE),
//	        Field.Store.YES, Field.Index.NOT_ANALYZED));

	    // Add the contents of the file to a field named "contents".  Specify a Reader,
	    // so that the text of the file is tokenized and indexed, but not stored.
	    // Note that FileReader expects the file to be in the system's default encoding.
	    // If that's not the case searching for special characters will fail.
	    doc.add(new Field("ocr", new StringReader(text)));
	    
	    doc.add(new Field("ui", new StringReader(text)));
	    
	    return doc; 
  }

  private FileDocument() {}
}
    
