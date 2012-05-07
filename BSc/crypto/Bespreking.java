import javax.swing.*;
import javax.swing.text.*;
import javax.swing.event.*;
import java.awt.*;
import java.awt.event.*;
import java.sql.*;

public class Bespreking extends JFrame
{
  private Queries queries;  
  private int gp_ID,f_count;
  private JLabel header,naam,id,woonadres,posadres,epos,tel,tel2,begin_datum,eind_datum;
  private JLabel[] verblyf;
  private JTextField i_naam,i_id,i_woonadres,i_posadres,i_epos,i_tel,i_tel2;
  private JTextField[] i_verblyf;
  private JButton bespreek,cancel;
  private JComboBox i_dd,i_mm,i_yy,i_dd2,i_mm2,i_yy2;
  private java.sql.Date begin,einde;
  private int[] verblyf_id;
	private String error;
	private SQLconnect sqlCon;
	private window parent;

  public Bespreking(window MyWindow,SQLconnect sqlCon,Queries queries)
  {
		parent = MyWindow;
		this.sqlCon = sqlCon;
    this.queries = queries;
		f_count = queries.aantalFasiliteite();
    Font f = new Font(null,1,16);
    verblyf = new JLabel[f_count];
    i_verblyf = new JTextField[f_count];
    verblyf_id = new int[f_count];
    Container contentPane = getContentPane();
    contentPane.setLayout(null);
		contentPane.setBackground(Color.white);


    //create content
    header = new JLabel();
    naam = new JLabel("Name");
    id = new JLabel("ID no");
    woonadres = new JLabel("Address");
    posadres = new JLabel("Post Address");
    epos = new JLabel("Email");
    tel = new JLabel("Telephone no");
		tel2 = new JLabel("Mobile no");
    begin_datum = new JLabel("Arrival");
    eind_datum = new JLabel("Departure");


    i_naam = new JTextField(300);
    i_id = new JTextField(300);
    i_woonadres = new JTextField(300);
    i_posadres = new JTextField(300);
    i_epos = new JTextField(300);
    i_tel = new JTextField(300);
		i_tel2 = new JTextField(300);

    String[] day = { "1","2","3","4","5","6","7","8","9","10","11","12","13","14","15","16","17","18","19","20","21","22","23","24","25","26","27","28","29","30","31"};
    String[] month = { "Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
    String[] year = {"2004","2005","2006","2007","2008","2009","2010","2011","2012","2013","2014"};
    i_dd = new JComboBox(day);
    i_mm = new JComboBox(month);
    i_yy = new JComboBox(year);
    i_dd2 = new JComboBox(day);
    i_mm2 = new JComboBox(month);
    i_yy2 = new JComboBox(year);

    bespreek = new JButton("Book");
		cancel = new JButton("Cancel");

		i_dd.setBackground(Color.white);
		i_mm.setBackground(Color.white);
		i_yy.setBackground(Color.white);
		i_dd2.setBackground(Color.white);
		i_mm2.setBackground(Color.white);
		i_yy2.setBackground(Color.white);

    //and add to window
    contentPane.add(header);
    contentPane.add(naam);
    contentPane.add(id);
    contentPane.add(woonadres);
    contentPane.add(posadres);
    contentPane.add(epos);
    contentPane.add(tel);
		contentPane.add(tel2);
    contentPane.add(begin_datum);
    contentPane.add(eind_datum);
    contentPane.add(i_naam);
    contentPane.add(i_id);
    contentPane.add(i_woonadres);
    contentPane.add(i_posadres);
    contentPane.add(i_epos);
    contentPane.add(i_tel);
		contentPane.add(i_tel2);
    contentPane.add(i_dd);
    contentPane.add(i_mm);
    contentPane.add(i_yy);
    contentPane.add(i_dd2);
    contentPane.add(i_mm2);
    contentPane.add(i_yy2);
    contentPane.add(bespreek);
		contentPane.add(cancel);

    //pre-setup

    header.setFont(f);

    //sit ons content op die regte posisie
		header.setBounds(80,5,340,25);
    naam.setBounds(5, 30, 240, 20);
    id.setBounds(5, 50, 240, 20);
    woonadres.setBounds(5, 70, 240, 20);
    posadres.setBounds(5, 90, 240, 20);
    epos.setBounds(5, 110, 240, 20);
    tel.setBounds(5, 130, 240, 20);
		tel2.setBounds(5, 150, 240, 20);
    begin_datum.setBounds(5, 180, 240, 20);
    eind_datum.setBounds(5, 210, 240, 20);

    i_naam.setBounds(120,30,240,20);
    i_id.setBounds(120, 50, 240, 20);
    i_woonadres.setBounds(120, 70, 240, 20);
    i_posadres.setBounds(120, 90, 240, 20);
    i_epos.setBounds(120, 110, 240, 20);
    i_tel.setBounds(120, 130, 240, 20);
		i_tel2.setBounds(120, 150, 240, 20);
    i_dd.setBounds(120,180,50,20);
    i_mm.setBounds(170,180,54,20);
    i_yy.setBounds(224,180,60,20);
    i_dd2.setBounds(120,210,50,20);
    i_mm2.setBounds(170,210,54,20);
    i_yy2.setBounds(224,210,60,20);

    bespreek.setBounds(280,340,100,25);
		cancel.setBounds(180,340,100,25);

    for (int i=0;i< f_count;i++)
    {
      verblyf[i] = new JLabel();
      contentPane.add(verblyf[i]);
      verblyf[i].setBounds(5,240+(i*20),240,20);
      i_verblyf[i] = new JTextField("0",1);
      contentPane.add(i_verblyf[i]);
      i_verblyf[i].setBounds(120,240+(i*20),30,20);
    }

	bespreek.addActionListener(new ActionListener()
  {
  	public void actionPerformed(ActionEvent e)
    {
    	//check of alles ingevul is
      error = "Please fill in ";
      if (i_naam.getText().equals("") || i_naam.getText().equals("*required"))
      {
        error = error + "name";
        i_naam.setText("*required");
      }
			if (i_naam.getText().length() > 30)
      {
        if (!(error.equals("Please fill in ")))
        {
          error = error +" ,";
        }
        error = error  + "shorter name";
      }
      if (i_id.getText().equals("")|| i_id.getText().equals("*required"))
      {
        if (!(error.equals("Please fill in ")))
        {
          error = error +" ,";
        }
        error = error  + "id";
        i_id.setText("*required");
      }
			if ((i_id.getText().length() != 13) || i_id.getText().equals("*required"))
      {
        if (!(error.equals("Please fill in ")))
        {
          error = error +" ,";
        }
        error = error  + "correct id length";
      }
      if (i_woonadres.getText().equals("")|| i_woonadres.getText().equals("*required"))
      {
        if (!(error.equals("Please fill in ")))
        {
          error = error +" ,";
        }
        error = error  + "address";
        i_woonadres.setText("*required");
      }
			if (i_woonadres.getText().length() > 50)
      {
        if (!(error.equals("Please fill in ")))
        {
          error = error +" ,";
        }
        error = error  + "address shorter than 50 characters";
      }
			if (i_posadres.getText().length() > 50)
      {
        if (!(error.equals("Please fill in ")))
        {
          error = error +" ,";
        }
        error = error  + "post address shorter than 50 characters";
      }
      if (i_tel.getText().equals("")|| i_tel.getText().equals("*required"))
      {
        if (!(error.equals("Please fill in ")))
        {
          error = error +" ,";
        }
        error = error  + "telephone no";
        i_tel.setText("*required");
      }
			if (i_tel.getText().length() > 18)
      {
        if (!(error.equals("Please fill in ")))
        {
          error = error +" ,";
        }
        error = error  + "incorrect telephone no";
      }
			if (i_tel2.getText().length() > 18)
      {
        if (!(error.equals("Please fill in ")))
        {
          error = error +" ,";
        }
        error = error  + "incorrect mobile no";
      }
      //check of korrekte datums ingevul is
      if (
      ((i_mm.getSelectedItem().equals("Apr")) && (i_dd.getSelectedItem().equals("31")) )||
      ((i_mm.getSelectedItem().equals("Jun")) && (i_dd.getSelectedItem().equals("31")) )||
      ((i_mm.getSelectedItem().equals("Sep")) && (i_dd.getSelectedItem().equals("31")) )||
      ((i_mm.getSelectedItem().equals("Nov")) && (i_dd.getSelectedItem().equals("31")) )||
      ((i_mm2.getSelectedItem().equals("Apr")) && (i_dd2.getSelectedItem().equals("31")) )||
      ((i_mm2.getSelectedItem().equals("Jun")) && (i_dd2.getSelectedItem().equals("31")) )||
      ((i_mm2.getSelectedItem().equals("Sep")) && (i_dd2.getSelectedItem().equals("31")) )||
      ((i_mm2.getSelectedItem().equals("Nov")) && (i_dd2.getSelectedItem().equals("31")) )
      )
      {
        if (!(error.equals("Please fill in ")))
        {
          error = error +" ,";
        }
        error = error  + "correct dates";
      }
      if (
      (( Integer.valueOf(i_yy.getSelectedItem().toString()).intValue() % 4 == 0) &&
      (i_mm.getSelectedItem().equals("Feb")) &&
      (Integer.valueOf(i_dd.getSelectedItem().toString()).intValue() > 29)) ||
      ((Integer.valueOf(i_yy.getSelectedItem().toString()).intValue() % 4 != 0) &&
      (i_mm.getSelectedItem().equals("Feb")) &&
      (Integer.valueOf(i_dd.getSelectedItem().toString()).intValue() > 28))
      ||
      (( Integer.valueOf(i_yy2.getSelectedItem().toString()).intValue() % 4 == 0) &&
      (i_mm2.getSelectedItem().equals("Feb")) &&
      (Integer.valueOf(i_dd2.getSelectedItem().toString()).intValue() > 29)) ||
      ((Integer.valueOf(i_yy2.getSelectedItem().toString()).intValue() % 4 != 0) &&
      (i_mm2.getSelectedItem().equals("Feb")) &&
      (Integer.valueOf(i_dd2.getSelectedItem().toString()).intValue() > 28))
      )
      {
        if (!(error.equals("Please fill in ")))
        {
          error = error +" ,";
        }
        error = error  + "correct dates";
      }

      begin = java.sql.Date.valueOf(i_yy.getSelectedItem().toString()+"-"
      	+(i_mm.getSelectedIndex()+1)+"-"+i_dd.getSelectedItem().toString());
      einde = java.sql.Date.valueOf(i_yy2.getSelectedItem().toString()+"-"
      	+(i_mm2.getSelectedIndex()+1)+"-"+i_dd2.getSelectedItem().toString());
			//date ordering reg
      if (einde.compareTo(begin) <= 0)
      {
        if (!(error.equals("Please fill in ")))
        {
          error = error +" ,";
        }
        error = error  + "chronological dates";
      }
			//check iets
			int check = 0;
  		for (int i=0;i<f_count;i++)
	  	{
				try
				{
  				if ( Integer.valueOf(i_verblyf[i].getText()).intValue() == 0)
					{
						check++;
					}
				}catch (NumberFormatException nfe)
				{}
			}
			if (f_count == check)
			{
    		if (!(error.equals("Please fill in ")))
	    	{
  	  		error = error +" ,";
    		}
    		error = error  + "bedding choice";
			}



  		if (error.equals("Please fill in "))
  		{
				if (checkTypes())
				{
					if (isDatumsBeskikbaar())
					{
    				bespreek();
					}else
					{
						JOptionPane.showMessageDialog(null, "Not enough facilities for booking size","Error", JOptionPane.ERROR_MESSAGE);
					}
				}else
				{
					JOptionPane.showMessageDialog(null, "Please input valid values","Error", JOptionPane.ERROR_MESSAGE);
				}
  		}else
  		{
				JOptionPane.showMessageDialog(null, error,"Error", JOptionPane.ERROR_MESSAGE);
  		}
		}
  });
	cancel.addActionListener(new ActionListener()
  {
  	public void actionPerformed(ActionEvent e)
    {
			setVisible(false);
			parent.repaint();
		}
	});
  addWindowListener(new WindowAdapter()
  {
    public void windowClosing(WindowEvent e)
    {
      setVisible(false);
			parent.repaint();
    }
  });
}
//=============================================================================
private void bespreek()
{
  ResultSet rs = null;
  rs = sqlCon.makeQuery("select * from Kontak where k_id = '"+i_id.getText()+"'");
  try
  {
 		if (!rs.next())
    { //nuwe kontakpersoon
	  	rs = sqlCon.makeQuery("insert into Kontak values('"+i_id.getText()
        	+"','"+i_naam.getText()+"','"+i_woonadres.getText()+"','"
	        +i_posadres.getText()+"','"+i_epos.getText()+"','"+i_tel.getText()+"','"+i_tel2.getText()+"');");
    }else
    {
      //update kontak info
    	if (!(
    	(rs.getString("k_naam").equals(i_naam.getText())) &&
      (rs.getString("k_adres").equals(i_woonadres.getText())) &&
      (rs.getString("k_posadres").equals(i_posadres.getText())) &&
      (rs.getString("k_epos").equals(i_epos.getText())) &&
      (rs.getString("k_tel").equals(i_tel.getText())) &&
			(rs.getString("k_tel").equals(i_tel2.getText()))
      ))
    	{
				rs = sqlCon.makeQuery("update Kontak set k_naam = '" +i_naam.getText()+
      		"', k_adres = '"+i_woonadres.getText()+"', k_posadres = '"+i_posadres.getText()+
        	"',k_epos = '"+i_epos.getText()+"',k_tel = '"+i_tel.getText()+"',k_tel2 = '"+i_tel2.getText()+
        	"' where k_id = '"+i_id.getText()+"'");
    	}
		}
    String bnr = queries.volgende_Bnr();

    rs = sqlCon.makeQuery("insert into Bespreking values('"+bnr+"','"+i_id.getText()+"','"
         	+begin+"','"+einde+"','"+gp_ID+"')");

    for (int i=0;i<f_count;i++)
   	{
        	if ( Integer.valueOf(i_verblyf[i].getText()).intValue() != 0 )
                {
                	rs = sqlCon.makeQuery("insert into Slaap_plek values('"+bnr+"',"
                	+verblyf_id[i]+","
                	+Integer.valueOf(i_verblyf[i].getText()).intValue()+")");
                }
	}

    }catch (SQLException sqe)
    {}


    this.setVisible(false);
  }
//=============================================================================
  private boolean isDatumsBeskikbaar()
  {
		ResultSet rs;
		int count;
		int[] aantal = new int[f_count];
		int f_id = 0;

		for (int i=0;i<f_count;i++)
		{
		//System.out.println(f_count);
			count = 0;
			aantal[i] = 0;


			if (Integer.valueOf(i_verblyf[i].getText()).intValue() > 0)
			{
 			rs = sqlCon.makeQuery("Select ball.f_id,sum(ball.s_aantal),max(ball.s_aantal),count(f_id) from (Select f_id,s_aantal,b_begin,b_eindig from slaap_plek sp, (select b_id,b_begin,b_eindig from bespreking where gp_id = "+gp_ID+" AND ((b_begin <= '"+begin+"' AND b_eindig >= '"+begin+"') OR (b_begin >= '"+begin+"' AND b_eindig <= '"+einde+"') OR (b_begin <= '"+einde+"' AND b_eindig >= '"+einde+"') OR (b_begin <= '"+begin+"' AND b_eindig >= '"+einde+"'))) as iets where sp.b_id  = iets.b_id) as ball where f_id = "+verblyf_id[i]+" group by ball.f_id");
				try
				{
					if (rs.next())
					{
						//System.out.println(verblyf_id[i]+" max="+rs.getInt("max") + " sum=" + rs.getInt("sum")+ " count="+rs.getInt("count"));
						count = rs.getInt("count");
						f_id = rs.getInt("f_id");
					}
				}catch (SQLException sqe)
				{}
				java.sql.Date[] eind = new java.sql.Date[count];
				java.sql.Date[] beg = new java.sql.Date[count];
				int[] blaf = new int[count];

				if (count > 0)
				{
					rs = sqlCon.makeQuery("Select f_id,s_aantal,b_begin,b_eindig from slaap_plek sp, (select 						b_id,b_begin,b_eindig from bespreking where gp_id = "+gp_ID+" AND ((b_begin <= '"+begin+"' AND b_eindig >= '"+begin+"')  OR (b_begin >= '"+begin+"' AND b_eindig <= '"+einde+"') OR (b_begin <= '"+einde+"' AND b_eindig >= '"+einde+"') OR (b_begin <= '"+begin+"' AND b_eindig >= '"+einde+"'))) as iets where sp.b_id  = iets.b_id AND sp.f_id = "+verblyf_id[i]);
					try
					{
						int j=0;
						while (rs.next())
						{
							blaf[j] = rs.getInt("s_aantal");
							beg[j] = rs.getDate("b_begin");
							eind[j] = rs.getDate("b_eindig");
							j++;
 							//System.out.println("f_id = "+rs.getInt("f_id")+" "+rs.getDate("b_begin") + " tot " + rs.getDate("b_eindig"));
						}
						aantal[i]=0;
      for (int index=0;index<count;index++)
						{
//System.out.println(aantal[i]);
							if (index == 0)
							{
								aantal[i] = blaf[index];
							}else
							{
								for (j=1;j<=index;j++)
								{
									if (eind[index-j].compareTo(beg[index]) < 0)
									{
										//eind eerste
										aantal[i] = Math.max(aantal[i],blaf[index]);
										//System.out.println("eind eerste= "+aantal[i]+" cmp begin="+beg[index]+" eind= "+eind[index-j]);
									}else
									{
										if ((j!=(count-1)) || ((count)==2))
										{
											aantal[i] = aantal[i] + blaf[index];
										}
											//System.out.println("begin eerste= "+aantal[i]+" cmp begin="+beg[index]+" eind="+eind[index-j]);
									}
								}
							}
						}
					}catch (SQLException sqe)
					{}
					//System.out.println("aantal = " + aantal[i]);
				}
			}
		//check of beskikbaar

		rs = sqlCon.makeQuery("select vf_totaal from verblyf_fasiliteite where gp_id = "+gp_ID+" and f_id = "+verblyf_id[i]);
		try
		{
			if (rs.next())
			{
				if ( (Integer.valueOf(i_verblyf[i].getText()).intValue() + aantal[i]) > rs.getInt("vf_totaal"))
				{
					return false;
				}
			}
		}catch (SQLException sqe)
		{}
		}

		return true;
  }
//=============================================================================
public boolean checkTypes()
{
	long id;
	try
	{
		id = Long.valueOf(i_id.getText()).longValue();
		for (int i=0;i<f_count;i++)
	  {
			Long.valueOf(i_verblyf[i].getText()).longValue();
		}
	}catch (NumberFormatException nfe)
	{return false;}
	return true;
}
//=============================================================================
  public void beginBespreking(int gp_ID)
  {
    this.gp_ID = gp_ID;
  	header.setText("Book at "+ queries.getPlaasNaam(gp_ID));
    i_naam.setText("");
    i_id.setText("");
    i_woonadres.setText("");
    i_posadres.setText("");
    i_epos.setText("");
    i_tel.setText("");
    error = "";
    queries.kryVerblyfInfo(gp_ID,verblyf,i_verblyf,verblyf_id);
  }
}
