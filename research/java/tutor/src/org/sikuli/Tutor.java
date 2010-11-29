package org.sikuli;
import java.awt.AWTException;
import java.awt.Font;
import java.awt.Rectangle;
import java.awt.Robot;
import java.awt.event.InputEvent;

import javax.swing.UIManager;
import javax.swing.plaf.FontUIResource;

import edu.mit.csail.uid.FindFailed;
import edu.mit.csail.uid.Match;
import edu.mit.csail.uid.Screen;
import edu.mit.csail.uid.SikuliScript;


public class Tutor {

	Robot robot;
	Screen screen;

	MessageOverlay messageOverlay;
	
	public Tutor(){
		screen = new Screen();

		//messageOverlay = new MessageOverlay(this, "Tutorial begins");
		
		try {
			robot = new Robot();
		} catch (AWTException e1) {
			e1.printStackTrace();
		}
	}

	public void click(String image_filename, String message){		
		try{
			Match m = screen.wait(image_filename);
			click(m,message);
		}
		catch(FindFailed e){
			e.printStackTrace();                    
		}
	}

	public void click(Match m, String message){
		//messageOverlay.setMessage(message);
		Rectangle r = new Rectangle(m.x,m.y,m.w,m.h);
		System.out.println(r);

		ClickOverlay f = new ClickOverlay(this, r, message);
		synchronized(this){
			try {
				wait();
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}

		robot.mousePress(InputEvent.BUTTON1_MASK);				
		robot.mouseRelease(InputEvent.BUTTON1_MASK);

		robot.mousePress(InputEvent.BUTTON1_MASK);				
		robot.mouseRelease(InputEvent.BUTTON1_MASK);		
	}


	public static void main(String[] args) throws AWTException {

		Tutor tutor = new Tutor();
		
		SikuliScript sk;
		sk = new SikuliScript();
		sk.switchApp("System Preferences");

		//tutor.message("This tutorial teaches you how to set the IP address manually");
		//tutor.message("Click on the network button");
		
		tutor.click("imgs/network.png","Click on the Network icon");

		//tutor.message("Click on the Advanced button");

		//		tutor.message("This is another message");
		tutor.click("imgs/advanced.png","Click on the Advanced button");
//		tutor.click("imgs/tcpip.png","Click on the TCP/IP tab");
//		tutor.click("imgs/ipv4.png","Select Manually");

	}

	private void message(String message) {
		messageOverlay.setMessage(message);
		messageOverlay.advance();
		synchronized(this){
			try {
				wait();
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
	}

}
