
#include <stdio.h>
#include <math.h>


#include "include/mpeg2dec.h"
#include "include/global.h"

bool train(){
	FILE* training;
	training = fopen("train","r");
	if (training == NULL){
		return false;
	}
	fscanf(training,"%lf %lf %lf\n",&rm,&gm,&bm);
	printf("%lf %lf %lf\n",rm,gm,bm);
	fclose(training);
	return true;
}

void find(char *in ){
	mean = true;
	sd = true;
	r_glob = 0;
	g_glob = 0;
	b_glob = 0;
	points = 0;
	spl = 0;
	last = 0;
	framecount = 0;
	//printf("Calculating mean\n");
	/*if (!train()){
		//calc mean
		themain(in);
		rm = r_glob/points;
		gm = g_glob/points;
		bm = b_glob/points;
		FILE* training;
		training = fopen("train","w");
		fprintf(training,"%lf %lf %lf\n",rm,gm,bm);
		fclose(training);
	}*/
	
	mean = false;
	
	printf("Finding cut positions\n");
	//split
	points = 0;
	
	sd = false;
	themain(in);
}







