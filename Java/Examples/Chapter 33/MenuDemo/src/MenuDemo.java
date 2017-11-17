// Demonstrate a simple main menu.
import java.awt.*;
import java.awt.event.*;
import javax.swing.*;

class MenuDemo implements ActionListener {
	
	JLabel jlab;
	JPopupMenu jpu;
	
	DebugAction setAct;
	DebugAction clearAct;
	DebugAction resumeAct;
	
	MenuDemo() {
		
		// Create a new JFrame container.
		JFrame jfrm = new JFrame("Menu Demo");

		//Specify FlowLayout for the layout manager.
		
		//Give the frame an initial size.
		jfrm.setSize(220, 200);

		//Terminate the program when the user closes the application.
		jfrm.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

		//Create a label that will display the menu selection.
		jlab = new JLabel();
		
		
		// Load the images.
		ImageIcon setIcon = new ImageIcon("analog.png");
		ImageIcon clearIcon = new ImageIcon("digital.png");
		ImageIcon resumeIcon = new ImageIcon("hourglass.png");
		
		
		// Create actions.
		setAct = new DebugAction("Set Breakpoint", setIcon,
				KeyEvent.VK_S,
				KeyEvent.VK_B,
				"Set a break point.");
		
		clearAct = new DebugAction("Clear Breakpoint", clearIcon,
				KeyEvent.VK_C,
				KeyEvent.VK_L,
				"Clear a break point.");
		
		resumeAct = new DebugAction("Resume", resumeIcon,
				KeyEvent.VK_R,
				KeyEvent.VK_R,
				"Resume execution after breakpoint.");
		
		// Initially disable the Clear Breakpoint option.
		clearAct.setEnabled(false);
		
		
		//Create the menu bar.
		JMenuBar jmb = new JMenuBar();
		//Create the File menu.
		JMenu jmFile = new JMenu("File");
		jmFile.setMnemonic(KeyEvent.VK_F);
		
		JMenuItem jmiOpen = new JMenuItem("Open", KeyEvent.VK_O);
		jmiOpen.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_O, InputEvent.CTRL_DOWN_MASK));
		
		JMenuItem jmiClose = new JMenuItem("Close",KeyEvent.VK_C);
		jmiClose.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_C, InputEvent.CTRL_DOWN_MASK));
		
		JMenuItem jmiSave = new JMenuItem("Save", KeyEvent.VK_S);
		jmiSave.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_S, InputEvent.CTRL_DOWN_MASK));
		
		JMenuItem jmiExit = new JMenuItem("Exit", KeyEvent.VK_E);
		jmiExit.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_E, InputEvent.CTRL_DOWN_MASK));
		
		jmFile.add(jmiOpen);
		jmFile.add(jmiClose);
		jmFile.add(jmiSave);
		//This visually separates Exit from the preceding three selections.
		jmFile.addSeparator();
		jmFile.add(jmiExit);
		jmb.add(jmFile);
		
		// Create the Options menu.
		JMenu jmOptions = new JMenu("Options");
		
		// Create the Colors submenu.
		JMenu jmColors = new JMenu("Colors");
		// Use check boxes for colors. This allows
		// the user to select more than one color.
		JCheckBoxMenuItem jmiRed = new JCheckBoxMenuItem("Red");
		JCheckBoxMenuItem jmiGreen = new JCheckBoxMenuItem("Green");
		JCheckBoxMenuItem jmiBlue = new JCheckBoxMenuItem("Blue");
		jmColors.add(jmiRed);
		jmColors.add(jmiGreen);
		jmColors.add(jmiBlue);
		jmOptions.add(jmColors);
		
		// Create the Priority submenu.
		JMenu jmPriority = new JMenu("Priority");
		// Use radio buttons for the priority setting.
		// This lets the menu show which priority is used
		// but also ensures that one and only one priority
		// can be selected at any one time. Notice that
		// the High radio button is initially selected.
		JRadioButtonMenuItem jmiHigh = new JRadioButtonMenuItem("High", true);
		JRadioButtonMenuItem jmiLow = new JRadioButtonMenuItem("Low");
		jmPriority.add(jmiHigh);
		jmPriority.add(jmiLow);
		jmOptions.add(jmPriority);
		// Create button group for the radio button menu items.
		ButtonGroup bg = new ButtonGroup();
		bg.add(jmiHigh);
		bg.add(jmiLow);
		
		// Create the Reset menu item.
		JMenuItem jmiReset = new JMenuItem("Reset");
		jmOptions.addSeparator();
		jmOptions.add(jmiReset);
		// Finally, add the entire options menu to
		// the menu bar
		jmb.add(jmOptions);
		
		//Create the Help menu.
		JMenu jmHelp = new JMenu("Help");
		ImageIcon icon = new ImageIcon("about.gif");
		JMenuItem jmiAbout = new JMenuItem("About", icon);
		jmiAbout.setToolTipText("Info about the MenuDemo program.");
		jmHelp.add(jmiAbout);
		jmb.add(jmHelp);
		
		//Add action listeners for the menu items.
		jmiOpen.addActionListener(this);
		jmiClose.addActionListener(this);
		jmiSave.addActionListener(this);
		jmiExit.addActionListener(this);
		jmiRed.addActionListener(this);
		jmiGreen.addActionListener(this);
		jmiBlue.addActionListener(this);
		jmiHigh.addActionListener(this);
		jmiLow.addActionListener(this);
		jmiReset.addActionListener(this);
		jmiAbout.addActionListener(this);
		
		//Add the label to the content pane.
		jfrm.add(jlab, BorderLayout.CENTER);
		//Add the menu bar to the frame.
		jfrm.setJMenuBar(jmb);
		//Display the frame.
		jfrm.setVisible(true);
		
		
		// Create an Edit popup menu.
		jpu = new JPopupMenu();
		// Create the popup menu items.
		JMenuItem jmiCut = new JMenuItem("Cut");
		JMenuItem jmiCopy = new JMenuItem("Copy");
		JMenuItem jmiPaste = new JMenuItem("Paste");
		// Add the menu items to the popup menu.
		jpu.add(jmiCut);
		jpu.add(jmiCopy);
		jpu.add(jmiPaste);
		// Add a listener for the popup trigger.
		jfrm.addMouseListener(new MouseAdapter() {
			public void mousePressed(MouseEvent me) {
				if(me.isPopupTrigger())
					jpu.show(me.getComponent(), me.getX(), me.getY());
			}
			public void mouseReleased(MouseEvent me) {
				if(me.isPopupTrigger())
					jpu.show(me.getComponent(), me.getX(), me.getY());
			}
		});
		
		jmiCut.addActionListener(this);
		jmiCopy.addActionListener(this);
		jmiPaste.addActionListener(this);
		
		
		// Create a Debug toolbar.
		JToolBar jtb = new JToolBar("Debug");
		
		// Create the toolbar buttons.
		JButton jbtnSet = new JButton(setAct);
		JButton jbtnClear = new JButton(clearAct);
		JButton jbtnResume = new JButton(resumeAct);
		
		// Add the buttons to the toolbar.
		jtb.add(jbtnSet);
		jtb.add(jbtnClear);
		jtb.add(jbtnResume);
		
		// Add the toolbar to the north position of
		// the content pane.
		jfrm.add(jtb, BorderLayout.NORTH);
		
		// Add the toolbar action listeners.
		jbtnSet.addActionListener(this);
		jbtnClear.addActionListener(this);
		jbtnResume.addActionListener(this);
		
		
		// Now, create a Debug menu that goes under the Options
		// menu bar item. Use the actions to create the items.
		JMenu jmDebug = new JMenu("Debug");
		JMenuItem jmiSetBP = new JMenuItem(setAct);
		JMenuItem jmiClearBP = new JMenuItem(clearAct);
		JMenuItem jmiResume = new JMenuItem(resumeAct);
		jmDebug.add(jmiSetBP);
		jmDebug.add(jmiClearBP);
		jmDebug.add(jmiResume);
		jmOptions.add(jmDebug);	
	}
	
	// A class to create an action for the Debug menu
	// and toolbar.
	class DebugAction extends AbstractAction {
		public DebugAction(String name, Icon image, int mnem, int accel, String tTip) {
			super(name, image);
			
			putValue(ACCELERATOR_KEY, KeyStroke.getKeyStroke(accel, InputEvent.CTRL_DOWN_MASK));
			putValue(MNEMONIC_KEY, new Integer(mnem));
			putValue(SHORT_DESCRIPTION, tTip);
		}
		
		// Handle events for both the toolbar and the
		// Debug menu.
		public void actionPerformed(ActionEvent ae) {
			String comStr = ae.getActionCommand();
			jlab.setText(comStr + " Selected");
			// Toggle the enabled status of the
			// Set and Clear Breakpoint options.
			if(comStr.equals("Set Breakpoint")) {
				clearAct.setEnabled(true);
				setAct.setEnabled(false);
			} 
			else if(comStr.equals("Clear Breakpoint")) {
				clearAct.setEnabled(false);
				setAct.setEnabled(true);
			}
		}
	}
	
	//Handle menu item action events.
	public void actionPerformed(ActionEvent ae) {
		//Get the action command from the menu selection.
		String comStr = ae.getActionCommand();
		//If user chooses Exit, then exit the program.
		if(comStr.equals("Exit")) System.exit(0);
		//Otherwise, display the selection.
		jlab.setText(comStr + " Selected");
	}
	
	public static void main(String args[]) {
		//Create the frame on the event dispatching thread.
		SwingUtilities.invokeLater(new Runnable() {
			public void run() {
				new MenuDemo();
			}
		});
	}
}