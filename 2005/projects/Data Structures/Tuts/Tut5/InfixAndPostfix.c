#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "InfixAndPostfix.h"

char *infixToPostfix(char *a){
	int i, j, N = strlen(a);		
	char *result; 
	stackNode s;
	itemType x;
	
	//Create space for the result string	
	//Initialize the stack s
	//for all the elements in a:
	//	1) if a[i] is a ")",
	//		pop the top item from the stack and write 
	//		the char value of the item in result.
	//		Also add a space to result.
	//		Be careful of memory leaks.
	//	
	//	2) if a[i] = "+" or "*",
	//		   push a[i] on the stack 	
	//
	//	3) if a[i] = "0 .. 9", 
	//		   while a[i] = "0 .. 9",
	//		   	 write a[i] in result
	//		   	 increment i
	//		   add a space to result
	//		   decrement i	
	//	
	//
	// Finally, add the "\0" at the end of result, delete the stack and return result.
	result = malloc(N);
	j = 0;
	i = 0;
	stackInit(&s);
	for(i=0;i<N;i++)
	{
		if (a[i] = ')')
		{
			x = stackPop(&s);
			result[j] = intValue(x);
			result[j+1] = ' ';
			j = j+2;
		}
		if ((a[i] == '+') || (a[i] == '*'))
		{
			x = createItem(CHAR, 12, a[i]);
			StackPush(&s,x);
		}
		if ((a[i] >= 0) & (a[i] <= 9))
		{
			while ((a[i] >= 0) & (a[i] <= 9))
			{
				result[j] = a[i];
				i++;
				j++;
			}
			result[j] = ' ';
			j++;
			i--;
		}
	}
}

itemType postfixEval(char *a){
	int i, N = strlen(a);
	stackNode s;
	itemType x, y;
	int tmp1;

	//Initialise the stack s
	//for all the characters in a:
	//	1) if a[i] = + or -, 
	//		   pop the two top items from the stack, add the 
	//   	 	 two it items, and push the result back on the stack.
	//   	 Be careful for memory leaks (items poped from the stack).
		
	//  2) if a[i] = 0 .. 9, 
	//  	 	 push a zero on the stack.
	//     	 while a[i] = 0 .. 9, 
	//     		 push(10 * pop() + (a[i] - '0'))
	//     		 increment i 
	//     Again, be careful of memory leaks.		
	//     
	//  3) Return the top item of the stack and delete the stack.

	stackInit(&s);
	for (i=0;i<N;i++)
	{
		if (a[i] == '+')
		{
			x = addItems(stackPop(&s),stackPop(&s));
			stackPush(&s, x);
		}

		if ((a[i] >= 0) & (a[i] <= 9))
		{
			stackPush(&s, NULL);
			while ((a[i] >= 0) & (a[i] <= 9))
			{
				x = stackPop(&s);
				tmp1 = itemIntValue(x);
				tmp1 = (10 * tmp1) + (a[i] - 0);
				y = createItem(INT, tmp1, 'a');
				stackPush(&s, y);
				i++;
			}
		}
	}
	x = stackPop(&s);

	return x;
}
