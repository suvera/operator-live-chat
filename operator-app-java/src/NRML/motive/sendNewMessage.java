/**
 * 
 */
package NRML.motive;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import NRML.Utils;
import NRML.NRMLServer;
import javax.swing.JTextArea;

/**
 * @author nramanarayana
 *
 */
public class sendNewMessage implements ActionListener {
	private JTextArea textarea;
	/**
	 * 
	 */
	public sendNewMessage(JTextArea text) {
		this.textarea = text;
	}

	/* (non-Javadoc)
	 * @see java.awt.event.ActionListener#actionPerformed(java.awt.event.ActionEvent)
	 */
	public void actionPerformed(ActionEvent e) {
		String text = this.textarea.getText();
		this.textarea.setText("");
		
		if (text != null && text.length() != 0) {
			try {
				NRMLServer.sendNewMessage(text, "online");
			} catch (Exception ex) {
				Utils.de(ex + " send message failed, sendNewMessage.actionPerformed() ");
			}
		}
	}

}
