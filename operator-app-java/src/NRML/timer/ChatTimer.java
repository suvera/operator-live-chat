/**
 * 
 */
package NRML.timer;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import livechat.*;
import NRML.NRMLServer;
import NRML.Utils;

/**
 * @author nramanarayana
 *
 */
public class ChatTimer implements ActionListener {
	
	private Visitor visitor;
	
	/**
	 * @param v
	 */
	public ChatTimer(Visitor v) {
		this.visitor = v;
	}
	
	/* (non-Javadoc)
	 * @see java.awt.event.ActionListener#actionPerformed(java.awt.event.ActionEvent)
	 */
	public void actionPerformed(ActionEvent evt) {
		try {
			NRMLServer.getNewMessages(this.visitor);
			NRMLServer.getVisitorDetails(this.visitor);
		} catch (Exception e) {
			Utils.de(e + " ChatTimer.actionPerformed() ");
		}
	}
}
