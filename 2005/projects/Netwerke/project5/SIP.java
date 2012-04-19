import java.net.*;
public class SIP
{
	/* Request Methods
	==================
	REGISTER;
	INVITE;
	ACK;
	BYE;
	CANCEL;
	OPTIONS;
	*/
	
	/*Response status
	================
	100 Trying
	180 Ringing
	182 Queued
	200 OK
	302 Moved temporarily
	*/
	
	static final String SIP_VER = "SIP/2.0";
	
	
	static DatagramPacket createRequest(String method,String uri,int port,String theheader,String thebody)
	{
		String start_line = method+" "+uri+" "+SIP_VER+"\r\n";
		String header = theheader+"\r\n";
		String body = thebody+"\r\n";
		String SIP_msg = new String(start_line+header+"\r\n"+body);
		
		byte bytearray[] = SIP_msg.getBytes();
		try
		{
			DatagramPacket msg = new DatagramPacket(bytearray,bytearray.length,InetAddress.getByName(uri),port);
			return msg;
		}catch (Exception e){e.printStackTrace();}
		return null;
	}
	
	static DatagramPacket createResponse(String status,String reason,InetAddress uri,int port)
	{
		String start_line = SIP_VER+" "+status+" "+reason+"\r\n";
		String header = reason+"\r\n";
		String body = "\r\n";
		String SIP_msg = new String(start_line+header+"\r\n"+body);
		
		byte bytearray[] = SIP_msg.getBytes();
		try
		{
			DatagramPacket msg = new DatagramPacket(bytearray,bytearray.length,uri,port);
			return msg;
		}catch (Exception e){}
		return null;
	}
	
	static String[] stripRequestHeader(DatagramPacket msg)
	{
		String SIP_msg = new String(msg.getData());
		
		String a[] = SIP_msg.split("\r\n");
		
		String header[] = a[0].split(" ");
		
		return header;
	}
	static String stripRequestBody(DatagramPacket msg)
	{
		String SIP_msg = new String(msg.getData());
		
		String a[] = SIP_msg.split("\r\n");
		
		String body = a[3];
		
		return body;
	}
	static String[] stripResponse(DatagramPacket msg)
	{
		String SIP_msg = new String(msg.getData());
		String a[] = SIP_msg.split("\r\n");
		/*for (int i=0;i<a.length;i++)
		{
			System.out.println(a[i]);
		}
		if (a.length >= 4)
		{
			System.out.println("invalid message");
			return null;	
		}*/
		String header[] = a[0].split(" ");
		return header;
	}
	
	static String[] stripBody(String body)
	{
		//System.out.println(body);
		String m[] = body.split(",");
		return m;
	}
	static String stripResponseHeader(DatagramPacket msg)
	{
		String SIP_msg = new String(msg.getData());
		String body[] = SIP_msg.split("\r\n");
		return body[1];
	}
	static String Header(String to,String from, String Via)
	{
		String Max = (new Integer(72)).toString();
		String CallId = (new Long(Math.round((Math.random())*10000000))).toString();
		String Cseq = (new Long(Math.round((Math.random())*10000000))).toString();
		//System.out.println(Cseq);
		String header = "Via: "+Via+"\nMax-Forwards: "+Max+"\nTo: "+to+"\nFrom: "+from+"\nCall-id: "+CallId+"\nCseq: "+Cseq;
		return header;
	}
	static String[] stripRequestMandatory(DatagramPacket msg)
	{
		String SIP_msg = new String(msg.getData());
		String a[] = SIP_msg.split("\r\n");
		String b[] = a[1].split("\n");
		for (int i=0;i<b.length;i++)
		{
			System.out.println("[Header] "+b[i]);
		}
		return b;
	}
	static String getHeader(DatagramPacket msg)
	{
		String SIP_msg = new String(msg.getData());
		String a[] = SIP_msg.split("\r\n");
		return a[1];
	}
	static void printHeader(DatagramPacket msg)
	{
		String SIP_msg = new String(msg.getData());
		String a[] = SIP_msg.split("\r\n");
		String b[] = a[1].split("\n");
		for (int i=0;i<b.length;i++)
		{
			System.out.println("[Header] "+b[i]);
		}
	}
	static String getNameFromInvite(DatagramPacket msg)
	{
		String[] body = SIP.stripRequestMandatory(msg);
		
		int index = -1;
		for (int i=0;i<body.length;i++)
		{
			if (body[i].startsWith("To:"))
			{
				index = i;
			}
		}
		
		String theHoff[] = body[index].split("<");
		theHoff = theHoff[0].split("To:");
	
		//get Name
		//String a[] = theHoff[0].split(":");
		String name = theHoff[1].trim();
		//System.out.println("\tinvite from:"+name+".");
		return name;
	}
	static int getPortRequest(DatagramPacket msg)
	{
		String SIP_msg = new String(msg.getData());
		String a[] = SIP_msg.split("\r\n");
		
		/*for (int i=0;i<a.length;i++)
		{
			System.out.println("[Body2] "+a[i]);
		}*/
		String b[] = a[3].split("port: ");
		/*for (int i=0;i<b.length;i++)
		{
			System.out.println("[Body] "+b[i]);
		}*/
		int port = (new Integer(b[1].trim())).intValue();
		//System.out.println("Voice port = "+port);
		return port;
	}
	static int getPortResponse(DatagramPacket msg)
	{
		//String ab[] = stripResponseHeader(msg);
		String SIP_msg = new String(msg.getData());
		String a[] = SIP_msg.split("\r\n");
		//System.out.println(SIP_msg);/*
		/*for (int i=0;i<a.length;i++)
		{
			System.out.println("[RESPBody2] "+a[i]);
		}*/
		String b[] = a[0].split("port: ");
		/*for (int i=0;i<b.length;i++)
		{
			System.out.println("[RESPBody] "+b[i]);
		}*/
		int port = (new Integer(b[1].trim())).intValue();
		//System.out.println("Vocie port = "+port);
		return port;
	}
}
