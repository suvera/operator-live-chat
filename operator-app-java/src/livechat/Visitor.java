package livechat;
/**
 * @author nramanarayana
 * 
 */
import NRML.*;
import NRML.timer.ChatTimer;

import java.util.Date;
import java.util.Vector;

public class Visitor implements LiveConstants {
	public String Name;
	public String ContactID;
	
	public String Site;
	public String IP;
	
	public String City;
	public String Country;
	
	public String status;
	public String chatTime = "0";
	public String waitTime;
	public String chatStartTime;
	
	public String last_msg_id = "0";
	public String chatText;
	public String infoText;
	public String firstTime = "yes";
	
	public int time;
	public boolean timerLock;
	public boolean syncLock;
	public NRMLTimer timer;
	

	public Visitor(String name, String id, String site, String isonline, String ip, String city, String country) {
		this.Name = name;
		this.ContactID = id;
		
		this.Site = site;
		this.status = isonline;
		
		this.IP = ip;
		
		this.City = city;
		this.Country = country;
		
		this.chatStartTime = new Date().toString();
		this.time = 4000 + (int) (Math.random() * 1000);
		this.timerLock = false;
		this.syncLock = false;
	}
	
	/**
	 * update Chat
	 */
	public void startTimer() {
		this.timer =  new NRMLTimer(this.time, new ChatTimer(this));
		this.timer.start();
	}
	
	/**
	 * update Chat
	 */
	public void stopTimer() {
		this.timer.stop();
	}

	/* (non-Javadoc)
	 * @see java.lang.Object#toString()
	 */
	@Override
	public String toString() {
		return this.Name;
	}
	
	/**
	 * @return Table Row Vector
	 */
	public Vector<Object> getTableRow() {
		Vector<Object> vect = new Vector<Object>();
		
		if(this.status.equals("online")) {
			vect.add(Utils.createImageIcon("images/online.png"));
		} else {
			vect.add(Utils.createImageIcon("images/offline.png"));
		}
		vect.add(this.Name);
		vect.add(this.IP);
		vect.add(this.status);
		
		vect.add(this.chatStartTime);
		vect.add("0");
		
		vect.add(this.City);
		vect.add(this.Country);
		
		return vect;
	}

	/**
	 * @return the chatText
	 */
	public String getChatText() {
		return chatText;
	}

	/**
	 * @param chatText the chatText to set
	 */
	public void setChatText(String chatText) {
		this.chatText = chatText;
	}

	/**
	 * @return the infoText
	 */
	public String getInfoText() {
		return infoText;
	}

	/**
	 * @param infoText the infoText to set
	 */
	public void setInfoText(String infoText) {
		this.infoText = infoText;
	}

	/**
	 * @return the status
	 */
	public String getStatus() {
		return status;
	}

	/**
	 * @param status the status to set
	 */
	public void setStatus(String status) {
		this.status = status;
	}
}
