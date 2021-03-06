#include <qvariant.h>
#include <qvector.h>
#include <qimage.h>
#include <math.h>
#include <cstdlib>

#include "include/ptrFrame.h"
#include "include/stats.h"


//constructor of the stats thread object
//in: vector list of video frames

Stats:: Stats( QVector<ptrFrame> *vectorlist)
{
	list = vectorlist;
	active = true;
	processing = false;
}

//this is the main running function of the thread

void Stats:: run(){
	
	while (active)
	{
		//needs to process a job?
		if (processing)
		{
			//calculate stats data
			calc_hotspots();
		}
	}
}

//Checks the status of the processing
//in: the status

bool Stats::Processing()
{
	return processing;
}

//notifies the thread that stats data needs to be calculated
//in: width and height of video, the edge data threshold value

void Stats::hotspots(int width, int height,double edge_thres)
{
	//processing starts
	processing = true;
	
	//find the amount of frames
	frames = list->size();
	
	//set some values
	edge_t = edge_thres;
	w = width;
	h = height;
	ml = w/2;
	ml2 = h/2;
	mr = w/2;
	mh = h/2;
}


//calculates all the necessary data

void Stats::calc_hotspots()
{
	
	long r,g,b,i,j,temp,temp2,r1,g1,b1;
	long dr,dg,db,points;
	double rcount,gcount,bcount;
	rcount = 0;
	gcount = 0;
	bcount = 0;
	points = 0;
	ptrFrame *current,*next;
	
	
	int cdx;
	//compute the edge mean
	
	for (cdx=0;cdx < frames-1;cdx++)
	{
		current = list->at(cdx);
		next = list->at(cdx+1);
		
		for (i=0;i<w;i++)
		{
			for (j=0;j<h;j++)
			{
				r=0;g=0;b=0;
				r1=0;g1=0;b1=0;
				temp = (long)current->r + i*w+j;
				temp2 = (long)next->r + i*w+j;
				memcpy(&r,(void*)temp,1);
				memcpy(&r1,(void*)temp2,1);
				
				temp = (long)current->g + i*w+j;
				temp2 = (long)next->g + i*w+j;
				memcpy(&g,(void*)temp,1);
				memcpy(&g1,(void*)temp2,1);
				
				temp = (long)current->b + i*w+j;
				temp2 = (long)next->b + i*w+j;
				memcpy(&b,(void*)temp,1);
				memcpy(&b1,(void*)temp2,1);
				
				dr = abs(r1-r);
				dg = abs(g1-g);
				db = abs(b1-b);
				
				//motion on pixel
				
				if ((dr > 50) && (dg > 50) && (db > 50))
				{
					rcount+= r1;
					gcount+= g1;
					bcount+= b1;
					points++;
				}
			}
		}
	}
	
	//the mean
	ur = rcount / points;
	ug = gcount / points;
	ub = bcount / points;
	
	//now calculate the standard deviation of the edges
	//current = first;
	rcount = 0;
	gcount = 0;
	bcount = 0;
	for (cdx=0;cdx < frames-1;cdx++) 
	{
		current = list->at(cdx);
		next = list->at(cdx+1);
		
		for (i=0;i<w;i++)
		{
			for (j=0;j<h;j++)
			{
				r=0;g=0;b=0;
				r1=0;g1=0;b1=0;
				temp = (long)current->r + i*w+j;
				temp2 = (long)next->r + i*w+j;
				memcpy(&r,(void*)temp,1);
				memcpy(&r1,(void*)temp2,1);
				
				temp = (long)current->g + i*w+j;
				temp2 = (long)next->g + i*w+j;
				memcpy(&g,(void*)temp,1);
				memcpy(&g1,(void*)temp2,1);
				
				temp = (long)current->b + i*w+j;
				temp2 = (long)next->b + i*w+j;
				memcpy(&b,(void*)temp,1);
				memcpy(&b1,(void*)temp2,1);
				
				dr = abs(r1-r);
				dg = abs(g1-g);
				db = abs(b1-b);
				
				if ((dr > 50) && (dg > 50) && (db > 50))
				{
					rcount+= pow(r1 - ur,2)/ (points-1);
					gcount+= pow(g1 - ug,2)/ (points-1);
					bcount+= pow(b1 - ub,2)/ (points-1);
					
					
					//also find the bounding search area
					if (i<ml)
					{
						ml = i;
					}
					if (i>mr)
					{
						mr = i;
					}
					if (j<mh)
					{
						mh = j;
					}
					if (j>ml2)
					{
						ml2 = j;
					}
				}
			}
		}
	}
	
	//the standard deviation
	sr = sqrt(rcount) ;
	sg = sqrt(gcount) ;
	sb = sqrt(bcount) ;
	
	printf("Edge:\nMean rgb = %f,%f,%f\n",ur,ug,ub);
	printf("Standard dev rgb = %f,%f,%f,\n",sr,sg,sb);
	
	//now isolate different areas
	Isolate();

	current = list->at(0);

	//done!
	processing = false;
}


//this isolates different areas

void Stats:: Isolate()
{
	long r,g,b,i,j,temp;
	
	ptrFrame *ptr = list->at(0);
	
	QImage mask,pic;
	mask.create(w,h,32,0,QImage::IgnoreEndian);
	
	pic.create(w,h,32,0,QImage::IgnoreEndian);
	
	for (i=0;i<w;i++)
	{
		for (j=0;j<h;j++)
		{
			r=0;g=0;b=0;

			temp = (long)ptr->r + i*ptr->w+j;
			memcpy(&r,(void*)temp,1);

			
			temp = (long)ptr->g + i*ptr->w+j;
			memcpy(&g,(void*)temp,1);

			temp = (long)ptr->b + i*ptr->w+j;
			memcpy(&b,(void*)temp,1);

			//this finds all movement areas and sets these positions in a mask
			
			
			if ((i>=ml) && (i<=mr) && (j>=mh) && (j<=ml2))
			{
				if (((ur - edge_t*sr) < r) && ((ur + edge_t*sr) > r) && ((ug - edge_t*sg) < g) && ((ug + edge_t*sg) > g) && ((ub - edge_t*sb) < b) && ((ub + edge_t*sb) > b))
				{
					
					mask.setPixel(i,j,qRgb(255,255,255));
				}
			}
			pic.setPixel(i,j,qRgb(r,g,b));
		}
	}
	
	//isolate body parts
	
	isolateHead(mask);
	
	isolateElbows(mask);
	
	isolateHands(mask);
	
	//from this try to find information on the person's skin
	
	skinData(mask,pic);
}


//get the coordinates of the head
//in: mask with set pixels

void Stats:: isolateHead(QImage mask)
{
	int i,j,l,r;
	l = w/3;
	r = 2*w/3;
	hl = w;
	hr = 0;
	ht = h;
	hb = 0;
	for (i=l;i<r;i++)
	{
		for (j=0;j<h/2;j++)
		{
			if (mask.pixel(i,j) == qRgb(255,255,255))
			{
				if (i < hl){
					hl = i;
				}
				if (i > hr){
					hr = i;
				}
				if (j < ht){
					ht = j;
				}
				if (j > hb){
					hb = j;
				}
			}
		}
	}
}

//get the coordinates of the elbows
//in: mask with set pixels

void Stats:: isolateElbows(QImage mask)
{
	int i,j,center,mt,mb,ley,rey,lex,rex;
	mt = h;
	mb = 0;
	center = hl+(hr-hl)/2;
	
	for (i=0;i<w;i++)
	{
		for (j=h/2;j<h;j++)
		{
			if (mask.pixel(i,j) == qRgb(255,255,255))
			{
				if (j<mt)
				{
					mt = j;
				}
				if (j>mb)
				{
					mb = j;
				}
			}
		}
	}
	
	
	ley = h;
	rey = h;
	for (i=0;i<center;i++)
	{
		for (j=h/2;j<(mt + (mb-mt)/2);j++)
		{
			if (mask.pixel(i,j) == qRgb(255,255,255))
			{
				if (j<ley)
				{
					ley = j;
					lex = i;
				}
			}
		}
	}
	
	for (i=center;i<w;i++)
	{
		for (j=h/2;j<(mt + (mb-mt)/2);j++)
		{
			if (mask.pixel(i,j) == qRgb(255,255,255))
			{
				if (j<rey){
					rey = j;
					rex = i;
				}
			}
		}
	}
	
	//sets initial frame values
	ptrFrame *ptr = list->at(0);
	
	ptr->lelbow->x = lex;
	ptr->lelbow->y = ley;
	ptr->relbow->x = rex;
	ptr->relbow->y = rey;
}

//get initial hand estimates
//in: mask with set pixels

void Stats:: isolateHands(QImage mask){
	
	int i,j,center,mt,mb,lhy,rhy,lhx,rhx;
	mt = h;
	mb = 0;
	center = hl+(hr-hl)/2;
	
	for (i=0;i<w;i++)
	{
		for (j=h/2;j<h;j++)
		{
			if (mask.pixel(i,j) == qRgb(255,255,255))
			{
				if (j<mt)
				{
					mt = j;
				}
				if (j>mb)
				{
					mb = j;
				}
			}
		}
	}
	

	lhy = 0;
	rhy = 0;
	for (i=0;i<center;i++)
	{
		for (j=(mt + (mb-mt)/2);j<h;j++)
		{
			if (mask.pixel(i,j) == qRgb(255,255,255))
			{
				if (j>lhy)
				{
					lhy = j;
					lhx = i;
				}
			}
		}
	}
	for (i=center;i<w;i++)
	{
		for (j=(mt + (mb-mt)/2);j<h;j++)
		{
			if (mask.pixel(i,j) == qRgb(255,255,255))
			{
				if (j>rhy)
				{
					rhy = j;
					rhx = i;
				}
			}
		}
	}
	
	//sets initial frame values
	ptrFrame *ptr = list->at(0);
	
	ptr->left->x = lhx;
	ptr->left->y = lhy;
	ptr->right->x = rhx;
	ptr->right->y = rhy;
	
	
}


//find out more about the person's skin
//in: mask with set pixels, original image

void Stats:: skinData(QImage mask,QImage pic)
{
	int i,j,r,g,b,points;
	double rc,gc,bc;
	float mel,mer,cl,cr;
	QColor col;
	ptrFrame *ptr = list->at(0);
	
	//left
	mel = ((ptr->lelbow->y - ptr->left->y)) / (ptr->lelbow->x - ptr->left->x);
	cl = ptr->lelbow->y - (mel*ptr->lelbow->x);
	
	for (j=ptr->lelbow->y;j<=ptr->left->y;j++)
	{
		for (i=ptr->left->x;i<ptr->lelbow->x;i++)
		{
			if ((int)(mel*i+cl) == j)
			{
				mask.setPixel(i,j-1,qRgb(0,255,0));
				mask.setPixel(i,j,qRgb(0,255,0));
				mask.setPixel(i,j+1,qRgb(0,255,0));
			}
		}
		for (i=ptr->left->x;i>=ptr->lelbow->x;i--)
		{
			if ((int)(mel*i+cl) == j)
			{
				mask.setPixel(i,j-1,qRgb(0,255,0));
				mask.setPixel(i,j,qRgb(0,255,0));
				mask.setPixel(i,j+1,qRgb(0,255,0));
			}
		}
	}
	
	//right
	
	mer = (ptr->relbow->y-ptr->right->y) / (ptr->relbow->x - ptr->right->x);
	cr = ptr->right->y - (mer*ptr->right->x);
	
	for (j=ptr->relbow->y;j<=ptr->right->y;j++)
	{	
		for (i=ptr->right->x;i<ptr->relbow->x;i++)
		{
			if ((int)(mer*i+cr) == j)
			{
				mask.setPixel(i,j-1,qRgb(0,255,0));
				mask.setPixel(i,j,qRgb(0,255,0));
				mask.setPixel(i,j+1,qRgb(0,255,0));
			}
		}
		for (i=ptr->right->x;i>=ptr->relbow->x;i--)
		{
			if ((int)(mer*i+cr) == j){
				mask.setPixel(i,j-1,qRgb(0,255,0));
				mask.setPixel(i,j,qRgb(0,255,0));
				mask.setPixel(i,j+1,qRgb(0,255,0));
			}
		}
	}
	
	//mean
	rc= 0;
	gc = 0;
	bc = 0;
	points = 0;
	
	for (i=ml;i<=mr;i++)
	{
		for (j=mh;j<=ml2;j++)
		{
			if (mask.pixel(i,j) == qRgb(0,255,0))
			{
				col = pic.pixel(i,j);
				r = col.red();
				g = col.green();
				b = col.blue();
	
				rc += r;
				gc += g;
				bc += b;
				points++;
			}
		}
	}
	s_ur = rc / points;
	s_ug = gc / points;
	s_ub = bc / points;
	
	
	//standard deviation
	rc= 0;
	gc = 0;
	bc = 0;
	for (i=ml;i<=mr;i++)
	{
		for (j=mh;j<=ml2;j++)
		{
			if (mask.pixel(i,j) == qRgb(0,255,0))
			{
				col = pic.pixel(i,j);
				r = col.red();
				g = col.green();
				b = col.blue();
				rc += pow(r - ur,2)/ (points-1);
				gc += pow(g - ug,2)/ (points-1);
				bc += pow(b - ub,2)/ (points-1);
			}
		}
	}
	s_sr = sqrt(rc);
	s_sg = sqrt(gc);
	s_sb = sqrt(bc);
	
	printf("Skin:\nMean rgb = %f,%f,%f\n",s_ur,s_ug,s_ub);
	printf("Standard dev rgb = %f,%f,%f,\n",s_sr,s_sg,s_sb);
}
