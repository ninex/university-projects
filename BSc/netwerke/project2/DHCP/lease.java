import java.io.*;
import java.util.*;
import java.net.*;

public class lease
{
	public String ip;
	//public String mac;
	public int lease_time;
	
	public lease()
	{
		
	}
	
	public lease(String ipFree)
	{
		ip = ipFree;
		//mac = myMac;
		//this.lease_time = lease_time;
	}
	
	public String getip() 
	{
		return ip;
	}
	
	public void setLease(int leaseTime)
	{
		lease_time = leaseTime;	
	}
}