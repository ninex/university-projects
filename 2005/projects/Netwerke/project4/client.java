import java.io.*;
import java.util.*;
import java.net.*;
import java.security.*;
import javax.swing.*;
import org.bouncycastle.jce.provider.*;
import javax.crypto.*;

public class client
{
	//port the server is listening to
	private static int PORT = 3000;

	//the main function
	public static void main(String args[]) throws Exception
	{
		Security.addProvider(new BouncyCastleProvider());
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
			}catch (Exception e){System.out.println("No server running");}
		}
		
		if ((server != null) && (server.isConnected()))
		{
			System.out.println("Connected to "+server.getInetAddress());
			
			//set a new messages to be sent

			GUI main_frame = new GUI(server);
			main_frame.show();
		}
	}
}
