/********************************************************
 * 	Parser.c					*
 *							*
 * 	Written by Mia Meyer and Abrie Greeff		*
 *							*
 * 	Part of Software design: Compiler project2005)  *
 *							*
 *	Reference:	Compiler Construction, Wirth	*
 *							*
 * 	Group: Andrew Mori, Eduard Bergh, Mia Meyer	*
 ********************************************************/


#include "include/types.h"
#include "include/opcodes.h"
#include "include/codegen.h"
#include "include/asm.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MEM_SIZE 1024*1024
#define REG_MAX 32
#define MAX_INTERMEDIATE_VALUE 0x10000
#define GFP 28
#define FP 29
#define SP 30
#define LNK 31


#ifdef DEBUG_CODEGEN
	#define print(x, ...) printf(x, ## __VA_ARGS__)
#else
	#define print(x, ...)
#endif

//global variables
unsigned long pc;
unsigned int count;
static long R;
static unsigned long baseAddr;
static int size,root;
static long pc0;
long memory[MEM_SIZE/4];
FILE *f;
FILE *d;
char output[255], debugFile[255];


/*
* printReg()
*
* Description : Prints the state of the register array to screen for debugging purposes
*
* Input : NONE
* Ouput : NONE
*
*/

 void printReg() {

  print("print_reg\n");
  unsigned int i;
  i = 0;
  while ( i < 32 ) {
  	if (!(bt(R,i))) {
  		print("R[%d] = %d\n", i, bt(R,i));
	}
	i++;
  }
}


/*
* initCodeGen()
* Description : Initilizes all registers to 0 (empty). Register 0 is reserved to stay 0. 
* Also opens output file that code is dumped to.
*
* Input : File name whose extension must be changed and if the debug file is enabled
* Output : NONE
*
*/

void initCodeGen(char *file,int debug) {

	int i,dotPos,len;
	char compileFile[255];
	i=0;
	count = 0;
	pc = 0;
	dotPos = 0;
	len = strlen(file);
	sprintf(compileFile,"%s",file);
	while (dotPos != len) {
		if (compileFile[dotPos] == '.') {
			compileFile[dotPos] = '\0';
			break;
		}
		dotPos = dotPos + 1;
	}
	
	sprintf(debugFile,"%s.dbg",compileFile);
	sprintf(compileFile,"%s.cpl",compileFile);
	
	sprintf(output,"rm -f %s",debugFile);
	system(output);
	sprintf(output,"rm -f %s",compileFile);
	system(output);
	f=fopen(compileFile,"w");
	if (debug) {
		d=fopen(debugFile,"w");
	}
	else {
		d = NULL;
	}
	R = 0;
	root = FALSE;
	baseAddr = FP;
}

/*
* start()
*
* Description : This procedure is called to Put the first code of all programs. The
*      global link (initialized to 0) and the frame pointer is initialized.
*
* Input :
* Output : NONE
* 
*/
void start(int disp) {

	pc0 = pc;
	Put(F1_PSH, LNK, SP, 4);   
 	Put(F1_ADD, GFP, 0, FP);
	size = disp;
	
}

/*
* Close()
*
* Description : This procedure is used to clean the stack after all code has been 
* executed. It pops the link and the interpreter stops as soons as it sees that the 
* last RET returns to 0.
* Input : NONE
* Output : NONE
* 
*/

void Close() {
	
	Put(F1_POP, LNK, SP, 4);   
 	Put(F2_RET, 0, 0, LNK);    
	
}


/*
* testRange()
*
* Description : Tests whether a constant value falls within the allowed bounds
*
* Input : A variable of type cg_item
* Output : Integer representing success or failure
* 
*/

int testRange(cg_item *x) {

	if ((x->c >= MAX_INTERMEDIATE_VALUE) || (x->c < -MAX_INTERMEDIATE_VALUE) ){
		//value too large
		return FALSE;
		
	}
	else {
		return TRUE;
	}
	
}

/*
* clearReg()
*
* Description : Used to clear a register
*
* Input : the register to clear
* Output : NONE
* 
*/

void clearReg(int reg) {

	btr(R,reg);
	
}

/*
* dumpToFile()
*
* Description :  This procedure dumps the generated code to the file to be 
* interpreted by the RISC machine. The code is initialized with the global FP,
* total memory size, the program counter where the first instruction should be executed.
* 
* Input : NONE
* Output : NONE
* 
*/

void dumpToFile() {
	
	long int global_FP = MEM_SIZE-size;
	long int total_mem = MEM_SIZE;
	//initialize the global FP
	fwrite(&global_FP,sizeof(long),1,f);
	//sets the total memory size (needed by the interpreter)
	fwrite(&total_mem,sizeof(long),1,f);
	//sets the program counter where the first instruction should be executed.
	fwrite(&pc0,sizeof(long),1,f);
	fwrite(&memory,sizeof(long),count,f);
	
	//dumps the fixed links to the debug file
	if (d != NULL) {
		dumpDebugFile();
		fclose(d);
		sprintf(output,"kwrite --caption Debug %s",debugFile);
		system(output);
	}
	
	fclose(f);
}

/*
* getReg()
*
* Description : This procedure returns an int to be used as index for free register. If 
* no registers are available a negative value (-1) is returned. The register set is 
* simulated by a long and bits are set to indicate that a register is in use. To free a 
* register clearReg() is called to set the bit to 0; Registers 0, 28, 29, 30 and 31 are 
* reserved. 
*
* Input : NONE
* Output : An available register number. -1 if no register is avalaible.
*
*/

int getReg() {

	unsigned int i;
	i=1;
	while (i < 28) {		
		
		if ( bt(R,i) == 1 ) {
			i++;
		}
		else {
			bts(R,i);
			return i;
		}
	}
	return -1;
	
}

/*
* rootLevel()
*
* Description : This procedure is used to set the baseAddress to the global frame 
* pointer. This will only occur if the current variable is global.
*
* Input :  None 
* Output : The baseAddress of the global frame pointer
*
*/

unsigned long rootLevel() {
		
	if (baseAddr == FP) {
		baseAddr = GFP;
	}
	return baseAddr;
}


/*
* changeLevel()
*
* Description : This procedure changes the baseAddress to the required baseAddress of 
* the current variable in question. 
*
* Input : NONE
* Output : The baseAddress is changed to the required baseAddress of the current 
* level of the variable in question. 
* 
*/

unsigned long changeLevel() {

	int r;
	int c;

	if (baseAddr == FP) {
		r = getReg();
		c = 0;
		Put(F1_LDW,r,baseAddr,c);
		baseAddr = GFP;
	}
	return baseAddr;
	
}

/*
* resetLevel()
*
* Description :  Sets the baseAddress to the current activation record's framepointer
*
* Input : NONE
* Output :  Returns the baseAddress of the current activation record's framepointer
* 
*/

unsigned long resetLevel() {

	if (baseAddr != FP) {
		baseAddr = FP;
	}
	return baseAddr;
	
}

/*
* dumpDebugFile()
*
* Description : This procedure dumps the final codes in user readable format if 
* debugging is enabled.
*
* Input : NONE
* Output : NONE
* 
*/
void dumpDebugFile() {

	unsigned long code,a,b,c,buffer,i;
	
	i = 0;
	while (i < pc) {
		buffer = memory[i];
		code = (unsigned long) (buffer >> 26);
		//sets up codes of the F1 type into user readable form and prints to file
		if ( (code > NO_CODE) && (code <= F1_RD) ) {
			a = (buffer << 6) >> 27;
			b = (buffer << 11) >> 27;
			c = (buffer << 16) >> 16;
			if (bt(c,15)) {
				btr(c,15);
				c = -c;
			}
			fprintf(d,"%s\t%lu,%lu,%d\n",getOpString(code),a,b,(int)c);
		}
		//sets up codes of the F2 type into user readable form and prints to file
		else if ((code >= F2_ADDI)&&(code <= F2_RET)) {
			a = (buffer<<6)>>27;
			b = (buffer<<11)>>27;
			c = (buffer<<16)>>16;
			if (bt(c,15)) {
				btr(c,15);
				c = -c;
			}
			fprintf(d,"%s\t%lu,%lu,%d\n",getOpString(code),a,b,(int)c);
		}
		//sets up codes of the F3 type into user readable form and prints to file
		else if (code >= F3_JSR) {
			c = (buffer<<6)>>6;
			if (bt(c,25)) {
				btr(c,25);
				c = -c;
			}
			fprintf(d,"%s\t%lu\n",getOpString(code),c);
		}
		else {
			fprintf(d,"Not a valid code constant\n");
		}
		i++;
	}
}
	
/*
* Put()
*
* Description : Writes the opcodes as an unsigned int to the memory array. Instructions 
* may be fixed later to add the appropriate branching displays. The parameters are 
* shifted to extract the desired operands. For debugging the instructions are also 
* written to the screen.
*
* Input :  The opcode and operands a,b and c
* Output : NONE
*
*/

void Put(enum opcodes opc, unsigned long a, unsigned long b,unsigned long c) {
	
	unsigned long buf;
	unsigned long op = (unsigned long) opc;
	op = op << 26;
	
	//            6     5   5   16
	//case F1: opcode | a | b | - 
	if (opc < F2_ADDI ) {
		print("%ld: %s\t%lu,%lu,%d\n",pc+1,getOpString(opc),a,b,(int)c);
		a = a << 21;
		b = b << 16;
		
		if ((int)c < 0) {
			c = abs((int)c);
			bts(c,15);
		}
		c = (c << 16) >> 16;
		buf = op | a | b | c; 
	}
	//	      6	    5	5   11   5
	//case F2: opcode | a | b | -- | c
	else if (opc < F3_JSR) {
		print("%ld: %s\t%lu,%lu,%d\n",pc+1,getOpString(opc),a,b,(int)c);
		a = a << 21;
		b = b << 16;
		if ((int)c < 0) {
			c = abs((int)c);
			bts(c,15);
		}
		buf = op | a | b | c;
	}
	else {
	//            6     26
	//case F3: opcode | c
		print("%ld: %s\t%lu\n",pc+1,getOpString(opc),c);
		if ((int)c < 0) {
			c = abs((int)c);
			bts(c,25);
		}
		buf = op | c;
	} 
	
	pc++;
	memory[count] = buf;
	count++;
}


/*
* Store()
*
* Description : Store a value in a variable. If the variable is a condition the 
* appropriate branching instructions are called.
*
* Input : 
* Output : NONE
* 
*/

void Store(struct cg_item *x,struct cg_item *y) {
	
	int r;
	
	//adds the branching instructions for boolean conditions
	if (y->mode == cond) {
		Put(F1_BEQ+negated(y->c),y->r,0,y->a);
		clearReg(y->r);
		y->a = pc -1;
		y->b = FixLink(y->b);
		y->r = getReg();
		Put(F2_ADDI,y->r,0,1);
		Put(F1_BEQ,0,0,2);
		y->a = FixLink(y->a);
		Put(F2_ADDI,y->r,0,0);
	}
	// For constants the value is loaded into a register (if not yet in
	// one),rangechecking is performed
	if (y->mode == cons) {
		if (x->mode != cons) {
			r = getReg();
			if (testRange(y)) {
				Put(F2_ADDI, r,0,y->c);
			}
			else {
				//splitting value (value out of range)
				int r2 = getReg();
				Put(F2_ADDI,r,0,(y->c)>> 16);
				Put(F2_LSHI,r,r,16);
				Put(F2_ADDI,r,r,(y->c)& 0x00007FFF);
				Put(F2_ADDI,r2,0,((y->c)& 0x00008000)>>2);
				Put(F2_LSHI,r2,r2,2);
				Put(F1_ADD,r,r,r2);
				clearReg(r2);
			}
			if (x->isArray) {
				Put(F1_STW,r,x->r,0);
			}
			else {
				Put(F1_STW,r,x->baseAddr,x->a);
			}
			clearReg(r);
			clearReg(y->r);
		}
	}
	else if (x->type == y->type) {
		if (y->mode != reg) {
			load(y);
		}
		if (y->isArray) {
			Put(F1_LDW,y->r,y->r,0);
		}
		if (x->isArray) {
			Put(F1_STW,y->r,x->r,0);
		}
		if (x->mode == var) {
			Put(F1_STW,y->r,x->baseAddr,x->a);
		}
		clearReg(x->r);
		clearReg(y->r);
	}
	else {
		print("Store: incompatible assignment\n");
	}
	
} 



/*
* load()
*
* Description : Loads an operand x into a register.
*
* Input :
* Output : NONE
*
*/

void load(cg_item *x) {
	int r;
	//if item is a variable
	if (x->mode == var) {
		r = getReg();
		Put(F1_LDW, r, baseAddr , x->a);
		x->r = r;
	}	
	// if item is a const or array
	else {
		if ( x->mode == cons) {
			if (x->c == 0) {
				//not needed to actually load (optimization)
				x->r = 0;
			}	
			//testing constant bigger than 16 bits
 			else {
				if (testRange(x)) {
					x->r = getReg();
					Put(F2_ADDI, x->r, 0, x->c);
				}
				else {
					print("splitting value\n");
					int r = getReg();
					int r2 = getReg();
	
					Put(F2_ADDI,r,0,(x->c)>> 16);
					Put(F2_LSHI,r,r,16);
					Put(F2_ADDI,r,r,(x->c)& 0x00007FFF);
					Put(F2_ADDI,r2,0,((x->c)& 0x00008000)>>2);
					Put(F2_LSHI,r2,r2,2);
					Put(F1_ADD,r,r,r2);
					x->r = r;
				}
			}
		}
		else {
			if (x->isArray) {
				Put(F1_LDW,x->r,x->r,0);
			}
		}
	}
	x->mode = reg;  // item is now stored in a register	
}


/*
* loadBool()
*
* Description : Loads a boolean item.and sets it to condition mode
*
* Input : cg_item
* Output : NONE
*
*/

void loadBool(cg_item *x) {

	if ((x->type != t_bool) && (x->type != a_bool) && (x->type != c_bool)){
		print("[codegen] item does not have a boolean type");
	}
	
	load(x);
	x->mode = cond;
	x->a = 0;
	x->b = 0;
	x->c = 1;
}

/*
* PutOp()
*
* Description : Receives an instruction of the form x op y and reformats the 
* instruction into a format readable by the RISC architecture. We assume that the parser 
* will forward a constant as the y item instead of the x item in all cases.
*
* Input : opcode, cg_item x and y
* Output : NONE
*
*/
void PutOp(enum opcodes cd, cg_item *x, cg_item *y) {

	unsigned long r;
	int r1;
	int r2;
	
	//make sure x is loaded into a register
	if (x->mode != reg ) {
		load(x);
	}
	if (x->r == 0) {
		r = 0;
	}
	else {
		r = x->r;
		if (x->isArray) {
			Put(F1_LDW,x->r,x->r,0);
			x->isArray = FALSE;
		}
	}
	//if y is a constant test integer out of bounds
	if (y->mode == cons) {
		if (testRange(y)) {
			Put(cd, r, x->r, y->c);
		}
		else {
			print("splitting value\n");
			r1 = getReg();
			r2 = getReg();

			Put(F2_ADDI,r1,0,(y->c)>> 16);
			Put(F2_LSHI,r1,r1,16);
			Put(F2_ADDI,r1,r1,(y->c)& 0x00007FFF);
			Put(F2_ADDI,r2,0,((y->c)& 0x00008000)>>2);
			Put(F2_LSHI,r2,r2,2);
			Put(F1_ADD,r1,r1,r2);
		}
	}
	//else load y into an open register, put instruction and clear register after use
	else {
		if (y->mode != reg) {
			load(y);
		}
		else {
			if (y->isArray) {
				Put(F1_LDW,y->r,y->r,0);
			}
		}
		Put(cd, x->r, r, y->r);
		clearReg(y->r);
	}
}


/*
* op1()
*
* Description : Called if only one item is used in the operation. op1() calls Put to the
* operation to the . This is for operations of the form x := op x;
*
* Input : the op and cg_item x the operation will be performed on
* Output : NONE
*
*/

void op1(enum opcodes op, cg_item *x) {

	long t;
	
	switch(op){
		case ( F3_UNARY ): 
			///check types
			if ((x->type != t_int ) && (x->type != c_int) && (x->type != a_int )) {
				print("Cannot perform unary minus operation on a boolean variable!!\n"  );
			}
			// item is a constant
			if (x->mode == cons) {
				x->c = -x->c;   
			}
			// item is a variable
			else {
				if (x->mode == var) {
					load(x);
				}
				if (x->isArray) {
					Put(F1_LDW,x->r,x->r,0);
					x->isArray = FALSE;
				}
				Put(F1_SUB,x->r,0,x->r); 
			}
			break;
		
		case ( F3_NOT ): 
			//if x is not yet loaded load x and set to type cond
			if (x->mode != cond) {
				loadBool(x);
			}
			x->c = negated(x->c);
			t = x->a;
			x->a = x->b;
			x->b = t;
			break;
			
		case ( F1_AND ):
			//if x is not yet loaded load x and set to type cond
			if (x->mode != cond) {
				loadBool(x);
			}
			//hierdie is code vir die true jump
			Put(F1_BEQ + negated(x->c), x->r, 0, x->a);
			clearReg(x->r);
			x->a = pc-1;
			FixLink(x->b);
			x->b = 0;
			break;
			
		case ( F1_OR ):
			//if x is not loaded yet load x and set to type cond
			if (x->mode != cond){
				loadBool(x);
			}
			//put instruction and fix link
			Put(F1_BEQ + x->c, x->r, 0, x->b);
			clearReg(x->r);
			x->b = pc-1;
			FixLink(x->a);
			x->a = 0;
			break;
			
		case ( F3_WRD ):
			//if x is not loaded yet load x and set to type cond
			if (x->mode != reg){
				load(x);
			}
			//put instruction
			Put(F3_WRD,x->r,x->baseAddr,x->r);
			break;
			
		case ( F1_BLT):
		 	//if x is not loaded yet load x and set to type cond
			if (x->mode != reg) {
				load(x);
			}
			//put instruction
			Put(F1_BLT,x->r,x->baseAddr,2);
			break;
			
		case ( F1_RD):
			//if x is not loaded yet load x and set to type cond
			if (x->mode != reg) {
				load(x);
			}
			//put instruction
			Put(F1_RD,x->r,x->baseAddr,x->a);
			break;
		default:
			printf("unknown opcode\n");
			break;
	}
}

/*
* op2()
*
* Description : For opcodes of the form x op y
*
* Input :  opcode op and items x and y
* Output : NONE
*
*/

void op2(enum opcodes op, cg_item *x, cg_item *y) {

	//handles op2 codes of type integer. Most ops are hanled by the default case
	if  ( ( (x->type == t_int) || (y->type == t_int) ) || 
  		((x->type == a_int) || (y->type == a_int) ) || 
  		((x->type == c_int) || (y->type == c_int) ) ) {
		switch (op) {
				case F1_STW:
					Store(x,y);
					break;
				default:
					PutOp(op,x,y);
		}
	}
	//handles op2 codes of type boolean
	else if   ( ( (x->type == t_bool) && (y->type == t_bool) ) || 
  		((x->type == a_bool) && (y->type == a_bool) ) || 
  		((x->type == c_bool) && (y->type == c_bool) ) ) {	 
		
		//loads y into register and set mode to cond
 		if (y->mode != cond) {
 			loadBool(y);
 		}
		//handle or
		if (op == F1_OR) {
			x->a = y->b;
			x->b = merged(y->b,x->b);
			x->c = y->c;
		}
		//handle and
		else if ( op == F1_AND) {
			x->a = merged(y->a,x->a);
			x->b = y->b;
			x->c = y->c;
		}
		//handle store
		else if (op == F1_STW) {
			Store(x,y);
		}
		else {
			print("[codegen] no op");
		}
	}
	else {
		
	}
}

/*
* relation()
*
* Description : Handles relations of the form x := x ? y; stores the answer as mode
*
* Input :   opcode and cg_items x and y
* Output :  None
*
*/

void relation(enum opcodes op, cg_item *x, cg_item *y) {
	
	enum rel_ops rel;
	
 	//do a compare between the items
 	if (y->mode == cons) {
		PutOp(F2_CMPI,x,y);
 		
 	}
	else {
		PutOp(F1_CMP,x,y);
	}
 	
	//set the relation operation
	switch(op) {
		case F1_BLT:
				rel = rel_lt;
				break;
		case F1_BEQ:	
				rel = rel_eq;
				break;
				
		case F1_BGT:
				rel = rel_gt;
				break;
		default:
				printf("Invalid relation\n");
				exit(-1);
	}
	
	//set the relation in the item's x->c field and set mode to condition
	x->c = rel;
	clearReg(y->r);
	x->mode = cond;
	x->type = t_bool;
	x->a = 0;
	x->b = 0;
}

/*
* merged()
*
* Description :  Merges the two memory addresses received and returns the merged address. 
* Used to fix up the instruction that should be jumped to with mutiple AND and OR relations.
* The first item's jump-to value will be set to this merged value.
*
* Input :	long L1 and L2
* Output :	merger of L1 and L2
*
*/

long merged(long L0,long L1) {

	long L2,L3;
	if (L0 != 0) {
		L2 = L0;
		while (1) {
			L3 = (memory[L2] << 16) >> 16;
			if (L3 == 0) {
				break;
			} 
			L2 = L3;
		}
		memory[L2] = memory[L2] - L3 + L1;
		return L0;
	}
	else {
		return L1;
	}
}

/*
* fix()
*
* Description :   Fixes the memory at index at with the new computed value. Called by the 
* parser to fixup a branch instruction when condition is false. Also called by fixLink.
* 		
*
* Input :  Memory addresses at and with.
* Output : NONE
*
*/

void fix(long at, long with) {
	memory[at] = ((memory[at] >> 16) << 16) + pc - with;	
}

/*
* fixUp()
*
* Description :   Called by fixLink to perform the patching of link
*
* Input :  memory index where fix should occur
* Output : NONE
*
*/
void fixUp(long L) {

	memory[L] = ((memory[L] >> 16) << 16) + pc - L;	
	
}

/*
* FixWith()
*
* Description :  Called by parser to fix a do statement.
*
* Input :  memory to be fixed at, memory to compute displacement with
* Output : NONE
*
*/

void FixWith(long L0, long L1) {

	long L2;
	while (L0 != 0) {
		L2 = (memory[L0] << 16) >> 16;
		fix(L0,L1);
		L0 = L2;
	}
	
}

/*
* FixLink()
*
* Description :  Called by parser to fix memory at parameter index.
*
* Input :  memory to be fixed at, memory to compute displacement with
* Output : The new memory index
*
*/

long FixLink(long L) {

	long L1;
	
	while (L != 0) {
		L1 = (memory[L] << 16) >> 16;
		fixUp(L);
		L = L1;
	}
	return L;
	
}

/*
* negated()
*
* Description :    This procedure finds the appropriate branching instruction for branching 
* instructions. The branching instruction must be the oppposite of the relation. 
*
* Input :	The current relation stored in cg_item field c
* Output :	returns the negated relation
*
*/

long negated(long cond) {
	
	if ((cond % 2) == 1) {
		return cond - 1;
		
	}
	else {
		return cond + 1;
	}
}

/*
* CJump()
*
* Description : Adds the opcodes for a conditional jump (when condition is true and if or do 
* should jump to end).
*
* Input : cg_item x
* Output : NONE
*
*/

void CJump(cg_item *x){
	//check for boolean type
	if ((x->type == t_bool) || (x->type == a_bool) || (x->type == c_bool)) {
		if (x->mode != cond) {
			loadBool(x);
		}
		Put(F1_BEQ + negated(x->c), x->r, 0, x->a);
		clearReg(x->r);
		
		x->b = FixLink(x->b);
		
		x->a = pc-1;
		
		}
	else {
		print("Cant perform CJump on non-boolean.\n");
	}
}

/*
* BJump()
*
* Description :  Sets the branch back instruction in a do statement as a jump relative to 
* the program counter.
*
* Input :	memory address L to jump back to. 
* Output : NONE
*
*/

void BJump(long L) {
	Put(F1_BEQ, 0, 0, L-pc);
}


/*
* FJump()
*
* Description :  Sets the forward jump instruction in a guarded command 
* the program counter.
*
* Input :  memory address L to jump back to. 
* Output : 
*
*/

long FJump(long L) {
	Put(F1_BEQ, 0, 0, L);
	return pc-1;
}

/*
* procEnter()
*
* Description :	Sets the instructions that always occurs when a procedure is called
*
* Input :	size = size of local variables
* Output :	Returns the pogram counter before the instructions occur. This is the first 
* 		instruction that should be executed when the procedure is called.
*
*/

unsigned long procEnter(int size) {
	unsigned long x=pc;
	//push link
	Put(F1_PSH,LNK,SP,4);
	//push frame pointer
	Put(F1_PSH, FP, SP, 4);
	//set framepointer to the stackpointer
	Put(F1_ADD, FP, 0, SP);
	//jump over local variables with stackpointer to assign local variable addresses
	
	if (size > 0) {
		Put(F2_SUBI, SP, SP, size/4);
	}
	return x;
}

/*
* procReturn()
*
* Description :  Sets the instructions that always occurs when a procedure returns
*
* Input :  NONE
* Output : NONE
*
*/

void procReturn() {
	//set the stackpointer to the framepointer
	Put(F1_ADD, SP, 0, FP);
	//pop framepointer
	Put(F1_POP, FP, SP, 4);
	//pop link
	Put(F1_POP, LNK, SP, 4); 
	//return to the link 
	Put(F2_RET,0,0,LNK);
}

/*
* procCall()
*
* Description :  Puts the branching instruction for a procedure call
*
* Input :	cg_item x
* Output : NONE
*
*/

void procCall(call_node *x) {
	Put(F1_BSR, 0, 0, x->pc );
}

/*
* recursiveCall()
*
* Description :	 Puts the branching instruction for a recursive procedure call
*
* Input : The current program counter
* Output : NONE
*
*/
void recursiveCall(int pc) {
	Put(F1_BSR, 0, 0, pc );
}

/*
* procPC()
*
* Description :  Returns the program counter
*
* Input : NONE
* Output : The current program counter
*
*/

unsigned long procPC() {
	return pc;
}

/*
* printItem()
*
* Description :
*
* Input :  cg_item to be printed
* Output : NONE
*
*/

void printItem(cg_item *x) {
	printf("Item contents\n");
	switch (x->mode) {
		case nomode: 
			printf("\tMode: nomode\n");
			break;
		case var: 
			printf("\tMode: var\n");
			break;
		case cons: 
			printf("\tMode: const\n");
			break;
		case reg: 
			printf("\tMode: reg\n");
			break;
		case cond:
			printf("\tMode: cond\n");
			break;
	}
	switch (x->type) {
		case no_type:
			printf("\tType: no type\n");
			break;
		case t_int:
			printf("\tType: integer\n");
			break;
		case t_bool:
			printf("\tType: boolean\n");
			break;
		case a_int:
			printf("\tType: array of integer\n");
			break;
		case a_bool:
			printf("\tType: array of boolean\n");
			break;
		case c_int:
			printf("\tType: integer constant\n");
			break;
		case c_bool:
			printf("\tType: boolean constant\n");
			break;
	}
	printf("\ta = %ld, r = %ld, b = %ld, c = %ld\n",x->a,x->r,x->b,x->c);
	if (x->isInt) {
		printf("\tisInt = TRUE\n");
	}
	else {
		printf("\tisInt = FALSE\n");
	}
	if (x->isArray) {
		printf("\tisArray = TRUE\n");
	}
	else {
		printf("\tisArray = FALSE\n");
	}
	if (x->varIndex) {
		printf("\tvarIndex = TRUE\n\n");
	}
	else {
		printf("\tvarIndex = FALSE\n\n");
	}
}

/*
* printItems()
*
* Description : prints cg_items to be printed adjacently
*
* Input :  cg_items to be printed adjacently
* Output : NONE
*
*/

void printItems(cg_item *x,cg_item *y) {
	printf("Item contents\n");
	switch (x->mode) {
		case nomode: 
			printf("\tMode: nomode\t");
			break;
		case var: 
			printf("\tMode: var\t");
			break;
		case cons: 
			printf("\tMode: const\t");
			break;
		case reg: 
			printf("\tMode: reg\t");
			break;
		case cond:
			printf("\tMode: cond\n");
			break;
	}
	switch (y->mode) {
		case nomode: 
			printf("\tMode: nomode\n");
			break;
		case var: 
			printf("\tMode: var\n");
			break;
		case cons: 
			printf("\tMode: const\n");
			break;
		case reg: 
			printf("\tMode: reg\n");
			break;
		case cond:
			printf("\tMode: cond\n");
			break;
	}
	switch (x->type) {
		case no_type:
			printf("\tType: no type\t");
			break;
		case t_int:
			printf("\tType: integer\t");
			break;
		case t_bool:
			printf("\tType: boolean\t");
			break;
		case a_int:
			printf("\tType: array of integer\t");
			break;
		case a_bool:
			printf("\tType: array of boolean\t");
			break;
		case c_int:
			printf("\tType: integer constant\t");
			break;
		case c_bool:
			printf("\tType: boolean constant\t");
			break;
	}
	switch (y->type) {
		case no_type:
			printf("\tType: no type\n");
			break;
		case t_int:
			printf("\tType: integer\n");
			break;
		case t_bool:
			printf("\tType: boolean\n");
			break;
		case a_int:
			printf("\tType: array of integer\n");
			break;
		case a_bool:
			printf("\tType: array of boolean\n");
			break;
		case c_int:
			printf("\tType: integer constant\n");
			break;
		case c_bool:
			printf("\tType: boolean constant\n");
			break;
	}
	printf("\ta = %ld, r = %ld, c = %ld",x->a,x->r,x->c);
	printf("\ta = %ld, r = %ld, c = %ld\n",y->a,y->r,y->c);
	if (x->isInt) {
		printf("\tisInt = TRUE\t");
	}
	else {
		printf("\tisInt = FALSE\t");
	}
	if (y->isInt) {
		printf("\tisInt = TRUE\n");
	}
	else {
		printf("\tisInt = FALSE\n");
	}
	if (x->isArray) {
		printf("\tisArray = TRUE\t");
	}
	else {
		printf("\tisArray = FALSE\t");
	}
	if (y->isArray) {
		printf("\tisArray = TRUE\n\n");
	}
	else {
		printf("\tisArray = FALSE\n\n");
	}
}

/*
* getopString()
*
* Description : Returns the opcode as a string to be written to screen for debugging purposes
*
* Input :	The opcode
* Output :	The opcode in string format
*
*/

char* getOpString(enum opcodes op) {

	switch (op) {
		case(NO_CODE):
			return "NO_CODE";
		case(F1_ADD):
			return "ADD";
		case(F1_SUB):
			return "SUB";
		case(F1_MUL):
			return "MUL";
		case(F1_DIV):
			return "DIV";
		case(F1_MOD):
			return "MOD";
		case(F1_CMP):
			return "CMP";
		case(F1_CHK):
			return "CHK";
		case(F1_AND):
			return "AND";
		case(F1_OR):
			return "OR";
		case(F1_LSH):
			return "LSH";
		case(F1_ASH):
			return "ASH";
		case(F1_LDW):
			return "LDW";
		case(F1_LDB):
			return "LDB";
		case(F1_STW):
			return "STW";
		case(F1_STB):
			return "STB";
		case(F1_POP):
			return "POP";
		case(F1_PSH):
			return "PSH";
		case(F1_BEQ):
			return "BEQ";
		case(F1_BNE):
			return "BNE";
		case(F1_BLT):
			return "BLT";
		case(F1_BGE):
			return "BGE";
		case(F1_BGT):
			return "BGT";
		case(F1_BLE):
			return "BLE";
		case(F1_BSR):
			return "BSR";
		case(F2_ADDI):
			return "ADDI";
		case(F2_SUBI):
			return "SUBI";
		case(F2_MULI):
			return "MULI";
		case(F2_DIVI):
			return "DIVI";
		case(F2_MODI):
			return "MODI";
		case(F2_CMPI):
			return "CMPI";
		case(F2_CHKI):
			return "CHKI";
		case(F2_ANDI):
			return "ANDI";
		case(F2_ORI):
			return "ORI";
		case(F2_LSHI):
			return "LSHI";
		case(F2_ASHI):
			return "ASHI";
		case(F2_RET):
			return "RET";
		case(F3_JSR):
			return "JSR";
		case(F3_WRD):
			return "WRD";
		case(F1_RD):
			return "RD";
		default:
			return "UNHANDLED OP";
	}
	return NULL;
}
