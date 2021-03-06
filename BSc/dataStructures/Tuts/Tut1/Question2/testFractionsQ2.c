/*! ============================================================================

COMPONENT:                  T E S T F R A C T I O N S . C
AUTHOR: J.J.D. Bull
DESCRIPTION: A test program for the fraction ADT


VERSION:          1
DATED:            2004/02/12 
LAST MODIFIED BY: jbull

============================================================================= */


/*! ___STANDARD INCLUDES_____________________________________________________ */
#include <stdlib.h>
#include <stdio.h>


/*! ___PROJECT INCLUDES______________________________________________________ */
#include "fractionsQ2.h"


/*! ___LOCAL DEFINITIONS_____________________________________________________ */


/*! ___LOCAL VARIABLES_______________________________________________________ */


/*! ___MAIN FUNCTION________________________________________________________ */

/*! ***************************************************************************
Parameters:   none.
Returns:      User option.
Description:  Get a menu option entered by a user
Notes: 				none.
*******************************************************************************/
int GetChoice(void){
	int choice;
				
	printf("1. Add two fractions \n");
	printf("2. Subtract two fractions \n");
	printf("3. Multiply two fractions \n");
	printf("4. Divide two fractions \n");
	printf("5. Toggle print mode (proper or improper) \n");
	printf("6. Print equivalent real number to fraction \n");
	printf("7. Exit program \n");
	while (1){
		printf("\nPlease enter your selection and press return > ");
		scanf("%d", &choice);
		if ((choice >= 1) && (choice <= 7)){
			return choice;
		}
		else{
			printf("Your selection must be between 1 and 7.");
		}
	}
	return 0;
}


/*! ***************************************************************************
Parameters:   f - a fraction.
Returns:      none.
Description:  Prompt user for a fraction and input as a string. If the 
							conversion to a fraction is not successful, the process is 
							repeated.   
Notes: 				none.
*******************************************************************************/
void GetFraction(fraction *f){
	char strFraction[MAXSTRLEN];
  char success;	

	success = 0;
	while (success == 0){
		printf("enter fraction > ");
		scanf("%s", strFraction);
		if ( (success = StringToFraction(strFraction, f)) == 0){
			printf("Conversion unseccessful, try again \n");
		}	
		else{
			success = 1;
		}
	}
}


int main(){
	fraction f1, f2, result;
	char inOutProperFunction, end;
	char strFraction[MAXSTRLEN];
	
	inOutProperFunction = 1;
	end = 0;
	while (end == 0){
		switch (GetChoice()){
			case 1 :
				GetFraction(&f1); GetFraction(&f2);
			  AddFractions(f1, f2, &result);	
				FractionToString(result, inOutProperFunction, strFraction);
				printf("result > %s \n", strFraction);
				break;
			case 2 :
				GetFraction(&f1); GetFraction(&f2);
			  SubtractFractions(f1, f2, &result);	
				FractionToString(result, inOutProperFunction, strFraction);
				printf("result > %s \n", strFraction);
				break;
			case 3 :					
				GetFraction(&f1); GetFraction(&f2);
			  MultiplyFractions(f1, f2, &result);	
				FractionToString(result, inOutProperFunction, strFraction);
				printf("result > %s \n", strFraction);
				break;
			case 4 :					
				GetFraction(&f1); GetFraction(&f2);
			  DivideFractions(f1, f2, &result);	
				FractionToString(result, inOutProperFunction, strFraction);
				printf("result > %s \n", strFraction);
				break;
			case 5 :
				inOutProperFunction = 1 - inOutProperFunction;				
				break;
			case 6 :
				GetFraction(&f1);
				printf("float value > %*.*f \n", 15, 6, FractionToFloat(f1));	
				break;
			case 7 :
				end = 1;				
				break;
		}
	}
	DisposeFraction(&f1); DisposeFraction(&f2); DisposeFraction(&result);
	return 0;
}
