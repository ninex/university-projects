import java.net.*;
import java.io.*;
import java.security.*;
import java.util.*;

public class validate implements Serializable
{
	public long t1 = 0;
	public long t2 = 0;
	public double q1 = 0;
	public double q2 = 0;
	public byte [] protected1 = null;
	public byte [] protected2 = null;
	public String username = null;
	public PublicKey publicKey = null;
	
	public validate(String user,String password, ObjectOutputStream outStream, PublicKey pubKey)throws IOException, NoSuchAlgorithmException 
	{
		username = user;
		publicKey = pubKey;
		t1 = (new Date()).getTime();
		q1 = Math.random();
		protected1 = Protection.makeDigest(user,password,t1,q1);
		
		t2 = (new Date()).getTime();
		q2 = Math.random();
		protected2 = Protection.makeDigest(protected1,t2,q2);
	}
}