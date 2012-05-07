/**
 * AccessLayer.java
 * An abstraction layer for access to the database for project 1 of cs344, 2004
 * @author Toby Louw
 */
import java.util.*;
import java.sql.*;
import javax.sql.*;
public final class AccessLayer {
	
        private static final String driverName = "com.mysql.jdbc.Driver";
        private static Connection con = null;
        private static String dbUrl = null;
        private static String username=null;
        private static String password=null;
        
	/* This is a singleton class */
        
        
        /* Connect to the database */
        private static synchronized void connect() throws Exception {
                // Load driver            
                Class.forName(driverName).newInstance();
                if (dbUrl == null)
                    throw new Exception("You have not specified the database url.");
                else if (username == null)
                    throw new Exception("You have not specified a username.");
                else if (password == null)
                    throw new Exception("You have not specified a password.");
                else if (con == null || con.isClosed())
                    con = DriverManager.getConnection(dbUrl, username, password); 
	}

        /* Disconnect from the database */
	private static synchronized void disconnect() {
            try {
                if (con != null && !con.isClosed())
                    con.close();
            } catch (Exception e) {
                // Do nothing
            }
	}

        
        /**
         * Compares given amount to balance in database
         * @param amount The cost of the activity
         * @return true if amount is smaller than or equal to balance, false
         * otherwise.
         */ 
        public static boolean iCanAffordIt(float amount) throws Exception {
		float balance;
                String query = "SELECT amount FROM Finances WHERE (fieldId='Balance');";
                connect();
                Statement s = con.createStatement();
                ResultSet rs = s.executeQuery(query);
                if (rs.next()) {
                    balance = rs.getFloat(1);
                    rs.close();
                    disconnect();
                }
                else {
                    disconnect();
                    throw new Exception("Database not using original tables.");
                }
                return( balance >= amount );
	}

	/**
         * Checks whether there is already an appointment in the
         * database which takes place any time between the Calender items specified 
         * in start and end. Note that java.util.Date is a timestamp that encompasses 
          * both a date and time. 
         * @param start The date and time of the beginning of this activity.
         * @param end The date and time of the end of this activity.
         * @return If there is an existing appointment, returns true, otherwise false.
         */
        public static synchronized boolean iAmFree(java.util.Date start, 
            java.util.Date end) throws Exception {
            
                boolean result;
            Timestamp ts = new Timestamp(start.getTime());
            Timestamp te = new Timestamp(end.getTime());

            connect();

            String query = "SELECT id FROM Appointments WHERE ";
            query += "(startTime BETWEEN '" + ts.toString() + "' and '" + te.toString()+ "') ";
            query += " OR (endTime BETWEEN '" + ts.toString() + "' and '" + te.toString()+ "')";
            query += " OR (startTime <= '" + ts.toString() + "' AND endTime >= '" 
                + te.toString() + "');";
            Statement s = con.createStatement();
            s.execute(query);
            ResultSet temp = s.getResultSet();
            // See if there was any result returned, if there was I am busy
            result = temp.next();
            temp.close();
            disconnect();
            return(!result);
        }

	
        /** 
         * Checks whether this activity is in the list of favourites
         * @param activity A string describing the activity name
         * @return If the uppercase value of the string in activity matches a
         * value in the FavouriteActivities table of the database, returns true
         * otherwise false.
         */
        public static boolean isFavouriteActivity(String activity) 
            throws Exception {
            boolean result = false;
            activity = activity.toUpperCase();
            String query = "SELECT id FROM FavouriteActivities " + 
                "WHERE (name='" + activity + "');";
            connect();
            Statement s = con.createStatement();
            ResultSet rs = s.executeQuery(query);
            // See if there was any result returned
            result = rs.next();
            disconnect();
            return(result);
 	}
        
        
        /**
         * Record a new appointment in the database and decrease the
         * available balance. NOTE: This procedure does not check for
         * conflicts with other times. Use iAmFree().
         * @param start A timestamp of the beginning of the activity
         * @param end A timestamp of the end of the activity
         * @param name The friend's name
         * @param activity The approved activity. If this activity is not
         * in the favourites list, an exception will be thrown.
         * @param cost The financial cost of the activity
         * @throws java.lang.Exception
         */
        public static synchronized void makeAppointment  (
            java.util.Date start, java.util.Date end,
            String name, String activity, float cost) 
            throws Exception {
            
            int activityId = -1;

            // Find the id of this activity in the database
            activity = activity.toUpperCase();
            String query = "SELECT id FROM FavouriteActivities WHERE (name='" 
                + activity + "');";
            connect();
            Statement s = con.createStatement();
            ResultSet rs = s.executeQuery(query);
            if (rs.next()) {
                activityId = rs.getInt(1);
                rs.close();
            } 
            else {
                disconnect();
                throw new Exception("That activity does not exist!");
            }
            
            // Make new appointments entry
            Timestamp ts = new Timestamp(start.getTime());
            Timestamp te = new Timestamp(end.getTime());
            
            PreparedStatement ps = con.prepareStatement(
                "INSERT INTO Appointments VALUES (0,?,?,?,?,?);");
            ps.setTimestamp(1, ts);
            ps.setTimestamp(2, te);
            ps.setString(3, name );
            ps.setInt(4, activityId);
            ps.setFloat(5, cost);
            ps.execute();
            
            // Decrease the balance
            ps = con.prepareStatement("UPDATE Finances SET amount=amount-? " + 
                "WHERE (fieldId='Balance');");
            ps.setFloat(1,cost);
            ps.execute();
            
            disconnect();
            
          
        }
        
        
        /**
         * Sets the database url.
         * @param url a string of the form "jdbc:mysql://bach.sun.ac.za/s12345678"
         */
        public static void setDatabaseLocation(String url) {
            dbUrl = url;
        }
        
        
        /**
         * Sets the username used when connecting to the database
         * @param s The username
         */
        public static void setUsername(String s) {
            username = s;
        }
        
        
        /**
         * Sets the password used when connecting to the database
         * @param s The password
         */
        public static void setPassword(String s) {
            password = s;
        }

            
}
