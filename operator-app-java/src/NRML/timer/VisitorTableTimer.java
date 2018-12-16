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
public class VisitorTableTimer implements ActionListener {
	
	/**
	 * Constructor
	 */
	public VisitorTableTimer() {
	}
	
	/* (non-Javadoc)
	 * @see java.awt.event.ActionListener#actionPerformed(java.awt.event.ActionEvent)
	 */
	public void actionPerformed(ActionEvent evt) {
		try {
			NRMLServer.getNewVisitors();
		} catch (Exception e) {
			Utils.de(e + " VisitorTableTimer.actionPerformed() ");
		}
	}
}
