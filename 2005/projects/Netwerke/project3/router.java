
import project3.*;
import java.net.*;

/**
 *The main calling program of the OSPF program
 */

public class router
{
	
	static ServerSocket connection = null;
	static MulticastSocket broad = null;
	//static DatagramSocket listen = null;
	static ipLookupTable lookthread = null;
	public static void main(String[] args) 
	{	
		int port = Integer.parseInt(args[0]);
		//Socket client = null;
		try
		{
			connection = new ServerSocket(port);
			broad = new MulticastSocket(1234);
			broad.joinGroup(InetAddress.getByName("224.0.0.0"));
			broad.setSoTimeout(100);
			connection.setSoTimeout(1);
			
		}catch (Exception e){System.out.println("error: "+e.toString());System.exit(1);}
		lookthread = new ipLookupTable(port-5000);
		ClientGUI main_frame = new ClientGUI(lookthread,connection,broad,port);
		main_frame.show();
		Socket client = null;
		System.out.println("Listening for connections...");
		byte buf[] = new byte[12];
		DatagramPacket p = new DatagramPacket(buf,buf.length);
		while (!connection.isClosed())
		{	
			try
			{
				client = connection.accept();
				new clientThread(client,lookthread,port).start();
			}catch (Exception e){}
			try
			{
				broad.receive(p);
				System.out.println("Received packet");
				byte msg[] = p.getData();
				String w = byteToPort(msg,0);
				String a = byteToPort(msg,4);
				String b = byteToPort(msg,8);
				int iw = (int)msg[0];
				System.out.println(a+" connected on port "+b+" weight "+iw);
				try
				{
					int ia = Integer.parseInt(a);
					int ib = Integer.parseInt(b);
					if (iw > 0)
					{
						System.out.println("Want to insert");
						lookthread.insert(ia-5000,ib-5000,iw);
						//main_frame.setNeighbours();
					}else
					{
						System.out.println("Want to remove");
						lookthread.remove(ia-5000,ib-5000);
						//main_frame.unsetNeighbours();
					}
				}catch (Exception e){e.printStackTrace();}
				main_frame.reCalculate();
				
				
				
				
				//**********************************************************************
				//run dijkstra
				
				p = new DatagramPacket(buf,buf.length);
			}catch (Exception e){/*System.out.println(e.toString());*/}
		}
		//main_frame.show();
	}
	private static String byteToIp(byte[] buf, int start)
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
	private static String byteToPort(byte[] buf, int start)
	{
		byte b[] = new byte[4];
		
		
		for (int i=0;i<4;i++)
		{		
			b[i] = buf[start+i];
		}
		String ip = new String(b);
		return ip;
	}
}
