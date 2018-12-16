package NRML.table;

import javax.swing.table.DefaultTableModel;
import NRML.Utils;
/**
 * @author nramanarayana
 *
 */
public class NTableModel extends DefaultTableModel{
	
	private String[] header = {
			" ",
			"Visitor Id",
			"IP Address",
			"Status",
			"Start Time",
			"Chat Time",
			"City",
			"Country"
	};
	
	private String[][] testData = {
		{"Ic1", "Test ID", "192.168.2.100", "Online", "0", "0", "Pune", "USA"},
		{"Ic2", "HJGHghj", "192.168.2.101", "Online", "0", "0", "Boston", "India"},
		{"Ic3", "Uhjhghj", "192.168.2.102", "Offline", "0", "0", "Chennai", "USA"}
	}; 
	
	public NTableModel() {
		super();
		
		//this.setDataVector(testData, header);
		this.setColumnIdentifiers(this.header);
	}

	/**
	 * @return the header
	 */
	public String[] getHeader() {
		return header;
	}

	/**
	 * @param header the header to set
	 */
	public void setHeader(String[] header) {
		this.header = header;
		this.setColumnIdentifiers(this.header);
	}
	
	@Override
	public boolean isCellEditable(int rowIndex, int columnIndex) {
	    return false;
	}
}
