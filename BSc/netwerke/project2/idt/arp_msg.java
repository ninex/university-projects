import java.net.*;
public class arp_msg
{
	byte arp_buf[] = new byte[28];
	int port;
	InetAddress group;
	DatagramPacket arp_packet;
	
	public arp_msg(String group_addr, int port)
	{
		this.port = port;
		try
		{
			group = InetAddress.getByName(group_addr);
		}catch (Exception e){}
		for (int i=0; i< arp_buf.length; i++) arp_buf[i] = (byte)0;
	}
	public arp_msg(byte[] buf, int port)
	{		
		this.port = port;
		for (int i=0; i< arp_buf.length; i++) 
			arp_buf[i] = buf[i];
	}
	
	public void setHost(short[] newip)
	{
		//this.ip = ip;
		arp_buf[14] = (byte)newip[0];
		arp_buf[15] = (byte)newip[1];
		arp_buf[16] = (byte)newip[2];
		arp_buf[17] = (byte)newip[3];
	}
	public void setTarget(short[] newip)
	{
		arp_buf[18] = (byte)newip[0];
		arp_buf[19] = (byte)newip[1];
		arp_buf[20] = (byte)newip[2];
		arp_buf[21] = (byte)newip[3];
	}
	public void setTarget(byte ip1,byte ip2,byte ip3,byte ip4)
	{
		arp_buf[18] = ip1;
		arp_buf[19] = ip2;
		arp_buf[20] = ip3;
		arp_buf[21] = ip4;
	}
	public void setHost(byte ip1,byte ip2,byte ip3,byte ip4)
	{
		
		arp_buf[14] = ip1;
		arp_buf[15] = ip2;
		arp_buf[16] = ip3;
		arp_buf[17] = ip4;
	}
	public void swap(short[] newip)
	{
		arp_buf[18] = arp_buf[14];
		arp_buf[19] = arp_buf[15];
		arp_buf[20] = arp_buf[16];
		arp_buf[21] = arp_buf[17];
		
		arp_buf[14] = (byte)newip[0];
		arp_buf[15] = (byte)newip[1];
		arp_buf[16] = (byte)newip[2];
		arp_buf[17] = (byte)newip[3];
	}
	public void swap(int[] newip)
	{
		arp_buf[18] = arp_buf[14];
		arp_buf[19] = arp_buf[15];
		arp_buf[20] = arp_buf[16];
		arp_buf[21] = arp_buf[17];
		
		arp_buf[14] = (byte)newip[0];
		arp_buf[15] = (byte)newip[1];
		arp_buf[16] = (byte)newip[2];
		arp_buf[17] = (byte)newip[3];
	}	
	public DatagramPacket getArp()
	{
		arp_packet = new DatagramPacket(arp_buf,arp_buf.length,group, port);
		
		return arp_packet;
	}
	public DatagramPacket getReplyArp(InetAddress ip)
	{
		arp_packet = new DatagramPacket(arp_buf,arp_buf.length,ip, port);
		
		return arp_packet;
	}
	public DatagramPacket getReplyArp(String ip)
	{
		try
		{
			arp_packet = new DatagramPacket(arp_buf,arp_buf.length,InetAddress.getByName(ip), port);
		}catch (Exception e){}
		
		return arp_packet;
	}
}