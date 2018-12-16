/**
 * 
 */
package NRML.timer;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import livechat.VisitorTable;
import NRML.Utils;
import NRML.table.*;

/**
 * @author nramanarayana
 *
 */
public class WatchTime implements ActionListener {
	private VisitorTable visitoTable;
	
	private int hour = 0;
	
	private int minute = 0;
	
	private int second = 0;
	
	/**
	 * Constructor
	 */
	public WatchTime(VisitorTable model) {
		this.visitoTable = model;
	}
	
	/* (non-Javadoc)
	 * @see java.awt.event.ActionListener#actionPerformed(java.awt.event.ActionEvent)
	 */
	public void actionPerformed(ActionEvent evt) {
		try {
			this.updateTime();
			int n = this.visitoTable.getRowCount();
			
			for(int i=0; i<n; i++) {
				this.visitoTable.setValueAt(this.getTime(), i, 5);
			}
			this.visitoTable.setVisitorStatus();
			
		} catch (Exception e) {
			Utils.de(e + " WatchTime.actionPerformed() ");
		}
	}
		
	/**
	 * updateHour
	 */
	private void updateHour() {
		hour++;
		if(hour > 23) {
			hour = 0;
		}
	}
	
	/**
	 * updateMinute
	 */
	private void updateMinute() {
		minute++;
		if(minute > 59) {
			minute = 0;
			this.updateHour();
		}
	}
	/**
	 * updateTime
	 */
	private void updateTime() {
		second++;
		if(second > 59) {
			second = 0;
			this.updateMinute();
		}
	}
	
	/**
	 * @return Time String
	 */
	private String getTime() {
		
		return (hour>9?hour:"0"+hour) + ":" + (minute>9?minute:"0"+minute) + ":" + (second>9?second:"0"+second);
	}

}
