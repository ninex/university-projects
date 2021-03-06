import java.net.*;
public class VoIP
{
	static int proxyPort = 5060;
	static int callPort = 5061;
	static int listenPort = 5062;
	
	
	
	public static void main(String args[])
	{
		UAC client = null;
		UAS server = null;
		int disp = (new Integer(args[1]).intValue()) * 2;
		String myName = args[0];
		callPort = callPort + disp;
		listenPort = listenPort + disp;
		
		DatagramSocket outgoing = null,incoming = null, proxy = null;
		try
		{
			outgoing = new DatagramSocket(callPort);
			incoming = new DatagramSocket(listenPort);
			//proxy = new DatagramSocket(proxyPort);
		}catch (Exception e){e.printStackTrace();}
		gui main_frame = new gui();
		main_frame.hide();
		client = new UAC(outgoing,proxy,disp,main_frame);
		server = new UAS(incoming,proxy,main_frame);
		client.Register(myName);
		client.start();
		server.start();
		main_frame.setClient(client);
		main_frame.show();
		main_frame.addMsg("My info\n========\ncall:"+callPort+"\nlisten:"+listenPort+"\n");
	}
}