/**
 * 
 */
package NRML.ui;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.GridLayout;
import java.awt.event.KeyEvent;

import javax.swing.BorderFactory;
import javax.swing.JButton;
import javax.swing.JEditorPane;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;

import NRML.motive.*;

/**
 * @author nramanarayana
 *
 */
public class PanelMessage extends JPanel {
	/**
	 * Sub JPanels
	 */
	private JPanel chatpanel;
	private JPanel visitor_info_panel;
	private JPanel invitepanel;
	private JPanel info_panel;
	private JPanel textareapanel;
	
	/**
	 * Sub JEditorPane
	 */
	private JEditorPane edit_chat;
	private JEditorPane edit_userinfo;
	
	/**
	 * Sub JButton
	 */
	private JButton sendURL;
	private JButton Invite;
	private JButton send;
	
	/**
	 * Sub JTextArea
	 */
	private JTextArea textarea;
	
	/**
	 * @param msg
	 */
	public void updateChatMessage(String msg) {
		edit_chat.setText(msg);
	}
	
	/**
	 * Get Chat Text
	 */
	public String getChatText() {
		return edit_chat.getText();
	}
	
	/**
	 * @param msg
	 */
	public void setVisitorInfo(String msg) {
		edit_userinfo.setText(msg);
	}
	
	/**
	 * Get Visitor Information
	 */
	public String getVisitorInfoText() {
		return edit_userinfo.getText();
	}
	
	/**
	 * 
	 */
	public PanelMessage() {
		chatpanel = new JPanel();
		visitor_info_panel = new JPanel();
		invitepanel = new JPanel();
		info_panel = new JPanel();
		textareapanel = new JPanel();

		edit_chat = new JEditorPane();
		edit_userinfo = new JEditorPane();
		edit_chat.setContentType("text/html");
		edit_userinfo.setContentType("text/html");
		edit_chat.setEditable(false);
		edit_userinfo.setEditable(false);
		
		JScrollPane chatScrollPane = new JScrollPane(edit_chat);
		JScrollPane editorScrollPane = new JScrollPane(edit_userinfo);

		chatScrollPane.setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_ALWAYS);
		chatScrollPane.setPreferredSize(new Dimension(250, 145));
		chatScrollPane.setMinimumSize(new Dimension(10, 10));
		
		editorScrollPane.setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_ALWAYS);
		editorScrollPane.setPreferredSize(new Dimension(250, 145));
		editorScrollPane.setMinimumSize(new Dimension(10, 10));
		
		sendURL = new JButton("Proactive URL");
		sendURL.setActionCommand("sendURL");
		sendURL.addActionListener(new ProActive());
		
		Invite = new JButton("Invite to Chat");
		Invite.setActionCommand("inviteToChat");
		Invite.addActionListener(new ProActive());
		
		
		invitepanel.setLayout(new GridLayout(0, 3));
		invitepanel.add(Invite);
		invitepanel.add(sendURL);
		invitepanel.add(new JLabel(
				"<html><font color=#000000>History</font></html>"));
		invitepanel.setBorder(BorderFactory.createEmptyBorder(1, 15, 1, 1));

		textareapanel.setLayout(new BorderLayout(5, 1));
		textareapanel.setBorder(BorderFactory.createEmptyBorder(0, 10, 0, 10));
		JPanel pantop = new JPanel();
		textareapanel.add(pantop, BorderLayout.PAGE_START);
		textarea = new JTextArea(3, 20);
		textarea.setEditable(true);
		textareapanel.add(new JScrollPane(textarea), BorderLayout.CENTER);
		send = new JButton("Send");
		send.setMnemonic(KeyEvent.VK_ENTER);
		send.addActionListener(new sendNewMessage(textarea));
		
		textareapanel.add(send, BorderLayout.LINE_END);
			
		JPanel panbot = new JPanel();
		textareapanel.add(panbot, BorderLayout.PAGE_END);
		pantop.setBackground(new Color(214, 211, 206));
		panbot.setBackground(new Color(214, 211, 206));
		textareapanel.setBackground(new Color(214, 211, 206));

		chatpanel.setLayout(new BorderLayout());
		chatpanel.add(invitepanel, BorderLayout.PAGE_START);
		chatpanel.add(chatScrollPane, BorderLayout.CENTER);
		chatpanel.add(textareapanel, BorderLayout.PAGE_END);
		chatpanel.setBackground(new Color(214, 211, 206));

		visitor_info_panel.setLayout(new BorderLayout());
		visitor_info_panel.add(info_panel, BorderLayout.PAGE_START);
		visitor_info_panel.add(editorScrollPane, BorderLayout.CENTER);

		setLayout(new GridLayout(0, 2, 6, 0));
		add(chatpanel);
		add(visitor_info_panel);

		invitepanel.setBackground(new Color(214, 211, 206));
		info_panel.setBackground(new Color(214, 211, 206));
		setBackground(new Color(214, 211, 206));

	}

}
