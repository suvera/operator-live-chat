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

import livechat.Visitor;

public class ProxyClient {
	
	/**
	 * proxyURI
	 */
	public String proxyURI;
	
	public String test_val = "Test";
	
	/**
	 * response
	 */
	public String response;
	
	
	/**
	 * @param URL
	 */
	public ProxyClient(String URL) {
		//Constructor
		this.proxyURI = URL;
	}
	
	/**
	 * @param module
	 * @param action
	 * @param args
	 * @return
	 * @throws Exception
	 */
	public ProxyResponseObject Connect(String module, String action, String[] args) {
		String decodedString;
		String result = "";
		String request = "";
		
		try {
			request = JSON.encode(module, action, args);
						
			URL url = new URL(this.proxyURI+"?");
			URLConnection connection = url.openConnection();
			connection.setDoOutput(true);
	
			OutputStreamWriter out = new OutputStreamWriter(connection.getOutputStream());
			out.write(request);
			out.close();
	
			BufferedReader in = new BufferedReader(new InputStreamReader(connection.getInputStream()));
			while ((decodedString = in.readLine()) != null) {
				result += decodedString;
			}
			this.response = result;
		} catch (Exception e) {
			Utils.de(e + " ProxyClient.Connect ");
		}
		
		Utils.de("Request:" + request);
		Utils.de("Response:" + result);
		
		try {
			return JSON.decode(result);
		}  catch (Exception e) {
			Utils.de(e + " ProxyClient.Connect  returned NULL");
			return null;
		}
	}
	
	/**
	 * @param module
	 * @param action
	 * @param args
	 * @param callBack
	 * @throws Exception
	 */
	public void lateConnect(String module, String action, String[] args, Visitor visitor, String callBack) throws Exception {
		
		String request = JSON.encode(module, action, args);
		new NRMLWorker(this.proxyURI, request, callBack, visitor).execute();
	}

}
