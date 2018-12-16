/**
 * @author nramanarayana
 *
 */
package livechat;

import javax.swing.ImageIcon;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTable;
import javax.swing.ListSelectionModel;
import javax.swing.SwingConstants;
import NRML.*;
import NRML.table.*;
import NRML.timer.*;
import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Dimension;
import java.util.Iterator;
import java.util.Vector;

public class VisitorTable extends JPanel {
	/**
	 * visitor list
	 */
	private Vector<Visitor> visitors;
	
	/**
	 * Table Object
	 */
	private JTable table;
		
	/**
	 * TableModel
	 */
	public NTableModel nmodel;
	
	
	
	/**
	 * Add a visitor to the table
	 * @return the visitors
	 */
	public void addVisitor(Visitor v) {
		try {
			if(!this.visitors.contains(v)) {
				this.visitors.add(v);
				this.nmodel.addRow(v.getTableRow());
				v.startTimer();
			} else {
				Utils.de("Visitor already exists "+v.Name);
			}
		} catch (Exception e) {
			Utils.de(e + "Visitors null VisitorTable.addVisitor");
		}
	}

	/**
	 * Remove Visitor from table
	 * @param Visitor v
	 */
	public void removeVisitor(Visitor v) {
		try {
			int key = this.visitors.indexOf(v);
			
			if(key != -1) {
				//Remove Visitor from Vector
				this.visitors.remove(v);
				//Remove Table Row
				this.nmodel.removeRow(key);
				v.stopTimer();
				
				//If selected row is the current Visitor
				if(ChatPanel.currentSelectedRow == key) {
					this.table.clearSelection();
					ChatPanel.currentSelectedRow = -1;
				}
			} else {
				Utils.de("Visitor doesn't exists " + v.Name);
			}
		} catch (Exception e) {
			Utils.de(e + " VisitorTable.removeVisitor");
		}
	}
	
	/**
	 * @return the visitors
	 */
	public Visitor getAVisitor(int row) {
		try {
			return (Visitor) this.visitors.elementAt(row);
		} catch (Exception e) {
			Utils.de("Visitor doesn't exists for row " + row);
			return null;
		}
	}

	/**
	 * @param set a Visitor as Current Visitor
	 */
	public void selectVisitor(Visitor v) {
		int key = this.visitors.indexOf(v);
		
		try {
			if(key != -1) {
				if(ChatPanel.currentSelectedRow != key)
					this.table.changeSelection(key, 0, false, false);
				
				ChatPanel.currentSelectedRow = key;
				ChatPanel.visitor = v;
			}
		} catch (Exception e) {
			Utils.de("Visitor doesn't exists name: " + v.Name + "VisitorTable.selectVisitor");
		}
	}
		
	/**
	 * @param set a Visitor as Current Visitor
	 */
	public void selectVisitor(int row) {
		try {
			if(row != -1) {
				if(ChatPanel.currentSelectedRow != row)
					this.table.changeSelection(row, 0, false, false);
				
				ChatPanel.currentSelectedRow = row;
				ChatPanel.visitor = this.getAVisitor(row);
			}
		} catch (Exception e) {
			Utils.de("Visitor already selected : " + row + " VisitorTable.selectVisitor");
		}
	}
	
	public int getVisitorCount() {
		try {
			return this.visitors.size();
		} catch (Exception e) {
			Utils.de(e + " visitor count is null VisitorTable.getVisitorCount");
			return 0;
		}
	}
	
	/**
	 * is Visitor Exists?
	 * @param String Name
	 */
	public boolean isVisitorExists(String name) {
		try {
			for(Visitor v:this.visitors) {
				if(v != null && v.Name.equals(name)) {
					return true;
				}
			}
		} catch (Exception e) {
			Utils.de(e + " visitor list null VisitorTable.isVisitorExists");
		}
		
		return false;
	}
	
	/**
	 * get Visitor by Name
	 * @param String Name
	 */
	public Visitor getVisitorByName(String name) {
		try {
			for(Visitor v:this.visitors) {
				if(v != null && v.Name.equals(name)) {
					return v;
				}
			}
		} catch (Exception e) {
			Utils.de(e + " VisitorTable.getVisitorByName");
		}
		
		return null;
	}
		
	/**
	 * Constructor
	 */
	public VisitorTable() {
		super(new BorderLayout(0, 0));
		setOpaque(true);
		
		this.visitors = new Vector<Visitor>();
		
		this.nmodel = new NTableModel();
		this.table = new JTable(this.nmodel);
		
		this.setTableProperties();
		
		//Create the scroll pane and add the table to it.
        JScrollPane scrollPane = new JScrollPane(table);
        scrollPane.setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_ALWAYS);
        
        //Add the scroll pane to this panel.
        add(scrollPane, BorderLayout.CENTER);
        setPreferredSize(new Dimension(450, 110));
        
        this.startChatTime();
	}
	
	/**
	 * set Table Properties
	 */
	private void setTableProperties() {
		
		this.table.setFillsViewportHeight(false);
		//this.table.setShowGrid(false);
		this.table.setIntercellSpacing(new Dimension(3, 3));
		//this.table.setShowVerticalLines(false);
		//this.table.setShowHorizontalLines(false);
		this.table.setGridColor(new Color(229, 229, 229));
		this.table.setRowHeight(20);
		this.table.setRowSelectionAllowed(true);
		this.table.setColumnSelectionAllowed(false);
		this.table.setForeground(new Color(0, 0, 204));
		
		//this.table.setAutoResizeMode( JTable.AUTO_RESIZE_OFF );
		
		this.table.getColumnModel().getColumn(0).setPreferredWidth(16);
		this.table.getColumnModel().getColumn(1).setPreferredWidth(200);
		this.table.getColumnModel().getColumn(2).setPreferredWidth(80);
		this.table.getColumnModel().getColumn(3).setPreferredWidth(40);
		this.table.getColumnModel().getColumn(4).setPreferredWidth(60);
		this.table.getColumnModel().getColumn(5).setPreferredWidth(60);
		//this.table.getColumnModel().getColumn(6).setPreferredWidth(25);
		//this.table.getColumnModel().getColumn(7).setPreferredWidth(25);
		
		//this.table.getColumnModel().getColumn(1).setCellRenderer(new NCellRenderer(SwingConstants.CENTER, SwingConstants.CENTER));
		
		ListSelectionModel rowSM = table.getSelectionModel();
	    rowSM.addListSelectionListener(new VisitorSelectionListener());
	}
	
	/**
	 * update ChatTime
	 */
	public void startChatTime() {
		new NRMLTimer(1000, new WatchTime(this)).start();
	}

	/**
	 * @return the visitors
	 */
	public Vector<Visitor> getVisitors() {
		return visitors;
	}

	/**
	 * @param visitors the visitors to set
	 */
	public void setVisitors(Vector<Visitor> visitors) {
		this.visitors = visitors;
	}
	
	/**
	 * @return Number of rows
	 */
	public int getRowCount() {
		return this.nmodel.getRowCount();
	}

	/**
	 * @param visitors setValueAt
	 */
	public void setValueAt(String obj, int r, int c) {
		this.nmodel.setValueAt(obj, r, c);
	}
	
	/**
	 * @param visitors setValueAt
	 */
	public void setValueAt(ImageIcon obj, int r, int c) {
		this.nmodel.setValueAt(obj, r, c);
	}
	
	
	/**
	 * @param setVisitorStatus
	 */
	public void setVisitorStatus() {
		try {
			int i = 0;
			for(Visitor v:this.visitors) {
				if(v != null) {
					if(v.status.equals("online")) {
						this.setValueAt(Utils.createImageIcon("images/online.png"), i, 0);
					} else {
						this.setValueAt(Utils.createImageIcon("images/offline.png"), i, 0);
					}
					this.setValueAt(v.getStatus(), i, 3);
				}
				i++;
			}
		} catch (Exception e) {
			Utils.de(e + " VisitorTable.setVisitorStatus");
		}
	}
	
	
	
}
