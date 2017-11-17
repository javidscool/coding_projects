// Demonstrate Lists.
import java.awt.*;
import java.awt.event.*;
import java.applet.*;
/*
<applet code="ListDemo" width=300 height=180>
</applet>
*/
public class ListDemo extends Applet implements ActionListener, ItemListener {
	List os, browser;
	String msg = "";

	public void init() {
		os = new List(4, true);
		browser = new List(4, false);

		// add items to os list
		os.add("Windows");
		os.add("Android");
		os.add("Solaris");
		os.add("Mac OS");

		// add items to browser list
		browser.add("Internet Explorer");
		browser.add("Firefox");
		browser.add("Chrome");
		browser.select(1);

		// add lists to window
		add(os);
		add(browser);

		// register to receive action events
		os.addActionListener(this);
		os.addItemListener(this);
		browser.addActionListener(this);
		browser.addItemListener(this);
	}
	
	public void actionPerformed(ActionEvent ae) {
		repaint();
	}
	
	public void itemStateChanged(ItemEvent ie) {
		repaint();
	}

	// Display current selections.
	public void paint(Graphics g) {
		int idx[];
		msg = "Current OS: ";
		idx = os.getSelectedIndexes();
		System.out.println(idx.length);
		for(int i=0; i<idx.length; i++){
			if(i == idx.length - 1){
				msg += os.getItem(idx[i]) + " ";
			}
			else{
				msg += os.getItem(idx[i]) + ", ";
			}
		}
		g.drawString(msg, 6, 120);
		msg = "Current Browser: ";
		msg += browser.getSelectedItem();
		g.drawString(msg, 6, 140);
	}
}