
public class Driver
{
	static int[] a = {2,7,8,15,13,20,101};
	private static void printMsg()
	{
		System.out.println("Hello World");
	}

	Driver()
	{
	}

	public static void main(String[] args) throws Exception
	{
		Driver D = new Driver();
		Simple.createSimple().printSimple();
		Simple.printMsg();
		D.printMsg();
		Simple.printArray(a);
	}

}
