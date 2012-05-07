public class list
{
	//private list next,prev;
	private String name, addr;
	int listen, call;
	boolean redirect;
	
	public list(String name, String Address, int listen, int call,boolean redirect)
	{
		this.name = name;
		this.addr = Address;
		this.listen = listen;
		this.call = call;
		this.redirect = redirect;
	}
	
	public String rName()
	{
		return name;
	}
	public String rAddress()
	{
		return addr;
	}
	public int rListen()
	{
		return listen;
	}
	public int rCall()
	{
		return call;
	}
	public boolean rRedir()
	{
		return redirect;
	}
}