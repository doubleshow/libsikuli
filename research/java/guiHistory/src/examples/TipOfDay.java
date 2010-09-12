package examples;
import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.GridLayout;
import java.awt.Insets;
import java.awt.LayoutManager;
import java.awt.TextArea;
import java.awt.Toolkit;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyEvent;
import java.text.DateFormat;
import java.util.Date;
import java.util.Locale;

import javax.swing.BorderFactory;
import javax.swing.Box;
import javax.swing.BoxLayout;
import javax.swing.DefaultListModel;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JCheckBox;
import javax.swing.JDialog;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JList;
import javax.swing.JPanel;
import javax.swing.JSeparator;
import javax.swing.JTextArea;
import javax.swing.JTextPane;
import javax.swing.JToolBar;
import javax.swing.border.LineBorder;

public class TipOfDay extends JDialog{

	private DefaultListModel model;
	private JList list;
	
	public TipOfDay() {
		
        setTitle("Tip of the Day");
        //setDefaultCloseOperation(EXIT_ON_CLOSE);

		JPanel panel = new JPanel();
		panel.setLayout(new BoxLayout(panel, BoxLayout.Y_AXIS));
		//panel.setAlignmentX(LEFT_ALIGNMENT);

		add(panel);
		
		JPanel heading = new JPanel(new BorderLayout(0,0));
		JLabel lb = new JLabel("JDeveloper Productivity Hints");
		heading.setMaximumSize(new Dimension(450, 0));
		lb.setBorder(BorderFactory.createEmptyBorder(0,25,0,0));
		heading.add(lb, BorderLayout.WEST);
		
		
		ImageIcon runIcon = new ImageIcon("run.png");
		JLabel runLabel = new JLabel(runIcon);
		heading.add(runLabel, BorderLayout.EAST);
		
		
		panel.add(heading, BorderLayout.NORTH);
		
		JSeparator separator = new JSeparator();
        separator.setForeground(Color.gray);
        heading.add(separator, BorderLayout.SOUTH);

        JPanel textPanel = new JPanel(new BorderLayout());
        textPanel.setBorder(BorderFactory.createEmptyBorder(15, 25, 15, 25));
        
        JTextPane pane = new JTextPane();
        pane.setContentType("text/html");
        String text = "<p><b>Closing windows using the mouse wheel</b></p>" +
            "<p>Clicking with the mouse wheel on an editor tab closes the window. " +
            "This method works also with dockable windows or Log window tabs.</p>";
        pane.setText(text);
        pane.setEditable(false);        
        
		textPanel.add(pane);
		panel.add(textPanel);
		
		JPanel boxPanel = new JPanel(new FlowLayout(FlowLayout.LEFT, 20, 0));

        JCheckBox box = new JCheckBox("Show Tips at startup");
        box.setMnemonic(KeyEvent.VK_S);

        boxPanel.add(box);
        panel.add(boxPanel);
		
        model = new DefaultListModel();
        list = new JList(model);
        list.setBounds(150, 30, 220, 150);
        panel.add(list);
        
        
		JPanel bottom = new JPanel(new FlowLayout(FlowLayout.RIGHT));
        
        JButton okButton = new JButton("OK");
        okButton.setMnemonic(KeyEvent.VK_ENTER);
        okButton.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent event) {

                Locale locale = Locale.getDefault();
                Date date = new Date();
                String s = DateFormat.getTimeInstance(DateFormat.SHORT,
                    locale).format(date);

                if ( !model.isEmpty() )
                    model.clear();

                if (event.getID() == ActionEvent.ACTION_PERFORMED)
                    model.addElement(" Event Id: ACTION_PERFORMED");

                model.addElement(" Time: " + s);

                String source = event.getSource().getClass().getName();
                model.addElement(" Source: " + source);

                int mod = event.getModifiers();

                StringBuffer buffer = new StringBuffer(" Modifiers: ");

                if ((mod & ActionEvent.ALT_MASK) > 0)
                    buffer.append("Alt ");

                if ((mod & ActionEvent.SHIFT_MASK) > 0)
                    buffer.append("Shift ");

                if ((mod & ActionEvent.META_MASK) > 0)
                    buffer.append("Meta ");

                if ((mod & ActionEvent.CTRL_MASK) > 0)
                    buffer.append("Ctrl ");

                model.addElement(buffer);
            }
        });
        
        
        
        JButton closeButton = new JButton("Close");
        closeButton.setMnemonic(KeyEvent.VK_C);

        bottom.add(okButton);
        bottom.add(Box.createRigidArea(new Dimension(5, 0)));
        bottom.add(closeButton);
        bottom.add(Box.createRigidArea(new Dimension(15, 0)));
        
        bottom.setMaximumSize(new Dimension(450, 0));
        
		panel.add(bottom, BorderLayout.SOUTH);
		
		setSize(new Dimension(450, 350));
		setDefaultCloseOperation(JDialog.DISPOSE_ON_CLOSE);
		setResizable(false);
		setLocationRelativeTo(null);
		setVisible(true);

	}
	
	public static void main(String[] args) {

		TipOfDay tod = new TipOfDay();
        tod.setVisible(true);
    } 
}
