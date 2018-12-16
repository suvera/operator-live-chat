/**
 * 
 */
package NRML;

/**
 * @author nramanarayana
 *
 */
import org.json.*;

public class ProxyResponseObject {
	private String id;
	
	private String[] response;
	
	private JSONArray[] response2;

	/**
	 * @return the response
	 */
	public String[] getResponse() {
		return response;
	}

	/**
	 * @param response the response to set
	 */
	public void setResponse(String[] response) {
		this.response = response;
	}

	/**
	 * @return the id
	 */
	public String getId() {
		return id;
	}

	/**
	 * @param id the id to set
	 */
	public void setId(String id) {
		this.id = id;
	}

	/**
	 * @return the response2
	 */
	public JSONArray[] getResponse2() {
		return response2;
	}

	/**
	 * @param response2 the response2 to set
	 */
	public void setResponse2(JSONArray[] response2) {
		this.response2 = response2;
	}
}
