<LINK href="site.css" type=text/css rel=STYLESHEET>
<body>
<?php
	function placeMenuItem($mID)
	{
		$link = mysql_connect('freelinuxhost.com', 'ninex_data', 'xj220');
		if( !$link)
		{
			die('error connecting to database');
		}
		mysql_select_db('ninex_data') or die('Could not select database');
		
		$query = "SELECT * FROM Menu WHERE menuID = '$mID'";
		$result = mysql_query($query);
		if (!$result)
		{
			die("Query failed : " . mysql_error());
		}else
		{
			$row = mysql_fetch_row($result);
			if ($row[0] == $mID)
			{
				echo "<a class=navref href='$row[2]' target = 'content'>$row[1]</a>";
			}else
			{
				return false;
			}
		}
		mysql_close($link);
		return true;
	}
	function placeAllMenuItems()
	{
		$counter = 0;
		/*while (placeMenuItem($counter))
		{
			$counter++;
		}*/
		echo "<a class=navref href='home.php' target = 'content'>Home</a>";
		echo "<a class=navref href='desc.php' target = 'content'>Description</a>";
		echo "<a class=navref href='sched.php' target = 'content'>Schedule</a>";
		echo "<a class=navref href='files.php' target = 'content'>Files</a>";
		echo "<a class=navref href='about.php' target = 'content'>Info</a>";
	}

	placeAllMenuItems();
?>
</body>