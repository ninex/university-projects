import java.io.*;
import java.util.*;
import java.net.*;


public class client
{

	//port the server is listening to	
	private static int PORT = 3000;
	
	
	//the main function
	public static void main(String args[]) throws Exception
	{
		//declaration of variables
		Socket server = null;
		ObjectOutputStream out = null;
		ObjectInputStream in = null;
		
		
		//try to get a connection to the server
		System.out.println("Waiting for connection to server...");
		while (server == null)
		{
			try
			{
				server = new Socket(args[0],PORT);
			}catch (Exception e){}
		}
		
		//get the streams
		out = new ObjectOutputStream(server.getOutputStream());   
		in = new ObjectInputStream(server.getInputStream());   
		
		if ((server != null) && (server.isConnected()))
		{
			System.out.println("Connected to "+server.getInetAddress());
			
			//set a new messages to be sent
			REQ_MSG msg_out = new REQ_MSG(1,1,"passw","content",3);
			REP_MSG msg_in = null;	
		
			//send the message
			out.writeObject(msg_out);
			
			//read the reply
			try {
				msg_in = (REP_MSG)(in.readObject());
			}catch (Exception e){System.out.println("error"+e);System.exit(1);}
			
			//try to print the message
			if (msg_in != null)
			{
				msg_in.print_msg();
			}else{
				System.out.println("null message recieved");
			}
			
			//close the streams
			in.close();	
			out.close();
		}
		//close the socket
		System.out.println("Closing socket");
		server.close();
	}
}
