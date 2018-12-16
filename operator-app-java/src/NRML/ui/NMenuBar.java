/**
 * 
 */
package NRML.ui;

import java.awt.Color;
import java.awt.event.ActionEvent;
import java.awt.event.KeyEvent;
import NRML.motive.*;
import livechat.*;
import javax.swing.BorderFactory;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;
import javax.swing.KeyStroke;

/**
 * @author nramanarayana
 *
 */
public class NMenuBar extends JMenuBar {
	
	private JMenuBar menuBar;
	private JMenu menu;
	private JMenuItem login, logout, exit;
	
	/**
	 * 
	 */
	public NMenuBar() {
		super();
		// Create the menu bar.
		setBorder(BorderFactory.createEmptyBorder(0, 10, 0, 0));
		setBackground(new Color(214, 211, 206));
		// Build the first menu.
		menu = new JMenu("File");
		menu.setMnemonic(KeyEvent.VK_F); // used constructor instead
		add(menu);

		// a group of JMenuItems
		login = new JMenuItem("Log In");
		login.setMnemonic(KeyEvent.VK_T); // used constructor instead
		login.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_L, ActionEvent.ALT_MASK));
		login.setActionCommand("login");
		login.addActionListener(new NMenuListener());

		menu.add(login);
		login.setEnabled(false);
		
		logout = new JMenuItem("Log Out");
		logout.setMnemonic(KeyEvent.VK_O); // used constructor instead
		logout.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_G,	ActionEvent.ALT_MASK));
		logout.setActionCommand("logout");
		logout.addActionListener(new NMenuListener());
		
		menu.add(logout);
		menu.addSeparator();
		
		exit = new JMenuItem("Exit");
		exit.setMnemonic(KeyEvent.VK_X); // used constructor instead
		exit.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_X, ActionEvent.ALT_MASK));
		exit.setActionCommand("exit");
		exit.addActionListener(new NMenuListener());
		
		menu.add(exit);
	}

}
