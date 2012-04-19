#include <stdio.h>

unsigned long main (int argc,char ** argv)
{
	unsigned long op=2;
	unsigned long a = 5;
	unsigned long b=4;
	
	unsigned long buf,d;
	
	op = op << 26;
	a = a << 21;
	b = b << 16;
	buf = op | a | b; 

	
	FILE *f=fopen("outa.txt","w");
	fwrite(&buf,sizeof(buf),1,f);
	fclose(f);
	
	FILE *g=fopen("outa.txt","r");
	fread(&d,sizeof(d),1,g);
	printf("waarde %ld\n",d);
}
