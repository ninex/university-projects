/*	******************************************************************** *\ 
 *	Program description : This is a simple implementation of the     		*
 *							  	 quick sort algorithm.						 	  		*
 *																								*	
 *	Program summary	  : The program accepts a unsorted list and 			*
 *								 sorts the elements from small to large.			*
 *																							   *
 *	Exported Functions  : void QuickSort(...);		 						   *
 *																							   *
 *	Return values 		  : Returns a sorted list form small to large      *
 *																								*
 *	Comments 			  : Nothing 													*
 *																								*
 *	Programmer			  : J.J.D. Bull 												*
 *																								*
 *	Last updated		  : 02/05/2003 												*
 *																								*
\*	******************************************************************** */ 

#include<stdio.h>


#define ascending  	0
#define descending 	1


short Ordered(int x, int y, short order) {

	if (order == ascending )
		return x < y;
	else
		return x > y;
}


void Swap(int *x, int *y) {
	int copy;

	copy= *x;
	*x = *y;
	*y = copy;
}


void QuickSort(int list[], int Left, int Right, short SortOrder) {
	int Lindex, Rindex, Pivot;
	
	if ( Left < Right) {
		Lindex = Left;
		Rindex = Right;
		Pivot = list[(Lindex + Rindex) / 2];
		do {
			while ( (Lindex < Right) && (Ordered(list[Lindex], Pivot, SortOrder)) )
				Lindex++;
			while ( (Rindex > Left) && (Ordered(Pivot, list[Rindex], SortOrder)) )
				Rindex--;
			if ( Lindex <= Rindex ) {
				Swap(&list[Lindex], &list[Rindex]);
				Lindex++;
				Rindex--;
			}
		} while (Lindex <= Rindex);
		QuickSort(list, Left, Rindex, SortOrder);
		QuickSort(list, Lindex, Right, SortOrder);
	}  
}


void PrintList(int list[], int length) {
	int index = 0;

	while (index != length) {
		printf("%d ",list[index]);
		index++;
	}
	printf("\n\n");
}


int main() {
	int list[20] = {20,11,18,4,17,6,12,1,19,7,13,2,14,9,3,15,5,8,10,16};
		  
	PrintList(list, 20);
	QuickSort(list, 0, 19, ascending);
	PrintList(list, 20);
	return 0;
}
