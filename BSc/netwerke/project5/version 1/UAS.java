import java.net.*;


public class UAS extends Thread implements Runnable
{
	static final int max_msg = 1024;
	private DatagramSocket incoming,proxy;
	private gui VoIp;
	
	public UAS(DatagramSocket incoming,DatagramSocket proxy, gui VoIp)
	{
		this.incoming = incoming;
		this.proxy = proxy;
		this.VoIp = VoIp;
	}
	public void run()
	{
		
		while(true)
		{
			waitForCall();
		}
	}
	
	public void waitForCall()
	{
		//wait for invite
		byte buf[] = new byte[max_msg];
		DatagramPacket msg = new DatagramPacket(buf,buf.length);
		
		try
		{
			incoming.setSoTimeout(100);
			incoming.receive(msg);
			try
			{
				String header[] = SIP.stripRequestHeader(msg);
				/*for (int i=0;i<header.length;i++)
				{
					System.out.println(header[i]);
				}*/
				if ((header[0].compareTo("INVITE")) == 0)
				{
					VoIp.addMsg("[UAS]Received invite from "+header[1]+" using protocol "+header[2]);
					//send trying=100, proceed
					DatagramPacket out_msg = SIP.createResponse("100","",msg.getAddress(),msg.getPort());
					try
					{
						incoming.send(out_msg);
						VoIp.addMsg("[UAS]Sending TRYING = 100 to "+header[1]+" using protocol "+header[2]);
					
						//send ringing=180, proceed
						out_msg = SIP.createResponse("180","",msg.getAddress(),msg.getPort());
						
						incoming.send(out_msg);
						VoIp.addMsg("[UAS]Sending RINGING = 180 to "+header[1]+" using protocol "+header[2]);
					}catch (Exception e){System.out.println("Invite"+e.toString());}
							//while queued send 182, wait for queue change
								
							//if queue empty send ok=200
							out_msg = SIP.createResponse("200","",msg.getAddress(),msg.getPort());
							incoming.send(out_msg);
								//wait for ACK
								incoming.receive(msg);
								header = SIP.stripRequestHeader(msg);
								if ((header[0].compareTo("ACK"))==0)
								{
									VoIp.addMsg("[UAS] ACK received from "+(msg.getAddress()).getCanonicalHostName());
								}
				}
			}catch (Exception e2){}
		}catch (Exception e){}
		
		
	}
	
	public void disconnect()
	{
		//send BYE
		//wait for message
		//if 200, disconected
	}
}