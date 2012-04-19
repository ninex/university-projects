public class Part2_4
{
	public static int GCD(int n, int m)
	{
		int temp;
		if (n > m)
		{
			temp = m;
			m=n;
			n=temp;
		}
		while ( (temp = n % m) != 0)
		{
			n = m;
			m = temp;
		}
		return m;
	}
}