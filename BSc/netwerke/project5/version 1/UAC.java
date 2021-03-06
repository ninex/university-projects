import java.net.*;
import java.io.*;

public class UAC extends Thread implements Runnable
{
	static final int proxyPort = 5060;
	static int callPort = 5061;
	static int listenPort = 5062;
	static final int max_msg = 1024;
	private DatagramSocket outgoing,proxy;
	private gui VoIp;
	private String proxyA;
	
	public UAC(DatagramSocket outgoing,DatagramSocket proxy,int disp,gui VoIp)
	{
		callPort = callPort + disp;
		listenPort = listenPort + disp;
		this.outgoing = outgoing;
		this.proxy = proxy;
		this.VoIp = VoIp;
	}
	public void run()
	{
		
		
		
		while(true)
		{
			
		}
	}
	
	public void Register(String name)
	{
		BufferedReader buf = new BufferedReader(new InputStreamReader(System.in));
		System.out.print("Proxy Address:");
		try
		{
			proxyA = buf.readLine();
		
			String body = "Name: "+name+",IP: "+(InetAddress.getLocalHost()).getHostName()+",Port: "+listenPort;
			DatagramPacket msg =SIP.createRequest("REGISTER",proxyA,proxyPort,body);
			outgoing.send(msg);
		}catch (Exception e){e.printStackTrace();}
	}
	
	public void call(String who,int port)
	{
		//send invite
		DatagramPacket msg = SIP.createRequest("INVITE",proxyA,proxyPort,"Name: "+who);
		DatagramPacket in_msg = null;
		byte buf[] = new byte[max_msg];
		try
		{
			outgoing.send(msg);
			in_msg = new DatagramPacket(buf,buf.length);
			outgoing.receive(in_msg);
			String header[] = SIP.stripResponse(in_msg);
			//if 100, proceed
			if ((header[1].compareTo("100")) == 0)
			{
				VoIp.addMsg("[UAC]Received TRYING = "+header[1]+" from proxy, protocol "+header[0]);
				
				outgoing.receive(in_msg);
				header = SIP.stripResponse(in_msg);
				//if 180, proceed
				if ((header[1].compareTo("180")) == 0)
				{
					//ring a bit
					VoIp.addMsg("[UAC]Received Ringing = "+header[1]+" from proxy, protocol"+header[0]);
					/*for (int i=0;i<100;i++)
					{
						VoIp.setLocation(i%5,i%5);
					}*/
					//while 182, wait for OK
					
					//if 200
					outgoing.receive(in_msg);
					header = SIP.stripResponse(in_msg);
					if ((header[1].compareTo("200")) == 0)
					{
						VoIp.addMsg("[UAC]Received OK = "+header[1]+" from proxy, protocol"+header[0]);
						//send ACK
						String body = SIP.stripResponseHeader(in_msg);
						String a[] = body.split(",");
						String theHoff[] = a[0].split("FROM:");
						String theHoff2[] = a[1].split("PORT:");
						VoIp.addMsg("[UAC]Sending ACK to "+theHoff[1]+" port:"+theHoff2[1]);
						msg = SIP.createRequest("ACK",theHoff[1],(new Integer(theHoff2[1])).intValue(),"");
						outgoing.send(msg);
					}
					
				}
			}
		}catch (Exception e){}
		
			//if 180, proceed
				//wait for message
				
				//wait for messages
				
				
		//else 302
			//send ACK
			//do call to new address
	}
	
	public void disconnect()
	{
		//send BYE
		//wait for message
		//if 200, disconected
	}
}