import java.io.*;
import java.util.*;
public class DHCPREP_MSG implements Serializable
{
        // variables needed in creating the request message
	byte[] op = {0,0};
	byte[] htype = {0,0};
	byte[] hlen = {0,0};
	byte[] hops = {0,0};
	byte[] xid = {0,0,0,0};
	byte[] secs = {0,0};
	byte[] flags = {0,0};
	byte[] ciaddr = {0,0,0,0};
	byte[] yiaddr = {0,0,0,0};
	byte[] siaddr = {0,0,0,0};
	byte[] giaddr = {0,0,0,0};
	byte[] chaddr = new byte[16];
	byte[] sname = new byte[64];
	byte[] file = new byte[128];
	Object reply;
	Date date;
	
        /* class constructor used to create the message. To create a message in any 
         * class which imports this message class, one only needs to call the constructor
         * with the correct parameters. At this point in the project, only the Object resp
         * seems to be of importance.
         */
	public DHCPREP_MSG()
	{
        //stamps each message with a date, implying that each message is unique.
		date = new Date();
	}
	
    // a procedure to display the message
	public void print_msg()
		System.out.println("date: "+date.toString());
	}
	
}