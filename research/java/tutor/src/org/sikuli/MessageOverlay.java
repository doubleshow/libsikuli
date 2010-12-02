package org.sikuli;
import java.awt.AWTException;
import java.awt.BasicStroke;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.GraphicsDevice;
import java.awt.GraphicsEnvironment;
import java.awt.Point;
import java.awt.Rectangle;
import java.awt.Robot;
import java.awt.Stroke;
import java.awt.Toolkit;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.ComponentAdapter;
import java.awt.event.ComponentEvent;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.image.BufferedImage;
import java.awt.image.RescaleOp;

import javax.swing.Box;
import javax.swing.BoxLayout;
import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JSeparator;
import javax.swing.JWindow;
import javax.swing.UIManager;
import javax.swing.plaf.FontUIResource;


class MessageOverlay extends JWindow{
		
		Rectangle target;
		BufferedImage screen = null;
		Robot robot;
		String message;
		
		GraphicsDevice gdev;
		Object o;
				
		JLabel messageLabel;
		JLabel stepLabel;
		int step = 1;
		
		class Button extends JButton{
			
			public Button(String text){
				super(text);
				Font f = new Font("sansserif", Font.BOLD, 16);
				setFont(f);
			}
		}
		
		public MessageOverlay(Object o_, String message_){

			this.o = o_;
			this.message = message_;
			
			JPanel panel = new JPanel();
			panel.setLayout(new BoxLayout(panel, BoxLayout.Y_AXIS));
			
			Font f = new Font("sansserif", Font.BOLD, 16);

			
			messageLabel = new JLabel(message);
			stepLabel = new JLabel(""+step);

			//UIManager.put("Label.font", new FontUIResource("Lucida Grande", Font.PLAIN, 16));
			messageLabel.setFont(f);
			stepLabel.setFont(f);

			
			Box row1 = new Box(BoxLayout.X_AXIS);
			row1.add(new JLabel("Step "));
			row1.add(stepLabel);
			row1.add(new JLabel(" : "));
			row1.add(messageLabel);
			
			JButton next = new Button("Next");
			JButton back = new Button("Back");
			JButton restart = new Button("Restart");
			JButton showme = new Button("Show Me");

			Box row2 = new Box(BoxLayout.X_AXIS);
			row2.add(back);
			row2.add(restart);
			row2.add(next);
			row2.add(showme);

			panel.add(row1);
			panel.add(row2);
			
			//panel.add(row2);
			
			add(panel);
			
			setMinimumSize(new Dimension(800,100));
			//pack();
			
			next.addActionListener(new ActionListener(){

				@Override
				public void actionPerformed(ActionEvent e) {
					//setVisible(false);
					synchronized(o){
						o.notify();
					}
				}
				
			});
			
			
			//setPreferredSize(new Dimension(100,100));
//			try {
//				robot = new Robot();
//			} catch (AWTException e1) {
//				e1.printStackTrace();
//			} 

			panel.addMouseListener(new MouseAdapter(){
				
				public void mouseClicked(MouseEvent e){
					System.out.println(e.getX() + "," + e.getY());
					
				//	if (target.contains(e.getPoint())){
				//		System.out.println("inside");
				//		gdev.setFullScreenWindow(null);
						//setVisible(false);
						//screen = null;
						synchronized(o){
							o.notify();
						}

//						toBack();
				//	}				
				}
						
			});
			
			addComponentListener(new ComponentAdapter() {
				public void componentHidden(ComponentEvent e) 
				{
					//System.out.println("I'm hidden");
					//dispose();
					
					synchronized(o){
						o.notify();
					}
				}
				});

			//start();
			setVisible(true);
		}
		
		public void advance(){
			step = step + 1;
			stepLabel.setText(""+step);
		}
		
		public void setMessage(String message){
			String html = "<html>" + message + "</html>";
			
			//UIManager.put("Label.font", new FontUIResource("Lucida Grande", Font.PLAIN, 16));

			messageLabel.setText(html);
			//pack();
			setVisible(true);
		}
		
//		public void start(){
//			screen = robot.createScreenCapture(new Rectangle(Toolkit.getDefaultToolkit().getScreenSize()));
//			
//			gdev = GraphicsEnvironment.getLocalGraphicsEnvironment().getDefaultScreenDevice();
//
//	         setVisible(true);
//			  if(gdev.isFullScreenSupported() ){
//		            gdev.setFullScreenWindow(this);
//		         }
//
//		}
				
		
//		public void paint(Graphics g) {
//
//		//	if (screen != null){
//				super.paint(g);
//				Graphics2D g2d = (Graphics2D) g;
//				
//				Point p = new Point(0,0);
//				
//				//g2d.drawRect(target.x, target.y, target.width, target.height);
//				
//				Stroke pen = new BasicStroke(3.0F);
//				g2d.setStroke(pen);
//				g2d.setColor(Color.red);
//				Font f = new Font("sansserif", Font.BOLD, 32);
//				g2d.setFont(f);
//				g2d.drawString(message, p.x, p.y);
//				setVisible(true);
////			}else{
////				setVisible(false);
////			}
//
//		}
	}