package project3;

import java.io.*;
import java.util.*;
import java.net.*;

/**
 * Tik hier stuff oor clientThread
 *
 */

public class clientThread extends Thread implements Runnable
{

	Socket client;
	private ipLookupTable lookthread;
	int port;
	private DenseRoutesMap themap;
	private boolean test;

	public clientThread(Socket client,ipLookupTable lookthread, int port)
	{		
		this.client = client;
		this.lookthread = lookthread;
		this.port = port;
	}
	
	/**
	 * the main function
	 */
	public void run()
	{
		//InetSocketAddress sa = (InetSocketAddress)client.getRemoteSocketAddress();
		
		ObjectInputStream in = null;
		ObjectOutputStream out = null;
	
		// Two messages needed when communicating between client and server (thread)
		
		String len=null;
        	// try and create input streams on which to pass and recieve
		try
		{
			in = new ObjectInputStream(client.getInputStream());
			out = new ObjectOutputStream(client.getOutputStream());
		}catch (Exception e){}
		
		// If a client has been connected to the socket the proceed
		if(client.isConnected())
		{
			test = false;
			Integer remote_port = null;
			//attempt to recive the message from the input stream
			String what = null;
			try {
				len = (String)(in.readObject());
				remote_port = (Integer)(in.readObject());
				what = (String)(in.readObject());
				//out.writeObject(getMap());
				//client.shutdownInput();
			}catch (Exception e){System.out.println(e.toString());}
			try
			{
				if (what.equals("receive"))
				{
					//System.out.println("ek stuur map");
					out.writeObject(getMap());
				}else
					if (what.equals("send"))
					{
						//System.out.println("ek kry map");
						DenseRoutesMap theMap = (DenseRoutesMap)(in.readObject());
						lookthread.mapFill(theMap);
					}else
					{
						System.out.println("status: "+what);
					}
			}catch (Exception e){}
			//if the message recieved exists
			if (len != null)
			{
				//lookthread.mapFill(themap);
				//print the message out
				lookthread.insert(client,len,port,remote_port.intValue());
				
			}
                        test = true;
           
		}	
	}
	public DenseRoutesMap getMap()
	{
		if (lookthread.getMax() == 0)
		{
			System.out.println("map is leeg");
			return null;
		}
		
		DenseRoutesMap map = new DenseRoutesMap(lookthread.getMax()+1);
		
		lookthread.fill(map);
		
		return map;
	}
	public boolean access()
	{
		return test;
	}
}
