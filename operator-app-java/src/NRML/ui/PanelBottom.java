/**
 * 
 */
package NRML.ui;

import java.awt.BorderLayout;
import java.awt.LayoutManager;

import javax.swing.BorderFactory;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.border.EtchedBorder;

/**
 * @author nramanarayana
 *
 */
public class PanelBottom extends JPanel {

	/**
	 * Constructor
	 */
	public PanelBottom() {
		super(new BorderLayout(0, 0));
		
		JLabel label = new JLabel(
				"<html><font color=#0000FF>Design And Developed By Ram.</font></html>",
				JLabel.CENTER);
		label.setBorder(BorderFactory.createEtchedBorder(EtchedBorder.LOWERED));
		
		add(label, BorderLayout.CENTER);
	}
}
