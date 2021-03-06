import javax.swing.*;
import java.io.*;
import java.awt.*;
import java.awt.event.*;

public class ClientGUI extends JFrame implements ActionListener
{	
	String myIp;
	JTextField text = new JTextField(20);
	
	JTextField text1 = new JTextField(3);
	JTextField text2 = new JTextField(3);
	JTextField text3 = new JTextField(3);
	JTextField text4 = new JTextField(3);

	JList list = new JList();
	
	JTextArea textArea = new JTextArea(20,35);
	JLabel last_msg = new JLabel();
	Container main_pane = null;
	private Timer timer;
	static clientThread thread;
	static ipLookupTable lookthread;
	static msg rec;
	
	public static ClientGUI main_frame = new ClientGUI();
	
	
	public ClientGUI() 
	{
		super("Chat");
		timer = new Timer(700,this);
		
        setSize(600, 500);
	    setVisible(true);
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		

		JScrollPane scrollPane = new JScrollPane(textArea);

		JButton send = new JButton("Send");
		
		JPanel pane1 = new JPanel();
		pane1.setLayout(new BorderLayout());
		pane1.add(scrollPane,BorderLayout.CENTER);
		pane1.add(text,BorderLayout.SOUTH);
		pane1.add(last_msg,BorderLayout.NORTH);
		
		JPanel pane3 = new JPanel();
		
		
		pane3.add(text1);
		pane3.add(text2);
		pane3.add(text3);
		pane3.add(text4);
		
		JPanel pane4 = new JPanel();
		pane4.setLayout(new BorderLayout());
		pane4.add(list,BorderLayout.NORTH);
		pane4.add(pane3,BorderLayout.SOUTH);
		
		JPanel pane2 = new JPanel();
		pane2.setLayout(new BorderLayout());
		pane2.add(send,BorderLayout.SOUTH);
		pane2.add(pane4,BorderLayout.CENTER);
		
		
		JPanel main_panel = new JPanel();
		main_panel.setLayout(new BorderLayout());
		main_panel.add(pane1,BorderLayout.WEST);
		main_panel.add(pane2,BorderLayout.EAST);
		
		main_pane = getContentPane();
		getContentPane().setLayout( new BorderLayout());
		main_pane.add(main_panel,BorderLayout.CENTER);
		
		send.addActionListener(new ActionListener()
		{
			public void actionPerformed(ActionEvent a)
			{
				thread.sendMsg(text1.getText() +"."+ text2.getText() +"."+text3.getText() +"."+text4.getText(),text.getText());
				text.setText("");
		;}});
		
		timer.start();
		
		}
	
	public void addMessage(String msg)
	{
		textArea.append(msg + "\n");
	}
	public void addMsg(String msg,String host)
	{
		textArea.append(msg + "\n");
		last_msg.setText("<html><font color=red>"+host+": </font>"+"<font color=blue>"+msg+"</font");
	}
	public void setIP()
	{
		myIp = text1.getText() +"."+ text2.getText() +"."+text3.getText() +"."+text4.getText();
	}
	public void actionPerformed(ActionEvent e)
	{
		lookthread.refresh(list);
	}
	public static void main(String[] args) 
	{
		lookthread = new ipLookupTable(main_frame);
		lookthread.start();
		
		thread = new clientThread(lookthread,main_frame,Integer.parseInt(args[1]),Integer.parseInt(args[0]));
		thread.start();
		lookthread.setClientThread(thread);
		
		rec = new msg(main_frame);
		rec.start();
		
		main_frame.show();
    }
}
