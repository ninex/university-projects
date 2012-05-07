#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "include/main.h"
#include "include/global.h"
#include "include/cutglobal.h"
#include "include/cut.h"
#include "include/parse.h"


 double starttime;
 double timecorrect;



 char basenamer[300];
 int   chunknumber,chunkcount;
 int confirmall;
 Byte Buffer[BUFFSIZE];


 float Byterate;
 offset audioframelength;
 int audiolayer;

 int EoF;
 offset VidSeqStart,VidSeqEnd;
 double* startT;
 offset* startO;
 offset*stopO;
 double* stopT;
 double Tmax;

 int demux;

 FILE* InFd,*OutFd;
 int splitnum;
 int FileType;
 offset FileSize;
 offset Start,End; //offset of the buffer in file

 offset MinimumCut,StuffingQuantum;

 double FrameRate; 





int FillBuffer(offset off){
	int readsize;
	fseek(InFd,off,SEEK_SET);
	readsize=fread(Buffer,1,BUFFSIZE,InFd);
	
	if (readsize <=0) {
	//    Start=End=-1;
		clearerr(InFd);
		return 0;
	}
	Start=off;
	End=Start+readsize;
	return 1;
}

Byte GetByte(offset off){
	if ((off>=Start)&&(off<End)){
		return Buffer[off-Start];
	}else{
		if(FillBuffer(off)){
			return Buffer[off-Start];
		}else{ // unrecoverable error
	
			if (off >=FileSize){
				printf("(bug) you shouldn't read this\n asked me to read at offset %ld ,File size %ld\n",off,FileSize);
			}else{
				//really weird!
				
				exit(1);
			}
			return 0xff;
		}
	}
}

unsigned short int GetSize(offset off){
	return GetByte(off)*256 +GetByte(off+1);
}

int EnsureMarker(offset* off,marker* mark){
	if ( (GetByte((*off)+0) == 0x00) && (GetByte((*off)+1) == 0x00) && (GetByte((*off)+2) == 0x01)){
		*mark=GetByte((*off)+3);
		*off=(*off)+4;
		return 1;
	}else{
		return 0;
	}
}


void init(int ARGC,char** ARGV){
	
	InFd=fopen(ARGV[1],"rb");
	if (InFd ==0) {
		printf("Unable to open input file ");
		exit(1);
	}else{
		printf("Opened %s\n",ARGV[1]);
	}
	
	first = NULL;
	frames = first;
	
	int i;
	marker mymark;
	offset myoff=0;
	
	demux=0;
	confirmall=1;
	
	startT=(double*)(calloc(ARGC-1,sizeof(double)));
	stopO=(offset*)(calloc(ARGC-1,sizeof(offset)));
	stopT=(double*)(calloc(ARGC-1,sizeof(double)));
	startO=(offset*)(calloc(ARGC-1,sizeof(offset)));
	
	chunkcount=0;
	splitnum=0;
	EoF=0;
	timecorrect=0;
	MinimumCut=-1;
	VidSeqStart=VidSeqEnd=-1;
	
	sprintf(basenamer,"chunk");
	chunknumber=0;

	chunkcount = 0;
	
	
	
	
	
		
	Tmax=0;
	for(i=0;i<chunkcount;i++){
		Tmax=(Tmax < (startT[i]))?startT[i]:Tmax;
		Tmax=(Tmax<(stopT[i]))?stopT[i]:Tmax;
	}
	fseek(InFd,0L,SEEK_END);
	FileSize=ftell(InFd);
	
	FillBuffer(0);
	
	//try to find file type (mpeg audio, video, system)
	if (EnsureMarker(&myoff,&mymark)){
		// good! first 4 bytes in file are an mpeg start code
		if (mymark == 0xba){ // system header
			FileType=SYSTEMFILE;
			printf("System mpeg file\n"); 
			//try to find valuable info
			if(!FindVideoSeqHeader(&myoff)){
				//didnt find video seq header in whole file
				printf("No Mpeg vid header found\n");
				exit(1);
			}
	// vid seq header found, PARSE IT !
		
			ParseVidSeq(&myoff);
			return ;
		}else if (mymark==0xb3){//video sequence header
			FileType=VIDEOFILE;
			ParseVidSeq(&myoff);
			printf("mpeg video only file\n");
			//	exit(0);
			return ;
		}else {
			printf("Unknown mpeg marker\n");
			exit(1);
		}
	}
	
	
	//file doesn't begin with a start code nor audio sync
	// well could *search* for something... exit for now
	FileType=UNKNOWNFILE;
	exit(1);
}


int main (int argc, char **argv){
	init(argc,argv);
		


	char filename[300];
	
	find(argv[1]);

	printf("Splitting file..\n");
	
	double fsize = FileSize / framecount;
	frames = first;
	if (frames != NULL)
	{
		while (frames->next != NULL)
		{
		
			sprintf(filename,"%s%d.mpg",basenamer,chunknumber++);
	
			CutPart(fsize*frames->begin,fsize*frames->end,filename);
			frames = frames->next;
		}
		sprintf(filename,"%s%d.mpg",basenamer,chunknumber++);
		CutPart(fsize*frames->begin,fsize*frames->end,filename);
		sprintf(filename,"%s%d.mpg",basenamer,chunknumber++);
		CutPart(fsize*frames->end,fsize,filename);
	}

	
	if(InFd != 0) fclose(InFd);
	
}



