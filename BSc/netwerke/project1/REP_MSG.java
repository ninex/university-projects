import java.io.*;
import java.util.*;
public class REP_MSG implements Serializable
{
        // variables needed in creating the reply message
	int id;
	Object resp;
	Date date;
	REP_MSG next;
	
        /* class constructor used to create the message. To create a message in any 
         * class which imports this message class, one only needs to call the constructor
         * with the correct parameters. At this point in the project, only the Object resp
         * seems to be of importance.
         */
	public REP_MSG(int id, Object resp)
	{
		this.id = id;
		this.resp = resp;
                //stamps each message with a date, implying that each message is unique.
		date = new Date();
	}
	
        // a procedure to display the message
	public void print_msg()
	{	
		System.out.println("id: "+id);
		System.out.println("response: "+resp.toString());
		System.out.println("time: "+date.toString());
	}
	
	public void setNext(REP_MSG next)
	{
		this.next = next;
	}
	

}