/**
 * 
 */
package NRML.ui;

import java.awt.BorderLayout;
import java.awt.Color;

import javax.swing.JComboBox;
import javax.swing.JLabel;
import javax.swing.JPanel;
import livechat.*;
import NRML.motive.*;

/**
 * @author nramanarayana
 *
 */
public class PanelTop extends JPanel implements LiveConstants {
	
	private JLabel no_of_visitors;
	
	private JPanel subpaneL1;
	
	private JPanel subpaneL2;
	
	private JPanel subpaneL3;
	
	private JLabel operator_name;
	
	
	/**
	 * @param operator_name the operator_name to set
	 */
	public void showOperatorName(String operator_name) {
		this.operator_name.setText(operator_name);
	}

	/**
	 * Constructor
	 */
	public PanelTop() {
		super(new BorderLayout());
		
		subpaneL1 = new JPanel();
		subpaneL2 = new JPanel();
		subpaneL3 = new JPanel();
		
		this.setPanel1();
		this.setPanel2();
		this.setPanel3();
		
		add(subpaneL1, BorderLayout.LINE_START);
		add(subpaneL2, BorderLayout.CENTER);
		add(subpaneL3, BorderLayout.LINE_END);
		setBackground(new Color(84, 82, 84));
	}
	
	/**
	 * @param num
	 */
	public void showNumOfVisitors(int num) {
		this.no_of_visitors.setText(""+num);
	}

	/**
	 * setPanel1
	 */
	private void setPanel1() {
		
		JLabel nametext = new JLabel("Welcome");
		nametext.setForeground(new Color(0xffffff));
		
		operator_name = new JLabel("Guest");
		operator_name.setForeground(new Color(0xffffff));
		
		subpaneL1.add(nametext, BorderLayout.LINE_START);
		subpaneL1.add(operator_name, BorderLayout.LINE_END);
		subpaneL1.setBackground(new Color(84, 82, 84));
	}
	
	/**
	 * setPanel2
	 */
	private void setPanel2() {
		
		no_of_visitors = new JLabel("Nil");
		no_of_visitors.setForeground(new Color(0xffffff));
	
		JLabel no_of_visitors_text = new JLabel("No. of Visitors: ");
		no_of_visitors_text.setForeground(new Color(0xffffff));
		
		subpaneL2.add(no_of_visitors_text, BorderLayout.LINE_START);
		subpaneL2.add(no_of_visitors, BorderLayout.LINE_END);
		subpaneL2.setBackground(new Color(84, 82, 84));
	}
	
	/**
	 * setPanel3
	 */
	private void setPanel3() {
		
		JLabel state = new JLabel("Status");
		state.setForeground(new Color(0xffffff));
		
		JComboBox ActList = new JComboBox(ACTIONCOMMANDS);
		ActList.setSelectedIndex(0);
		ActList.addActionListener(new StateChanger(ActList));
	
		subpaneL3.add(state, BorderLayout.LINE_START);
		subpaneL3.add(ActList, BorderLayout.LINE_END);
		subpaneL3.setBackground(new Color(84, 82, 84));
	}

}
