// Demonstrate JLabel and ImageIcon.
import java.awt.*;
import javax.swing.*;
/*
<applet code="JLabelDemo" width=250 height=200>
</applet>
*/
public class JLabelDemo extends JApplet {
	public void init() {
		try {
			SwingUtilities.invokeAndWait(
					new Runnable() {
						public void run() {
							makeGUI();
						}
					});
		} catch (Exception exc) {
			System.out.println("Can't create because of " + exc);
		}
}
	
	private void makeGUI() {
		// Create an icon.
		ImageIcon ii = new ImageIcon("accept.png");
		// Create a label.
		JLabel jl = new JLabel("check", ii, JLabel.CENTER);
		// Add the label to the content pane.
		add(jl);
	}
}