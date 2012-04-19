package project3;

import java.io.*;
import java.util.*;
import java.net.*;
import javax.swing.*;

/**
 * Tik hier stuff oor ipLookupTable
 *
 */

public final class ipLookupTable extends Thread implements Runnable
{
	boolean running,update;
	int items,max,me;
	//clientThread thread;
	
	list first = null;
	list last = null;
	list dfirst = null;
	list dlast = null;
	
	public ipLookupTable(int me) 
	{
		//this.gui = gui;
		running = true;
		items = 0;
		update = false;
		this.me = me;
	}
	
	public void run()
	{
		while(running)
		{}
	}
	
	public synchronized void fill(DenseRoutesMap map)
	{
		list temp = dfirst;
		while (temp != null)
		{
			map.addUndirectRoute(temp.a(),temp.b(),temp.w());
			temp = temp.getNext();
			System.out.println("New route added");
		}
	}
	public synchronized void mapFill(DenseRoutesMap map)
	{
		//if (dfirst != null)
		//{
			int num = map.getNodes();
			System.out.println("size = "+num);
			for (int i=0;i<num;i++)
			{
				for (int j=0;j<num;j++)
				{
					if (map.getDistance(i,j) > 0)
					{
						insert(i,j,map.getDistance(i,j));
					}
				}
			}
		//}
		System.out.println("Filled map");
		/*
		list temp = dfirst;
		while (temp != null)
		{
			map.addUndirectRoute(temp.a(),temp.b(),temp.w());
			temp = temp.getNext();
			System.out.println("New route added");
		}*/
	}
	
	public synchronized void crash(MulticastSocket broad)
	{
		list temp = dfirst;
		byte msg[] = new byte[12];
		while (temp != null)
		{
			if ((temp.a() == me) || (temp.b() == me))
			{
				//map.addUndirectRoute(temp.a(),temp.b(),temp.w());
				msg[0] = (byte) (0);
				
				Integer s = new Integer(5000+temp.a());
				
				byte po[] = (s.toString()).getBytes();
				getPacket(po,msg,4);
				
				s = new Integer(5000+temp.b());
				
				po = (s.toString()).getBytes();
				getPacket(po,msg,8);
				
				try
				{
					DatagramPacket packet = new DatagramPacket(msg,msg.length,InetAddress.getByName("224.0.0.0"),1234);
				
				
					broad.send(packet);
				}catch (Exception e){}
			}
			
			temp = temp.getNext();
		}
	}
	
	public synchronized void needRefresh()
	{
		list l = first;
		if (first != null)
		{
			while ((l!= null))
			{
				try
				{
					(l.returnSocka()).sendUrgentData(1);
					//System.out.println("Still connected to "+l.host()+" on "+l.port());
					l = l.getNext();
				}catch (Exception e)
				{
					//System.out.println("failed");
					list k = l;
					l = l.getNext();
					System.out.println("A connected router has failed/disconnected");
					remove(k);
				}
			}
		}
	}
	
	public synchronized void shutdown()
	{
		list l = first;
		if (first != null)
		{
			while ((first!= null))
			{
				try
				{
					first.returnSocka().close();
				}catch (Exception e){System.out.println(e.toString());}
				remove(first);
			}
		}
		System.out.println("Shutdown done...");
		running = false;
	}
	 
	public synchronized void remove(list l)
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
		items--;
		update = true;
	}
	public synchronized void dremove(list l)
	{
		if (l.getNext() != null)
		{
			(l.getNext()).setPrev(l.getPrev());
		}
		if (l.getPrev() != null)
		{
			(l.getPrev()).setNext(l.getNext());
		}
		if ((l == dfirst) & (l == dlast))
		{
			dfirst = null;
			dlast = null;
		}else if (l == dlast)
		{
			dlast = l.getPrev();
		}else if (l == dfirst)
		{
			dfirst = l.getNext();
		}
		
		update = true;
	}
	
	public synchronized void remove(int a, int b)
	{
		list temp = dfirst;
		while (temp != null)
		{
			if ((temp.a() == a) && (temp.b() == b))
			{
				dremove(temp);
				return;
			}
			temp = temp.getNext();
		}
	}
	
	public synchronized int getMax()
	{
		max = 0;
		list temp = dfirst;
		while (temp != null)
		{
			if (temp.a() > max)
			{
				max = temp.a();
			}
			if (temp.b() > max)
			{
				max = temp.b();
			}
			temp = temp.getNext();
		}
		
		return max;
	}
	
	public synchronized void insert(Socket socka, String len, int local, int remote)
	{
		list l = new list(socka,len,local,remote);
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
	private boolean lookup(int a, int b, int w)
	{
		System.out.println("in lookup");
		list temp = dfirst;
		while (temp != null)
		{
			if ((temp.a() == a) && (temp.b() == b) && (temp.w() == w))
			{
				return true;
			}
			temp = temp.getNext();
		}
		//boolean ret = lookup(b,a,w);
		return false;
	}
	public synchronized void insert(int a, int b, int w)
	{
		list l = new list(a,b,w);
		if (dfirst == null)
		{
			dfirst = l;
			dlast = dfirst;
		}else
		{
			if (!(lookup(a,b,w)) && !(lookup(b,a,w)))
			{
				System.out.println("setting");
				dlast.setNext(l);
				l.setPrev(dlast);
				dlast = l;
			}else
			{
				System.out.println("exists");
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
				objs[i] = temp.returnSa();
				i++;
				temp = temp.getNext();
			}
			jlist.setListData(objs);
			update = false;
		}
	}
	public int getItems()
	{
		return items;
	}
	
	private void getPacket(byte[] ip, byte[] shorty,int s)
	{
		for (int i = 0;i<4;i++)
		{
			if (i < ip.length)
			{
				shorty[s+i] = ip[i];
			}else
			{
				shorty[s+i] = 0;
			}
		}
	}
}
