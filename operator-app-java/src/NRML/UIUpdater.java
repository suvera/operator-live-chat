/**
 * 
 */
package NRML;

import java.util.Vector;

import org.json.JSONArray;

import livechat.ChatExplorer;
import livechat.ChatPanel;
import livechat.Visitor;

/**
 * @author nramanarayana
 *
 */

public class UIUpdater {
		
	/**
	 * responseText
	 */
	public String responseText;
	
	/**
	 * proxyResponseObj
	 */
	ProxyResponseObject proxyResponseObj;
	
	/**
	 * visitor
	 */
	public Visitor visitor;
	
	
	/**
	 * @param response
	 * @param prxyObj
	 */
	public UIUpdater(String response, ProxyResponseObject prxyObj, Visitor v) {
		this.responseText = response;
		this.proxyResponseObj = prxyObj;
		this.visitor = v;
	}
	
	/**
	 * @param func
	 */
	public void callBack(String func) {
		if(func.equals("MessageUpdater")) {
			this.MessageUpdater();
			
		} else if(func.equals("VisitorUpdater")) {
			this.VisitorUpdater();
			
		} else if(func.equals("VisitorTrackerUpdater")) {
			this.VisitorTrackerUpdater();
			
		} else {
		}
	}
	
	/**
	 * 
	 */
	private void MessageUpdater() {
		this.visitor.timerLock = false;
		
		try {
			ProxyResponseObject prxyres = JSON.decode2(this.responseText);
			JSONArray[] res = prxyres.getResponse2();
			
			String htmltext = this.visitor.getChatText(); //ChatExplorer.chatpanel.getChatText();

			try {
				for(int i=0; i<res.length; i++) {
					if(res[i] != null) 
					{
						String from = "Visitor";
						String[] messages = Utils.get_json_array(res[i]);
						
						if(messages[3] != null && messages[3].equals(ChatPanel.operator.ID)) {
							from = "me";
						}
						
						if(messages[0] != null && !messages[0].equals("")) {
							htmltext = Utils.append_table_row(htmltext, from, messages[0]);
						}
						if(messages[1] != null && !messages[1].equals("")) {
							this.visitor.last_msg_id = messages[1];
						}
					}
				}
			} catch (Exception e) {
				Utils.de(e + " FOR loop in UIUpdater.MessageUpdater");
			}
			this.visitor.setChatText(htmltext);
			
			if(ChatPanel.visitor != null && ChatPanel.visitor.equals(this.visitor))
				ChatExplorer.chatpanel.updateChatMessage(htmltext);
			
		} catch (Exception e) {
			Utils.de(e + " UIUpdater.MessageUpdater");
		}
	}
	
	private void VisitorUpdater() {
		NRMLServer.syncLockNewVis = false;
		
		try {
			ProxyResponseObject prxyres = JSON.decode2(this.responseText);
			JSONArray[] res = prxyres.getResponse2();
						
			Vector<Visitor> new_visitor_list = new Vector<Visitor>();
			
			try { 
				for(int i=0; i < res.length; i++) {
					if(res[i] != null)
					{
						String[] visitorArr = Utils.get_json_array(res[i]);
						Visitor v = null;
						
						try {
							v = new Visitor(visitorArr[0], visitorArr[1], visitorArr[2], visitorArr[3],visitorArr[4], visitorArr[5], visitorArr[6]);
							
							if(!ChatExplorer.chatpanel.isVisitorExists(visitorArr[0])) {
								ChatExplorer.chatpanel.addVisitor(v);
							} else {
								Visitor myVisitor = ChatExplorer.chatpanel.getVisitorByName(visitorArr[0]);
								myVisitor.setStatus(v.getStatus());
							}
							
						} catch (Exception e) {
							Utils.de(e + "  Create New Visitor UIUpdater.VisitorUpdater()");
						}
						if(v != null)
							new_visitor_list.add(v);
					}
				}
				ChatExplorer.chatpanel.setNumOfVisitors();
			} catch (Exception e) {
				Utils.de(e + " for loop in UIUpdater.VisitorUpdater()");
			}
			
			//Remove Expired Visitors
			NRMLServer.removeExpiredVisitors(new_visitor_list);
		} catch (Exception e) {
			Utils.de(e + " UIUpdater.VisitorUpdater() ");
		}
	}
	
	/**
	 * VisitorTrackerUpdater
	 */
	private void VisitorTrackerUpdater() {
		this.visitor.syncLock = false;
		
		try {
			String infohtml = Utils.get_border_html();
			String[] infores = this.proxyResponseObj.getResponse();
		
			infohtml = Utils.append_html(infohtml, infores[0]);
			
			this.visitor.setInfoText(infohtml);
						
			if(ChatPanel.visitor != null && ChatPanel.visitor.equals(this.visitor))
				ChatExplorer.chatpanel.setVisitorInfo(infohtml);
			
		} catch (Exception e) {
			Utils.de(e + " UIUpdater.VisitorTrackerUpdater ");
		}
	}
}
