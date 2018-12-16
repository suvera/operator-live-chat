/**
 * 
 */
package NRML.motive;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JOptionPane;

import livechat.ChatExplorer;
import NRML.NRMLServer;
import NRML.Utils;

/**
 * @author nramanarayana
 *
 */
public class ProActive implements ActionListener {

	/**
	 * 
	 */
	public ProActive() {
		// TODO Auto-generated constructor stub
	}

	/* (non-Javadoc)
	 * @see java.awt.event.ActionListener#actionPerformed(java.awt.event.ActionEvent)
	 */
	public void actionPerformed(ActionEvent e) {
		if ("sendURL".equals(e.getActionCommand())) {
			try {
				String s = (String)JOptionPane.showInputDialog(
						ChatExplorer.frame,
	                    "Please enter the URL",
	                    "Proactive URL",
	                    JOptionPane.PLAIN_MESSAGE,
	                    Utils.createImageIcon("images/but_url.gif"),
	                    null,
	                    "http://");
				if ((s != null) && (s.length() > 0)) {
					NRMLServer.sendNewMessage(Utils.getWinOpenJS(s), "winopen");
				}
			} catch (Exception ex) {
				Utils.de(ex + " in ProActive.actionPerformed()");
			}
		} else if("inviteToChat".equals(e.getActionCommand())) {
			Utils.de("Sent!");
			NRMLServer.addToBuffer("", "invite");
		}

	}

}
