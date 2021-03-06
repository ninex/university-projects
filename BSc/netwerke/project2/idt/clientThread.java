import java.io.*;
import java.util.*;
import java.net.*;


public class clientThread extends Thread implements Runnable
{

	private static final int sender_start = 14;
	private static final int target_start = 18;
	private static int PORT = 3000;
	private int broadcastPort, unicastPort = 2500;
	int gatePort;
	static String group = "224.0.0.0";
	String myIp;
	int[] addr = new int[4];
	MulticastSocket s = null;
	DatagramSocket r = null;
	private boolean connected = true;
	ipLookupTable lookthread;
	ClientGUI gui;	
	ObjectOutputStream out = null;

		
	public clientThread(ipLookupTable lookthread,ClientGUI gui,int broadcastPort, int gatePort)
	{		
		this.lookthread = lookthread;
		this.gui = gui;
		this.broadcastPort = broadcastPort;
		this.gatePort = gatePort;
		for (int i=0;i<4;i++)
		{
			addr[i] = (new Long((Math.round(Math.random()*255)))).intValue();
		}
		gui.addMessage("My ip is: "+whatIsMyIp());
	}
	
	//the main function
	public void run()
	{
		
		myIp = intToIp(addr[0],addr[1],addr[2],addr[3]);
		try
		{
			r = new DatagramSocket(unicastPort);
			s = new MulticastSocket(broadcastPort);			
			s.joinGroup(InetAddress.getByName(group));
			
			
			s.setSoTimeout(300);
			r.setSoTimeout(300);
			
		}catch (Exception e){System.out.println("Error: "+e);System.exit(1);}
		
		connected = true;
		
		myIp = intToIp(addr[0],addr[1],addr[2],addr[3]);
		
		System.out.println("Waiting for packets on port:"+broadcastPort);
		System.out.println("Connected to gateway on port:"+gatePort);
		System.out.println("\nListening for broadcast...");
		while (connected)
		{
		listen();
		}

		try
		{
			s.leaveGroup(InetAddress.getByName(group));
			s.close();
		}catch (Exception e){System.out.println("Error: "+e);}
	}
	
	public boolean isConnected()
	{
		return connected;
	}
	
	public void disconnect()
	{
		connected = false;
	}
	
	private void listen()
	{
		byte arp_buf[] = new byte[28];
		DatagramPacket arp_packet = new DatagramPacket(arp_buf,arp_buf.length);
		
		
		try
		{
			s.receive(arp_packet);
			byte[] buf = arp_packet.getData();
			String target = byteToIp(arp_packet.getData(),target_start);
			String sender = byteToIp(arp_packet.getData(),sender_start);
			gui.addMessage("request to return HA target "+target+" sender "+sender);
			
			if  (myIp.compareTo(target) == 0)
			{
				if (myIp.compareTo(sender) == 0)
				{
					gui.addMessage("ARP from myself?");
				}else
				{
					gui.addMessage("HA request was for me from" + (arp_packet.getAddress()).getHostAddress());
					if (lookthread.lookFor(sender))
					{
						lookthread.refresh(sender);
						gui.addMessage("Refresh HA:"+(arp_packet.getAddress()).getHostAddress());
					}else
					{
						lookthread.insert(arp_packet.getAddress().getHostAddress(),sender,buf);
					}
					
					arp_msg msg = new arp_msg("224.0.0.0",unicastPort);
					msg.setHost((byte)addr[0],(byte)addr[1],(byte)addr[2],(byte)addr[3]);
					msg.setTarget(buf[14],buf[15],buf[16],buf[17]);
					
					gui.addMessage("Return Mac to " +sender);
					
					DatagramPacket packet = msg.getReplyArp(arp_packet.getAddress().getHostAddress());
					
					if (arp_packet.getPort() == broadcastPort)
					{
						packet.setPort(gatePort+1);
					}else
					{
						packet.setPort(unicastPort);
					}
					if (arp_packet.getPort() != (gatePort+1))
					{
						try
						{
							r.send(packet);
						}catch (SocketException se){gui.addMessage(se.toString());}
						
						gui.addMessage("reply sent on port "+packet.getPort());
					}
				}
			}else
			{
				if ( lookthread.lookFor(sender) == true)
				{
					gui.addMessage("I am going to refresh");
					lookthread.refresh(sender);
				}else
				{
					gui.addMessage("Discarding");
				}
			}
		}catch (Exception e){}
	}
	private String intToIp(int ip1, int ip2, int ip3, int ip4)
	{
		return ""+ip1 +"."+ ip2 +"."+ ip3 +"."+ ip4;
	}
	public String whatIsMyIp()
	{
		return intToIp(addr[0],addr[1],addr[2],addr[3]);
	}
	
	private String byteToIp(byte[] buf, int start)
	{
		byte b;
		Integer a;
		int c;
		String ip = new String();
		for (int i=0;i<3;i++)
		{		
			b = buf[start+i];
			c = b >= 0 ? b : 256 + b;
			a = new Integer(c);
			ip = ip + a.toString() + ".";
		}
		b = buf[start+3];
		c = b >= 0 ? b : 256 + b;
		a = new Integer(c);
		ip = ip + a.toString();
		
		return ip;
	}
	public void sendMsg(String targetip,String message)
	{
		String actual;
		if ( lookthread.lookFor(targetip) == true)
        {
			lookthread.refresh(targetip);
			gui.addMessage("I still have the Hardware Address");
			actual = lookthread.getActual(targetip);
        }else
		{
			if (lookthread.getArp(targetip) == null)
			{
				arp_msg msg = new arp_msg("224.0.0.0",broadcastPort);
				msg.setHost((byte)addr[0],(byte)addr[1],(byte)addr[2],(byte)addr[3]);
				msg.setTarget(getInty(targetip));
		
				gui.addMessage("sending packet to "+targetip);
			
				try
				{
					s.send(msg.getArp(),(byte)255);
				}catch (Exception e){}
				
			}
			try
			{
				byte arp_buf[] = new byte[28];
				DatagramPacket arp_packet = new DatagramPacket(arp_buf,arp_buf.length);
				
				gui.addMessage("Waiting for reply from"+targetip);
				r.receive(arp_packet);
				gui.addMessage((arp_packet.getAddress()).getHostAddress()+" replied");
				byte[] buf = arp_packet.getData();
				String target = byteToIp(arp_packet.getData(),target_start);
				String sender = byteToIp(arp_packet.getData(),sender_start);
				gui.addMessage("get HA target "+target+" sender "+sender);
				
				if  (myIp.compareTo(target) == 0)
				{
					if (myIp.compareTo(sender) == 0)
					{
						gui.addMessage("ARP from myself?");
					}else
					{
						gui.addMessage("return message was for me");
						if (lookthread.lookFor(sender))
						{
							lookthread.refresh(sender);
							gui.addMessage("Refreshing HA");
						}else
						{
							lookthread.insert((arp_packet.getAddress()).getHostAddress(),sender,buf);
						}
					}
				}else
				{
					if ( lookthread.lookFor(sender) == true)
					{
						gui.addMessage("Refresh HA of "+sender);
						lookthread.refresh(sender);
					}else
					{
						gui.addMessage("Discarding arp message");
					}                          
				}
				actual = (arp_packet.getAddress()).getHostAddress();
			}catch (Exception e)
			{
				arp_msg msg = new arp_msg("224.0.0.0",gatePort);
				msg.setHost((byte)addr[0],(byte)addr[1],(byte)addr[2],(byte)addr[3]);
				msg.setTarget(getInty(targetip));
				
				gui.addMessage("sending packet to "+targetip+" on the gateway with port:"+gatePort);
			
				try
				{
					s.send(msg.getArp(),(byte)255);
				}catch (Exception e2){}
				
				DatagramPacket arp_packet;
				byte arp_buf[] = new byte[28];
				try
				{
					arp_packet = new DatagramPacket(arp_buf,arp_buf.length);
					gui.addMessage("Waiting for reply");
					r.receive(arp_packet);					
				}catch (Exception e3){gui.addMessage("Unknown host");return;}
				
				gui.addMessage(targetip+" replied");
				byte[] buf = arp_packet.getData();
				String target = byteToIp(arp_packet.getData(),target_start);
				String sender = byteToIp(arp_packet.getData(),sender_start);
				gui.addMessage("get HA target "+target+" sender "+sender);
				
				if  (myIp.compareTo(target) == 0)
				{
					if (myIp.compareTo(sender) == 0)
					{
						gui.addMessage("ARP from myself?");
					}else
					{
						if (lookthread.lookFor(sender))
						{
							lookthread.refresh(sender);
							gui.addMessage("Refresh HA: "+sender);
						}else
						{
							lookthread.insert((arp_packet.getAddress()).getHostAddress(),sender,buf);
						}
					}
				}else
				{
					if ( lookthread.lookFor(sender) == true)
					{
						gui.addMessage("I am going to refresh");
						lookthread.refresh(sender);
					}else
					{
						gui.addMessage("Discarding");
					}                          
				}
				actual = (arp_packet.getAddress()).getHostAddress();
			}
		}   
		
		try
		{
			System.out.println(actual);
			Socket client = new Socket(actual,PORT);
			out = new ObjectOutputStream(client.getOutputStream());
			out.writeObject(myIp);
			out.writeObject(message);
			client.shutdownOutput();
			client.close();
		}catch (Exception e){}
	}
	private short[] getInty(String ip)
	{
		short[] shorty = new short[4];
		int index=0,str_index=0;
		while (str_index < ip.length())
		{
			if (ip.charAt(str_index) == '.')
			{
				index++;
			}else
			{				
				shorty[index] = (short)((shorty[index] * 10) + (Integer.parseInt(ip.substring(str_index,str_index+1))));
			}
			str_index++;
		}
		return shorty;
	}
}
