import java.io.*;
import java.util.*;
public class REQ_MSG implements Serializable
{
        // variables needed in creating the request message
	int type;
	int id;
	String password;
	Object content;
	int req_res;
	Date date;
	REQ_MSG next;
	
        /* class constructor used to create the message. To create a message in any 
         * class which imports this message class, one only needs to call the constructor
         * with the correct parameters. At this point in the project, only the Object resp
         * seems to be of importance.
         */
	public REQ_MSG(int type,int id, String password, Object content, int req_res)
	{
		this.type = type;
		this.id = id;
		this.password = password;
		this.content = content;
		this.req_res = req_res;
                //stamps each message with a date, implying that each message is unique.
		date = new Date();
	}
	
        // a procedure to display the message
	public void print_msg()
	{
		System.out.println("Type: "+type);
		System.out.println("id: "+id);
		System.out.println("password: "+password);
		System.out.println("content: "+content.toString());
		System.out.println("req_res: "+req_res);
		System.out.println("date: "+date.toString());
	}
	
        /* This procedure is of no use of yet. We kept it here as we have an idea for
         * a later part of the project
         */
	public void setNext(REQ_MSG next)
	{
		this.next = next;
	}
}
