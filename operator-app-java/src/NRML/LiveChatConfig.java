/**
 * 
 */
package NRML;

/**
 * @author nramanarayana
 *
 */
import java.util.Properties;

public class LiveChatConfig {
	protected Properties prop;
	
	public LiveChatConfig() {
		try {
			prop = new Properties();
			prop.load(ClassLoader.getSystemResourceAsStream("Config.properties"));
		} catch(Exception e) {
			Utils.de(e+" LiveChatConfig");
		}
	}
	
	public String getOption(String key) {
		try {
			return prop.getProperty(key);
		} catch(Exception e) {
			Utils.de(e+" LiveChatConfig.getOption");
			return "";
		}
	}

}
