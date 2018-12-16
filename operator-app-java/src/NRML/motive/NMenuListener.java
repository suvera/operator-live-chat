/**
 * 
 */
package NRML.motive;

import livechat.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import javax.swing.JOptionPane;
import NRML.NRMLServer;

import com.jeans.trayicon.WindowsTrayIcon;

/**
 * @author nramanarayana
 *
 */
public class NMenuListener implements ActionListener {

	/**
	 * 
	 */
	public NMenuListener() {
		// TODO Auto-generated constructor stub
	}

	/* (non-Javadoc)
	 * @see java.awt.event.ActionListener#actionPerformed(java.awt.event.ActionEvent)
	 */
	public void actionPerformed(ActionEvent event) {
		if ("login".equals(event.getActionCommand())) {
			Login.process();
			
		} else if("logout".equals(event.getActionCommand()) || "exit".equals(event.getActionCommand()) ) {
			if (JOptionPane.showConfirmDialog(ChatExplorer.frame,
					"Are you sure you want to quit NRML Chat ?",
					"Quit NRML Chat", JOptionPane.OK_CANCEL_OPTION,
					JOptionPane.QUESTION_MESSAGE, null) == JOptionPane.YES_OPTION) {
				
				NRMLServer.operatorLogout();
				
				WindowsTrayIcon.cleanUp();
				System.exit(0);
			}
		}
	}

}
