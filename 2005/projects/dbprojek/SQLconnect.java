import java.sql.*;

//class to connect to the database and receive queries
public class SQLconnect
{
  private static final String username = "s13557343";
  private static final String passw = "123a456";
  private static final String url = "jdbc:postgresql://beethoven.sun.ac.za:5432/s13557343";
  private static final String driver = "org.postgresql.Driver";

  private static Statement stmnt;
	private Connection conn;
  private ResultSet rs;

	public SQLconnect()
	{
		conn = null;
    rs = null;
    stmnt = null;

    try
    {
      try //loading driver
      {
        Class.forName(driver);
      }catch (ClassNotFoundException cnf) //no driver found
      {
        System.out.println(cnf.getMessage());
      }
			//get a connection and prepare a statement
      conn = DriverManager.getConnection(url,username,passw);
      stmnt = conn.createStatement();
		}catch (SQLException sqe)
    {//possible errors
        System.out.println("Error "+sqe.getMessage());
    }
	}
//========================================================
  public ResultSet makeQuery(String query)
  {
	  //execute the query
		try
		{
    	if (stmnt.execute(query))
    	{
      	rs = stmnt.getResultSet();
   		}
    }catch (SQLException sqe)
		{System.out.println("database error: "+sqe.getMessage());}

    //return the resultset
    return rs;
  }
//=========================================================================================
public void disconnect()
{
	try
  {
    stmnt.close();
  }catch (SQLException sqe)
  { // ignore }
    stmnt = null;
  }
	//first clear up resources
  try
  {
    if (!conn.isClosed())
		{
			conn.close();
		}
  }catch (SQLException sqe)
  {
    System.out.println("Error" + sqe.getMessage());
  }
}

}
