package org.sikuli.history.db;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.ArrayList;
import java.util.Properties;

import org.sikuli.history.HistoryScreen;

public class DerbyDB {



	Connection conn = null;
	/* This ArrayList usage may cause a warning when compiling this class
	 * with a compiler for J2SE 5.0 or newer. We are not using generics
	 * because we want the source to support J2SE 1.4.2 environments. */
	ArrayList statements = new ArrayList(); // list of Statements, PreparedStatements
	PreparedStatement psInsert = null;
	PreparedStatement psUpdate = null;
	Statement s = null;

	ResultSet rs = null;

	String _dbName = "derbyDB"; // the name of the database
	String protocol = "jdbc:derby:";


	public void connect(String dbName){
		_dbName = dbName;

		Properties props = new Properties();
		try {
			conn = DriverManager.getConnection(protocol + _dbName
					+ ";create=true", props);

			System.out.println("Connected to and created database " + _dbName);

			// We want to control transactions manually. Autocommit is on by
			// default in JDBC.
			conn.setAutoCommit(false);

		} catch (SQLException sqle) {
			printSQLException(sqle);
		}


	}

	public void disconnect(){
		try
		{
			// the shutdown=true attribute shuts down Derby
			DriverManager.getConnection("jdbc:derby:;shutdown=true");

			// To shut down a specific database only, but keep the
			// engine running (for example for connecting to other
			// databases), specify a database in the connection URL:
			//DriverManager.getConnection("jdbc:derby:" + dbName + ";shutdown=true");
		}
		catch (SQLException se)
		{
			if (( (se.getErrorCode() == 50000)
					&& ("XJ015".equals(se.getSQLState()) ))) {
				// we got the expected exception
				System.out.println("Derby shut down normally");
				// Note that for single database shutdown, the expected
				// SQL state is "08006", and the error code is 45000.
			} else {
				// if the error code or SQLState is different, we have
				// an unexpected exception (shutdown failed)
				System.err.println("Derby did not shut down normally");
				printSQLException(se);
			}
		}
	}

	public HistoryScreen findScreen(int id){
		PreparedStatement s = null;
		//Statement s = null;
		try{

			s = conn.prepareStatement(
			"select * from screen where num = ?");
			s.setInt(1, id);
			ResultSet rs = s.executeQuery();
			conn.commit();

			if (rs.next()){

				int num = rs.getInt("num");
				String filename = rs.getString("filename");
				//System.out.println(num + "\t" + filename);

				return new HistoryScreen(num,filename);

			}

		}catch (SQLException sqle){
			printSQLException(sqle);
		}finally{
			try {
				if (s != null)
					s.close();
			} catch (SQLException e) {
				printSQLException(e);
			}
		}

		return null;
	}


	public void insertScreen(HistoryScreen screen){
		PreparedStatement s = null;
		try{

			s = conn.prepareStatement(
					"insert into screen values (?,?)");

			s.setInt(1, screen.getId());
			s.setString(2, screen.getFilename());
			s.executeUpdate();

			conn.commit();

		}catch (SQLException sqle){
			printSQLException(sqle);
		}finally{
			try {
				if (s != null)
					s.close();
			} catch (SQLException e) {
				printSQLException(e);
			}
		}
	}

	public ArrayList<HistoryScreen> getScreens() {
		ArrayList<HistoryScreen> screens = new ArrayList<HistoryScreen>();

		try{
			s = conn.createStatement();

			ResultSet rs = s.executeQuery("select * from screen");
			conn.commit();

			while (rs.next()) {
				int num = rs.getInt("num");
				String filename = rs.getString("filename");
				System.out.println(num + "\t" + filename);

				screens.add(new HistoryScreen(num, filename));
			}

		}catch (SQLException sqle){
			printSQLException(sqle);
		}finally{
			try {
				s.close();
			} catch (SQLException e) {
				printSQLException(e);
			}
		}

		return screens;
	}

	public void list(){
		try{
			s = conn.createStatement();

			ResultSet rs = s.executeQuery("select * from screen");
			conn.commit();

			//		ResultSet rs = s.executeQuery(query);
			while (rs.next()) {
				int num = rs.getInt("num");
				String filename = rs.getString("filename");
				System.out.println(num + "\t" + filename);
			}


		}catch (SQLException sqle){
			printSQLException(sqle);
		}finally{
			try {
				s.close();
			} catch (SQLException e) {
				printSQLException(e);
			}
		}

	}

	public void insert(){

		try{

			psInsert = conn.prepareStatement(
					"insert into screen values (?, ?)");

			psInsert.setInt(1, 1956);
			psInsert.setString(2, "screen1.png");
			psInsert.executeUpdate();
			System.out.println("Inserted 1");

			conn.commit();

		}catch (SQLException sqle){
			printSQLException(sqle);
		}

	}

	public void reset(){
		try{


			/* Creating a statement object that we can use for running various
			 * SQL statements commands against the database.*/
			s = conn.createStatement();
			statements.add(s);

			s.execute("drop table screen");


			// We create a table...
			s.execute("create table screen(num int, filename varchar(40))");

			//`id` SMALLINT UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY ,


			System.out.println("Created table screen");
			conn.commit();

			//			psInsert = conn.prepareStatement(
			//			"insert into screen values (?, ?)");
			//			statements.add(psInsert);
			//
			//			psInsert.setInt(1, 1956);
			//			psInsert.setString(2, "screen1.png");
			//			psInsert.executeUpdate();
			//			System.out.println("Inserted 1");
			//			
			//			conn.commit();
			//            System.out.println("Committed the transaction");




		}catch (SQLException sqle){
			printSQLException(sqle);
		}finally {
			// release all open resources to avoid unnecessary memory usage

			// ResultSet
			try {
				if (rs != null) {
					rs.close();
					rs = null;
				}
			} catch (SQLException sqle) {
				printSQLException(sqle);
			}

			// Statements and PreparedStatements
			int i = 0;
			while (!statements.isEmpty()) {
				// PreparedStatement extend Statement
				Statement st = (Statement)statements.remove(i);
				try {
					if (st != null) {
						st.close();
						st = null;
					}
				} catch (SQLException sqle) {
					printSQLException(sqle);
				}
			}

		}


	}

	public void createTables(){

		try{


			/* Creating a statement object that we can use for running various
			 * SQL statements commands against the database.*/
			s = conn.createStatement();
			statements.add(s);

			// We create a table...
			s.execute("create table screen(num int, filename varchar(40))");

			//`id` SMALLINT UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY ,


			System.out.println("Created table screen");
			conn.commit();

			//			psInsert = conn.prepareStatement(
			//			"insert into screen values (?, ?)");
			//			statements.add(psInsert);
			//
			//			psInsert.setInt(1, 1956);
			//			psInsert.setString(2, "screen1.png");
			//			psInsert.executeUpdate();
			//			System.out.println("Inserted 1");
			//			
			//			conn.commit();
			//            System.out.println("Committed the transaction");




		}catch (SQLException sqle){
			printSQLException(sqle);
		}finally {
			// release all open resources to avoid unnecessary memory usage

			// ResultSet
			try {
				if (rs != null) {
					rs.close();
					rs = null;
				}
			} catch (SQLException sqle) {
				printSQLException(sqle);
			}

			// Statements and PreparedStatements
			int i = 0;
			while (!statements.isEmpty()) {
				// PreparedStatement extend Statement
				Statement st = (Statement)statements.remove(i);
				try {
					if (st != null) {
						st.close();
						st = null;
					}
				} catch (SQLException sqle) {
					printSQLException(sqle);
				}
			}

		}


	}

	public static void printSQLException(SQLException e){
		// Unwraps the entire exception chain to unveil the real cause of the
		// Exception.
		while (e != null)
		{
			System.err.println("\n----- SQLException -----");
			System.err.println("  SQL State:  " + e.getSQLState());
			System.err.println("  Error Code: " + e.getErrorCode());
			System.err.println("  Message:    " + e.getMessage());
			// for stack traces, refer to derby.log or uncomment this:
			//e.printStackTrace(System.err);
			e = e.getNextException();
		}
	}

	/**
	 * @param args
	 * @throws SQLException 
	 */
	public static void main(String[] args) throws SQLException {
		DerbyDB t = new DerbyDB();

		t.connect("rec/derby");
		t.createTables();
		//t.insert();
		//t.list();
		//t.findScreen(182);
		t.disconnect();

	}

}
