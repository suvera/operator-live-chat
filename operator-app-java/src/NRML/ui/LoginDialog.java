package NRML.ui;
/**
 * @author nramanarayana
 *
 */
import java.awt.Container;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;

import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPasswordField;
import javax.swing.JTextField;

import livechat.ChatExplorer;
import livechat.LiveConstants;
import livechat.Operator;
import NRML.Utils;
import com.jeans.trayicon.WindowsTrayIcon;

import NRML.*;

public class LoginDialog extends JDialog implements ActionListener, LiveConstants {

	private String _username;
	private String _password;
	private String _site;
	private JLabel label1;
	private JLabel label2;
	private JLabel label3;
	private JTextField user;
	private JTextField site;
	private JPasswordField password;
	private JButton ok;
	private JButton cancel;
	private Container container;
	private Operator operator;
	private JLabel status;

	public LoginDialog(JFrame jframe) {
		super(jframe, "Login", true);
		_username = null;
		_password = null;
		_site = null;
		initDialogBox(jframe);
	}

	private void initDialogBox(JFrame jframe) {
		container = getContentPane();
		container.setLayout(null);
		
		label3 = new JLabel("Site ID :");
		label3.setBounds(10, 10, 80, 20);
		label1 = new JLabel("Login name :");
		label1.setBounds(10, 40, 80, 20);
		label2 = new JLabel("Password :");
		label2.setBounds(10, 70, 80, 20);
		
		site = new JTextField("12345");
		site.setBounds(100, 10, 100, 20);
		user = new JTextField("sureshg");
		user.setBounds(100, 40, 100, 20);
		password = new JPasswordField("raman123");
		password.setBounds(100, 70, 100, 20);
		
		ok = new JButton("Login");
		ok.setBounds(30, 130, 70, 20);
		cancel = new JButton("Cancel");
		cancel.setBounds(110, 130, 80, 20);
		
		this.status = new JLabel("<html><font color=#0000CC>Connecting...</font></html>");
		this.status.setBounds(20, 100, 80, 20);
		this.status.setVisible(false);
		
		container.add(label3);
		container.add(site);
		container.add(label1);
		container.add(user);
		container.add(label2);
		container.add(password);
		container.add(ok);
		container.add(cancel);
		container.add(this.status);

		site.addActionListener(this);
		user.addActionListener(this);
		password.addActionListener(this);
		ok.addActionListener(this);
		cancel.addActionListener(this);
		setSize(220, 190);
		setResizable(false);
		setLocationRelativeTo(jframe);
		setLocation(450, 220);
		setVisible(true);
		
		setDefaultCloseOperation(JDialog.DO_NOTHING_ON_CLOSE);
		addWindowListener(new WindowAdapter() {
            public void windowClosing(WindowEvent we) {
            	WindowsTrayIcon.cleanUp();
				System.exit(0);
            }
		});
	}

	public void actionPerformed(ActionEvent actionevent) {
		if (actionevent.getSource() == ok
				|| actionevent.getSource() == password
				|| actionevent.getSource() == user
				|| actionevent.getSource() == site) {
			
			_username = user.getText();
			_site = site.getText();
			_password = new String(password.getPassword());
			
			if (_username.length() == 0 || _password.length() == 0 || _site.length() == 0) {

				JOptionPane.showMessageDialog(this,
						"Please enter a valid username and password", "Error",
						2);
				return;
			} else {
				this.status.setVisible(true);
				
				try {
					String[] args = {_username, _password, _site};
					ProxyResponseObject response = ChatExplorer.proxy.Connect("Operators", "login", args);
					String[] res = response.getResponse();
					
					this.status.setVisible(false);
					
					if (res[0].equals("success")) {
						operator = new Operator(_site, _username, res[1],	res[2], "Online");
					} else {
						JOptionPane.showMessageDialog(this,
								"Please enter a valid username and password\n",
								"Error", 2);
						return;
					}
					
				} catch (Exception e) {
					this.status.setVisible(false);
					
					JOptionPane.showMessageDialog(this,
							"SERVER is not responding...Connection failed.\n",
							"Error", 2);
					System.out.println(e + " Login failed ");
					return;
				}
			}
		} else if (actionevent.getSource() == cancel) {
			WindowsTrayIcon.cleanUp();
			System.exit(0);
		}
		setVisible(false);
	}

	public String getUserName() {
		return _username;
	}

	public String getPassword() {
		return _password;
	}

	public String getSite() {
		return _site;
	}

	public Operator getOperator() {
		return (Operator) operator;
	}

}
