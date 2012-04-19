{for (i = 0;i < NF;i++) {
	print "sed -e 's/start/START/g' -i " $i
	}}
