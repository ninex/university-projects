#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "include/cutglobal.h"
#include "include/cut.h"
#include "include/split.h"


double picture_rates [9] = { 0., 24000./1001., 24., 25., 
			     30000./1001., 30., 50., 
			     60000./1001., 60. };


int FindNextMarker(offset* off,marker* mark){
	offset myoffset=*off;
	while(myoffset<FileSize-3){
		// is there enough ?
		
		if ((myoffset-Start) > (BUFFSIZE-16)){
			FillBuffer(myoffset);
		}
		if ( (GetByte(myoffset+0) == 0x00) &&	(GetByte(myoffset+1) == 0x00) &&
			(GetByte(myoffset+2) == 0x01)){
			*off=myoffset+4;
			*mark=GetByte(myoffset+3);
			//    printf("Trouv�marqueur %02x offset %ld [%04lx]\n",
			//   GetByte(myoffset+3),myoffset,myoffset);
			return 1;
			
		}
		myoffset++;
	}
	
	EoF=1;
	return 0;
}


void ReadGOPHeader(offset off, double* timestamp){
	Byte hour,min,sec,count;
	*timestamp=0;
	//  printf(" [%02x %02x %02x %02x]\n",
	// GetByte(off),GetByte(off+1),GetByte(off+2),GetByte(off+3));
	//  printf("     Drop frame marker: ");
	//  if (GetByte(off)&0x80) printf("1\n");
	//  else printf("0\n");
	hour=GetByte(off);
	hour &= !0x80; //clears drop frame marker bit
	hour >>=2;      //hour :bits[6-2]
	*timestamp+=hour*3600;
	//  printf("     hour : %d\n",hour);
	min=GetByte(off+1);
	min >>=4;
	hour=GetByte(off);
	hour <<= 6 ;
	hour >>=2;
	min ^= hour;
	*timestamp+=min*60;
	//  printf("     minutes : %d\n",min);
	min=GetByte(off+1);
	min <<= 5;
	min >>= 2;
	sec=GetByte(off+2);
	sec >>=5;
	sec ^= min;
	*timestamp+=sec;
	//  printf("     secondes : %d\n",sec);
	count=GetByte(off+2);
	count <<=3;
	count >>=2;
	sec=GetByte(off+3);
	if (sec & 0x80) count |= 0x01;
	*timestamp += count / FrameRate;
	//  printf("     count : %d\n",count);
	//  printf ("     Closed GOP flag : ");
	//  if (sec & 0x40) printf("1\n");
	//  else printf("O\n");
	//  printf("     Broken link flag : ");
	//  if (sec & 0x20) printf("1\n");
	//  else printf("O\n");
	
	

}

void ParseVidSeq(offset* off){
	int horsize,versize;
	int pictrate;
	marker mymark;
	offset saveoff=*off;
	starttime=-1;
	
	VidSeqStart=(*off)-4;
	horsize=(GetSize(*off)>>4);
	versize=(GetSize((*off)+1)&0x0fff);
	
	printf("mpeg video is %d x %d\n",horsize,versize);
	
	pictrate=(GetByte((*off)+3)&0x0f);
	if (pictrate>8) {
		FrameRate=0;
		printf("invalid picture rate... strange\n");
	}
	else {
		FrameRate=picture_rates[pictrate];
		printf("picture rate is %f fps\n",picture_rates[pictrate]);
	}
	//we'd have to really parse the remaining but too lazy 
	// let's just find next marker which should be a GOP
	*off+=6; //at least
	
	while (1==1){
		if (FindNextMarker(off,&mymark)&&(mymark==0xb8)){
			//we have it! aren't we too far?
			if (((*off) - saveoff)<150 ){
				//yeah Y 150? -> because (hint 2 matrix)
				if (FileType==VIDEOFILE){
					ReadGOPHeader(*off,&starttime); //get time written in first GOP
				}/*else{
					BackwardFindBoundaries(*off,&pstart,&pend);
					if (GetByte(pstart+3)==0xe0) {
						// that's a video packet ok
						offset pstart2=pstart+6;
						ReadPacketTS(&pstart2,&pts,&dts);
						if(dts!=-1){
							starttime=dts;
						}else{
							starttime=pts;
						}
					}
				}*/
				//	printf("Video Sequence header seems to end at [0x%lx]\n",
				//     (*off)-4);
				VidSeqEnd=(*off-4);
				if (FileType==SYSTEMFILE){
					MinimumCut=VidSeqStart;
					while(1){
						MinimumCut--;
						if (MinimumCut<=0) {
							printf("lost, exiting\n");
							exit(1);
						}
						if (EnsureMarker(&MinimumCut,&mymark)){
							if (mymark==0xba){
								MinimumCut-=4;break;
							}else{
								MinimumCut -=4;
							}
						}
				
					}
				}else{
					MinimumCut=(*off)-4;
				}
				break;
			}else{
				//mmm it seems we're lost let's givit a try
				printf("Video Sequence header seems to end at [0x%lx] but we might be lost :/\n",
				(*off)-4);
				VidSeqEnd=(*off-4);
				if (FileType==SYSTEMFILE){
					MinimumCut=VidSeqStart;
					while(1){
						MinimumCut--;
						if (MinimumCut<=0) {
							printf("lost, exiting\n");
							exit(1);
						}
						if (EnsureMarker(&MinimumCut,&mymark)) {
							if (mymark==0xba){
								MinimumCut-=4;break;
							}else{
								MinimumCut -=4;
							}
						}
				
					}
				}else{
					MinimumCut=(*off)-4;
				}
				break;
			}
		}
		if(EoF==1){ // end of file reached :/
			printf("couldn't find first GOP! let's die\n");
			exit(1);
		}
	//printf("Strange I just passed a 00 00 01 %02x sequence...\n",
	//    mymark);
	}
}


int FindNext(offset* off,marker mark){
	offset myoffset=*off;
	while(myoffset<FileSize-3){
		// is there enough ?
	
		if ((myoffset-Start) > (BUFFSIZE-16)){
			FillBuffer(myoffset);
		}
		if ( (GetByte(myoffset+0) == 0x00) &&
		(GetByte(myoffset+1) == 0x00) &&
		(GetByte(myoffset+2) == 0x01) &&
		(GetByte(myoffset+3) == mark)){
			*off=myoffset+4;
			//    printf("Trouv�marqueur %02x offset %ld [%04lx]\n",
			//   GetByte(myoffset+3),myoffset,myoffset);
			return 1;
			
		}
		myoffset++;
	}
	//  printf("Fin du fichier atteinte :/ \n");
	EoF=1;
	return 0;

}


int FindVideoSeqHeader(offset* off){
  //we're parsing a system file... 
  //ugly way : find 00 00 01 B3
  //smart way : parse PACK headers and packets type
  //            try to find a video seq in video packet
  // let's be ugly :>
  return FindNext(off,0xb3);
}



