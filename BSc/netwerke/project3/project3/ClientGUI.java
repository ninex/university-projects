package project3;


import javax.swing.*;
import java.io.*;
import java.awt.*;
import java.awt.event.*;
import java.net.*;
/**
 * A simulation of OSPF. It computes the shortest path (in network congestion)
 * to all routers in the network.
 */
 
public class ClientGUI extends JFrame implements ActionListener
{	
	
	JTextField ip = new JTextField(12);
	JTextField len = new JTextField(2);
	JTextField tport = new JTextField(3);
	JTextField dest = new JTextField(3);
	JTextArea Debug = new JTextArea(10,8);

	JList list = new JList();

	Container main_pane = null;

	Timer timer;
	
	int neighbours = 0;
	
	static ServerSocket connection = null;
	static MulticastSocket broad = null;
	static ipLookupTable lookthread;
	static int port;
	static Dijkstra routes = null;
	public ClientGUI(ipLookupTable a_lookthread, ServerSocket a_connection,MulticastSocket a_broad, int a_port) 
	{
		super("Router");
		lookthread = a_lookthread;
		connection = a_connection;
		broad = a_broad;
		port = a_port;
		timer = new Timer(700,this);
		
        	setSize(600, 500);
		setVisible(true);
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		
		this.addWindowListener(new WindowListener()
		{
			public void windowOpened(WindowEvent a){}
			public void windowClosing(WindowEvent a)
			{
				ClientGUI.lookthread.crash(broad);
				ClientGUI.lookthread.shutdown();
				try
				{
					ClientGUI.connection.close();
				}catch (Exception e){e.printStackTrace();}
				
			}
			public void windowActivated(WindowEvent a){}
			public void windowDeactivated(WindowEvent a){}
			public void windowIconified(WindowEvent a){}
			public void windowDeiconified(WindowEvent a){}
			public void windowClosed(WindowEvent a){}});

		JScrollPane scrollPane = new JScrollPane(list);

		JButton send = new JButton("Connect");
		JButton Route = new JButton("Route");
		
		JPanel pane1 = new JPanel();
		pane1.setLayout(new BorderLayout());
		pane1.add(scrollPane,BorderLayout.EAST);
		
		JPanel pane2 = new JPanel();
		JPanel pane3 = new JPanel();
		JPanel pane4 = new JPanel();
		JPanel pane5 = new JPanel();
		JPanel pane6 = new JPanel();
		
		pane3.add(ip,BorderLayout.NORTH);
		pane3.add(len,BorderLayout.NORTH);
		pane3.add(tport,BorderLayout.NORTH);
		pane3.add(send,BorderLayout.SOUTH);

		pane4.add(dest,BorderLayout.NORTH);
		pane4.add(Route,BorderLayout.SOUTH);
		
		pane6.setLayout(new BorderLayout());
		pane6.add(Debug,BorderLayout.CENTER);
		
		pane5.setLayout(new BorderLayout());
		pane5.add(pane4,BorderLayout.SOUTH);
		pane5.add(pane6,BorderLayout.NORTH);
		
		pane2.setLayout(new BorderLayout());
		pane2.add(pane3,BorderLayout.NORTH);
		pane2.add(pane5,BorderLayout.SOUTH);
		
		JPanel main_panel = new JPanel();
		main_panel.setLayout(new BorderLayout());
		main_panel.add(pane1,BorderLayout.WEST);
		main_panel.add(pane2,BorderLayout.EAST);
		
		main_pane = getContentPane();
		getContentPane().setLayout( new BorderLayout());
		main_pane.add(main_panel,BorderLayout.CENTER);
		
		send.addActionListener(new ActionListener()
		{
			public void actionPerformed(ActionEvent a)
			{
				Socket server = null;
				try
				{
					server = new Socket(ip.getText(),Integer.parseInt(tport.getText()));
					ObjectOutputStream out = null;
					ObjectInputStream in = null;
					//DenseRoutesMap themap = null;
					try
					{
						out = new ObjectOutputStream(server.getOutputStream());
						in = new ObjectInputStream(server.getInputStream());
						out.writeObject(len.getText());
						out.writeObject(new Integer(port));
						if (routes == null)
						{
							out.writeObject("receive");
							DenseRoutesMap themap = (DenseRoutesMap)(in.readObject());
							//System.out.println("Received the map");
							ClientGUI.lookthread.mapFill(themap);
						}else
						{
							out.writeObject("send");
							out.writeObject(getMap());
							//System.out.println("Sent the map");
						}
						
					}catch (Exception e){System.out.println("e1 "+e.toString());}
					/*if (themap != null)
					{
						System.out.println("Mapping..;......");
						
						reCalculate();
					}*/
					ClientGUI.lookthread.insert(server,len.getText(),port,Integer.parseInt(tport.getText()));
					//while (lookthread.busy()){}	
						
					byte buf[] = new byte[12];
					buf[0] = (byte)(Integer.parseInt(len.getText()));
					
					Integer s = new Integer(ClientGUI.port);
					
					byte po[] = (s.toString()).getBytes();
					getPacket(po,buf,4);
					
					s = new Integer(tport.getText());
					
					po = (s.toString()).getBytes();
					getPacket(po,buf,8);
					
					DatagramPacket packet = new DatagramPacket(buf,buf.length,InetAddress.getByName("224.0.0.0"),1234);
					
					try
					{
						broad.send(packet);
					}catch (Exception e){System.out.println("e2 "+e.toString());}
						
					
					
				}catch (Exception e){e.printStackTrace();}
					
		;}});
		Route.addActionListener(new ActionListener()
		{
			public void actionPerformed(ActionEvent a)
			{
				try
				{
					reCalculate();
				}catch (Exception e){}
				if (routes != null)
				{
					shortest(routes,node.valueOf(Integer.parseInt(dest.getText())));
				}else
				{
					Debug.setText("No connected routers");
				}
		;}});
		
		lookthread.start();
		timer.start();
		}

	public void actionPerformed(ActionEvent e)
	{
		lookthread.refresh(list);
	}

	public void setNeighbours()
	{
		//neighbours++;
	}
	public void unsetNeighbours()
	{
		//neighbours--;
	}

	public DenseRoutesMap getMap()
	{
		if (lookthread.getMax() == 0)
		{
			System.out.println("Map leeg");
			return null;
		}
		
		DenseRoutesMap map = new DenseRoutesMap(lookthread.getMax()+1);
		
		lookthread.fill(map);
		
		return map;
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
	public void shortest(Dijkstra routes,node anode)
	{
		Debug.setText("Shortest route to me from Router "+anode.getIndex());
		Debug.append("\n_______________________");
		Debug.append("\nDistance: "+routes.getShortestDistance(anode));
		if (routes.getShortestDistance(anode) == Dijkstra.INFINITE_DISTANCE)
		{
			Debug.append("\nNo route exist");
		}else
		{
			while (anode.getIndex() != (port-5000))
			{
				Debug.append("\nRouter "+anode.getIndex());
				anode = routes.getPredecessor(anode.getIndex());	
			}
			Debug.append("\nRouter "+anode.getIndex());
		}
		
		
	}
	public void reCalculate()
	{
		//System.out.println("in reCalculate");
		System.out.println("Recalculating for "+lookthread.getMax()+" routers");
		DenseRoutesMap map = new DenseRoutesMap(lookthread.getMax()+1);
		
		lookthread.fill(map);
		routes = new Dijkstra(map);
		
		routes.execute(node.valueOf(port - 5000),null);
	}
	
}
