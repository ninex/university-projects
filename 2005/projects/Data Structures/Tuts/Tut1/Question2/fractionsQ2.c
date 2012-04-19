/*! ============================================================================

COMPONENT:                  F R A C T I O N S . C
AUTHOR: J.J.D. Bull
DESCRIPTION: Provides operations to create, dispose, add, subtract, multiply,
						 and divide fractions. There are three conversion routines: string
						 to fraction, fraction to string and fraction to real. Fraction is 
						 an opaque type. Fractions are always stored and processed in 
						 reduced form.  


VERSION:          1
DATED:            2004/02/12 
LAST MODIFIED BY: jbull

============================================================================= */


/*! ___STANDARD INCLUDES_____________________________________________________ */
#include <stdlib.h>


/*! ___PROJECT INCLUDES______________________________________________________ */
#include "fractionsQ2.h"


/*! ___LOCAL DEFINITIONS_____________________________________________________ */


/*! ___LOCAL VARIABLES_______________________________________________________ */


/*! ___MAIN FUNCTION________________________________________________________ */

/*! ***************************************************************************
Parameters:   n1 - first number 
							n2 - second number
Returns:      Returns the greatest common divisor of two numbers >= 1.
Description:  Computes the greatest common divisor of two numbers >= 1
Notes: 
*******************************************************************************/
int GCD(int n1, int n2){

	return 1;
}


/*! ***************************************************************************
Parameters:   f - a fraction 
Returns:      A reduced fraction.
Description:  Reduce fraction f so that numerator and denominator have no 
							common factors.
Notes: 
*******************************************************************************/
void Reduce(fraction *f){
	int commonFactor;
	
}


/*! ***************************************************************************
Parameters:   numerator - the numerator of a fraction.
							denominator - the denominator of a fraction.
							f - a pointer to a fraction pointer.
Returns:      1 or -1 on error.
Description:  Create a fraction with the specified components. Only the 
							numerator may be negative and the denominator must be greater 
							than zero. Values greater than one are specified in improper 
							form, such as 5/2 for 2 1/2. Fractions will automatically be 
							reduced.
Notes: 
*******************************************************************************/
int CreateFraction(int numerator, int denominator, fraction *f){
	
	return 1;
}


/*! ***************************************************************************
Parameters:   f - a pointer to a fraction pointer..
Returns:      1 or -1 on error.
Description:  Dispose of the fraction f, which becomes undifined (NULL)
Notes: 
*******************************************************************************/
int DisposeFraction(fraction *f){

	return 1;
}


/*! ***************************************************************************
Parameters:   f1 - a pointer to a fraction.
							f2 - a pointer to a fraction.
							result - a pointer to a fraction pointer.
Returns:      1 or -1 on error.
Description:  Add fractions f1 and f2 to produce result. 
Notes: 
*******************************************************************************/
int AddFractions(fraction f1, fraction f2, fraction *result){
	int numerator, denominator;
	
	return 1;
}


/*! ***************************************************************************
Parameters:   f1 - a pointer to a fraction.
							f2 - a pointer to a fraction.
							result - a pointer to a fraction pointer.
Returns:      1 or -1 on error.
Description: Subtract fraction f2 from f1 to produce result. 
Notes: 
*******************************************************************************/
int SubtractFractions(fraction f1, fraction f2, fraction *result){
	int numerator, denominator;
	
	return 1;
}


/*! ***************************************************************************
Parameters:   f1 - a pointer to a fraction.
							f2 - a pointer to a fraction.
							result - a pointer to a fraction pointer.
Returns:      1 or -1 on error.
Description: Multiply fractions f1 and f2 to produce result. 
Notes: 
*******************************************************************************/
int MultiplyFractions(fraction f1, fraction f2, fraction *result){
	int numerator, denominator;
	
	return 1;
}


/*! ***************************************************************************
Parameters:   f1 - a pointer to a fraction.
							f2 - a pointer to a fraction.
							result - a pointer to a fraction pointer.
Returns:      1 or -1 on error.
Description: Divide fractions f1 and f2 to produce result. 
Notes: If f2 is initially equal to zero, CreateFraction will return error.
*******************************************************************************/
int DivideFractions(fraction f1, fraction f2, fraction *result){
	int numerator, denominator;
	
	return 1;
}


/*! ***************************************************************************
Parameters:   ch - a character.
Returns:      1 if true, 0 if false.
Description:  Return true (1) if ch is a digit.
Notes: 
*******************************************************************************/
int IsDigit(char ch){

	return 0;
}


/*! ***************************************************************************
Parameters:   s - a character string representing a fraction.
Returns:      nothing.
Description:  Advances index to the position of a nonblank character.
Notes: 
*******************************************************************************/
void SkipBlanks(char *s, int *index){
	
}


/*! ***************************************************************************
Parameters:   s - a character string representing a fraction.
							index  - a pointer to the index in s
							number - the numerical value of the number in s
Returns:      nothing.
Description:  Assuming index points to a digit character in s, GetNumber scans
							all continuous digits forming the corresponding number. Index
							ends up on the first nondigit character.
Notes: 
*******************************************************************************/
void GetNumber(char *s, int *index, int *number){

}


/*! ***************************************************************************
Parameters:   negative - boolean that indicates if the fraction is negative.
							whole - 
							denom -
							f - pointer to a fraction pointer.
Returns:      1 if successful and 0 if not.
Description:  Using the negative flag and numbers for the whole number part,
							numerator and denominator, this function forms the corresponding 
							reduced fraction, provided that the denominator is nonzero.
Notes: 
*******************************************************************************/
char AssignFraction(char negative, int whole, int num, int denom, fraction *f){
	char success;

	return 1;
}


/*! ***************************************************************************
Parameters:  s - a character string representing a fraction.
						 f - a pointer to a fraction pointer.
Returns:     1 on success, else 0;
Description: Attempt to convert the string input to a fraction. The string 
						 input may either be in proper form, such as -2 1/3, or improper
						 form, such as 17/3. Whole numbers are accepted and set to fraction
						 form, such as 5/1. If successful, f is set to the fraction, else
						 f is undifined (NULL).
Notes: 
*******************************************************************************/
int StringToFraction(char *s, fraction *f){
	int index, firstNum, secondNum, thirdNum;
	char negative;
				
	return 0;
}


/*! ***************************************************************************
Parameters:   num - a number.
							s - a pointer to a string
							index - the index into s
Returns:      nothing.
Description:  Convert num into a sequence of digit characters that are assigned 
							to the string at position index.
Notes: 
*******************************************************************************/
void ConvertNumber(int num, char *s, int *index){
	int digit;
				
}


/*! ***************************************************************************
Parameters:   f - a pointer to a fraction.
							properFraction - an indication how the fraction should be 
							printed.
						  s	- the resulting string.
Returns:      1 on success, else 0.
Description: Convert the fraction f to string format either as a mixed number,
						 such as 2 1/5, or as an improper fraction, such as 11/5, depending
						 on the boolean parameter properFraction.
Notes: 
*******************************************************************************/
int FractionToString(fraction f, char properFraction, char *s){
	int index, wholeNumber, numerator;
	
	return 1;
}


/*! ***************************************************************************
Parameters:   f - a pointer to a fractrion.
Returns:      a float with the value of f.
Description:  Convert fraction f to the corresponding float number.
Notes: 
*******************************************************************************/
float FractionToFloat(fraction f){

	return 0.0; 
}


