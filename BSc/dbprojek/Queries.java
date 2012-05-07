import javax.swing.*;
import java.sql.*;
import java.awt.*;

public class Queries
{
  private int gp_aantal;
  private int f_aantal;
	private int akw_aantal;
	private plaas[] plase;
	public static String selected;
	private SQLconnect sqlCon;
	private String[] akti,fasi;
//====================================================================
  public Queries(SQLconnect sqlCon)
  {
		this.sqlCon = sqlCon;
    gp_aantal = stoorAantalPlase();
    f_aantal = stoorAantalFasiliteite();
		akw_aantal = stoorAantalAktiwiteite();

		plase = new plaas[gp_aantal];
		for (int i=0;i<gp_aantal;i++)
		{
			plase[i] = new plaas();
		}
		stoorVollePlaasInfo();
  }
//====================================================================
  public int aantalPlase()
  {
    return gp_aantal;
  }
//====================================================================
	public int aantalAktiwiteite()
  {
    return akw_aantal;
  }
//====================================================================
  public int aantalFasiliteite()
  {
    return f_aantal;
  }
//====================================================================
  private int stoorAantalPlase()
  {
    int count = 0;
    try
    {
      ResultSet rs = sqlCon.makeQuery("Select count(*) from Gasteplaas");
      try
      {
        if (rs.next())
      {
        count = rs.getInt("count");
      }
      }catch (SQLException sqe)
      {}
    }catch (NullPointerException npe)
    {System.exit(0);}


    return count;
  }
//====================================================================
  private int stoorAantalAktiwiteite()
  {
    int count = 0;
    try
    {
      ResultSet rs = sqlCon.makeQuery("Select count(*) from tipe_aktiwiteite");
      try
      {
        if (rs.next())
      	{
        	count = rs.getInt("count");
      	}
				akti = new String[count];
				rs  =sqlCon.makeQuery("Select * from tipe_aktiwiteite");

        while (rs.next())
        {
          akti[rs.getInt("akw_id")-1] = rs.getString("akw_tipe");
        }
      }catch (SQLException sqe)
      {}
    }catch (NullPointerException npe)
    {System.exit(0);}


    return count;
  }
//====================================================================
  private int stoorAantalFasiliteite()
  {
    int count = 0;
    try
    {
      try
      {
				ResultSet rs = sqlCon.makeQuery("Select count(*) from fasiliteite");
        if (rs.next())
        {
          count = rs.getInt("count");

        }
				fasi = new String[count];
				rs  =sqlCon.makeQuery("Select * from fasiliteite");

        while (rs.next())
        {
          fasi[rs.getInt("f_id")-1] = rs.getString("f_tipe");
        }
			}catch (SQLException sqe)
      {System.out.println("ERROR");}
    }catch (NullPointerException npe)
    {System.out.println("ERROR");}


    return count;
  }

//====================================================================
  public void fillMenu(JMenu menu,JMenuItem[] m_plase)
  {
			for (int i=0;i<gp_aantal;i++)
      {
				m_plase[i] = new JMenuItem(plase[i].naam())
				{
				 	// This method is called whenever the selection status of
        	// this menu item is changed
        	public void menuSelectionChanged(boolean isSelected)
					{
          	// Always forward the event
            super.menuSelectionChanged(isSelected);

						if (isSelected)
						{

							// Get the selected menu path
    					MenuElement[] path = MenuSelectionManager.defaultManager().getSelectedPath();

    					// Retrieve the labels of all the menu elements in the path
    					for (int i=0; i<path.length; i++)
							{
        				Component c = path[i].getComponent();

        				if (c instanceof JMenuItem)
								{
            			JMenuItem mi = (JMenuItem)c;
									selected = mi.getText();
            			// Note: JMenu is a subclass of JMenuItem; also JMenuBar does not have a label
        				}
								//selected = rs.getInt("gp_id");
							}
            }
					}
				};
				menu.add(m_plase[i]);
				m_plase[i].setBackground(Color.white);
      }
  }
//====================================================================
  public String getPlaasNaam(int gp_ID)
  {
    String naam = "";

		for (int i=0;i<gp_aantal;i++)
		{
			if (plase[i].id() == gp_ID)
			{
				naam = plase[i].naam();
			}
		}
    return naam;
  }
//====================================================================
	 public int getPlaasId(String gp_naam)
  {
    int id = 0;

		for (int i=0;i<gp_aantal;i++)
		{
			if (plase[i].naam().equals(gp_naam))
			{
				id = plase[i].id();
			}
		}

    return id;
  }

//====================================================================
  public void kryVerblyfInfo(int gp_id,JLabel[] teks,JTextField[] i_teks,int[] verblyf_id)
  {
		int i;
		int index=0;
		for (i=0;i<f_aantal;i++)
    {
			teks[i].setText("");
			i_teks[i].setText("0");
			verblyf_id[i] = 0;
			i_teks[i].setVisible(false);
		}

		for (i=0;i<gp_aantal;i++)
		{
			if (plase[i].id() == gp_id)
			{
				index = i;
			}
		}
		int j=0;
    for (i=0;i<f_aantal;i++)
    {
			if (plase[index].verblyf(i))
			{
				teks[j].setText(fasi[i]);
				i_teks[j].setVisible(true);
				verblyf_id[j] = i+1;
				j++;
			}
    }
  }
//====================================================================
  public void vollePlaasInfo(int gp_id,JLabel[] t)
  {
		int i=0;
		int index=0;
		for (i=0;i<gp_aantal;i++)
		{
			if (plase[i].id() == gp_id)
			{
				index = i;
			}
		}

		for (i=0;i<t.length;i++)
		{
			t[i].setText("");
		}
     // if (rs.next())
     // {}
		if (gp_id != 0)
		{
			t[2].setText("Nearby Cities/Towns:");
			t[5].setText("Activities: ");
			t[7].setText("<html>Accommodation<br>Facilities: ");
		}

    //kry die plaas naam en Provinsie
    t[0].setText(plase[index].naam());
    t[1].setText("Province: "+plase[index].provinsie());


    //kry die aanliggende dorpe en hulle afstande
		for (i=3;i<3+plase[index].countDorp();i++)
		{
			t[i].setText(plase[index].dorp(1+i-3) + " " + plase[index].afstand(1+i-3)+"km");
		}

    //kry die lys van aktiwiteite wat aangebied word
    t[6].setText(plase[index].aktiwiteite(akti));


    //kry die verblyf fasiliteite wat beskikbaar is
		t[8].setText(plase[index].fasiliteite(fasi));
  }
	//====================================================================
  public void stoorVollePlaasInfo()
  {
		int i = 0;
		int count = 0;
    //kry die plaas naam en Provinsie
    ResultSet rs = sqlCon.makeQuery("SELECT * FROM Gasteplaas");
    try
    {
      while (rs.next())
      {
				plase[i].setFirst(akw_aantal,f_aantal,rs.getInt("gp_id"),rs.getString("gp_naam"), rs.getString("gp_provinsie"));
				i++;
      }
    }catch (SQLException sqe)
    {}

		rs = sqlCon.makeQuery("Select count(d_Naam) from Dorp");
		try
		{
			if (rs.next())
				count = rs.getInt("count");
		}catch (SQLException sqe)
		{}
		int b;
		String[] pnaam = new String[count];
		int[] afstand = new int[count];
		{
			 //kry die aanliggende dorpe en hulle afstande
    	rs = sqlCon.makeQuery("Select * from Dorp order by gp_id");
    	try
    	{
				i=0;
				rs.next();
      	while (i<gp_aantal)
      	{
					b=0;
					while ((rs.getInt("gp_id") == plase[i].id()) && !(rs.isLast()))
					{
						pnaam[b] = rs.getString("d_Naam");
						afstand[b] = rs.getInt("d_afstand");
						rs.next();
						b++;
					}
					if (rs.isLast())
					{
						pnaam[b] = rs.getString("d_Naam");
						afstand[b] = rs.getInt("d_afstand");
					}

     			plase[i].setSecond(b,pnaam,afstand);

					i++;
      	}
    	}catch (SQLException sqe)
    	{}
		}
		boolean[] akw = new boolean[akw_aantal];
    //kry die lys van aktiwiteite wat aangebied word
    rs = sqlCon.makeQuery("SELECT * FROM Aktiwiteite order by gp_id");
    try
    {
			i=0;
      	rs.next();
      	while (i<gp_aantal)
      	{
					for (int j=0;j<akw_aantal;j++)
					{
						akw[j] = false;
					}
					while ((rs.getInt("gp_id") == plase[i].id()) && !(rs.isLast()))
					{
						akw[rs.getInt("akw_id")-1] = true;
						rs.next();
					}
					if (rs.isLast())
					{
						akw[rs.getInt("akw_id")-1] = true;
					}
					plase[i].setThird(akw);
					i++;
      	}
    }catch (SQLException sqe)
    {}
    //kry die verblyf fasiliteite wat beskikbaar is
		akw = new boolean[f_aantal];
    rs = sqlCon.makeQuery("SELECT gp_id,f_id FROM Verblyf_Fasiliteite order by gp_id");
    try
    {
			i=0;
      	rs.next();
      	while (i<gp_aantal)
      	{
					b=0;
					for (int j=0;j<f_aantal;j++)
					{
						akw[j] = false;
					}
					while ((rs.getInt("gp_id") == plase[i].id()) && !(rs.isLast()))
					{
						akw[rs.getInt("f_id")-1] = true;
						rs.next();
						b++;
					}
					if (rs.isLast())
					{
						akw[rs.getInt("f_id")-1] = true;
					}
					plase[i].setFourth(akw);
					i++;
      	}
    }catch (SQLException sqe)
    {}

  }
//====================================================================
  public String volgende_Bnr()
  {
  	int bnr;
        String s_bnr = "B001";
        char[] out = new char[4];
	ResultSet rs = sqlCon.makeQuery("Select max(b_id) from Bespreking");
        try
        {
	try
        {
        	if ((rs.next()) && (rs.getString("max") != null))
                {
                	s_bnr = rs.getString("max");
                        if (s_bnr.charAt(3) == '9')
                        {
                        	out[3] = '0';
                                if (s_bnr.charAt(2) == '9')
                                {
                                	out[2] = '0';
                                        if (s_bnr.charAt(1) == '9')
                                        {
                                        	out[1] = '0';
                                        }else
                                        {
                                        	bnr = Integer.valueOf(String.valueOf(s_bnr.charAt(1))).intValue() + 1;
                                                out[1] = (String.valueOf(bnr)).charAt(0);
                                        }
                                }else
                                {
                                	out[1] = s_bnr.charAt(1);
                                	bnr = Integer.valueOf(String.valueOf(s_bnr.charAt(2))).intValue() + 1;
                                        out[2] = (String.valueOf(bnr)).charAt(0);
                                }
                        }else
                        {
                        	out[1] = s_bnr.charAt(1);
                                out[2] = s_bnr.charAt(2);
                        	bnr = Integer.valueOf(String.valueOf(s_bnr.charAt(3))).intValue() + 1;
                                out[3] = (String.valueOf(bnr)).charAt(0);
                        }
                        out[0] = 'B';
                        s_bnr = String.valueOf(out);
                }
        }catch (SQLException sqe)
        {}
        }catch (NullPointerException npe)
        {s_bnr = "B001";}
	return s_bnr;
  }
}
