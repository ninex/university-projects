import java.io.*;
import java.util.*;
public class DHCP_MSG implements Serializable
{

	public static final byte DHCP_discover = 1;
	public static final byte DHCP_offer = 2;
	public static final byte DHCP_request = 3;
	public static final byte DHCP_ack = 4;
	public static final byte DHCP_decline = 5;
	
	public static final byte IpLeaseTime = 51;
	public static final byte DHCP_msg_type = 53;
	
	public static final int OptionsMin = 312;
	public static final int OPTION_END = 255;
	
    // variables needed in creating the request message
	public byte op;
	public byte htype;
	public byte hlen;
	public byte hops;
	public int xid;
	public short secs;
	public short flags;
	public byte[] ciaddr;
	public byte[] yiaddr;
	public byte[] siaddr;
	public byte[] giaddr;
	public byte[] chaddr;
	public byte[] sname;
	public byte[] file;
	public byte[] options;
	
        /* class constructor used to create the message. To create a message in any 
         * class which imports this message class, one only needs to call the constructor
         * with the correct parameters. At this point in the project, only the Object resp
         * seems to be of importance.
         */
	public DHCP_MSG(int xid)
	{
		op = (byte)0;
		htype = (byte)0;
		hlen = (byte)0;
		hops = (byte)0;
		secs = (short)0;
		flags = (short)0;
		this.xid = xid;
		ciaddr = new byte[4];
		yiaddr = new byte[4];
		siaddr = new byte[4];
		giaddr = new byte[4];
		chaddr = new byte[16];
		sname = new byte[64];
		file = new byte[128];
		options = new byte[OptionsMin];
	}
	
	/*public saveState(DHCP_MSG msg)
	{
		DHCP_MSG ren_msg = new DHCP_MSG(1)
		ren_msg = msg;
	}*/
	
    // a procedure to display the message
	public void print_msg()
	{
		System.out.println("X id: "+xid);
	}
	
	public String getReqIp()
	{
		String ret_ip = byteToIp(yiaddr);
		if(ret_ip != null )
		{
			return ret_ip;
		}
		else
		{
			System.out.print("Error : no ip to get");
			return null;
		}
		
	}
	
	void print_magic_cookie()
	{
		System.out.print("Magic Cookie: ");
		for(int i = 0; i < 4;i++)
		{
			Integer b = new Integer(options[i]>=0?options[i]:256 + options[i]);
			
			if(i == 3)
			{
				System.out.print(b.toString());
			}
			else
			{
				System.out.print(b.toString() + "-");
			}
		}
		System.out.println();
	}
	
	public void createOptionsCookie()
	{
		options[0] = byteValue(99);
		options[1] = byteValue(130);
		options[2] = byteValue(83);
		options[3] = byteValue(99);
		
		options[4] = byteValue(OPTION_END);
	}
	
	public void createDiscovery()
	{
		options[4] = DHCP_msg_type;
		options[5] = (byte)1; // specifying the length
		options[6] = DHCP_discover;
		options[7] = byteValue(OPTION_END);
	}
	
	public void createReply(String IpOffer, int lease_time)
	{
		yiaddr = getBytey(IpOffer);
		
		options[4] = DHCP_msg_type;
		options[5] = (byte)1;
		options[6] = DHCP_offer;
		options[7] = IpLeaseTime;
		options[8] = (byte)1;
		options[9] = (byte)lease_time;
		options[10] = byteValue(OPTION_END);
	}
	
	public void createRequest()
	{
		options[6] = DHCP_request;
		/*Big hack - should have done all this dynamically but should work*/
	}
	
	public void createAck()
	{
		options[6] = DHCP_ack;
	}
	
	public int getLeaseTime()
	{
		int index = 4;
		int timeToLease = 0;
		int length = 0;
		
		while((options[index] != IpLeaseTime) && (options[index] != (byte)OPTION_END) && (options[index] != 0))
		{
			index = index + 1;
		}
		
		if(options[index] == IpLeaseTime)
		{
			index = index + 1;
			length = (int)options[index];
			
			if(length == 1)
			{
				index = index + 1;
				timeToLease = (int)options[index];
				return timeToLease;
			}
			else
			{
				System.out.println("Error : incorect options parameters***");
				return -1;
				//System.exit(0);
			}
		}
		else
		{
			System.out.println("***Error :" + options[index] + " incorect options parameters"+ index);
			return -1;
			//System.exit(0);
		}
	}
	
	public String getType()
	{
		int index = 4;
		int length = 0;
		
		/*please check the byte cast here */
		while((options[index] != DHCP_msg_type) && (options[index] != (byte)OPTION_END) && (options[index] != 0))
		{
			index = index + 1;
		}
		
		if(options[index] == 53)
		{
			index = index + 1;
			length = (int)options[index];
			
			if(length == 1)
			{
				index = index + 1;
				if(options[index] == DHCP_discover)
				{
					return "DHCPDiscovery";
				}
				else if (options[index] == DHCP_offer)
				{
					return "DHCPOffer";
				}
				else if (options[index] == DHCP_request)
				{
					return "DHCPRequest";
				}
				else if (options[index] == DHCP_ack)
				{
					return "DHCPAck";
				}
				else if (options[index] == DHCP_decline)
				{
					return "DHCPDecline";
				}
				else
				{
					System.out.println("Error : Uknown message type");
					return null;
				}
			}
			else
			{
				System.out.println("Error : incorect options parameters");
				return null;
			}
			
		}
		else
		{
			System.out.println("Error : incorect options parameters");
			return null;
		}
	}
	
	private byte[] getBytey(String ip)
	{
		byte[] bytey = new byte[4];
		int index=0,str_index=0;
		while (str_index < ip.length())
		{
			if (ip.charAt(str_index) == '.')
			{
				index++;
			}else
			{
				//Integer 
				bytey[index] = (byte)((bytey[index] * 10) + (Integer.parseInt(ip.substring(str_index,str_index+1))));
			}
			str_index++;
		}
		return bytey;
	}
	
	private String byteToIp(byte[] buf)
	{
		byte b;
		Integer a;
		int c;
		String ip = new String();
		for (int i=0;i<3;i++)
		{		
			b = buf[i];
			c = b >= 0 ? b : 256 + b;
			a = new Integer(c);
			ip = ip + a.toString() + ".";
		}
		b = buf[3];
		c = b >= 0 ? b : 256 + b;
		a = new Integer(c);
		ip = ip + a.toString();
		
		return ip;
	}
	
	byte byteValue(short s)
	{
		return (byte)(s);
	}
	
	byte byteValue(int i)
	{
		return (byte)(i);
	}
	
}
