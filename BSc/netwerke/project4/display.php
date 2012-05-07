<?php
	// Connect to DB server
	$con = mysql_connect("localhost", $_POST['username'], $_POST['password'])
		or die("Could not connect: " . mysql_error());
	// Select the required DB
	mysql_select_db($_POST['dbname'])
		or die("Could not connect: " . mysql_error());
	// Get balance
	$query = "SELECT amount FROM Finances WHERE (fieldId='Balance')";
       	$result = mysql_query($query) or die("Could not perform query: " . mysql_error());
	$balance = mysql_fetch_row($result);
	$balance = $balance[0];
	mysql_free_result($result);	
	
	// Get cursor for appointments
	$query = "SELECT startTime, endTime, friendName, activityId, cost " . 
			"FROM Appointments ORDER BY startTime ASC";
	$result = mysql_query($query) or die("Could not perform query: " . mysql_error()); 
	
?>

<html>
<head>
	<title>SRP Appointment Manager: Your Appointments</title>
</head>

<body>
	<!-- Display line detailing financial plight -->
	<h3>Financial Situation:</h3>
	<p>Your social life will deplete your financial reserves to a sorry R <?php echo $balance; ?>.</p>
	<!-- Display formatted table of appointments -->
	<h3>Here is a list of your appointments:</h3>
	<p>
	<table border=1px>
		<!-- Column headings -->
		<tr>
			<td><b>Start</b></td>
			<td><b>End<b/></td>
			<td><b>Name</b></td>
			<td><b>Activity</b></td>
			<td><b>Cost</b></td>
		</tr>
		
	<?php
		// Fetch result as an indexed array
		// $line = (startTime, endTime, Name, activityId, cost) 
		while ($line = mysql_fetch_array($result))
		{
		
	        	// Get activity name instead of the activityId
			$query = "SELECT name FROM FavouriteActivities WHERE (id= $line[3]);";
			$actResult = mysql_query($query) or die("Could not peform query: " .mysql_error());
			$activity = mysql_fetch_row($actResult);
			$activity = $activity[0];
			mysql_free_result($actResult);	
			
			// Write line to page
			echo "<tr>";
			echo "\t<td>$line[0]</td>";   // write start time
			echo "\t<td>$line[1]</td>";   // write end time
			echo "\t<td>$line[2]</td>";   // write name
			echo "\t<td>$activity</td>";  // write activity
			echo "\t<td>$line[4]</td>";   // write cost
			echo "</tr>";
		}
		
		// Clean up and close connection
		mysql_free_result($result);
		mysql_close($con);
	?>
	</table>
	
	</p>
</body>
</html>
