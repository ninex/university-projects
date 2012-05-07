import javax.swing.*;
import javax.swing.border.*;
import java.awt.*;
import java.awt.event.*;
import java.sql.*;

public class window extends JFrame implements ActionListener
{
  private Queries queries;
  private boolean laidOut = false;
  private Bespreking myBespreking;
	private Search search;
	private JMenuBar menubar;
	private JMenu m_plase,m_book,m_search;
	private JMenuItem itemMake,itemEdit,itemSearch,itemReset;
	private JMenuItem[] itemPlase;
	private JPanel contentPane,viewPane;
	private int gp_ID,aantal_plase,image_index;
	private JLabel[] t;
	private	JLabel id,besprekings,lab,head;
	private JTextField i_id;
	private JButton b1,b2,b3;
	private int count;
	private String[] nr;
	private Date[] dates;
	private boolean poefter;
	private Font f;
	private SQLconnect sqlCon;
	private Timer	timer;
	private String[] image_string = {"1.jpg","2.jpg","3.jpg","4.jpg","5.jpg","6.jpg","7.jpg","8.jpg","9.jpg","10.jpg","11.jpg","12.jpg"};
	private ImageIcon[] images;
	private JLabel img,back;
	private Border etched;
	private Container main;
	private JComboBox l_lab;

  //constructor
  public window()
  {
		main = getContentPane();
		main.setLayout(null);

		timer = new Timer(7500,this);

		image_index=0;
		images = new ImageIcon[image_string.length];
		for (int i=0;i<images.length;i++)
		{
			images[i] = new ImageIcon("./pics/"+image_string[i]);
		}

		sqlCon = new SQLconnect();
    queries = new Queries(sqlCon);
		gp_ID = 1;
		poefter = false;
    aantal_plase = queries.aantalPlase();

		contentPane();
		viewPane();

    myBespreking = new Bespreking(this,sqlCon,queries);
		Insets myinsets = myBespreking.getInsets();
    myBespreking.setTitle("Make a Booking");
    myBespreking.setSize(400+ myinsets.left + myinsets.right,410 + myinsets.top + myinsets.bottom);
    myBespreking.setResizable(false);

		search = new Search(this,queries);
		myinsets = search.getInsets();
		search.setTitle("Search");
		search.setSize(300 + myinsets.left + myinsets.right,480 + myinsets.top + myinsets.bottom);
		search.setResizable(false);
    //center on screen
    myBespreking.setLocationRelativeTo(null);
		search.setLocationRelativeTo(null);


		main.add(viewPane);
		main.add(contentPane);
		main.add(img);

		viewPane.setVisible(false);

		makeMenu();

    //all die listeners
    addMyListeners();

    //sit 'n window listener by vir wanneer die user wil quit
    addWindowListener(new WindowAdapter()
    {
      public void windowClosing(WindowEvent e)
      {
				sqlCon.disconnect();
        System.exit(0);
      }
			public void windowIconified(WindowEvent e)
			{
				stopAnimation();
			}
      public void windowDeiconified(WindowEvent e)
			{
				startAnimation();
			}
    });

  }
//===========================================================================
  //al die listeners wat my komponente moontlik kan he
  //van hieraf word alles beheer
  private void addMyListeners()
  {
    itemMake.addActionListener(new ActionListener()
    {
      public void actionPerformed(ActionEvent e)
      {
				if (gp_ID != 0)
				{
					myBespreking.beginBespreking(gp_ID);
        	myBespreking.setVisible(true);
				}
      }
    });
		itemEdit.addActionListener(new ActionListener()
    {
      public void actionPerformed(ActionEvent e)
      {
				viewPane.setVisible(true);
				contentPane.setVisible(false);
				itemReset.setEnabled(false);
				itemSearch.setEnabled(false);
				itemMake.setEnabled(false);
      }
    });
		itemSearch.addActionListener(new ActionListener()
    {
      public void actionPerformed(ActionEvent e)
      {
				search.setVisible(true);
      }
    });
		itemReset.addActionListener(new ActionListener()
    {
      public void actionPerformed(ActionEvent e)
      {
				int i;
				for (i=0;i<aantal_plase;i++)
				{
					itemPlase[i].setEnabled(true);
				}
				search.clearTicks();
      }
    });

		for (int i=0;i<aantal_plase;i++)
		{
			itemPlase[i].addActionListener(new ActionListener()
    	{
      	public void actionPerformed(ActionEvent e)
      	{
							gp_ID = queries.getPlaasId(queries.selected);
							queries.vollePlaasInfo(gp_ID,t);
							itemReset.setEnabled(true);
							itemSearch.setEnabled(true);
							itemMake.setEnabled(true);
							contentPane.setVisible(true);
							viewPane.setVisible(false);
      	}
    	});
		}
		b1.addActionListener(new ActionListener()
    {
      public void actionPerformed(ActionEvent e)
      {
				wysBesprekings();
				main.repaint(0,0,0,640,480);
      }
    });

		b2.addActionListener(new ActionListener()
		{
			public void actionPerformed(ActionEvent e)
			{
				if (poefter)
				{
					try
					{
						int num = l_lab.getSelectedIndex()+1;
						if ((num <= count) && (num > 0))
						{
							java.util.Date today = new java.util.Date();
							today.setMonth(today.getMonth()+1);

							if (dates[num-1].compareTo(today) > 0)
							{
								remove(nr[num-1]);
							}else
							{
								JOptionPane.showMessageDialog(null, "Too late to cancel booking","Error", JOptionPane.ERROR_MESSAGE);
							}
						}
					}catch (NumberFormatException nfe)
					{}
				}
				repaint();
			}
		});

		b3.addActionListener(new ActionListener()
    {
      public void actionPerformed(ActionEvent e)
      {
				contentPane.setVisible(true);
				viewPane.setVisible(false);
				besprekings.setText("");
				i_id.setText("");
				l_lab.removeAllItems();
				itemReset.setEnabled(true);
				itemSearch.setEnabled(true);
				itemMake.setEnabled(true);
      }
    });
  }

//===========================================================================
public void changeMenu(boolean[] plase)
{
	gp_ID = 0;
	for (int i=0;i<plase.length;i++)
	{

		if (plase[i])
		{
			itemPlase[i].setEnabled(false);
		}else
		{
			itemPlase[i].setEnabled(true);
			if (gp_ID == 0)
				gp_ID = queries.getPlaasId(itemPlase[i].getText());
		}
	}
	queries.vollePlaasInfo(gp_ID,t);
}
//===========================================================================
private void makeMenu()
{
	itemPlase = new JMenuItem[aantal_plase];
	//create menu
	menubar = new JMenuBar();
	setJMenuBar(menubar);
	menubar.setBackground(Color.white);

	//create submenus and items
	m_plase = new JMenu("Guesthouses");
	menubar.add(m_plase);
	m_plase.setBackground(Color.white);
	queries.fillMenu(m_plase,itemPlase);

	m_book = new JMenu("Bookings");
	menubar.add(m_book);
	m_book.setBackground(Color.white);
	itemMake = new JMenuItem("Make Booking");
	m_book.add(itemMake);
	itemMake.setBackground(Color.white);
	itemEdit = new JMenuItem("View/Cancel Bookings");
	m_book.add(itemEdit);
	itemEdit.setBackground(Color.white);

	m_search = new JMenu("Search");
	menubar.add(m_search);
	m_search.setBackground(Color.white);
	itemSearch = new JMenuItem("Specify Filter");
	m_search.add(itemSearch);
	itemSearch.setBackground(Color.white);
	itemReset = new JMenuItem("Reset Search");
	m_search.add(itemReset);
	itemReset.setBackground(Color.white);
}
//===========================================================================
private void contentPane()
{
		contentPane = new JPanel(null);
		contentPane.setBounds(120,50,340,400);

		f = new Font(null,1,24);
    t = new JLabel[9];

	    //create content
    t[0] = new JLabel();
    t[1] = new JLabel();
    t[2] = new JLabel("Nearby Cities/Towns:");
    t[3] = new JLabel();
    t[4] = new JLabel();
    t[5] = new JLabel("Activities: ");
    t[6] = new JLabel();
    t[7] = new JLabel("<html>Accommodation<br>Facilities: ");
    t[8] = new JLabel();
		img = new JLabel();
		back = new JLabel();

    //and add to window

    for (int i=0;i<t.length;i++)
    {
      contentPane.add(t[i]);
    }

		contentPane.add(back);


    //pre-setup
    t[0].setFont(f);
		t[6].setVerticalAlignment(1);
		t[8].setVerticalAlignment(1);
    queries.vollePlaasInfo(gp_ID,t);

    //neem borders en titlebar in ag
    Insets insets = contentPane.getInsets();

    //sit ons content op die regte posisie
    t[0].setBounds(75 + insets.left, 20 + insets.top, 240, 30);
    t[1].setBounds(50 + insets.left, 60 + insets.top, 160, 20);
    t[2].setBounds(50 + insets.left, 100 + insets.top, 160 , 20);
    t[3].setBounds(210 + insets.left, 100 + insets.top, 160, 20);
    t[4].setBounds(210 + insets.left, 125 + insets.top, 160, 20);
    t[5].setBounds(50 + insets.left, 200 + insets.top, 160, 20);
    t[6].setBounds(210 + insets.left, 200 + insets.top, 160, 140);
    t[7].setBounds(50 + insets.left, 300 + insets.top, 160, 40);
    t[8].setBounds(210 + insets.left, 300 + insets.top, 160, 140);

		back.setBounds(0,0,340,400);
		img.setBounds(0,0,640,480);
		img.setIcon(images[image_index]);

		etched = BorderFactory.createEtchedBorder();


		Color col = new Color(255,255,255,97);
		back.setBorder(etched);
		contentPane.setOpaque(true);
		contentPane.setBackground(col);

}
//===========================================================================
private void viewPane()
{
		viewPane = new JPanel(null);
		viewPane.setSize(640,480);

		head = new JLabel("View/Cancel Bookings");
		b1 = new JButton("Show");
		id = new JLabel("ID no");
		i_id = new JTextField(13);
		besprekings = new JLabel();
		lab = new JLabel("<html><font color=blue><b>Booking no. to cancel</font>");
		b2 = new JButton("Cancel Booking");
		b3 = new JButton("Back");
		l_lab = new JComboBox();

		viewPane.add(head);
		viewPane.add(b1);
		viewPane.add(id);
		viewPane.add(i_id);
		viewPane.add(besprekings);
		viewPane.add(lab);
		viewPane.add(l_lab);
		viewPane.add(b2);
		viewPane.add(b3);

		head.setBounds(150,15,400,30);
		b1.setBounds(200,60,80,20);
		id.setBounds(5,60,34,20);
		i_id.setBounds(50,60,110,20);
		besprekings.setBounds(5,90,630,360);
		lab.setBounds(5,430,200,20);
		l_lab.setBounds(145,430,40,20);
		b2.setBounds(200,430,140,20);
		b3.setBounds(360,430,80,20);

		head.setFont(f);
		besprekings.setVerticalAlignment(1);

		Color col = new Color(255,255,255,98);

		viewPane.setOpaque(true);
		viewPane.setBackground(col);
		l_lab.setBackground(Color.white);
}
//=================================================================
private void wysBesprekings()
{
	poefter = false;
	try
	{
		l_lab.removeAllItems();
		count = 0;
		long id = Long.valueOf(i_id.getText()).longValue();
		String b_id = "";
		int bcount = 0;
		ResultSet rs = sqlCon.makeQuery("select count(b_id) from bespreking where k_id = '"+id+"'");
		try
		{
			if (rs.next())
			{
				count = rs.getInt("count");
			}
			if (count > 0)
			{
				nr = new String[count];
				dates = new Date[count];
				count = 0;
				besprekings.setText("<html>");
				besprekings.setVisible(false);
				rs = sqlCon.makeQuery("Select b_id,gp.gp_naam,f_tipe,s_aantal,b_begin,b_eindig from fasiliteite f,gasteplaas gp, (Select s.b_id,f_id,s_aantal,b_begin,b_eindig,gp_id from slaap_plek s, (select b_id,b_begin,b_eindig,gp_id from bespreking where k_id = '"+id+"') bs where s.b_id = bs.b_id ) sp where sp.f_id = f.f_id AND gp.gp_id = sp.gp_id");
				while (rs.next())
				{
					if (bcount == 0)
					{
						bcount = 1;
						besprekings.setText(besprekings.getText()+"<font color=blue>Booking no."+1+"</font><br>");
						b_id = rs.getString("b_id");
						nr[count] = b_id;
						dates[count] = rs.getDate("b_begin");
      			l_lab.addItem(String.valueOf(bcount));
						count++;
					}else
					{
						if (!b_id.equals(rs.getString("b_id")))
						{
							bcount++;
							besprekings.setText(besprekings.getText()+"<br><font color=blue>Booking no."+bcount+"</font><br>");
							b_id = rs.getString("b_id");
							nr[count] = b_id;
							dates[count] = rs.getDate("b_eindig");
							l_lab.addItem(String.valueOf(bcount));
							count++;
						}
					}
					besprekings.setText(besprekings.getText()+rs.getDate("b_begin")+" from "+rs.getDate("b_eindig")+" at "+rs.getString("gp_naam")+" for "+rs.getInt("s_aantal")+"  ["+rs.getString("f_tipe")+"] accommodation<br>");
				}
				besprekings.setVisible(true);
				poefter = true;
			}else
			{
				besprekings.setText("No bookings");
			}
		}catch (SQLException sqe)
		{}
	}catch (NumberFormatException nfe)
	{}
}
//=================================================================
	private void remove(String b_id)
	{
		ResultSet rs = sqlCon.makeQuery("delete from slaap_plek where b_id = '"+b_id+"'; delete from bespreking where b_id = '"+b_id+"'");
		wysBesprekings();
	}
//===========================================================================
public void actionPerformed(ActionEvent e)
{
   //Advance the animation frame.
	image_index++;
	if (image_index == images.length)
	{
		image_index =0;
	}
	img.setIcon(images[image_index]);
}
		  //Can be invoked by any thread (since timer is thread-safe).
    public void startAnimation() {

            //Start animating!
            if (!timer.isRunning()) {
                timer.start();
            }

    }

    //Can be invoked by any thread (since timer is thread-safe).
    public void stopAnimation() {
        //Stop the animating thread.
        if (timer.isRunning()) {
            timer.stop();
        }
    }
//===========================================================================
  public static void setup()
  {
    //maak 'n nuwe window
    window myWindow = new window();
    Insets insets = myWindow.getInsets();

    //stel sy title en grootte
    myWindow.setTitle("RW 242: Databasis projek");
    myWindow.setSize(640 + insets.left + insets.right, 540 + insets.top + insets.bottom);
    myWindow.setResizable(false);
    //center on screen
    myWindow.setLocationRelativeTo(null);
    //wys dit
    myWindow.setVisible(true);
		myWindow.startAnimation();
  }
}
