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
		return null;
	}
	public void insert(String name, String Address, int listen, int call)
	{
		System.out.println("[Registrar] Inserting "+name+"@"+Address+" who listens on "+listen+" and calls on "+call);
		list l = new list(name,Address,listen,call);
		info.add(l);
	}
}