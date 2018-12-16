/**
 * 
 */
package NRML.table;

import javax.swing.ListSelectionModel;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;

import livechat.*;
import NRML.Utils;

/**
 * @author nramanarayana
 *
 */
public class VisitorSelectionListener implements ListSelectionListener {

	/**
	 * 
	 */
	public VisitorSelectionListener() {
		// TODO Auto-generated constructor stub
	}

	/* (non-Javadoc)
	 * @see javax.swing.event.ListSelectionListener#valueChanged(javax.swing.event.ListSelectionEvent)
	 */
	public void valueChanged(ListSelectionEvent e) {
        
        //Ignore extra messages.
        if (e.getValueIsAdjusting()) return;

        ListSelectionModel lsm = (ListSelectionModel)e.getSource();

        if (!lsm.isSelectionEmpty()) {
            int selectedRow = lsm.getMinSelectionIndex();
            
            Utils.de("Selected Row:"+selectedRow);
            
            ChatExplorer.chatpanel.setVisitor(selectedRow);
        }
    }

}
