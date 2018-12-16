package NRML;


/**
 * @author nramanarayana
 *
 */
import java.util.Enumeration;
import java.util.Vector;

import javax.swing.tree.DefaultMutableTreeNode;

import livechat.ChatExplorer;
import livechat.ChatPanel;
import livechat.Visitor;


public class NRMLServer {
	/**
	 * 
	 */
	public static boolean syncLockMsg = false;
	
	/**
	 * 
	 */
	public static boolean syncLockVis = false;
	
	/**
	 * 
	 */
	public static boolean syncLockNewVis = false;
		
	
	/**
	 * 
	 */
	public static void getNewMessages(Visitor visitor) {
		try {
			
			if (visitor != null) {
				if(visitor.timerLock) return;
				
				String[] args = {
						"last_msg_id:" + visitor.last_msg_id, 
						"site_id:" + ChatExplorer._site, 
						"contact_id:" + visitor.ContactID,  
						"receiver:" + ChatPanel.operator.ID,
						"firstTime:" + visitor.firstTime
				};
				visitor.firstTime = "no";
				
				visitor.timerLock = true;
				ChatExplorer.proxy.lateConnect("Message", "get_new_message", args, visitor, "MessageUpdater");
			} else {
				visitor.timerLock = false;
				ChatExplorer.chatpanel.updateChatMessage(Utils.get_html());
			}
		} catch (Exception e) {
			visitor.timerLock = false;
			Utils.de(e + " NRMLServer.getNewMessages");
		}
	}
	
	/**
	 * 
	 */
	public static void getVisitorDetails(Visitor visitor) {
		if(visitor.syncLock) return;
		
		if (visitor != null) {		
			try {
				String[] infoargs = {
						"contact_id:" + visitor.ContactID
				};
				visitor.syncLock = true;
				
				ChatExplorer.proxy.lateConnect("Visitors", "get_info", infoargs, visitor, "VisitorTrackerUpdater");
			} catch (Exception e) {
				visitor.syncLock = false;
				System.out.println(e + " NRMLServer.getVisitorDetails ");
			}
		} else {
			ChatExplorer.chatpanel.setVisitorInfo(Utils.get_border_html());
		}
	}
	
	/**
	 * @param msg
	 */
	public static void sendNewMessage(String newmsg, String status) {
		if(newmsg == null || newmsg.length() == 0) return;
		
		try {
			if (ChatPanel.visitor != null) {
				String encodetext = newmsg.replaceAll(" ", " ");
				
				String htmltext = ChatExplorer.chatpanel.getChatText();
				ChatExplorer.chatpanel.updateChatMessage(Utils.append_table_row(htmltext, "me", encodetext));
				
				String[] args = {
						"last_msg_id:" + ChatPanel.visitor.last_msg_id, 
						"message:" + Utils.encodeurl(encodetext), 
						"site_id:" + ChatExplorer._site, 
						"contact_id:" + ChatPanel.visitor.ContactID, 
						"sender:" + ChatPanel.operator.ID, 
						"receiver:" + ChatPanel.visitor.ContactID,
						"status:" + status
				};
				try {
					ChatExplorer.proxy.Connect("Message", "add", args);
				} catch (Exception e) {
					System.out.println(e + " send message in proxy, NRMLServer.sendNewMessage() ");
				}
			}
		} catch (Exception e) {
			System.out.println(e + " send message, NRMLServer.sendNewMessage() ");
		}
	}
	
	/**
	 * 
	 */
	public static void getNewVisitors() {
		if(syncLockNewVis) return;
		
		try {
			String[] args = {
					ChatExplorer._site
			};
			syncLockNewVis = true;
			ChatExplorer.proxy.lateConnect("VisitorsOnline", "get_online_visitors", args, null, "VisitorUpdater");
		} catch (Exception e) {
			syncLockNewVis = false;
			System.out.println(e + " NRMLServer.getNewVisitors() ");
		}
	}
	
	/**
	 * @param num_new_vstrs
	 * @param newVstrs
	 */
	public static void removeExpiredVisitors( Vector<Visitor> newVisitors) {
		try {
			Vector<Visitor> currentVisitors = ChatExplorer.chatpanel.getAllVisitors();

			for(Visitor v:currentVisitors) {
				if(v != null && !Utils.inVector(newVisitors, v.Name) ) {
					ChatExplorer.chatpanel.removeVisitor(v);
				}
			}
		} catch (Exception e) {
			System.out.println(e + " NRMLServer.removeExpiredVisitors ");
		}
	}
	
	/**
	 * @param operatorId
	 * @param newStatus
	 */
	public static void setOperatorStatus(String operatorId, String newStatus) {
				
		String[] args = {
				"operator_id:" + operatorId, 
				"new_status:" + newStatus
		};
		try {
			ChatExplorer.proxy.Connect("OperatorOnline", "change_status", args);
		} catch (Exception e) {
			System.out.println(e + " NRMLServer.setOperatorStatus");
		}
	}
	
	/**
	 * 
	 */
	public static void operatorLogout() {
		try {
			String[] args = {
					ChatPanel.operator.ID
			};
			ChatExplorer.proxy.Connect("Operators", "logout", args);
		} catch (Exception e) {
			System.out.println(e + " NRMLServer.operatorLogout ");
		}
	}
	
	/**
	 * @param msg
	 */
	public static void addToBuffer(String newmsg, String status) {
		try {
			if (ChatPanel.visitor != null) {
				String encodetext = newmsg.replaceAll(" ", " ");
								
				String[] args = {
						"message:" + Utils.encodeurl(encodetext), 
						"site_id:" + ChatExplorer._site, 
						"contact_id:" + ChatPanel.visitor.ContactID, 
						"sender:" + ChatPanel.operator.ID, 
						"receiver:" + ChatPanel.visitor.ContactID,
						"status:" + status
				};
				try {
					ChatExplorer.proxy.Connect("ChatBuffer", "add", args);
				} catch (Exception e) {
					Utils.de(e + " NRMLServer.addToBuffer() ");
				}
			}
		} catch (Exception e) {
			Utils.de(e + " NRMLServer.addToBuffer() ");
		}
	}
	
}
