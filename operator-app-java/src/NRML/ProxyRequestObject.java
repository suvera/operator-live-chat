/**
 * 
 */
package NRML;

import java.util.HashMap;
import java.util.Map;

/**
 * @author nramanarayana
 *
 */
public class ProxyRequestObject {
	
	private String id;
	
	private String module;
	
	private String action;
	
	private Map<String,String> args;
	
	/**
	 * @return the module
	 */
	public String getModule() {
		return module;
	}
	/**
	 * @param module the module to set
	 */
	public void setModule(String module) {
		this.module = module;
	}
	/**
	 * @return the action
	 */
	public String getAction() {
		return action;
	}
	/**
	 * @param action the action to set
	 */
	public void setAction(String action) {
		this.action = action;
	}
	/**
	 * @return the args
	 */
	public Map<String,String> getArgs() {
		return args;
	}
	/**
	 * @param args the stargs to set
	 */
	public void setArgs(String[] stargs) {
		
		args = new HashMap<String,String>();
		
		for(int i=0; i< stargs.length; i++) {
			String[] values = stargs[i].split(":");
			if(values.length <= 1) {
				args.put("argv"+i, stargs[i]);
			} else {
				args.put(values[0], values[1]);
			}
		}
	}
	/**
	 * @return the id
	 */
	public String getId() {
		return Utils.getRandomId();
	}
	/**
	 * @param id the id to set
	 */
	public void setId(String id) {
		this.id = id;
	}
	
}
