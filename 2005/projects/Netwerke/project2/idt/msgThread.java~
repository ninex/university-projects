import java.net.*;
import java.io.*;

public class msgThread extends Thread implements Runnable
{
	Socket client = null;
	ClientGUI gui;
	public msgThread(Socket client, ClientGUI gui)
	{
		this.client = client;
		this.gui = gui;
	}
	
    // this function is called when you start to execute the thread
	public void run()
	{
		ObjectInputStream in = null;
	
		// Two messages needed when communicating between client and server (thread)
		String msg_in = null;
		String host=null;
        // try and create input streams on which to pass and recieve
		try
		{
			in = new ObjectInputStream(client.getInputStream());			
		}catch (Exception e){}
		
		// If a client has been connected to the socket the proceed
		if(client.isConnected())
		{
			//attempt to recive the message from the input stream
			try {
				host = (String)(in.readObject());
				msg_in = (String)(in.readObject());
			}catch (Exception e){}
			
			//if the message recieved exists
			if (msg_in != null)
			{
				//print the message out
				gui.addMsg(msg_in,host);
			}
                        
            // simply to display that the conection was closed
			if (client.isClosed())
			{
				System.out.println("Connection was closed");
			}
		}
	}
}
