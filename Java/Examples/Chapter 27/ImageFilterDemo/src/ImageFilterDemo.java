/*
* <applet code=ImageFilterDemo width=400 height=345>
* <param name=img value=Lilies.jpg>
* <param name=filters value="Grayscale+Invert+Contrast+Blur+Sharpen">
* </applet>
*/
import java.applet.*;
import java.awt.*;
import java.awt.event.*;
import java.util.*;
public class ImageFilterDemo extends Applet implements ActionListener {
	Image img;
	PlugInFilter pif;
	Image fimg;
	Image curImg;
	LoadedImage lim;
	Label lab;
	Button reset;

	public void init() {
		setLayout(new BorderLayout());
		Panel p = new Panel();
		add(p, BorderLayout.SOUTH);
		
		reset = new Button("Reset");
		reset.addActionListener(this);
		p.add(reset);
		
		StringTokenizer st = new StringTokenizer("Grayscale+Invert+Contrast+Blur+Sharpen", "+");
		while(st.hasMoreTokens()) {
			Button b = new Button(st.nextToken());
			b.addActionListener(this);
			p.add(b);
		}
		
		lab = new Label("");
		add(lab, BorderLayout.NORTH);
		img = getImage(getDocumentBase(), "image.jpg");
		lim = new LoadedImage(img);
		add(lim, BorderLayout.CENTER);
	}
	
	public void actionPerformed(ActionEvent ae) {
		String a = "";
		try {
			a = ae.getActionCommand();
			if (a.equals("Reset")) {
				lim.set(img);
				lab.setText("Normal");
			}
			else {
				pif = (PlugInFilter) Class.forName(a).newInstance();
				fimg = pif.filter(this, img);
				lim.set(fimg);
				lab.setText("Filtered: " + a);
			}
			repaint();
		} catch (ClassNotFoundException e) {
			lab.setText(a + " not found");
			lim.set(img);
			repaint();
		} catch (InstantiationException e) {
			lab.setText("couldn�t new " + a);
		} catch (IllegalAccessException e) {
			lab.setText("no access: " + a);
		}
	}
}