package livechat;
/**
 * @author nramanarayana
 *
 */
public interface LiveConstants
{

    public static final String SERVER_HOST = "http://livechat.naramala.com/service/OperatorProxy.php";
    public static final String[] ACTIONCOMMANDS = { "Online", "Invisible", "Back in 5 minutes" };
    public static final int PING_TIME = 5000;
    public static final int BACKLOG = 10;
    public static final int MAX_MESSAGE_SIZE = 2048;
    
    public static final int CLIENT_LOGIN = 1;
    public static final int CLIENT_LOGOUT = 2;
    public static final int PUBLIC_CHAT = 3;
    public static final int PRIVATE_CHAT = 4;
    public static final int SERVER_DOWN = 5;
    public static final int USERS_LIST = 6;
    public static final int CHANGE_STATUS = 7;
	
    public static final int ONLINE = 0;
    public static final int OFFLINE = 1;
    public static final int BACK5 = 2;
    public static final int IDLE = 3;
	
	public static final String IMAGE_ICON = "images/live.jpg";
	//ClassLoader.getSystemClassLoader().getResource("images/naramala.gif").toString();
	public static final String WINDOW_IMAGE_ICON = "images/icon.jpg";
	
}
