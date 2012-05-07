#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "include/cutglobal.h"
#include "include/cut.h"
#include "include/split.h"
#include "include/parse.h"

void Confirm(char* filename){
	struct stat mystat;
	int mychar;
	int mytrash;
	if (!confirmall){
		return;
	}
	if (stat(filename,&mystat)==-1){
		
		return;
	}else{
		printf("%s exists, overwrite? [Yes/No/Always] :",filename);
		mychar=getchar();
		do{
			mytrash=getchar();
		}while (mytrash!='\n');
		
		switch(mychar){
			case 'Y':
			case 'y': return; 
			case 'a':
			case 'A': confirmall=0; return;
			default:
			printf("Canceled by user\n");
			exit(1);
		}
	}
}

void copy(FILE* InFd,FILE* OutFd,offset from,offset to){
	if ((InFd)&&(OutFd)){
		//printf("Copying from [%lx] to [%lx] (%ld)\n",from,to,to-from);
		long rsize=0;
		long size=0;
		FillBuffer(from);
		fseek(OutFd,0L,SEEK_END);
		while(rsize < to -from){
			rsize+=BUFFSIZE;
			if (rsize >( to-from)){
				size = (to-from)%BUFFSIZE;
			}else{
				size=BUFFSIZE;
			}
			fwrite(Buffer,1,size,OutFd);
			FillBuffer(from+rsize);
		}
	}
	return ;
}
int BackwardFindBoundaries(offset from, offset* packetstart, offset* packetend){
	//find the packet boundaries in which "from" is
	offset myfrom=from;
	marker mymark;
	//TODO : heavy disk load when buffer's not good. FillBuffer starts @0
	if ((myfrom <0)||(myfrom >FileSize)){
		return(0);
	}
	
	for(;myfrom>=0;myfrom--){
		if (EnsureMarker(&myfrom,&mymark)){
			//a marker was found at offset from -4
			if((mymark==0xe0)){
				//video packet
				*packetstart=myfrom-4;
				*packetend=myfrom+GetSize(myfrom)+2;
				if((from<*packetstart)||(from>*packetend)){
					// found packet boundaries but
					// from wasn't inside...
					return 0; 
				}else{
					return 1;
				}
			}else{
				myfrom -=4;
			}
		}
	}
	return 0;
}

int ReadTimeStamp(offset off,double* timestamp){
	Byte hiBit;
	unsigned long low4Bytes;
	hiBit= (GetByte(off)>>3)&0x01;
	low4Bytes = ((GetByte(off) >> 1)&0x03)<<30;
	low4Bytes |= GetByte(off+1)<< 22;
	low4Bytes |= (GetByte(off+2)>>1)<<15;
	low4Bytes |= GetByte(off+3) << 7;
	low4Bytes |= GetByte(off+4) >> 1;
	#define FLOAT_0x10000 (double)((unsigned long)1 << 16)
	#define STD_SYSTEM_CLOCK_FREQ (unsigned long)90000
	if (hiBit != 0 && hiBit != 1) {
		*timestamp = -1;
		return 0;
	}
	*timestamp 
	= (double)hiBit*FLOAT_0x10000*FLOAT_0x10000 + (double)low4Bytes;
	*timestamp /= (double)STD_SYSTEM_CLOCK_FREQ;
	return 1;
}

int ReadPacketTS(offset* off,double* pts,double* dts){
	//just after the packet start code. and the length field
	offset myoff=*off;
	Byte mybyte;
	//get rid of stuffing bytes
	mybyte=GetByte(myoff);
	while(mybyte & 0x80){
		//    printf("sb ");
		//this has to be a stuffing byte
		mybyte=GetByte(++myoff);
	}
	//  printf("->%x ",mybyte);
	//here mybyte is the first valid byte
	if((mybyte>>6)==0x01){
		//std buffer scale stuff... let's skip it
		myoff+=2;
		mybyte=GetByte(myoff);
		//  printf("std buff, skip ->%x ",mybyte);
	}
	
	if((mybyte>>4)==0x02){
		// dts Time stamp
		//  printf(" PTS\n");
		ReadTimeStamp(myoff,pts);
		*dts=-1;
		*off=myoff+5;
		return 1;
	}else{
		if ((mybyte>>4)==0x03){
			//    printf(" PTS & DTS\n");
			//both pts and dts time stamps
			ReadTimeStamp(myoff,pts);
			myoff+=5;
			ReadTimeStamp(myoff,dts);
			*off=myoff+5;
			return 1;
		}else {
		// curious byte, skip it
		//    printf("unknown\n");
		myoff++;
		}
	}
	*dts=*pts=-1;
	*off=myoff;
	return 0;
}


int CutPart(offset begin, offset end, char* filename){
	offset myoff=0;
	//  double begin,end;
	//  double Goptime=0;
	//  marker mymark;
	double trash;
	offset mybeg,myend;
	offset packetstart,packetend;
	offset from,GOPbeg,to,GopToEOP;
	offset packetheaderlength;
	offset sizeofnewpacket;
	Byte highsize,lowsize;
	if((begin >=FileSize)||(begin<0)) {
		printf(" beyond end of file skiping\n");
		return 0;
	}
	
	//write mpeg file headers
	Confirm(filename);
	printf("%s ",filename);
	fflush(stdout);
	
	mybeg=begin;
	myend=end;
	from=GOPbeg=to=GopToEOP=-1;
	OutFd = fopen(filename,"wb");
	if (OutFd ==0) {
		perror("Unable to open output file ");
		return 0;
	}
	if((MinimumCut >=0)){
		copy(InFd,OutFd,0,MinimumCut);
		fflush(OutFd);
		//    stuff(OutFd,StuffingQuantum);
		myoff=MinimumCut;
	}else{
		return 0;
	}
	
	//finds first pack including gop
	
	if(!FindNext(&mybeg,0xb8)){
		//couldn't find anything after begining
		return 0;
	}else {
		// I have a gop!
		if (FileType==VIDEOFILE) {
			from=mybeg-4;
		}else{
	
			GOPbeg=mybeg-4;
			if(!BackwardFindBoundaries(mybeg,&packetstart,&packetend)){
				printf("Error couldn't find video packet in file\n");
				return 0;
			}
			from=packetstart-12; //till PACK start
			//    stufbeg=(myoff-4)-(packetstart+4+5);
			GopToEOP=packetend-GOPbeg;
		}
	}
	//we have the begining
	
	if(!FindNext(&myend,0xb8)){
		//couldn't find anything after begining
		to=FileSize;
		if(FileType==VIDEOFILE){
			copy(InFd,OutFd,from,to);
			fclose(OutFd);
			printf("\n");
			return 1;
		}
	}else{
		// I have a gop!
		if (FileType==VIDEOFILE) {
			to=myend-4;
			copy(InFd,OutFd,from,to);
			fclose(OutFd);
			printf("\n");
			return 1;
		}else{
			to=myend-4;
		}
	}
	
	//okay here we are.
	// we already copied the mandatory headers i.e. until first video pack
	// what we have to do here is copy the pack and pack header
	// compute the new packet length
	// write it along with video packet header
	// write the Video Sequence header
	// write the video sequence starting from GOPbeg, til to
	// then we're done.
	
	//computes the packet header length by parsing it and then getting the modified offset
	//from is the PACK start from +12 +4 +2 has to be after the video packet length field.
	packetheaderlength=from+12+4+2;
	ReadPacketTS(&packetheaderlength,&trash,&trash);
	packetheaderlength-=(from+12+4+2);
	
	
	
	
	//compute the new packet size
	// size of added VideoSequence(i.e. 00 00 01 b3 -> til GOP)
	// + GOP to end of packet
	// + packet header length
	
	
	
	
	
	sizeofnewpacket =VidSeqEnd-VidSeqStart+GopToEOP+packetheaderlength;
	highsize=(Byte)(sizeofnewpacket >> 8);
	lowsize=(Byte)(sizeofnewpacket & 0xFF);
	
	
	
	copy(InFd,OutFd,from,from+12+4); 
	fwrite(&highsize,1,1,OutFd);
	fwrite(&lowsize,1,1,OutFd);
	fflush(OutFd);
	copy(InFd,OutFd,(from+12+4+2),(from+12+4+2+packetheaderlength)); 
	fflush(OutFd);
	copy(InFd,OutFd,VidSeqStart,VidSeqEnd);
	fflush(OutFd);
	copy(InFd,OutFd,GOPbeg,to);
	fclose(OutFd);
	printf("\n");
	return(1);

}

void Offset(double time,offset off){
	int i;
	for(i=0; i<chunkcount;i++){
		if ((startO[i]==-1)&&(startT[i]<=time)) {
			startO[i]=off;
		}
		if ((stopO[i]==-1)&&(stopT[i]<=time)){
			stopO[i]=off;
		}
	}
}

void TimeToOffsets(){
	int i;
	double pts,dts;
	double timesave=0;
	offset myoff=0;
	offset skipto=0;
	int counter=0;
	if(FileType==SYSTEMFILE){
		while(FindNext(&myoff,0xe0)&&((timesave+timecorrect-starttime)<Tmax)){
			skipto=myoff+GetSize(myoff);
			myoff+=2; //skip size
			ReadPacketTS(&myoff,&pts,&dts);
			if(pts!=-1){ 
				if (dts!=-1){
					if ((dts<timesave)||(dts>(timesave+1))){
						timecorrect=timecorrect+timesave-dts;
						//printf("time corrected by %f",timecorrect);}
					}
		//printf ("pts %8.3f dts %8.3f\n",pts+timecorrect-starttime,dts+timecorrect-starttime);
					timesave=dts;
					Offset(timesave+timecorrect-starttime,myoff);
					counter--;
					if (counter<0) {
						printf("Seeking input file [%1.1f%%]     \r",(timesave+timecorrect-starttime)*100.0/Tmax);
						counter=100;
						fflush(stdout);
					}
				}else{
					if((pts<timesave)||(pts>(timesave+1))){
						timecorrect=timecorrect+timesave-pts;
						//printf("time corrected by %f",timecorrect);}
					}
					//printf("pts %8.3f\n",pts+timecorrect-starttime);
					timesave=pts;
					Offset(timesave+timecorrect-starttime,myoff);
					counter--;
					if (counter<0) {
						printf("Seeking input file [%1.1f%%]      \r",(timesave+timecorrect-starttime)*100.0/Tmax);counter=100;
						fflush(stdout);
					}
				}
				myoff=skipto;
			}
		}
	}else{
		while(FindNext(&myoff,0xb8)&&((timesave+timecorrect-starttime)<Tmax)){
			ReadGOPHeader(myoff,&pts);
			if((pts<timesave)||(pts>(timesave+1))) {
				timecorrect=timecorrect+timesave-pts;
			}
			timesave=pts;
			Offset(timesave+timecorrect-starttime,myoff);
			counter--;
			if (counter<0) {
				printf("Seeking input file [%1.1f%%] %1.3f      \r",(timesave+timecorrect-starttime)*100.0/Tmax,timesave+timecorrect-starttime);
				counter=3;
				fflush(stdout);
			}
		}
	}
	
	for(i=0; i<chunkcount;i++){
		if (stopO[i]==-1)  stopO[i]=FileSize;
	}
	
	printf("Seeking input file [100%%]              \n");
}

