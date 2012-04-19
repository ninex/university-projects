import java.net.*;
import java.io.*;

public class serverThread extends Thread
{
	Socket client = null;
	public IpPool pool;
	private int lease_time = 120;
	
	//recieves a socket passed from the server class
	public serverThread(Socket client,IpPool pool)
	{
		this.client = client;
		this.pool = pool;
	}
	
	// this function is called when you start to execute the thread
	public void run()
	{
		ObjectInputStream in = null;
		ObjectOutputStream out = null;
	
		// Two messages needed when communicating between client and server (thread)
		DHCP_MSG msg_in = null;
		DHCP_MSG msg_out = null;
		
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
			try {
				msg_in = (DHCP_MSG)(in.readObject());
			}catch (Exception e){}
			
			//if the message recieved exists
			if (msg_in != null)
			{
				String type_msg = msg_in.getType();
				if(type_msg == "DHCPDiscovery")
				{
					System.out.println("DHCP Server: Recieving DHCPDiscover message from A.");
					// create a new reply on the same id that the original was created on
					int rep_id = msg_in.xid;
					msg_out = new DHCP_MSG(rep_id);
					
					
					String IpOffer = pool.IpRequest();
					
					msg_out.createOptionsCookie();
					msg_out.createReply(IpOffer,lease_time);
					
					if (msg_out != null)
					{
						//try and pass the message to the outputstream
						try {
							out.writeObject(msg_out);
							System.out.println("DHCP Server: Offering " + IpOffer + " in DHCPOffer message to A");
						}catch (Exception e){}	
					
					}
					
					try {
						msg_in = (DHCP_MSG)(in.readObject());
					}catch (Exception e){}

					if (msg_in != null)
					{
						type_msg = msg_in.getType();
						if(type_msg == "DHCPRequest")
						{
							System.out.println("DHCP Server: Recieving DHCPRequest message from A.");
							IpOffer = msg_in.getReqIp();
							System.out.println("DHCP Server: Allocating "+IpOffer+" and starting lease");
							
							 
							msg_out = msg_in;
							msg_out.createAck();
							 
							try {
								out.writeObject(msg_out);
								System.out.println("DHCP Server: Sending DHCPAck message to A");
							}catch (Exception e){}	

							/* need to start timer on lease in all places now!!!
							 *
							 */
							pool.start(lease_time,IpOffer);
							
							//	while ( (msg_in = (DHCP_MSG)(in.readObject())) != null) {
							
							while(client.isConnected())
							{
								try {
									msg_in = (DHCP_MSG)(in.readObject());
								}
								catch (Exception e){
									e.printStackTrace();
								}
				
								if(msg_in != null)
								{
									type_msg = msg_in.getType();
									if(type_msg == "DHCPRequest")
									{
										System.out.println("DHCP Server: Recieving DHCPRequest message from A.");
										
										IpOffer = msg_in.getReqIp();
										pool.refresh(IpOffer);
										
										msg_out = msg_in;
										msg_out.createReply(IpOffer,lease_time);
										
										try {
											out.writeObject(msg_out);
											System.out.println("DHCP Server: Refreshing lease and Sending DHCPOffer message to A");
										}catch (Exception e){}	
	
									}
									else
									{
										System.out.println("***Expecting DHCPRequest but found " + type_msg);
										System.exit(0);
									}
								}
								else
								{
									System.out.println("Error : Unable to recieve DHCP message");
									System.exit(0);
								}
							}
							
							
							
						}
						else
						{
							System.out.println("Expecting DHCPRequest but found " + type_msg);
							System.exit(0);
						}
					}
					else
					{
						System.out.println("Error : Unable to recieve DHCP message");
						System.exit(0);
					}
					
					
					
					// close the outputstream and the socket
					//out.close();
					//client.close();
				}
				else
				{
					System.out.println("Expecting DHCPDiscovery but found " + type_msg);
					System.exit(0);
				}
			}
			else
			{
				System.out.println("Error : Unable to recieve DHCP message");
				System.exit(0);
			}
			
			// simply to display that the conection was closed
			if (client.isClosed())
			{
				System.out.println("Connection was closed");
			}
		}
	}
}
