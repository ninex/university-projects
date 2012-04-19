public class plaas
{
	private int gp_id,c_dorp,c_akw,c_fas;
	private String gp_naam,gp_provinsie;
	private int[] afstande;
	private String[] dorpe;
	private boolean[] akw,fas;
//====================================================
	public void setFirst(int c_akw2,int c_fas2,int gp_id2,String gp_naam2,String gp_provinsie2)
	{
		c_akw = c_akw2;
		c_fas = c_fas2;
		gp_id = gp_id2;
		gp_naam = gp_naam2;
		gp_provinsie = gp_provinsie2;
	}
//====================================================
	public void setSecond(int c_dorp2,String[] dorpe2,int[] afstande2)
	{
		c_dorp = c_dorp2;
		dorpe = new String[c_dorp];
		afstande = new int[c_dorp];
		for (int i=0;i<c_dorp;i++)
		{
			dorpe[i] = dorpe2[i];
			afstande[i] = afstande2[i];
		}
	}
//====================================================
	public void setThird(boolean[] akw2)
	{
		akw = new boolean[akw2.length];
		for (int i=0;i<akw2.length;i++)
		{
			akw[i]=akw2[i];
		}
	}
//====================================================
	public void setFourth(boolean[] fas2)
	{
		fas = new boolean[fas2.length];
		for (int i=0;i<fas2.length;i++)
		{
			fas[i]=fas2[i];
		}
	}

//====================================================
	public int id()
	{
		return gp_id;
	}
//====================================================
	public int countDorp()
	{
		return c_dorp;
	}
//====================================================
	public String naam()
	{
		return gp_naam;
	}
//====================================================
	public String provinsie()
	{
		return gp_provinsie;
	}
//====================================================
	public String dorp(int i)
	{
		if ((c_dorp < i) || (c_dorp == 0))
		{
			return "";
		}else
			return dorpe[i-1];
	}
//====================================================
	public int afstand(int i)
	{
		if ((c_dorp < i) || (c_dorp == 0))
		{
			return 0;
		}else
			return afstande[i-1];
	}
//====================================================
	public String aktiwiteite(String[] akti)
	{
  	String out = "<html>";
		for (int i=0;i<akw.length;i++)
		{
			if (akw[i])
			{
				if (out.equals("<html>"))
				{
					out = out + akti[i];
				}else
				{
					out = out + "<br>" + akti[i];
				}
			}
		}
		return out;
	}
//====================================================
	public String fasiliteite(String[] fasi)
	{
  	String out = "<html>";
		for (int i=0;i<fas.length;i++)
		{
			if (fas[i])
			{
				if (out.equals("<html>"))
				{
					out = out + fasi[i];
				}else
				{
					out = out + "<br>" + fasi[i];
				}
			}
		}
		return out;
	}
//=================================================
	public boolean verblyf(int index)
	{
		return fas[index];
	}
}
