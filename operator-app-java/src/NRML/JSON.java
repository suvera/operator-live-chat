/**
 * 
 */
package NRML;

/**
 * @author nramanarayana
 *
 */
import org.json.*;
import java.util.*;

public class JSON {

	public static String encode(String module, String action, String[] args) throws JSONException {
		ProxyRequestObject prxy = new ProxyRequestObject();
		prxy.setModule(module);
		prxy.setAction(action);
		prxy.setArgs(args);
		
		JSONObject json = new JSONObject(prxy);
		
		//json.put("args", map);
		
		return json.toString();
	}
	
	public static ProxyResponseObject decode(String msg) throws JSONException {
		ProxyResponseObject res = new ProxyResponseObject();
		
		JSONObject json = new JSONObject(msg);
		
		JSONArray jsonarr = json.getJSONArray("result");
		String[] output = new String[100];
		
		for(int i=0; i< jsonarr.length(); i++) {
			output[i] = jsonarr.getString(i);
		}
		
		res.setResponse(output);
		res.setId(json.getString("id"));

		return res;
	}
	
	public static ProxyResponseObject decode2(String msg) throws JSONException {
		ProxyResponseObject res = new ProxyResponseObject();
		
		JSONObject json = new JSONObject(msg);
		
		JSONArray jsonarr = json.getJSONArray("result");
		JSONArray[] output = new JSONArray[100];
		
		for(int i=0; i< jsonarr.length(); i++) {
			output[i] = jsonarr.getJSONArray(i);
		}
		
		res.setResponse2(output);
		res.setId(json.getString("id"));

		return res;
	}
}
