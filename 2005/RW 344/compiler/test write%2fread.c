#include <stdio.h>

int main (int argc,char ** argv)
{
	unsigned long buf = 6452134,a;
	FILE *f=fopen("outa.txt","w");
	fwrite(&buf,sizeof(buf),1,f);
	fclose(f);
	
	FILE *g=fopen("outa.txt","r");
	fread(&a,sizeof(a),1,g);
	printf("waarde %ld\n",a);
}
