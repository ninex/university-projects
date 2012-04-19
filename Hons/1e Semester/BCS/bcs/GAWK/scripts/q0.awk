BEGIN {n = 0; id = ""; de = "";}
{ 
SY = $9
id = $1
if (id ~ /DE/) {
	de = $0
}
if (SY == "") {
	n = 0;
}
if (SY ~ /'G';/) {


	if ((n != 0) && (n != 1)) 
		n = 0
	else
		if (n == 1) 
			n = 2
		else
			if (n == 0) 
				n = 1
	
}
if (SY ~ /'T';/) {
	if ((n != 2) && (n != 3)) 
		n = 0
	else
		if (n == 3) 
			n = 4
		else
			if (n == 2) 
				n = 3
	
}
if (SY ~ /'A';/) {
	if ((n != 4) && (n != 5)) 
		n = 0
	else
		if (n == 5) 
			n = 0
			print de 
		else
			if (n == 4) 
				n = 5
	
}
if (SY ~ /'C';/) {
	n = 0
}
}
