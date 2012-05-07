public class Gnome
{
	private String name;
	private int age;
	private Gnome buddy;
	private int height;
	private boolean magical;
	
	public static void makeKing(Gnome h)
	{
		Gnome k = new Gnome("King" + h.name,h.age, null, h.height);
		h=k;
		k.magical = true;
	}
	Gnome()
	{
	}
	Gnome(String Gname, int Gage, Gnome g, int Gheight)
	{
		name = Gname;
		age = Gage;
		buddy = g;
		height = Gheight;
	}
}