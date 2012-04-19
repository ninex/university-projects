import java.net.*;
import java.io.*;

public class serverThread extends Thread implements Runnable
{
	Socket client = null;
	
        //recieves a socket passed from the server class
	public serverThread(Socket client)
	{
		this.client = client;
	}
	
        // this function is called when you start to execute the thread
	public void run()
	{
		ObjectInputStream in = null;
		ObjectOutputStream out = null;
	
		// Two messages needed when communicating between client and server (thread)
		REQ_MSG msg_in = null;
		REP_MSG msg_out = null;
		
                // try and create input/ouput streams on which to pass and recieve
		try
		{
			in = new ObjectInputStream(client.getInputStream());
			out = new ObjectOutputStream(client.getOutputStream());
		}catch (Exception e){}
		
                // If a client has been connected to the socket the proceed
		if(client.isConnected())
		{
                        //attempt to recive the message from the input stream
			try {
				msg_in = (REQ_MSG)(in.readObject());
			}catch (Exception e){}
			
                        //if the message recieved exists
			if (msg_in != null)
			{
                                //print the message out
				msg_in.print_msg();
                                                                
                                // create a new reply message using the constructor of the reply msg class
				msg_out = new REP_MSG(1,"reply");
				
                                //if it has been created
				if (msg_out != null)
				{
                                        //try and pass the message to the outputstream
					try {
						out.writeObject(msg_out);
						System.out.println("Reply sent");
                                                // close the outputstream and the socket
						out.close();
						client.close();
					}catch (Exception e){}	
					
				}
		
			}
                        
                        // simply to display that the conection was closed
			if (client.isClosed())
			{
				System.out.println("Connection was closed");
			}
		}
	}
}
