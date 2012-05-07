public class Simple
{
	Simple()
	{
	}
	public static void printArray(int[] a)
	{
		int count= 0;
		while (count < a.length)
		{
			System.out.print(a[count]);
			count += 1;
		}
	}
	public static Simple createSimple()
	{
		Simple s = new Simple();
		return s;
	}
	public void printSimple()
	{
		System.out.println("Simple..");
	}
	public static void printMsg() throws Exception
	{
		java.io.BufferedReader stdin;
		System.out.print("Enter an integer");
		stdin = new java.io.BufferedReader(new java.io.InputStreamReader(System.in));
		String s = stdin.readLine();
		if ( s!= null)
		{
			int i = Integer.valueOf(s).intValue();
			System.out.println(i);
		}
	}
}