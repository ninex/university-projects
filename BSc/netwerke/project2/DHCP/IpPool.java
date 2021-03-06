import java.io.*;
import java.util.*;
import java.net.*;

public class IpPool
{
	public static final String IpStart = "146.168.0.15";
	public static int numOfIps = 30;
	
	private static lease[] leaseArray;
	
	public IpPool()
	{
		leaseArray = new lease[numOfIps];
	}
	
	public String IpRequest()
	{
		int i = 0;
		
		while(leaseArray[i] != null)
		{
			i++;
		}
		
		if(leaseArray[i] == null)
		{
			//System.out.println("Free Ip addres found in Ip Pool: Offering");
			short[] temp = new short[4];
			
			temp = getInty(IpStart);
			temp[3] = (short)(temp[3]+(short)i);
			
			String temp1 = shortToIp(temp);
			leaseArray[i] = new lease(temp1);
			
			//System.out.println("Allocating" + leaseArray[i].ip);

			return leaseArray[i].ip;
		}
		else
		{
			System.out.println("There are no existing Ip adresses available in the Ip Pool");
			return null;
		}
		
	}
	
	public void start(int leaseTime, String searchIp)
	{
		int index = 0;
		//String temp = getBytey(searchIp);
		/* compare ips till you find the right one*/
		/*while()
		{
			
		}
		
		if()
		{
			leaseArray[i].setLease(leaseTime);
		}*/
	}
	
	public void refresh(String ip_renew)
	{
		
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
				//Integer 
				shorty[index] = (short)((shorty[index] * 10) + (Integer.parseInt(ip.substring(str_index,str_index+1))));
			}
			str_index++;
		}
		return shorty;
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
	
	private String shortToIp(short[] buf)
	{
		short b;
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
}