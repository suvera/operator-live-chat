/**
 * 
 */
package NRML;

/**
 * @author nramanarayana
 *
 */

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.net.URL;
import java.net.URLConnection;
import javax.swing.SwingWorker;
import livechat.Visitor;
import java.util.concurrent.*;

public class NRMLWorker extends SwingWorker<String, Void> {
	
	/**
	 * Request Data
	 */
	public String request;
	
	/**
	 * Response Data
	 */
	public String response;
	
	/**
	 * proxyURI
	 */
	public String proxyURI;
	
	/**
	 * callBack Method Name
	 */
	public String callBack;
	
	/**
	 * visitor
	 */
	public Visitor visitor;
	
	
	/**
	 * @param url
	 * @param reg
	 * @param callback
	 * @param v
	 */
	public NRMLWorker(String url, String reg, String callback, Visitor v) {
		super();
		this.proxyURI = url;
		this.request = reg;
		this.callBack = callback;
		this.visitor = v;
	}
	
    /* (non-Javadoc)
     * @see javax.swing.SwingWorker#doInBackground()
     */
    public String doInBackground() {  	
    	String result = "";
    	String decodedString;
    	
    	try {
			URL url = new URL(this.proxyURI + "?");
			URLConnection connection = url.openConnection();
			connection.setDoOutput(true);
	
			OutputStreamWriter out = new OutputStreamWriter(connection.getOutputStream());
			out.write(this.request);
			out.close();
	
			BufferedReader in = new BufferedReader(new InputStreamReader(connection.getInputStream()));
					
			while ((decodedString = in.readLine()) != null) {
				result += decodedString;
			}		
    	} catch(Exception e) {
    		Utils.de(e + " NRMLWorker.doInBackground");
    	}
    	//Utils.de("Request:" + this.request);
		
		return result;
    }

    /* (non-Javadoc)
     * @see javax.swing.SwingWorker#done()
     */
    protected void done() {
        try {
        	this.response = get();
        	ProxyResponseObject proxyResponseObj = null;
        	
        	//Utils.de("Response:" + this.response);
        	
        	try {
        		proxyResponseObj = JSON.decode(this.response);
        	} catch (Exception e) {
        		Utils.de(e + " NRMLWorker.doInBackground JSON.decode ");
        	}
        	
        	new UIUpdater(this.response, proxyResponseObj, this.visitor).callBack(this.callBack);
        	
        } catch (Exception ignore) {
        	Utils.de(ignore + " NRMLWorker.doInBackground");
        }
    }
}
