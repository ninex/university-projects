public class list
{
	//private list next,prev;
	private String name, addr;
	int listen, call;
	
	
	public list(String name, String Address, int listen, int call)
	{
		this.name = name;
		this.addr = Address;
		this.listen = listen;
		this.call = call;
		//this.next = null;
		//this.prev = null;
	}
	/*public void setNext(list next)
	{
		this.next = next;
	}
	public void setPrev(list prev)
	{
		this.prev = prev;
	}
	public list getNext()
	{
		return next;
	}
	public list getPrev()
	{
		return prev;
	}*/
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
}