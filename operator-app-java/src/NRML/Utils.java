/**
 * 
 */
package NRML;

/**
 * @author nramanarayana
 *
 */
import java.util.UUID;
import java.util.Vector;

import org.json.*;
import java.net.URLEncoder;
import java.net.URLDecoder;

import javax.swing.ImageIcon;

import livechat.Visitor;

public class Utils {
	
	/**
	 * @return
	 */
	public static String getRandomId() {
		return UUID.randomUUID().toString();
	}
	
	/**
	 * @param msg
	 */
	public static void de(String msg) {
		System.out.println(msg);
	}
	
	/**
	 * @return
	 */
	public static String get_html() {
		return "<html><body><table width='100%'>\n<!--APPEND-->\n</table></body></html>";
	}
	
	/**
	 * @return
	 */
	public static String get_border_html() {
		return "<html><body><table width='100%' border=1>\n<!--APPEND-->\n</table></body></html>";
	}
	
	/**
	 * @param text
	 * @param replace
	 * @return
	 */
	public static String append_html(String text, String replace) {
		return text.replace("<!--APPEND-->", replace+"\n<!--APPEND-->");
	}
	
	/**
	 * @param text
	 * @param name
	 * @param msg
	 * @return
	 */
	public static String append_table_row(String text, String name, String msg) {
		String td;
		
		if(name.equals("me")) {
			td = "<b>"+name+":</b>";
		} else {
			td = "<font color='#0000CC'><b>"+name+":</b></font>";
		}
		msg = decodeurl(msg);
		return append_html(text, "<tr><td width='10%'>"+td+"</td><td width='90%'>"+msg+"</td></tr>");
	}
	
	public static String[] get_json_array(JSONArray json) throws JSONException {
			
		String[] output = new String[100];
		
		for(int i=0; i< json.length(); i++) {
			output[i] = json.getString(i);
		}
		
		return output;
	}
	
	public static String encodeurl(String msg) {
		try {
			msg = URLEncoder.encode(msg, "UTF-8");
		} catch (Exception e) {
			System.out.println(e + " encodeurl");
		}
		return msg;
	}
	
	public static String decodeurl(String msg) {
		try {
			msg = URLDecoder.decode(msg, "UTF-8");
		} catch (Exception e) {
			System.out.println(e + " decodeurl");
		}
		return msg;
	}
	
	/** Returns an ImageIcon, or null if the path was invalid. */
	public static ImageIcon createImageIcon(String path) {
		java.net.URL imgURL = ClassLoader.getSystemClassLoader().getResource(path);
		if (imgURL != null) {
			return new ImageIcon(imgURL);
		} else {
			System.err.println("Couldn't find file: " + path);
			return null;
		}
	}
	
	/**
	 * @param url
	 * @return
	 */
	public static String getWinOpenJS(String url) {
		return url;
	}
	
	/**
	 * @param Vector e
	 * @param name
	 * @return
	 */
	public static boolean inVector(Vector<Visitor> e, String name) {
		try {		
			for(Visitor v:e) {
				if(v != null && v.Name.toString().equals(name.toString()) ) {
					return true;
				}
			}
		} catch (Exception ex) {
			System.out.println(ex + " Utils.inVector");
		}
		
		return false;
	}
	
}
