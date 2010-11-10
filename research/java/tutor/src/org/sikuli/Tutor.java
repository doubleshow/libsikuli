package org.sikuli;
import java.awt.AWTException;
import java.awt.Rectangle;
import java.awt.Robot;
import java.awt.event.InputEvent;

import edu.mit.csail.uid.FindFailed;
import edu.mit.csail.uid.Match;
import edu.mit.csail.uid.Screen;
import edu.mit.csail.uid.SikuliScript;


public class Tutor {

	Robot robot;
	Screen screen;

	public Tutor(){
		screen = new Screen();

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


		tutor.click("imgs/network.png","Click on the Network icon");
		tutor.click("imgs/advanced.png","CLick on the Advanced button");
		tutor.click("imgs/tcpip.png","Click on the TCP/IP tab");
		tutor.click("imgs/ipv4.png","Select Manually");

	}

}
