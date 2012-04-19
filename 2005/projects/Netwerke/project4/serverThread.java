import java.net.*;
import java.io.*;
import java.security.*;
import java.util.*;
import org.bouncycastle.jce.provider.*;
import javax.crypto.*;

//export CLASSPATH=$CLASSPATH:/misc/users/cs/cs3/13557343/netwerke/project4/bcprov-jdk14-130.jar

public class serverThread extends Thread implements Runnable
{
	Socket client = null;
	validate secure = null;
	validate auth = null;
	String password_lookup = null;
	public byte [] auth_digest1 = null;
	public byte [] auth_digest2 = null;
	
	// Two messages needed when communicating between client and server (thread)
	REQ_MSG msg_in = null;
	REP_MSG msg_out = null;
	
	ObjectInputStream in = null;
	ObjectOutputStream out = null;
	
	long now = 0;
	long delay = 15000; //15 second delay
	
	boolean time_check = false;
	boolean is_secure = false;
	
	KeyPairGenerator key_gen = null;
	KeyPair keyPair = null;
	PrivateKey privateKey = null;
	PublicKey publicKey = null;
	
	PublicKey clientPublicKey = null;
	
	Key AES_key = null;
	
	SealedObject secretobject = null;
	Cipher cipher = null;
	
	//recieves a socket passed from the server class
	public serverThread(Socket client)
	{
		this.client = client;
	}
	
	// this function is called when you start to execute the thread
	public void run()
	{
		Security.addProvider(new BouncyCastleProvider());
		
		// try and create input/ouput streams on which to pass and recieve
		try
		{
			in = new ObjectInputStream(client.getInputStream());
			out = new ObjectOutputStream(client.getOutputStream());
		}catch (Exception e){}
		
		// If a client has been connected to the socket the proceed
		if(client.isConnected())
		{
			//attempt to recive the message from the input stream
			try 
			{
				secure = (validate)(in.readObject());
			}
			catch (Exception e)
			{
				e.printStackTrace();
			}
			
            //if the message recieved exists
			if(secure != null)
			{	
				System.out.println("Object accepted: authenticating");

				//have got the clients publickey
				clientPublicKey = secure.publicKey;
				generateRSAkeys();
				
				//find the users password in the database
				password_lookup = pass_lookup(secure.username);
				if(password_lookup == null)
				{
						System.out.println("failure: unknown user");
						msg_out = new REP_MSG(0,null,false,false);
						if (msg_out != null)
						{
							//try and pass the message to the outputstream
							try {
								out.writeObject(msg_out);
								System.out.println("Reply sent");
							}catch (Exception e){e.printStackTrace();}
						}
				}
				else
				{
					// do i get the same digest value? is everything correct?
					is_secure = authorize(secure);
					//remeber i need to do a time check now.
					time_check = timeCheck(secure.t1,secure.t2);
					
					if((is_secure == true) && (time_check == true))
					{
						System.out.println("Authentic");
						
						//reply informing the client you have succesfully logged on
						msg_out = new REP_MSG(secure.q1,publicKey,true,time_check);
						//if it has been created
						if (msg_out != null)
						{
							//try and pass the message to the outputstream
							try{
								out.writeObject(msg_out);
								System.out.println("Reply sent");
							}
							catch (Exception e)
							{
								e.printStackTrace();
							}
						}
						
						// i want the client to know that he is communicating with the correct server
						signTheMush();
					}
					else
					{
						System.out.println("failure");
						msg_out = new REP_MSG(secure.q1,null,false,time_check);
						if (msg_out != null)
						{
							//try and pass the message to the outputstream
							try {
								out.writeObject(msg_out);
								System.out.println("Reply sent");
							}catch (Exception e){e.printStackTrace();}
						}
					}
					
					try 
					{
						secretobject = (SealedObject)(in.readObject());
						if(secretobject != null)
						{
							System.out.println("encrypted message accepted:");
							Cipher cipher = Cipher.getInstance("RSA");
							cipher.init(Cipher.DECRYPT_MODE,privateKey);
							
							//now this is the session key!!!!!. can now communicate clearly.
							AES_key = (Key)secretobject.getObject(cipher);
						}
						
						if(AES_key != null)
						{
							msg_out = new REP_MSG(0,null,true,false);
							if (msg_out != null)
							{
								//try and pass the message to the outputstream
								try {
									out.writeObject(msg_out);
									System.out.println("Decrypted Session key");
								}catch (Exception e){e.printStackTrace();}
							}
						}
						else
						{
							msg_out = new REP_MSG(0,null,false,false);
							if (msg_out != null)
							{
								//try and pass the message to the outputstream
								try {
									out.writeObject(msg_out);
									System.out.println("Cannot Decrypt session key");
								}catch (Exception e){e.printStackTrace();}
							}
						}
						
						while(client.isConnected())
						{
								decrypt();
						}
					}catch (Exception e){e.printStackTrace();}
				}
			}
			else
			{
				System.out.println("Unable to accept message");
			}
			// simply to display that the conection was closed
			if (client.isClosed())
			{
				System.out.println("Connection was closed");
			}
		}
	}
	public String pass_lookup(String user)
	{
		String line; 
		String user1;
		            
		try
		{    
			BufferedReader in = new BufferedReader(new InputStreamReader(new FileInputStream("password.txt"))); 
			while ((line = in.readLine()) != null)
			{ 
				StringTokenizer st = new StringTokenizer(line, ":"); 
				user1 = st.nextToken(); 
				if (user.equals(user1))
				{ 
					return st.nextToken(); 
				} 
			}
			
		} catch (Exception e){e.printStackTrace();}
		return null;
	}
	
	public boolean timeCheck(long time1,long time2)
	{
		now = (new Date()).getTime();
		
		if((time1 + delay > now ) && (time2 + delay > now) && (time1 < now) && time2 < now )
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	
	public boolean authorize(validate secure)
	{
		try 
		{
			auth_digest1 = Protection.makeDigest(secure.username,password_lookup,secure.t1,secure.q1);
			auth_digest2 = Protection.makeDigest(auth_digest1,secure.t2,secure.q2);
		}catch (Exception e){e.printStackTrace();}
		
		if (MessageDigest.isEqual(secure.protected2,auth_digest2) == true)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	
	private void decrypt()
	{
		try 
		{
			secretobject = (SealedObject)(in.readObject());
			
			if(secretobject != null)
			{
				System.out.println("encrypted message accepted:");
				Cipher cipher = Cipher.getInstance("AES");
				cipher.init(Cipher.DECRYPT_MODE,AES_key);
				//now this is the actual decrypted msg !!!!! eat pee repeat!!
				msg_out = (REP_MSG)secretobject.getObject(cipher);
				System.out.println(msg_out.resp);
				encrypt();
			}
		}catch(Exception e)
		{
			System.out.println("System Exit: Closing");
			System.exit(1);
		}
	}
	
	private void encrypt()
	{
		String response = "Server: recieved message: encrypted reply:\n";
		msg_out = new REP_MSG(response);
		
		if (msg_out != null)
		{
			try
			{
				cipher = Cipher.getInstance("AES");
				cipher.init(Cipher.ENCRYPT_MODE,AES_key);
				//i am sending the encrypted session key as a secret object
				SealedObject so = new SealedObject(msg_out,cipher);
				out.writeObject(so);
				
			}catch (Exception e){e.printStackTrace();}
		}
		else
		{
			System.out.println("unable to create message to encrypt");
		}
	}
	
	private void generateRSAkeys()
	{
	//befor i reply i need to create a private key & public key.
		try 
		{
			key_gen = KeyPairGenerator.getInstance("RSA");
			key_gen.initialize(2048);
			keyPair = key_gen.generateKeyPair();
			privateKey = keyPair.getPrivate();
			publicKey = keyPair.getPublic();
		}
		catch (Exception e){e.printStackTrace();}
	}
	
	public void signTheMush()
	{
		/* am signing this packet by encrypting the password using RSA.
		 * when the client recievces this, he/she can decrypt the packet
		 * with its private key. If he gets the right password then the
		 * client knows that he must be communicating with the correct server
		 */
		try
		{	
			msg_out = new REP_MSG(password_lookup);
			System.out.println("Encrypting the Server side password through RSA");
			cipher = Cipher.getInstance("RSA");
			cipher.init(Cipher.ENCRYPT_MODE,clientPublicKey);
			
			//i am sending the encrypted session key as a secret object
			SealedObject so = new SealedObject(msg_out,cipher);
			out.writeObject(so);
			
		}catch (Exception e){e.printStackTrace();}
	}
}
