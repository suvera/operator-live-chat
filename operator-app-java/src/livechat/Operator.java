package livechat;
/**
 * 
 */

/**
 * @author nramanarayana
 *
 */
public class Operator {
	public String siteID;
	public String Login;
	public String Name;
	public String ID;
	public String status;
	
	public Operator(String site,String login,String name,String id,String status)
	{
		this.Login=login;
		this.Name=name;
		this.ID=id;
		this.status=status;
		this.siteID=site;	
	}
	
	public void setStatus(String status)
	{
		this.status=status;
	}
	
	public String getStatus()
	{
		return status;
	}
}
