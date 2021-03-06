import java.net.*;
import java.util.*;


public class UAS extends Thread implements Runnable
{
	static final int max_msg = 1024;
	private DatagramSocket incoming,proxy;
	private gui VoIp;
	private boolean running,busy;
	private LinkedList queue = new LinkedList();
	private ServerSocket wait = null;
	private int remotePort = 0;
	private microphone phone = null;
	private speaker speak = null;
	private String remoteAddr = "Leeg";
	
	public UAS(DatagramSocket incoming,DatagramSocket proxy, gui VoIp)
	{
		this.incoming = incoming;
		this.proxy = proxy;
		this.VoIp = VoIp;
		running = true;
		busy = false;
		queue.clear();
		boolean getport = true;
		while (getport)
		{
			try{
				wait = new ServerSocket((new Long(Math.round((Math.random())*10000)+1024)).intValue());
				getport = false;
			}catch(Exception e){}
		}
		System.out.println("[UAS]Voice port = "+wait.getLocalPort());
	}
	public void run()
	{
		
		while(running)
		{
			waitForCall();
			Queue();
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
				System.out.println("\n[UAS]Package contents\n============");
				SIP.printHeader(msg);
				/*for (int i=0;i<header.length;i++)
				{
					System.out.println(header[i]);
				}*/
				if ((header[0].compareTo("INVITE")) == 0)
				{
					String from = SIP.getNameFromInvite(msg);
					
					VoIp.addMsg("[UAS]Received invite from "+from+" using protocol "+header[2]);
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
							if (busy)
							{
								try
								{
									DatagramPacket newmsg = new DatagramPacket(msg.getData(),(msg.getData()).length,msg.getAddress(),msg.getPort());
									//	newmsg.
									VoIp.addMsg("[UAS] Call from "+header[1]+" queued");
									queue.addLast(newmsg);
								}catch (Exception e){e.printStackTrace();}
								
							}else
							{
							//if queue empty send ok=200
								out_msg = SIP.createResponse("200","port: "+wait.getLocalPort(),msg.getAddress(),msg.getPort());
								VoIp.addMsg("[UAS] Sending OK = 200 to "+(msg.getAddress()).getCanonicalHostName());
								//System.out.println("\tExpect "+"port: "+wait.getLocalPort());
								incoming.send(out_msg);
								//wait for ACK
								incoming.receive(msg);
								header = SIP.stripRequestHeader(msg);
								if ((header[0].compareTo("ACK"))==0)
								{
									VoIp.addMsg("[UAS] ACK received from "+(msg.getAddress()).getCanonicalHostName());
									System.out.println("[UAS]Remote port = "+SIP.getPortRequest(msg));
									remotePort = SIP.getPortRequest(msg);
									remoteAddr = (msg.getAddress()).getCanonicalHostName();
								}else
								{
									VoIp.addMsg("[UAS] wrong header was "+header[0]);
								}
								voiceConnect();
								busy = true;
							}
				}else
				{
					if ((header[0].compareTo("BYE")) == 0)
					{
						VoIp.addMsg("[UAS]BYE received. Initiating disconnect");
						DatagramPacket out_msg = SIP.createResponse("200","",msg.getAddress(),msg.getPort());
						incoming.send(out_msg);
						VoIp.addMsg("[UAS]Sent OK = 200");
						if (phone != null)
						{
							phone.stop();
							phone = null;
						}
						VoIp.addMsg("[UAS]Disconnected");
						busy = false;
					}else
					{
						VoIp.addMsg("[UAS]Garbage data received");
					}
				}
			}catch (Exception e2){}
		}catch (Exception e){}
		
		
	}
	
	private void Queue()
	{
		if (busy)
		{
			//VoIp.addMsg("[UAS]Still connected");
			if (queue.size() > 0)
			{
				DatagramPacket out_msg;
				Object[] objs = queue.toArray();
				for (int i=0;i<objs.length;i++)
				{
					DatagramPacket msg = (DatagramPacket)objs[i];
					out_msg = SIP.createResponse("182","",msg.getAddress(),msg.getPort());
					try
					{
						incoming.send(out_msg);
					}catch (Exception e){e.printStackTrace();}
				}
			}
		}else
		{
			
			if (queue.size() > 0)
			{
				VoIp.addMsg("Servicing first queued connection");
				try
				{
					DatagramPacket out_msg,msg = (DatagramPacket)queue.removeFirst();
					VoIp.addMsg("Sending OK = 200 to "+msg.getAddress()+":"+msg.getPort());
					out_msg = SIP.createResponse("200","port: "+wait.getLocalPort(),msg.getAddress(),msg.getPort());
					//System.out.println("\tExpect "+"port: "+wait.getLocalPort());
					incoming.send(out_msg);
					//wait for ACK
					incoming.receive(msg);
					String header[] = SIP.stripRequestHeader(msg);
					if ((header[0].compareTo("ACK"))==0)
					{
						VoIp.addMsg("[UAS] ACK received from "+(msg.getAddress()).getCanonicalHostName());
						remoteAddr = (msg.getAddress()).getCanonicalHostName();
						System.out.println("[UAS] Remote port = "+SIP.getPortRequest(msg));
						remotePort = SIP.getPortRequest(msg);
						
						voiceConnect();
					}
					
					busy = true;
				}catch (Exception e){e.printStackTrace();}
			}
		}
	}
	
	public void disconnect()
	{
		//send BYE
		//wait for message
		//if 200, disconected
	}
	public void stopThread()
	{
		running = false;
	}
	
	public void voiceConnect()
	{
		if (VoIp.audioEnabled())
		{
			System.out.println("[UAS]Connecting voice to "+remoteAddr);
			try
			{
				Socket talk = wait.accept();
				//Socket talk2 = new Socket(remoteAddr,remotePort);
				phone = new microphone(talk);
				phone.start();/*
				while (phone.mustWait()){}
				Socket talk2 = new Socket(remoteAddr,remotePort);
				speak = new speaker(talk2);
				speak.start();*/
				VoIp.addMsg("[UAS] You have microphone capabilities");
			}catch (Exception e){e.printStackTrace();}
		}
	}
}