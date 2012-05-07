import javax.swing.*;
import javax.swing.event.*;
import javax.swing.text.*;
import java.awt.*;
import java.awt.event.*;
import java.sql.*;

public class Search extends JFrame
{
	private JLabel head;
	private JCheckBox[] akw;
	private int[] id;
	private JButton b_search;
	private boolean[] plase;
	private window parent;
	private int akw_aantal,gp_aantal;
	private SQLconnect sqlCon;
	private ResultSet rs;



	public Search(window MyWindow,Queries queries)
	{
		parent = MyWindow;
		sqlCon = new SQLconnect();
		akw_aantal = queries.aantalAktiwiteite();
		gp_aantal = queries.aantalPlase();

		Container contentPane = getContentPane();
		contentPane.setLayout(null);

		contentPane.setBackground(Color.white);

		akw = new JCheckBox[akw_aantal];
		id = new int[akw_aantal];

		head = new JLabel("<html><font color=blue><b>Choose Filter Options");
		b_search = new JButton("Search");
		contentPane.add(head);
		contentPane.add(b_search);
		head.setBounds(80,5,140,20);
		b_search.setBounds(80,60+akw_aantal*25,100,20);

		for (int i=0;i<akw_aantal;i++)
		{
			akw[i] = new JCheckBox();
			contentPane.add(akw[i]);
			akw[i].setBounds(5,40+i*25,280,20);
			akw[i].setBackground(Color.white);
		}

		kryLabels();

		b_search.addActionListener(new ActionListener()
    {
      public void actionPerformed(ActionEvent e)
      {
        String out = kryTicked();

				plase = new boolean[gp_aantal];
				for (int i=0;i<plase.length;i++)
				{
					plase[i]=true;
				}

				int count=0;
				//System.out.println("Select gp_id from Gasteplaas"+out);

    		rs = sqlCon.makeQuery("Select gp_id from Gasteplaas"+out);
    		try
	    	{
					while (rs.next())
    	  	{
						plase[rs.getInt("gp_id")-1] = false;
						count++;
      		}
				}catch (SQLException sqe)
    		{}
				if (count == 0)
				{
					JOptionPane.showMessageDialog(null, "No guesthouses for filter choice","Warning", JOptionPane.ERROR_MESSAGE);
				}else
				{
					parent.changeMenu(plase);
					parent.repaint();
					setVisible(false);
				}
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
	private void kryLabels()
	{
		ResultSet rs = sqlCon.makeQuery("select * from tipe_aktiwiteite");
		try
		{
			int i=0;
			while (rs.next())
			{
				akw[i].setText(rs.getString("akw_tipe"));
				id[i] = rs.getInt("akw_id");
				i++;
			}
		}catch (SQLException sqe)
		{}
	}
	private String kryTicked()
	{
 		String out;
		out = "";
		for (int i=0;i<akw_aantal;i++)
		{
			if (akw[i].isSelected())
			{
				if (out.equals(""))
				{
					out = " where gp_id in ";
				}else
				{
					out = out+" AND gp_id in ";
				}
				out = out+"(select gp_id from aktiwiteite where akw_id = "+id[i]+")";
			}
		}
		return out;
	}
	public void clearTicks()
	{
		for (int i=0;i<akw_aantal;i++)
		{
			akw[i].setSelected(false);
		}
	}
}
