#include <qimage.h>
#include <qvector.h>
#include <math.h>
#include <cstdlib>

#include "include/ptrFrame.h"
#include "include/stats.h"
#include "include/tracker.h"



//Constructor for the tracker object
//in: the vector list containing the video data

Tracker:: Tracker(QVector<ptrFrame> *vector_list)
{
	list = vector_list;
}

//This function sets all the stats values from edge detection
//in: the rgb mean and the rgb standard deviation

void Tracker::  setAllStatsEdgeValues(double stats_ur,double stats_ug,double stats_ub,double stats_sr,double stats_sg,double stats_sb)
{
	ur = stats_ur;
	ug = stats_ug;
	ub = stats_ub;
	sr = stats_sr;
	sg = stats_sg;
	sb = stats_sb;
}


//This function sets all the stats values from skin detection
//in: the rgb mean and rgb standard deviation

void Tracker::  setAllStatsSkinValues(double stats_s_ur,double stats_s_ug,double stats_s_ub,double stats_s_sr,double stats_s_sg,double stats_s_sb)
{
	s_ur = stats_s_ur;
	s_ug = stats_s_ug;
	s_ub = stats_s_ub;
	s_sr = stats_s_sr;
	s_sg = stats_s_sg;
	s_sb = stats_s_sb;
}

//This stores the information associated with the position of the head
//the centerline of the body is also calculated
//in: the rectangular coordinates of the head

void Tracker::  setAllHeadValues(int stats_hl,int stats_hr,int stats_ht,int stats_hb)
{
	hl = stats_hl;
	hr = stats_hr;
	hb = stats_hb;
	ht = stats_ht;

	//computes the centerlines
	cx = hl + (hr-hl)/2;
	cy = hb;
}

//This sets all the bounding values, as well as the size of the video
//This means the areas in which must be searched for movement
//in: rectangular search area and width,height of video

void Tracker::  setAllBoxValues(int stats_ml,int stats_mr,int stats_ml2,int stats_mh,int width, int height)
{
	ml = stats_ml;
	mr = stats_mr;
	ml2 = stats_ml2;
	mh = stats_mh;
	
	w = width;
	h = height;
}

//compute the minimum and maximum arm sizes in the video

void Tracker:: notifyArms()
{
	ptr = list->at(0);
	maxArmLeft = sqrt( pow(ptr->lelbow->x - ptr->left->x,2) + pow(ptr->lelbow->y - ptr->left->y,2) );
	maxArmRight = sqrt( pow(ptr->lelbow->x - ptr->left->x,2) + pow(ptr->lelbow->y - ptr->left->y,2) );
	
	minArmLeft = maxArmLeft * 0.3;
	minArmRight = maxArmRight * 0.3;
}

//The main calling function of the object
//This function decides which method must be called and then the tracking is performed on the frame
//in: The image on which coordinates must be drawn, the framenumber, the threshold values, and the method

void Tracker:: trackFrame(QImage pic,int frame,double edge,double skin,QString type)
{
	edge_t = edge;
	skin_t = skin;
	
	framecount = frame;
	
	//loads the previous frame and the current one
	ptr  = list->at(frame);
	prev = list->at(frame-1);
	
	//The first method that can be selected
	if (type.contains("Linear Equation"))
	{
		hybrid = false;
		track(pic);
	}
	else
	{
		//second method
		if (type.contains("Motion Vector"))
		{
			hybrid = false;
			motiontrack(pic);
		}
		else
		{
			//third method
			if (type.contains("Hybrid"))
			{
				hybrid = true;
				track(pic);
				hybrid = false;
			}
		}
	}
	//remember that the person is facing us, so left and right switches
	ptr->lx = cx - ptr->right->x;
	ptr->ly = cy - ptr->right->y;
	ptr->rx = cx- ptr->left->x;
	ptr->ry = cy - ptr->left->y;
	
}


//The main method of the Vector motion method
//in: Image which hand positions are drawn

void Tracker:: motiontrack(QImage pic)
{
	
	int rex,rey,ley,lex;
	int oldrx,oldry,oldlx,oldly;
	
	//the old coordinates
	oldrx = prev->right->x;
	oldry = prev->right->y;
	oldlx = prev->left->x;
	oldly = prev->left->y;
	
	
	
	if (framecount >= 10)
	{
	
		//values for estimates from x = x0 + vt
		rex = prev->right->x + prev->right->vx;
		rey = prev->right->y + prev->right->vy;
		lex = prev->left->x + prev->left->vx;
		ley = prev->left->y + prev->left->vy;
		
		//check bounds
		if (rex < 0 || rex >= w)
		{
			rex = prev->right->x;
		}
		if (lex < 0 || lex >= w)
		{
			lex = prev->left->x;
		}
		if (rey < 0 || rey >= h)
		{
			rey = prev->right->y;
		}
		if (ley < 0 || ley >= h)
		{
			ley = prev->left->y;
		}
		
		searchLeftEstimates(pic,lex,ley);
		searchRightEstimates(pic,rex,rey);
		
	}
	else
	{
		track(pic);
	}
	
	//calculate the new velocity
	ptr->right->vx = (ptr->right->x - oldrx);
	ptr->right->vy = (ptr->right->y - oldry);
	ptr->left->vx = (ptr->left->x - oldlx);
	ptr->left->vy = (ptr->left->y - oldly);
	
	
	int i,j;
	//draw the left hand
	for (i=ptr->left->x-4;i<ptr->left->x+4;i++)
	{
		for (j=ptr->left->y-4;j<ptr->left->y+4;j++)
		{
			pic.setPixel(i,j,qRgb(0,0,255));
		}
	}
	
	//draw the right hand
	for (i=ptr->right->x-4;i<ptr->right->x+4;i++)
	{
		for (j=ptr->right->y-4;j<ptr->right->y+4;j++)
		{
			pic.setPixel(i,j,qRgb(255,0,0));
		}
	}
	
}


//This searches for possible left hand positions from the provided estimates
//in: pic to search, estimates

void Tracker:: searchLeftEstimates(QImage pic,int lex,int ley)
{
	int bestlx = lex,bestly = ley;
	
	double best = w*h;
	
	//search limit
	int searchx = (int)(40)/2;
	int searchy = (int)(40)/2;
	int i,j,r,g,b;
	QRgb col;
	
	//left x
	for (i = lex-searchx;i<lex+searchx;i++)
	{
		//inside limits?
		if (i>0 && i<w)
		{
			//left y
			for (j = ley-searchy;j<ley+searchy;j++)
			{
				//inside limits?
				if (j>0 && j<h)
				{
					//not over the face?
					if (!(i >= hl && i<= hr && j>=ht && j<=hb))
					{
						//pixel rgb
						col = pic.pixel(i,j);
						r = qRed(col);
						g = qGreen(col);
						b = qBlue(col);
						
						//test against skin data
						if (((s_ur - skin_t*s_sr) < r) && ((s_ur + skin_t*s_sr) > r) && ((s_ug - skin_t*s_sg) < g) && ((s_ug + skin_t*s_sg) > g) && ((s_ub - skin_t*s_sb) < b) && ((s_ub + skin_t*s_sb) > b))
						{
							//is it a better estimate?
							if ( sqrt( pow(lex-i,2)+ pow(ley-j,2) ) < best)
							{
								best = sqrt( pow(lex-i,2)+ pow(ley-j,2) );
								bestlx = i;
								bestly = j;
							}
						}
					}
				}
			}
		}
	}
	
	//what estimate won?
	if (best != w*h)
	{
		ptr->left->x = bestlx;
		ptr->left->y = bestly;
	}
	else
	{
		ptr->left->x = prev->left->x;
		ptr->left->y = prev->left->y;
	}
}


//This searches for possible right hand positions from the provided estimates
//in: pic to search, estimates

void Tracker:: searchRightEstimates(QImage pic,int rex,int rey)
{
	int bestrx = rex,bestry = rey;
	
	double best = w*h;
	
	//search limit
	int searchx = (int)(40)/2;
	int searchy = (int)(40)/2;
	int i,j,r,g,b;
	QRgb col;
	
	//right x
	for (i = rex-searchx;i<rex+searchx;i++)
	{
		//limits?
		if (i>0 && i<w)
		{
			//right y
			for (j = rey-searchy;j<rey+searchy;j++)
			{
				//limits?
				if (j>0 && j<h)
				{
					//not covering face?
					if (!(i >= hl && i<= hr && j>=ht && j<=hb))
					{
						//pixel rgb
						col = pic.pixel(i,j);
						r = qRed(col);
						g = qGreen(col);
						b = qBlue(col);
						
						//test against skin data
						if (((s_ur - skin_t*s_sr) < r) && ((s_ur + skin_t*s_sr) > r) && ((s_ug - skin_t*s_sg) < g) && ((s_ug + skin_t*s_sg) > g) && ((s_ub - skin_t*s_sb) < b) && ((s_ub + skin_t*s_sb) > b))
						{
							//better estimate?
							if ( sqrt( pow(rex-i,2)+ pow(rey-j,2) ) < best)
							{
								best = sqrt( pow(rex-i,2)+ pow(rey-j,2) );
								bestrx = i;
								bestry = j;
							}
						}
					}
				}
			}
		}
	}
	//choose best estimate
	if (best != w*h)
	{
		ptr->right->x = bestrx;
		ptr->right->y = bestry;
	}else
	{
		ptr->right->x = prev->right->x;
		ptr->right->y = prev->right->y;
	}
}

//This searches for possible left elbow positions from the provided estimates
//in: pic to search, estimates

void Tracker:: searchLeftElbowEstimates(QImage pic,int lex,int ley)
{
	int bestlx = lex,bestly = ley;
	
	double best = w*w+h*h;
	
	//search limit
	int searchx = (int)(40)/2;
	int searchy = (int)(40)/2;
	int i,j,r,g,b;
	QRgb col;
	
	//left x
	for (i = lex-searchx;i<lex+searchx;i++)
	{
		//limits?
		if (i>0 && i<w)
		{
			//left y
			for (j = ley-searchy;j<ley+searchy;j++)
			{
				//limits?
				if (j>0 && j<h)
				{
					//covering head?
					if (!(i >= hl && i<= hr && j>=ht && j<=hb))
					{
						//pixel rgb
						col = pic.pixel(i,j);
						r = qRed(col);
						g = qGreen(col);
						b = qBlue(col);
						
						//test against skin data
						if (((s_ur - skin_t*s_sr) < r) && ((s_ur + skin_t*s_sr) > r) && ((s_ug - skin_t*s_sg) < g) && ((s_ug + skin_t*s_sg) > g) && ((s_ub - skin_t*s_sb) < b) && ((s_ub + skin_t*s_sb) > b))
						{
							//better estimate?
							if ( sqrt( pow(lex-i,2)+ pow(ley-j,2) ) < best)
							{
								best = sqrt( pow(lex-i,2)+ pow(ley-j,2) );
								bestlx = i;
								bestly = j;
							}
						}
					}
				}
			}
		}
	}
	
	
	//choose best estimate
	if (best != w*w+h*h)
	{
		ptr->lelbow->x = bestlx;
		ptr->lelbow->y = bestly;
	}else{
		ptr->lelbow->x = lex;
		ptr->lelbow->y = ley;
	}
}

//This searches for possible right elbow positions from the provided estimates
//in: pic to search, estimates

void Tracker:: searchRightElbowEstimates(QImage pic,int rex,int rey)
{
	int bestrx = rex,bestry = rey;
	
	double best = w*w+h*h;
	
	//search area
	int searchx = (int)(40)/2;
	int searchy = (int)(40)/2;
	int i,j,r,g,b;
	QRgb col;
	
	//right x
	for (i = rex-searchx;i<rex+searchx;i++)
	{
		//bounds?
		if (i>0 && i<w)
		{
			//right y
			for (j = rey-searchy;j<rey+searchy;j++)
			{
				//bounds
				if (j>0 && j<h)
				{
					//not covering head?
					if (!(i >= hl && i<= hr && j>=ht && j<=hb))
					{
						col = pic.pixel(i,j);
						r = qRed(col);
						g = qGreen(col);
						b = qBlue(col);
						
						//test against skin data
						if (((s_ur - skin_t*s_sr) < r) && ((s_ur + skin_t*s_sr) > r) && ((s_ug - skin_t*s_sg) < g) && ((s_ug + skin_t*s_sg) > g) && ((s_ub - skin_t*s_sb) < b) && ((s_ub + skin_t*s_sb) > b))
						{
							//better estimate?
							if ( sqrt( pow(rex-i,2)+ pow(rey-j,2) ) < best)
							{
								best = sqrt( pow(rex-i,2)+ pow(rey-j,2) );
								bestrx = i;
								bestry = j;
							}
						}
					}
				}
			}
		}
	}
	
	//choose best
	if (best != w*w+h*h)
	{
		ptr->relbow->x = bestrx;
		ptr->relbow->y = bestry;
	}else
	{
		ptr->relbow->x = rex;
		ptr->relbow->y = rey;
	}
}

//this calculates the positions of the elbows
//in:image to draw to, estimate mask, if left and right must be ignored, equation variables

void Tracker:: elbowMotiontrack(QImage pic,QImage mask,bool skipLeft,bool skipRight,double a,double c,double ra,double rc)
{
	int i,j;
	int rex,rey,ley,lex;
	int oldrx,oldry,oldlx,oldly;
	bool elb = false;
	double ctemp;
	
	//previous elbow values
	oldrx = prev->relbow->x;
	oldry = prev->relbow->y;
	oldlx = prev->lelbow->x;
	oldly = prev->lelbow->y;
	
	
	//used by hybrid method
	if (framecount >= 10 && hybrid)
	{
	
		//values for estimates from x = x0 + vt + at^2
		rex = prev->relbow->x + prev->relbow->vx;
		rey = prev->relbow->y + prev->relbow->vy;
		lex = prev->lelbow->x + prev->lelbow->vx;
		ley = prev->lelbow->y + prev->lelbow->vy;
		
		//search the estimates
		searchLeftElbowEstimates(pic,lex,ley);
		searchRightElbowEstimates(pic,rex,rey);
		
	}
	else
	{
		//can calculate?
		if (!skipLeft)
		{
			for (i=ml;i<=mr;i++)
			{
				//not found yet?
				if (!elb)
				for (ctemp = a-10;ctemp<a+10;ctemp++)
				{
					j = (int)(ctemp + c*i);
					
					if ( (i>=0 && i<w && j>=0 && j<h) &&  (mask.pixel(i,j) == qRgb(0,255,0)) )
					{
						//head check
						if (!(i >= hl && i<= hr && j>=ht && j<=hb))
						{
							
							if (!elb)
							{
								ptr->lelbow->x = i;
								ptr->lelbow->y = j;
								elb = TRUE;
								break;
							}
						}
					} 
				}
				
			}
		}
		else
		{
			//do an estimate
			searchLeftElbowEstimates(pic,prev->lelbow->x,prev->lelbow->y);
		}
		
		//can calculate
		if (!skipRight)
		{
			for (i=ml;i<=mr;i++)
			{
				for (ctemp = ra-10;ctemp<ra+10;ctemp++)
				{
					j = (int)(ctemp + rc*i);
					
					if ( (i>=0 && i<w && j>=0 && j<h) &&  (mask.pixel(i,j) == qRgb(255,0,0)) )
					{
						//head check
						if (!(i >= hl && i<= hr && j>=ht && j<=hb))
						{
							ptr->relbow->x = i;
							ptr->relbow->y = j;
						}
					} 
				}
				
			}
		}
		else
		{
			//do an estimate
			searchRightElbowEstimates(pic,prev->relbow->x,prev->relbow->y);
		}
	}
		
	//new velocity values
	ptr->relbow->vx = (ptr->relbow->x - oldrx);
	ptr->relbow->vy = (ptr->relbow->y - oldry);
	ptr->lelbow->vx = (ptr->lelbow->x - oldlx);
	ptr->lelbow->vy = (ptr->lelbow->y - oldly);

	
}


//The main entry point of the linear equation method

void Tracker:: track(QImage pic)
{
	
	int i,j,r,g,b;
	QRgb col;
	//set all the least squares values
	double x = 0,y = 0,x2 = 0,xy = 0,n = 0;
	double rx = 0,ry = 0,rx2 = 0,rxy = 0,rn = 0;
	bool skipLeft = false,skipRight=false;
	QImage mask;
	mask.create(w,h,32,0,QImage::IgnoreEndian);
	
	//calculate the equations of the arms
	
	if (prev->lelbow->x == prev->left->x){
		//perp
	}
	else
	{
		prev->larm->m =((float)(prev->lelbow->y - prev->left->y) / (float)(prev->lelbow->x - prev->left->x));
		prev->larm->c = prev->lelbow->y - (prev->larm->m*prev->lelbow->x);
		prev->larm->length = sqrt( pow( (prev->lelbow->y - prev->left->y),2) + pow(prev->lelbow->x - prev->left->x,2) );
	}
	
	if (prev->relbow->x == prev->right->x){
		//perp
	}
	else
	{
		prev->rarm->m =((float)(prev->relbow->y - prev->right->y) / (float)(prev->relbow->x - prev->right->x));
		prev->rarm->c = prev->relbow->y - (prev->rarm->m*prev->relbow->x);
		prev->rarm->length = sqrt( pow( (prev->relbow->y - prev->right->y),2) + pow(prev->relbow->x - prev->right->x,2) );
	}
	bool lef,rig,bot;
	
	//MARK the skin and edge detection areas
	
	for (i=ml;i<=mr;i++)
	{
		for (j=mh;j<=ml2;j++)
		{
			col = pic.pixel(i,j);
			r = qRed(col);
			g = qGreen(col);
			b = qBlue(col);
			
			if ( (((s_ur - skin_t*s_sr) < r) && ((s_ur + skin_t*s_sr) > r) && ((s_ug - skin_t*s_sg) < g) && ((s_ug + skin_t*s_sg) > g) && ((s_ub - skin_t*s_sb) < b) && ((s_ub + skin_t*s_sb) > b)) ||
			 (((ur - edge_t*sr) < r) && ((ur + edge_t*sr) > r) && ((ug - edge_t*sg) < g) && ((ug + edge_t*sg) > g) && ((ub - edge_t*sb) < b) && ((ub + edge_t*sb) > b)) )
			 {
			
				// left area
				if (searchLeftParams(i,j))
				{
					lef = TRUE;
					mask.setPixel(i,j,qRgb(0,255,0));
					//pic.setPixel(i,j,qRgb(0,255,0));
				}
				
				//right area
				if (searchRightParams(i,j))
				{
					if (!lef)
					{
						rig = TRUE;
						mask.setPixel(i,j,qRgb(255,0,0));
						//pic.setPixel(i,j,qRgb(255,0,0));
					}
					else
					{
						//we do not want shared data
						bot = TRUE;
						lef = FALSE;
						mask.setPixel(i,j,qRgb(0,0,0));
						//pic.setPixel(i,j,qRgb(0,0,0));
					}
				}
				
				//summations for the least squares algorithm
				
				if (lef)
				{
					x += i;
					y += j;
					x2 += pow(i,2);
					xy += i*j;
					n++;
				}
				else if (rig)
				{
					rx += i;
					ry += j;
					rx2 += pow(i,2);
					rxy += i*j;
					rn++;
				}
				lef = FALSE;
				rig = FALSE;
				bot = FALSE;
				
			}
		}
	}

	
	/*
	
	Least squares
	
	a = sum(y) * sum(x^2) - sum(x) * sum(x*y)
		-------------------------------------
	n * sum(x^2) - 	sum(x)^2
	
	
	b = n * sum(x*y)  - sum(x) * sum(y)
	-------------------------------------
	n * sum(x^2) - 	sum(x)^2
	
	
	y = a +bx
	
	*/
	
	
	//Do least squares estimation on left hand
	double a,c,b1,b2;
	
	b1 = (n * x2 - pow(x,2));
	
	if (b1 == 0)
	{
		skipLeft = true;
	}else{
		x2 /= b1;
		xy /= b1;
	
		a = (y*x2 - x*xy);
		c = n*xy - x*y/b1;
	}
	
	
	
	//Do least squares estimation on right hand
	
	double ra,rc;
	
	b2 = (rn * rx2 - pow(rx,2));
	
	if (b2 == 0)
	{
		skipRight = true;
	}else{
		rx2 /= b2;
		rxy /= b2;
	
		ra = (ry*rx2 - rx*rxy) ;
		rc = rn*rxy - rx*ry/b2;
	}
	
	//find the elbows	
	elbowMotiontrack(pic,mask,skipLeft, skipRight,a,c,ra,rc);
	
	
	
	//calculate the new estimates
	findIt(pic,c,rc,skipLeft,skipRight);
	
	//draw the left arm
	for (i=ptr->lelbow->x;i<=ptr->left->x;i++)
	{
		for (j=mh;j<=ml2;j++)
		{
			if ( j == (int)(a + c*i) )
			{
				pic.setPixel(i,j,qRgb(0,0,255));
				pic.setPixel(i,j-1,qRgb(0,0,255));
				pic.setPixel(i,j+1,qRgb(0,0,255));
			}
		}
	}
	
	//draw the right arm
	for (i=ptr->right->x;i<=ptr->relbow->x;i++)
	{
		for (j=mh;j<=ml2;j++)
		{
			if ( j == (int)(ra + rc*i) )
			{
				pic.setPixel(i,j,qRgb(0,0,255));
				pic.setPixel(i,j-1,qRgb(0,0,255));
				pic.setPixel(i,j+1,qRgb(0,0,255));
			}
		}
	}
	
	//draw the left hand
	for (i=ptr->left->x-4;i<ptr->left->x+4;i++)
	{
		for (j=ptr->left->y-4;j<ptr->left->y+4;j++)
		{
			pic.setPixel(i,j,qRgb(255,255,255));
		}
	}
	//draw the left elbow
	for (i=ptr->lelbow->x-4;i<ptr->lelbow->x+4;i++)
	{
		for (j=ptr->lelbow->y-4;j<ptr->lelbow->y+4;j++)
		{
			pic.setPixel(i,j,qRgb(0,128,255));
		}
	}
	
	//draw the right hand
	for (i=ptr->right->x-4;i<ptr->right->x+4;i++)
	{
		for (j=ptr->right->y-4;j<ptr->right->y+4;j++)
		{
			pic.setPixel(i,j,qRgb(255,0,255));
		}
	}
	//draw the right elbow
	for (i=ptr->relbow->x-4;i<ptr->relbow->x+4;i++)
	{
		for (j=ptr->relbow->y-4;j<ptr->relbow->y+4;j++)
		{
			pic.setPixel(i,j,qRgb(0,128,255));
		}
	}
	
	
	
}

//Tries to find the hands
//in: pic, the left, the gradients of the hands, if there must be skipped

void Tracker:: findIt(QImage pic,double c,double rc,bool skipLeft,bool skipRight)
{
	//left
	
	double angle,x,y;
	int lex,rex,ley,rey,lex2,ley2,rex2,rey2,ex,ey;
	
	//skip?
	if (!skipLeft)
	{
		//the angle
		angle = atan(c);
		
		//the first estimate
		x = prev->larm->length * cos(angle);
		y = prev->larm->length * sin(angle);
		
		ex = ptr->lelbow->x + (int)x;
		ey = ptr->lelbow->y + (int)y;
		
		//the shorter estimate
		x = prev->larm->length*0.7 * cos(angle);
		y = prev->larm->length*0.7 * sin(angle);
		
		lex = ptr->lelbow->x + (int)x;
		ley = ptr->lelbow->y + (int)y;
		
		//the longer estimate
		x = prev->larm->length*1.2 * cos(angle);
		y = prev->larm->length*1.2 * sin(angle);
		
		lex2 = ptr->lelbow->x + (int)x;
		ley2 = ptr->lelbow->y + (int)y;

		//try the estimates
		if (!searchLeftHandEstimates(pic,ex,ey,lex,ley,lex2,ley2))
		{
			ptr->left->x = ex;
			ptr->left->y = ey;
		}
	}
	else
	{
		ptr->left->x = prev->left->x;
		ptr->left->y = prev->left->y;
	}
	
	//right
	
	//skip?
	if (!skipRight)
	{
		//the angle
		angle = atan(rc);
		
		//first estimate
		x = prev->rarm->length * cos(angle);
		y = prev->rarm->length * sin(angle);
		
		ex = ptr->relbow->x - (int)x;
		ey = ptr->relbow->y - (int)y;
		
		//shorter estimate
		x = prev->rarm->length*0.7 * cos(angle);
		y = prev->rarm->length*0.7 * sin(angle);
				
		rex = ptr->relbow->x - (int)x;
		rey = ptr->relbow->y - (int)y;
		
		//longer estimate
		x = prev->rarm->length*1.2 * cos(angle);
		y = prev->rarm->length*1.2 * sin(angle);
		
		rex2 = ptr->relbow->x - (int)x;
		rey2 = ptr->relbow->y - (int)y;

		//try the estimates
		if (!searchRightHandEstimates(pic,ex,ey,rex,rey,rex2,rey2))
		{
			ptr->right->x = ex;
			ptr->right->y = ey;
		}
	}
	else
	{
		ptr->right->x = prev->right->x;
		ptr->right->y = prev->right->y;
	}
	
	
}


//Tries to find the best estimate of the left hand's position
//in:image to check, the estimates of 1. first, shorter,longer
//out:if able to find estimates

bool Tracker:: searchLeftHandEstimates(QImage pic,int ex,int ey,int x,int y,int x2, int y2)
{
	//minimum
	if (  (pow(ptr->lelbow->x-ex,2)+ pow(ptr->lelbow->y-ey,2)) < 25)
	{
		return false;
	}
	
	//check if the loop will execute right way around
	int x1,y1;
	if (x > x2)
	{
		x1 = x2;
		x2 = x;
	}else
	{
		x1 = x;
	}
	if (y > y2)
	{
		y1 = y2;
		y2 = y;
	}else
	{
		y1 = y;
	}
	
	double bestDist = w*w+h*h,dist;
	int bestx,besty;
	bool found = false;
	int i,j,r,g,b;
	QRgb col;
	
	
	for (i = x1;i<x2;i++)
	{
		if (i>0 && i<w)
		{
			for (j = y1;j<y2;j++)
			{
				if (j>0 && j<h)
				{
					if (!(i >= hl && i<= hr && j>=ht && j<=hb))
					{
						col = pic.pixel(i,j);
						r = qRed(col);
						g = qGreen(col);
						b = qBlue(col);
						
						//test skin data
						if (((s_ur - skin_t*s_sr) < r) && ((s_ur + skin_t*s_sr) > r) && ((s_ug - skin_t*s_sg) < g) && ((s_ug + skin_t*s_sg) > g) && ((s_ub - skin_t*s_sb) < b) && ((s_ub + skin_t*s_sb) > b))
						{
							//better estimate?
							if (sqrt( pow(ex-i,2) + pow(ey-j,2) ) < bestDist)
							{
								//size correct?
								dist = sqrt(pow(ptr->lelbow->x - i,2) + pow(ptr->lelbow->y-j,2));
								if (dist >= minArmLeft && dist <= maxArmLeft)
								{
									found = true;
									bestx = i;
									besty = j;
								}
							}
						}
					}
				}
			}
		}
	}
	//yes!!!
	if (found)
	{
		ptr->left->x = bestx;
		ptr->left->y = besty;
	}
	return found;
}


//Tries to find the best estimate of the right hand's position
//in:image to check, the estimates of 1. first, shorter,longer
//out:if able to find estimates

bool Tracker:: searchRightHandEstimates(QImage pic,int ex,int ey,int x,int y,int x2, int y2)
{
	if (  (pow(ptr->relbow->x-ex,2)+ pow(ptr->relbow->y-ey,2)) < 25)
	{
		return false;
	}
	
	//check if the loop will execute right way around
	int x1,y1;
	if (x > x2)
	{
		x1 = x2;
		x2 = x;
	}else
	{
		x1 = x;
	}
	if (y > y2)
	{
		y1 = y2;
		y2 = y;
	}else
	{
		y1 = y;
	}
	
	double bestDist = w*w+h*h,dist;
	int bestx,besty;
	bool found = false;
	int i,j,r,g,b;
	QRgb col;
	
	for (i = x1;i<x2;i++)
	{
		if (i>0 && i<w)
		{
			for (j = y1;j<y2;j++)
			{
				if (j>0 && j<h)
				{
					if (!(i >= hl && i<= hr && j>=ht && j<=hb))
					{
						col = pic.pixel(i,j);
						r = qRed(col);
						g = qGreen(col);
						b = qBlue(col);
						
						//skin test
						if (((s_ur - skin_t*s_sr) < r) && ((s_ur + skin_t*s_sr) > r) && ((s_ug - skin_t*s_sg) < g) && ((s_ug + skin_t*s_sg) > g) && ((s_ub - skin_t*s_sb) < b) && ((s_ub + skin_t*s_sb) > b))
						{
							//better estimate?
							if (sqrt( pow(ex-i,2) + pow(ey-j,2) ) < bestDist)
							{
								//size correct?
								dist = sqrt(pow(ptr->relbow->x - i,2) + pow(ptr->relbow->y-j,2));
								if (dist >= minArmRight && dist <= maxArmRight)
								{
									found = true;
									bestx = i;
									besty = j;
								}
							}
						}
					}
				}
			}
		}
	}
	//yes!!!
	if (found)
	{
		ptr->right->x = bestx;
		ptr->right->y = besty;
	}
	return found;
}

//this performs an area check using an oriented bounding box on the left arm
//in: coordinate to check
//out:if it passes as a suitable choice

bool Tracker:: searchLeftParams(int x,int y)
{
	//check eerste die kop
	if (x >= hl && x<= hr && y>=ht && y<=hb)
	{
		return false;
	}
	
	
	int place = (int)(prev->larm->length);
	
	int center_x = prev->lelbow->x + (prev->left->x - prev->lelbow->x)/2;
	
	//nou of dit pas in die bounding box
	
	float c = y - prev->larm->m*x;
	
	if (abs((int)(prev->larm->c - c)) < place/2)
	{	
		if (x < center_x)
		{
			if (x > ( center_x-0.66*place) )
			{
				return true;
			}
		}else{
			if (x < (center_x+place*0.66) )
			{
				return true;
			}
		}
		
	}
	return false;
}

//this perfroms an area check using an oriented bounding box on the right arm
//in: coordinate to check
//out:if it passes as a suitable choice

bool Tracker:: searchRightParams(int x,int y)
{
	//check eerste die kop
	if (x >= hl && x<= hr && y>=ht && y<=hb)
	{
		return false;
	}
	
	
	int place = (int)(prev->rarm->length);
	
	int center_x = prev->relbow->x + (prev->right->x - prev->relbow->x)/2;
	
	//nou of dit pas in die bounding box
	
	float c = y - prev->rarm->m*x;
	
	if (abs((int)(prev->rarm->c - c)) < place/2)
	{
		
		if (x < center_x)
		{
			if (x > ( center_x-place*0.66) )
			{
				return true;
			}
		}else{
			if (x < (center_x+0.66*place) )
			{
				return true;
			}
		}
		
	}
	return false;
}

