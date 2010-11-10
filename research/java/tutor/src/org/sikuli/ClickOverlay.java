package org.sikuli;
import java.awt.AWTException;
import java.awt.BasicStroke;
import java.awt.Color;
import java.awt.Font;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.GraphicsDevice;
import java.awt.GraphicsEnvironment;
import java.awt.Rectangle;
import java.awt.Robot;
import java.awt.Stroke;
import java.awt.Toolkit;
import java.awt.event.ComponentAdapter;
import java.awt.event.ComponentEvent;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.image.BufferedImage;
import java.awt.image.RescaleOp;

import javax.swing.JWindow;


class ClickOverlay extends JWindow{
		
		Rectangle target;
		BufferedImage screen = null;
		Robot robot;
		String message;
		
		GraphicsDevice gdev;
		Object o;
		
		public ClickOverlay(Object o_, Rectangle target_){
			this(o_, target_, "Click Here");
		}
		
		public ClickOverlay(Object o_, Rectangle target_, String message_){
			this.target = target_;
			this.o = o_;
			this.message = message_;
			
			try {
				robot = new Robot();
			} catch (AWTException e1) {
				e1.printStackTrace();
			} 

			addMouseListener(new MouseAdapter(){
				
				public void mouseClicked(MouseEvent e){
					System.out.println(e.getX() + "," + e.getY());
					
					if (target.contains(e.getPoint())){
						System.out.println("inside");
						gdev.setFullScreenWindow(null);
						setVisible(false);
						screen = null;
//						toBack();
					}				
				}
						
			});
			
			addComponentListener(new ComponentAdapter() {
				public void componentHidden(ComponentEvent e) 
				{
					System.out.println("I'm hidden");
					dispose();
					synchronized(o){
						o.notify();
					}
				}
				});

			start();
		}
		
		public void start(){
			screen = robot.createScreenCapture(new Rectangle(Toolkit.getDefaultToolkit().getScreenSize()));
			
			gdev = GraphicsEnvironment.getLocalGraphicsEnvironment().getDefaultScreenDevice();

	         setVisible(true);
			  if(gdev.isFullScreenSupported() ){
		            gdev.setFullScreenWindow(this);
		         }

		}
				
		
		public void paint(Graphics g) {

			if (screen != null){
				super.paint(g);
				Graphics2D g2d = (Graphics2D) g;
				
				
				RescaleOp op = new RescaleOp(0.3f, 0, null); 
				BufferedImage image_really_darken = op.filter(screen, null);
				g2d.drawImage(image_really_darken, 0, 0, null);
				
				BufferedImage subimage = screen.getSubimage(target.x,target.y,target.width,target.height);
				g2d.drawImage(subimage, target.x, target.y, null);

				
				g2d.drawRect(target.x, target.y, target.width, target.height);
				Stroke pen = new BasicStroke(3.0F);
				g2d.setStroke(pen);
				g2d.setColor(Color.red);
				Font f = new Font("sansserif", Font.BOLD, 32);
				g2d.setFont(f);
				g2d.drawString(message, target.x + target.width + 10, target.y+target.height);
				setVisible(true);
			}else{
				setVisible(false);
			}

		}
	}