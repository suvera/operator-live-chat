package livechat;

/**
 * @author nramanarayana
 *
 */
import javax.swing.JOptionPane;

import NRML.ui.*;
import NRML.*;


public class Login {
	public static String username;
	
	public static String password;
	
	public static String site;
	
	public static LoginDialog dialog;
	

	public static void process() {
		showLoginDialog();
		
		if (username == null || password == null) {
			JOptionPane.showMessageDialog(ChatExplorer.frame, "Please enter a valid username and password", "Error", 2);
			process();
		}
		
		if ((username.length() != 0) || (password.length() != 0) || (site.length() == 0)) {
			ChatExplorer.chatpanel.startTimer();
			ChatExplorer._site = site;
			ChatPanel.operator = dialog.getOperator();
			ChatExplorer.chatpanel.setOperatorName(ChatPanel.operator.Name);
		} else {
			JOptionPane.showMessageDialog(ChatExplorer.frame, "Please enter a valid username and password", "Error", 2);
			process();
		}
	}

	public static void showLoginDialog() {
				
		dialog = new LoginDialog(ChatExplorer.frame);
		
		username = dialog.getUserName();
		password = dialog.getPassword();
		site = dialog.getSite();
	}
}
