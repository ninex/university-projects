import java.io.*;
import java.util.*;
import java.net.*;



public class server
{
	//the port this server is listening on
	private static int PORT = 3000;
	
	public static void main(String args[]) throws Exception
	{
		ServerSocket connection = null;
		IpPool pool = new IpPool();
		//get a socket for the server
		try
		{
			connection = new ServerSocket(PORT);
		}catch (Exception e){System.out.println("error: "+e.toString());System.exit(1);}
                
		//while the connection is bound to a specific adress (isBound returns true if bound)
		while (connection.isBound())
		{	
			System.out.println("Listening for connection...");
			/* listen for a new connection to the server and accept it.
                         * this socket is passed to a thread which will now be created
                         * and the thread will handle the connection leaving this server
                         * to wait, listening for other attempted connections.
			 */
			new serverThread(connection.accept(),pool).start();
		}
	}
}
