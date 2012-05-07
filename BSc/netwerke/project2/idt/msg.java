import java.io.*;
import java.util.*;
import java.net.*;

public class msg extends Thread implements Runnable
{
	ClientGUI gui;
	public msg(ClientGUI gui)
	{
		this.gui = gui;
	}
	public void run()
	{
		int PORT = 3000;
		
		ServerSocket connection = null;
		
		//get a socket for the server
		try
		{
			connection = new ServerSocket(PORT);
		}catch (Exception e){System.out.println("error: "+e.toString());System.exit(1);}

		while (connection.isBound())
		{	
			System.out.println("Listening for messages...");
			try
			{
				new msgThread(connection.accept(),gui).start();
			}catch (Exception e){}
		}
	}
	
}