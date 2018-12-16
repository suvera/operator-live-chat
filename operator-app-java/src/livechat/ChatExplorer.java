package livechat;
/**
 * @author nramanarayana
 *
 */
import java.awt.*;
import javax.swing.*;
import java.awt.event.*;
import com.jeans.trayicon.*;
import NRML.*;
import NRML.ui.NMenuBar;

public class ChatExplorer extends JFrame implements LiveConstants {
	/*
	 * Screen Resolution Constants
	 */
	private Toolkit toolkit = Toolkit.getDefaultToolkit();
	private Dimension screensize = toolkit.getScreenSize();
	private int width = screensize.width, height = screensize.height;
	
	//Container
	private Container container;
	
	//Current Main Component
	public static ChatExplorer frame;
	
	//Static Objects
	public static String _site; //Site ID
	public static ChatPanel chatpanel;
	public static ProxyClient proxy;
	public static LiveChatConfig config;

	/**
	 * Main
	 * @param args
	 */
	public static void main(String[] args) {
		new SplashScreen(1000);
		javax.swing.SwingUtilities.invokeLater(new Runnable() {
			public void run() {
				try {
					frame = new ChatExplorer();
				} catch (Exception e) {
				}

			}
		});
	}

	/**
	 * Application Object
	 * @throws Exception
	 */
	public ChatExplorer() throws Exception {
		config =  new LiveChatConfig();
		proxy = new ProxyClient(config.getOption("SERVER_HOST"));
				
		initTrayIcon();
		initAwtContainer();
		Login.process();
	}
	
	/**
	 * Initialization of Tray Icon
	 * @throws Exception
	 */
	private void initTrayIcon() throws Exception {
		
		if (WindowsTrayIcon.isRunning("NRML Chat")) {
			// App already running, show error message and exit
		}
		
		WindowsTrayIcon.initTrayIcon("NRML Chat");
		Image image = Toolkit.getDefaultToolkit().getImage(
				ClassLoader.getSystemClassLoader().getResource(
						WINDOW_IMAGE_ICON));
		WindowsTrayIcon icon = new WindowsTrayIcon(image, 16, 16);
		icon.setToolTipText("NRML Live Chat");
		WindowsTrayIcon.initJAWT();
		WindowsTrayIcon.setAlwaysOnTop(frame, true);
		
		icon.addActionListener(new RestoreListener(true));
		icon.setVisible(true);
	}

	private void initAwtContainer() throws Exception {
		// Create Main Frame
		JFrame.setDefaultLookAndFeelDecorated(false);

		this.setJMenuBar(new NMenuBar());
		
		chatpanel = new ChatPanel();
		chatpanel.setOpaque(true);
		this.setContentPane(chatpanel);
		
		java.net.URL imgURL = ClassLoader.getSystemClassLoader().getResource(WINDOW_IMAGE_ICON);
		this.setIconImage(new ImageIcon(imgURL).getImage());

		this.setVisible(true);

		this.addWindowListener(new WindowAdapter() {

			public void windowIconified(WindowEvent e) {
				ChatExplorer.hideMe();
			}

			public void windowClosing(WindowEvent e) {
				if (JOptionPane.showConfirmDialog(container,
						"Are you sure you want to quit NRML Chat ?",
						"Quit NRML Chat", JOptionPane.OK_CANCEL_OPTION,
						JOptionPane.QUESTION_MESSAGE, null) == JOptionPane.YES_OPTION) {
					sendLogout();
					WindowsTrayIcon.cleanUp();
					System.exit(0);
				}
			}
		});
		this.setSize(3 * width / 4, 3 * height / 4);
		this.setLocationRelativeTo(null);
		this.setVisible(true);
		this.setTitle("NRML LIVE CHAT");
	}

	public void sendLogout() {
		NRMLServer.operatorLogout();
	}

	public static void showMe() {
		frame.setVisible(true);
	}

	public static void hideMe() {
		frame.setVisible(false);
	}

	//for Tray Icon Callback listener handles restore (click left on any icon / show popup menu)
	private class RestoreListener implements ActionListener {
		
		protected boolean from_menu;
	
		public RestoreListener(boolean fromMenu) {
			from_menu = fromMenu;
		}

		public void actionPerformed(ActionEvent evt) {
			if (from_menu) {				
				setVisible(true);
				toFront(); 
				requestFocus();
			} else if (evt.getActionCommand().equals("Left")) {
				if (!isVisible()) {
					setVisible(true);
					toFront();
					requestFocus();
				} else {
					setVisible(false);
				}
			}
		}
	}
}