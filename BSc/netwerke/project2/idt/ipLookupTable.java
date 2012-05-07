import java.io.*;
import java.util.*;
import java.net.*;
import javax.swing.*;

public class ipLookupTable extends Thread implements Runnable
{
	boolean running,update;
	int items;
	clientThread thread;
	
	list first = null;
	list last = null;
	ClientGUI gui;
	
	public ipLookupTable(ClientGUI gui) 
	{
		this.gui = gui;
		running = true;
		items = 0;
		update = false;
	}
	
	public void run()
	{
		while(running)
		{}
	}
	
	public void needRefresh()
	{
		list l = first;
		if (first != null)
		{
			while ((l!= null))
			{
				if ((System.currentTimeMillis() - (l.getTime()))> (long)10000)
				{
					remove(l);
					gui.addMessage(l.returnIp() + " needs to be refreshed");
				}
				l = l.getNext();
			}
		}
	}
	
	public void stopIt()
	{
		running = false;
	}
	
	public void remove(list l)
	{
		if (l.getNext() != null)
		{
			(l.getNext()).setPrev(l.getPrev());
		}
		if (l.getPrev() != null)
		{
			(l.getPrev()).setNext(l.getNext());
		}
		if ((l == first) & (l == last))
		{
			first = null;
			last = null;
		}else if (l == last)
		{
			last = l.getPrev();
		}else if (l == first)
		{
			first = l.getNext();
		}
		update = true;
	}
	
	public void insert(String actual, String virtual, byte[] buf)
	{
		list l = new list(actual,virtual,buf);
		if (first == null)
		{
			first = l;
			last = first;
		}else
		{
			last.setNext(l);
			l.setPrev(last);
			last = l;
			
		}
		items++;
		update = true;
	}
	
	public void refresh(String virtual)
	{
		list l = first;
		if (first != null)
		{
			while ((l!= null) && ((l.returnIp()).compareTo(virtual) != 0))
			{	
				l = l.getNext();
			}
			if ((l != null)&& ((l.returnIp()).compareTo(virtual) == 0))
			{
				gui.addMessage(virtual + " is being refreshed");
				l.refreshTime();
			}
		}
	}	
	public void refresh(JList jlist)
	{
		needRefresh();
		if (update)
		{
			String[] objs = new String[items];
			list temp = first;
			int i=0;
			while (temp != null)
			{
				objs[i] = temp.returnIp();
				i++;
				temp = temp.getNext();
			}
			jlist.setListData(objs);
			update = false;
		}
	}
	public boolean lookFor(String ip)
	{
		list l = first;
		if (first == null)
		{
			return false;
		}else
		{
			while ((l!= null) && ((l.returnIp()).compareTo(ip) != 0))
			{	
				l = l.getNext();
			}
			if ((l != null)&& ((l.returnIp()).compareTo(ip) == 0))
			{
				return true;
			}else
			{
				return false;
			}
		}
	}
	public byte[] getArp(String ip)
	{
		list l = first;
		if (first == null)
		{
			return null;
		}else
		{
			while ((l!= null) && ((l.returnIp()).compareTo(ip) != 0))
			{
				l = l.getNext();
			}
			if ((l!=null)&&((l.returnIp()).compareTo(ip) == 0))
			{
				return l.getArp();
			}else
			{
				return null;
			}
		}
	}
	public String getActual(String ip)
	{		
		list l = first;
		if (first == null)
		{
			return null;
		}else
		{
			while ((l!= null) && ((l.returnIp()).compareTo(ip) != 0))
			{
				l = l.getNext();
			}
			if ((l!=null)&&((l.returnIp()).compareTo(ip) == 0))
			{
				return l.getActual();
			}else
			{
				return null;
			}
		}
	}
	public void setClientThread(clientThread thread)
	{
		this.thread = thread;
	}
}
