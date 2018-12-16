package livechat;
/**
 * @author nramanarayana
 *
 */
import javax.swing.*;
import java.awt.*;
import java.util.Vector;

import NRML.*;
import NRML.timer.*;
import NRML.ui.*;

// Main class
public class ChatPanel extends JPanel implements LiveConstants {
	
	private PanelBottom bottompanel;
	private PanelCenter centerpanel;
	private PanelMessage messagepanel;
	private PanelTop toppanel;
	
	public VisitorTable visitortable;
	
	//Static Objects
	public static Operator operator;
	public static Visitor visitor;
	public static int currentSelectedRow = -1;
	

	/**
	 * Constructor
	 */
	public ChatPanel() {
		super(new BorderLayout());
		
		this.toppanel = new PanelTop();
		this.bottompanel = new PanelBottom();
		this.visitortable = new VisitorTable();
		this.centerpanel = new PanelCenter();
		this.messagepanel = new PanelMessage();
		
		add(createGUI(), BorderLayout.CENTER);
	}
	
	/**
	 * update ChatTime
	 */
	public void startTimer() {
		new NRMLTimer(PING_TIME, new VisitorTableTimer()).start();
	}
	
	/**
	 * Add a visitor to the table
	 * @return the visitors
	 */
	public void addVisitor(Visitor v) {
		this.visitortable.addVisitor(v);
	}

	/**
	 * Remove Visitor from table
	 * @param Visitor v
	 */
	public void removeVisitor(Visitor v) {
		this.visitortable.removeVisitor(v);
	}
	
	/**
	 * set Visitor as current
	 * @param Visitor v
	 */
	public void setVisitor(Visitor v) {
		this.visitortable.selectVisitor(v);
		this.displayVisitorChat(v);
	}
	
	/**
	 * set Visitor as current by Table row
	 * @param Visitor v
	 */
	public void setVisitor(int row) {
		this.visitortable.selectVisitor(row);
		this.displayVisitorChat(this.visitortable.getAVisitor(row));
	}
	
	/**
	 * is Visitor Exists?
	 * @param String Name
	 */
	public boolean isVisitorExists(String name) {
		return this.visitortable.isVisitorExists(name);
	}
	
	/**
	 * get Visitor by Name
	 * @param String Name
	 */
	public Visitor getVisitorByName(String name) {
		return this.visitortable.getVisitorByName(name);
	}
	
	/**
	 * createGUI
	 * @return
	 */
	public JPanel createGUI() {

		centerpanel.add(visitortable, BorderLayout.PAGE_START);
		centerpanel.add(this.messagepanel, BorderLayout.CENTER);
			
		JPanel primary = new JPanel();
		primary.setLayout(new BorderLayout());
		
		primary.add(this.toppanel, BorderLayout.PAGE_START);
		primary.add(this.centerpanel, BorderLayout.CENTER);
		primary.add(this.bottompanel, BorderLayout.PAGE_END);
			
		primary.setBorder(BorderFactory.createEmptyBorder(0, 1, 4, 2));

		JScrollPane jspane = new JScrollPane(primary);
		jspane.setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_ALWAYS);
		add(jspane, BorderLayout.CENTER);
		
		return primary;
	}
	
	/**
	 * @param v
	 */
	private void displayVisitorChat(Visitor v) {
		try {
			if (v.chatText == null) {
				this.updateChatMessage(Utils.get_html());
			} else {
				this.updateChatMessage(v.chatText);
			}
			if (v.infoText == null) {
				this.setVisitorInfo(Utils.get_html());
			} else {
				this.setVisitorInfo(v.infoText);
			}
		} catch (Exception e) {
			Utils.de(e.getMessage() + " ChatPanel.displayVisitorChat ");
		}
	}
	
	/**
	 * @return the visitors
	 */
	public Vector<Visitor> getAllVisitors() {
		return this.visitortable.getVisitors();
	}
	
	/**
	 * @param name
	 */
	public void setOperatorName(String name) {
		this.toppanel.showOperatorName(name);
	}
	
	/**
	 * @param num
	 */
	public void setNumOfVisitors() {
		int num = this.visitortable.getVisitorCount();
		this.toppanel.showNumOfVisitors(num);
	}
	
	/**
	 * @param msg
	 */
	public void updateChatMessage(String msg) {
		visitor.setChatText(msg);
		this.messagepanel.updateChatMessage(msg);
	}
	
	/**
	 * @param msg
	 */
	public void setVisitorInfo(String msg) {
		visitor.setInfoText(msg);
		this.messagepanel.setVisitorInfo(msg);
	}
	
	
	/**
	 * Get Chat Text
	 */
	public String getChatText() {
		return this.messagepanel.getChatText();
	}
	
	/**
	 * Get Visitor Information
	 */
	public String getVisitorInfoText() {
		return this.messagepanel.getVisitorInfoText();
	}
	
	
	
	
} // Main class ends here
