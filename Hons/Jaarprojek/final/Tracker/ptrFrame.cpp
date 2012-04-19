#include "include/ptrFrame.h"
#include <stdio.h>
#include <stdlib.h>

//constructor of this object

ptrFrame:: ptrFrame()
{
	allocBody();
}

//allocate memory for body parts
void ptrFrame:: allocBody()
{
	if (!(lelbow = (point*)malloc(sizeof(struct point))))
	{
		printf("Malloc failed\n");
	}
	
	if (!(relbow = (point*)malloc(sizeof(struct point))))
	{
		printf("Malloc failed\n");
	}
	
	if (!(left = (point*)malloc(sizeof(struct point))))
	{
		printf("Malloc failed\n");
	}
	
	if (!(right = (point*)malloc(sizeof(struct point))))
	{
		printf("Malloc failed\n");
	}
	
	if (!(larm = (equation*)malloc(sizeof(struct equation))))
	{
		printf("Malloc failed\n");
	}
	
	if (!(rarm = (equation*)malloc(sizeof(struct equation))))
	{
		printf("Malloc failed\n");
	}
}
