/**
 * 
 */
package NRML.table;

import javax.swing.table.DefaultTableCellRenderer;
/**
 * @author nramanarayana
 *
 */
public class NCellRenderer extends DefaultTableCellRenderer {
	   
    /**
     * @param align
     */
    public NCellRenderer(int align, int valign) {
        super();
        this.setVerticalAlignment(valign);
        this.setHorizontalAlignment(align);
    }
}

