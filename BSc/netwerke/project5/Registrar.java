import java.util.*;
public class Registrar extends Thread implements Runnable
{
	private LinkedList info = null;
	public Registrar()
	{
		info = new LinkedList();
	}
	public void run()
	{
		while (true);
	}
	
	public list lookup(String name)
	{
		//return address
		if (info.size() == 0)
		{
			return null;
		}
		ListIterator iter = info.listIterator(0);
		list l = (list)info.getFirst();
		if ((l.rName()).equalsIgnoreCase(name))
		{
			return l;
		}
		while (iter.hasNext())
		{
			l = (list)iter.next();
			if ((l.rName()).equalsIgnoreCase(name))
			{
				return l;
			}
		}
		System.out.println("[REGISTRAR] Lookup: Returning null");
		return null;
	}
	public void insert(String name, String Address, int listen, int call)
	{
		System.out.println("[REGISTRAR] Inserting record for: "+name);
		list l = lookup(name);
		
		if (l == null)
		{
			System.out.println("[Registrar] Inserting "+name+"@"+Address+" who listens on "+listen+" and calls on "+call);
			l = new list(name,Address,listen,call,false);
			info.add(l);
		}else
		{
			info.remove(l);
			System.out.println("[Registrar] Updating "+name+"@"+Address+" who listens on "+listen+" and calls on "+call);
			l = new list(name,Address,listen,call,true);
			info.add(l);
		}
	}
}