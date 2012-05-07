import java.net.*;
public class ProxyServer extends Thread implements Runnable
{
	static final int proxyPort = 5060;
	static final int callPort = 5061;
	static final int listenPort = 5062;
	static final int max_msg = 1024;
	Registrar regs = null;
	
	private DatagramSocket incoming,outgoing,proxy;
	
	public ProxyServer(DatagramSocket incoming,DatagramSocket outgoing, DatagramSocket proxy)
	{
		this.incoming = incoming;
		this.outgoing = outgoing;
		this.proxy = proxy;
		regs = new Registrar();
		regs.start();
	}
	public void run()
	{
		System.out.println("Proxy server running...");
		System.out.println("Registrar running...");
		while (true)
		{
			waitForCalls();
		}
	}
	
	public void waitForCalls()
	{
		byte buf[] = new byte[max_msg];
		DatagramPacket msg = new DatagramPacket(buf,buf.length);
		
		
		//wait for invite from a client
		try
		{
			proxy.setSoTimeout(100);
			proxy.receive(msg);
			try
			{
				String header[] = SIP.stripRequestHeader(msg);
				
				if ((header[0].compareTo("INVITE")) == 0)
				{
					System.out.println("Received INVITE from "+(msg.getAddress()).getCanonicalHostName()+" using protocol "+header[2]);
					InetAddress clientA = msg.getAddress();
					int rec_port = msg.getPort();
					//send trying=100 to client
					DatagramPacket out_msg = SIP.createResponse("100","",msg.getAddress(),msg.getPort());
					try
					{
						proxy.send(out_msg);
						System.out.println("Sending TRYING = 100 to "+(msg.getAddress()).getCanonicalHostName()+" using protocol "+header[2]);
					}catch (Exception e){System.out.println(e.toString());}
					//do lookup of address of server
					list l = regs.lookup(getNameFromInvite(msg));
					if (l == null)
					{
						//if address difference after lookup
							//send moved=302 to client
							//wait for ACK
					}else
					{
						System.out.println("Successful lookup for "+l.rName());
						//send invite to server
						out_msg = SIP.createRequest("INVITE",l.rAddress(),l.rListen(),"");
						try
						{
							proxy.send(out_msg);
							System.out.println("Sending INVITE to "+l.rName()+"@"+l.rAddress()+" using protocol "+header[2]+" port:"+l.rListen());
							//wait for trying=100 from server
							try
							{
								proxy.receive(msg);
								header = SIP.stripResponse(msg);
				
								if ((header[1].compareTo("100")) == 0)
								{
									System.out.println("Received TRYING = "+header[1]+" from UAS using protocol "+header[0]);
									//wait for ringing=180 from server
									try
									{
										proxy.receive(msg);
										header = SIP.stripResponse(msg);
										if ((header[1].compareTo("180")) == 0)
										{
											System.out.println("Received RINGING = "+header[1]+" from UAS using protocol "+header[0]);
											//send ringing=180 to client
											out_msg = SIP.createResponse("180","",clientA,rec_port);
											System.out.println("Sending RINGING = 180 to "+clientA.getCanonicalHostName()+":"+rec_port);
											proxy.send(out_msg);
										
										//wait for possible 182=queued messages from server
										
											proxy.receive(msg);
											header = SIP.stripResponse(msg);
											//wait for OK from server
											if ((header[1].compareTo("200")) == 0)
											{
												System.out.println("Received OK = "+header[1]+" from UAS, protocol "+header[0]);
												String stuff = "FROM:"+l.rAddress()+",PORT:"+l.rListen();
												out_msg = SIP.createResponse("200",stuff,clientA,rec_port);
												//send OK to client
												System.out.println("Sending OK = 200 to "+clientA.getCanonicalHostName()+":"+rec_port);
												proxy.send(out_msg);
											}
										}
									}catch (Exception e2){}
									
									
									
									
								
								
								}
								
							}catch (Exception e){}
							
						}catch (Exception e){e.printStackTrace();}
						
					}
				}else
				{
					if ((header[0].compareTo("REGISTER")) == 0)
					{
						Register(msg);
					}else
					{
						System.out.println("Garbage data received. Possible Attack.");
					}
				}
			}catch (Exception e2){}
		}catch (Exception e){}
		
	}
	
	public void Register(DatagramPacket msg)
	{
		String body = SIP.stripRequestBody(msg);
		String theHoff[] = SIP.stripBody(body);
		
		//get Name
		String a[] = theHoff[0].split(":");
		String name = a[1];
		
		//get Address
		a = theHoff[1].split(": ");
		String Address = a[1];
		System.out.println(Address);
		//get listenPort
		a = theHoff[2].split(": ");
		Integer lPort = new Integer(a[1]);
		
		//get callPort
		int cPort = msg.getPort();
		
		//register in lookup service
		regs.insert(name,Address,lPort.intValue(),cPort);
	}
	
	public String getNameFromInvite(DatagramPacket msg)
	{
		String body = SIP.stripRequestBody(msg);
		String theHoff[] = SIP.stripBody(body);
		
		
		//get Name
		String a[] = theHoff[0].split(":");
		String name = a[1];
		//System.out.println("\t\tinvite from "+name);
		return name;
	}
	
	public static void main(String args[])
	{
		
		
		DatagramSocket outgo = null,incom = null, prox = null;
		try
		{
			//outgo = new DatagramSocket(proxyPort);
			//incom = new DatagramSocket(listenPort);
			prox = new DatagramSocket(proxyPort);
		}catch (Exception e){e.printStackTrace();}
		ProxyServer proxyserver = new ProxyServer(incom,outgo,prox);
		proxyserver.start();
		
	}
	
}
