#include <stdio.h>
#include "InfixAndPostfix.h"


void getInfixExpression(char a[]){
	char c;
	int index;
				
	printf("Enter an infix expression > ");
	fflush(NULL);
	c =  getchar();
	if (c == 10){     //! The newline received after enter was hit
		c =  getchar();
	}
	index = 0;
	while (c != 10){
		a[index] = c;
		c =  getchar();
		index++;
	}
	a[index] = '\0';
}		

int main(){
	itemType x;
	char a[128];	
	char *b;
	
	getInfixExpression(a);
	b = infixToPostfix(a);
	x = postfixEval(b);	
	printf("Result > %s \n", itemToString(x));
	return 0;
}


