/**
 * 
 */
package NRML.motive;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JComboBox;
import NRML.Utils;
import livechat.ChatPanel;
import livechat.LiveConstants;

import NRML.NRMLServer;

/**
 * @author nramanarayana
 *
 */
public class StateChanger implements ActionListener,LiveConstants {
	
	private JComboBox stateList;
		
	/**
	 * 
	 */
	public StateChanger(JComboBox ta) {
		this.stateList = ta;
	}

	/* (non-Javadoc)
	 * @see java.awt.event.ActionListener#actionPerformed(java.awt.event.ActionEvent)
	 */
	public void actionPerformed(ActionEvent e) {
		JComboBox cb = (JComboBox) e.getSource();
		int stat = (int) cb.getSelectedIndex();
		
		String item = "no";
		
		switch (stat) {
			case ONLINE:
				ChatPanel.operator.setStatus("Online");
				item = "Online";
				break;
			case OFFLINE:
				ChatPanel.operator.setStatus("Offline");
				item = "Offline";
				break;
			case BACK5:
				ChatPanel.operator.setStatus("Back5");
				item = "Back5";
				break;
		}
		
		try {
			NRMLServer.setOperatorStatus(ChatPanel.operator.ID, item);
		} catch (Exception ex) {
			Utils.de(ex + " in StateChanger.actionPerformed()");
		}
	}

}
