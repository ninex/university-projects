import javax.swing.*;
import java.io.*;
import java.awt.*;
import java.awt.event.*;
import java.net.*;
import java.util.*;
import org.bouncycastle.jce.provider.*;
import javax.crypto.*;
import java.security.*;

 
public class GUI extends JFrame implements ActionListener
{	
	
	JTextField ip = new JTextField(12);
	JTextField len = new JTextField(2);
	JTextField tport = new JTextField(3);
	JTextField dest = new JTextField(3);
	JTextArea Debug = new JTextArea(10,8);
	JTextArea text = new JTextArea(20,25);
	JTextArea msgArea = new JTextArea();
	JTextField textfield = new JTextField("Enter text here",20);
	Container main_pane = null;
	Date time = null;

	static Socket server = null;
	ObjectOutputStream out = null;
	ObjectInputStream in = null;
	int count = 0;
	
	//String userName = null;
	String temp = null;
	KeyGenerator aes_generator = null;
	PublicKey ServerPublicKey = null;
	Key Aes_key = null;
	Cipher cipher = null;
	
	REP_MSG msg_in = null;
	REP_MSG msg_out = null;
	
	boolean secure_and_safe = false;
	String userName = null;
	//should maybe make some of these private and protected;
	KeyPairGenerator keyGen = null;
	KeyPair keyPair = null;
	PublicKey pubKey = null;
	PrivateKey privKey = null;
	
	SealedObject acceptMush = null;
	
	public GUI(Socket a_server) 
	{
		super("Secure Connection");
		Security.addProvider(new BouncyCastleProvider());
		server = a_server;
        setSize(600, 500);
		setVisible(true);
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		
		this.addWindowListener(new WindowListener()
		{
			public void windowOpened(WindowEvent a){}
			public void windowClosing(WindowEvent a)
			{
				System.out.println("exiting:");
			}
			public void windowActivated(WindowEvent a){}
			public void windowDeactivated(WindowEvent a){}
			public void windowIconified(WindowEvent a){}
			public void windowDeiconified(WindowEvent a){}
			public void windowClosed(WindowEvent a){}
		});

		JScrollPane scrollPane = new JScrollPane(text);
		JScrollPane msgPane = new JScrollPane(msgArea);
		JButton connect = new JButton("Connect");
		JButton send = new JButton("Send");
		JButton login = new JButton("login");
		JPanel pane1 = new JPanel();
		JPanel pane3 = new JPanel();
		pane3.setLayout(new BorderLayout());
		pane3.add(textfield,BorderLayout.CENTER);
		pane3.add(send,BorderLayout.SOUTH);
		pane1.setLayout(new BorderLayout());
		pane1.add(scrollPane,BorderLayout.NORTH);
		pane1.add(pane3,BorderLayout.SOUTH);
		JPanel pane2 = new JPanel();
		JPanel pane5 = new JPanel();
		pane5.setLayout(new BorderLayout());
		pane5.add(login,BorderLayout.NORTH);
		pane5.add(msgPane,BorderLayout.CENTER);
		pane2.setLayout(new BorderLayout());
		pane2.add(pane5,BorderLayout.CENTER);
		JPanel main_panel = new JPanel();
		main_panel.setLayout(new BorderLayout());
		main_panel.add(pane1,BorderLayout.WEST);
		main_panel.add(pane2,BorderLayout.CENTER);
		
		main_pane = getContentPane();
		getContentPane().setLayout( new BorderLayout());
		main_pane.add(main_panel,BorderLayout.CENTER);
		
		login.addActionListener(new ActionListener()
		{
			public void actionPerformed(ActionEvent a)
			{
				if (count == 0)
				{
					count = count + 1;
					//prompt for username as well as password
					userName = JOptionPane.showInputDialog(main_pane,"PLease Enter your username");
					temp = JOptionPane.showInputDialog(main_pane,"PLease Enter your password");
					
					generateMyRSAkeys();
					
					System.out.println("\nAuthentication in process:\n");
					msgArea.append("\nAuthentication in process:\n");
					//get the streams
					validate secure = null;
					try
					{
						out = new ObjectOutputStream(server.getOutputStream());   
						in = new ObjectInputStream(server.getInputStream());
						secure = new validate(userName,temp,out,pubKey);
						out.writeObject(secure);
						msg_in = (REP_MSG)(in.readObject());
					}catch (Exception e){e.printStackTrace();}
					
					
					if(msg_in != null)
					{
						if((msg_in.valid == true) &&(msg_in.timeCheck == true))
						{
							System.out.println("Authentication process complete\n");
							System.out.println("Authentication verified:\n\n");
							msgArea.append("Authentication process complete:\n");
							msgArea.append("Authentication verified:\n\n");
							System.out.println("reply recieved");
							msgArea.append("\nWelcome "+ userName +"\nYou have succesfully logged in:\n");
							time = new Date();
							msgArea.append("\nThe time is :" + time + "\n");
							
							if(msg_in.publicKey!= null)
							{
								System.out.println("Server has provided access to the public key\n");
								msgArea.append("\n\nServer has provided access to the public key");
								ServerPublicKey = msg_in.publicKey;
								
								// need to verify that the server is the correct one
								AcceptTheMush();
								
								generateKey();
								
								if(Aes_key != null)
								{
									System.out.println("Session Key Generated\n");
									msgArea.append("\nSession Key Generated");
									
									//attempting to send the "session" key through RSA encryption
									try
									{
										cipher = Cipher.getInstance("RSA");
										cipher.init(Cipher.ENCRYPT_MODE,ServerPublicKey);
										//i am sending the encrypted session key as a secret object
										SealedObject so = new SealedObject(Aes_key,cipher);
										out.writeObject(so);
										
									}catch (Exception e){e.printStackTrace();}
								}
								else
								{
									msgArea.append("\nError: Unable to generate session key");
								}
							}
							else
							{
								msgArea.append("\nError: Unable to access the public key");
							}
							
							try
							{
								msg_in = (REP_MSG)(in.readObject());
							}catch (Exception e){e.printStackTrace();}
							
							if(msg_in != null)
							{
								System.out.println("secure connection made\n");
								msgArea.append("\nSecure connection created.\nSafe communication is now possible");
								secure_and_safe = true;
							}
							else
							{
								msgArea.append("\nError: Unable to communicate: exit and try again.");
							}
						}
						else
						{
							System.out.println("reply recieved");
							if(msg_in.id == 0)
							{
								msgArea.append("Authentication failure:\n");
								msgArea.append("Unable to log in: Unknown user\n");
							}
							else if(msg_in.timeCheck == false)
							{
								msgArea.append("Authentication failure:\n");
								msgArea.append("Unable to log in: Timeout Error\n");
							}
							else
							{
								msgArea.append("Authentication failure:\n");
								msgArea.append("General failure: unable to log in:\n");
							}
						}
					}
					else
					{
						System.out.println("Unable to recieve reply");
					}
				}
				else
				{
					msgArea.append("\nlogin error: exit and retry");
				}
		;}});
		
		send.addActionListener(new ActionListener()
		{
			public void actionPerformed(ActionEvent a)
			{
				if(secure_and_safe == true)
				{
					String text_to_send = textfield.getText();
					msg_out = new REP_MSG(text_to_send);
					
					if (msg_out != null)
					{
						encrypt(msg_out);
						text.append("Client: " + text_to_send + " :\n");
						decrypt();
					}
					else
					{
						System.out.println("unable to create message to encrypt");
					}
				}
				else
				{
					msgArea.append("\nWarning: No secure connection available:\nTry login or exit and try again");
				}
		;}});
		
		}
		
		private void generateKey()
		{
			msgArea.append("\nGenerating AES key for secure communication");
			
			try 
			{
				//generate a single AES key
				aes_generator = KeyGenerator.getInstance("AES");
				Aes_key = aes_generator.generateKey();
			}
			catch (Exception e){e.printStackTrace();}
		}
		
		private void encrypt(REP_MSG sealedObject)
		{
			try
			{
				cipher = Cipher.getInstance("AES");
				cipher.init(Cipher.ENCRYPT_MODE,Aes_key);
				//i am sending the encrypted session key as a secret object
				SealedObject so = new SealedObject(sealedObject,cipher);
				out.writeObject(so);
				
			}catch (Exception e){e.printStackTrace();}
		}
		
		private void decrypt()
		{
			try 
			{
				SealedObject secretobject = (SealedObject)(in.readObject());
				
				if(secretobject != null)
				{
					System.out.println("encrypted message accepted:");
					Cipher cipher = Cipher.getInstance("AES");
					cipher.init(Cipher.DECRYPT_MODE,Aes_key);
					//now this is the actual decrypted msg !!!!!
					msg_out = (REP_MSG)secretobject.getObject(cipher);
					text.append(msg_out.resp);
				}
				else
				{
					System.out.println("Unable to accept encrypted message:\n");
				}
				
			
			}catch(Exception e){e.printStackTrace();}
		}
		
		private void generateMyRSAkeys()
		{
			try 
			{
				keyGen = KeyPairGenerator.getInstance("RSA");
				keyGen.initialize(2048);
				keyPair = keyGen.generateKeyPair();
				privKey = keyPair.getPrivate();
				pubKey = keyPair.getPublic();
			}
			catch (Exception e){e.printStackTrace();}
		}
		
		public void AcceptTheMush()
		{
			try
			{
			
				acceptMush = (SealedObject)(in.readObject());
				if(acceptMush != null)
				{
					System.out.println("Validating server connection:");
					msgArea.append("\n\nValidating server connection:");
					Cipher cipher = Cipher.getInstance("RSA");
					cipher.init(Cipher.DECRYPT_MODE,privKey);
					msg_out = (REP_MSG)acceptMush.getObject(cipher);
					
					if(temp.contentEquals(new StringBuffer(msg_out.resp)))
					{
						System.out.println("Authentic Server: Valid server connection found:");
						msgArea.append("\nAuthentic Server: Valid server connection found:\n");
					}
					else
					{
						msgArea.append("\nError: Unknown Server. Possible attack");
						msgArea.append("\nPlease shutdown and restart");
						while(true);
					}
				}
			}
			catch(Exception e){e.printStackTrace();}
		}
		
		public void actionPerformed(ActionEvent e)
		{
			System.out.println("Thanks for the crap abrie");
		}
}
