{for (i = 0;i < NF;i++) {
	print "echo Changing " $i
	print "gawk -f ../q4_2.awk " $i " > temp"
	print "cat temp > " $i
	}
	print "rm -f temp"}
