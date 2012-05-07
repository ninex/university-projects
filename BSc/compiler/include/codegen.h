/********************************************************
 * 	Parser.h					*
 *							*
 * 	Written by Mia Meyer and Abrie Greeff		*
 *							*
 * 	Part of Software design: Compiler project (2005)*
 *							*
 *	Reference:	Compiler Construction, Wirth    *					*
 * 	Group: Andrew Mori, Eduard Bergh, Mia Meyer	*
 ********************************************************/



#ifndef CODEGEN_H
#define CODEGEN_H
/*
* printReg()
*
* Description : Prints the state of the register array to screen for debugging purposes
*
* Input : NONE
* Ouput : NONE
*
*/
void printReg();

/*
* testRange()
*
* Description : Tests whether a constant value falls within the allowed bounds
*
* Input : A variable of type cg_item
* Output : Integer representing success or failure
* 
*/
int testRange(cg_item *x);
/*
* initCodeGen()
* Description : Initilizes all registers to 0 (empty). Register 0 is reserved to stay 0. 
* Also opens output file that code is dumped to.
*
* Input : File name whose extension must be changed and if the debug file is enabled
* Output : NONE
*
*/
void initCodeGen(char *file, int debug);

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
void dumpToFile();

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
long negated(long cond);

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
void dumpDebugFile();

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
void Put(enum opcodes opc, unsigned long a, unsigned long b, unsigned long c);

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
int getReg();

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
void start(int disp);

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
void Close();

/*
* load()
*
* Description : Loads an operand x into a register.
*
* Input :
* Output : NONE
*
*/
void load(cg_item *x);

/*
* testRange()
*
* Description : Tests whether a constant value falls within the allowed bounds
*
* Input : A variable of type cg_item
* Output : Integer representing success or failure
* 
*/
void clearReg(int reg);

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
void op1(enum opcodes op, cg_item *x);

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
void op2(enum opcodes op, cg_item *x, cg_item *y);

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
void PutOp(enum opcodes cd, cg_item *x, cg_item *y);

/*
* printItem()
*
* Description :
*
* Input :  cg_item to be printed
* Output : NONE
*
*/
void printItem(cg_item *x);

/*
* printItems()
*
* Description : prints cg_items to be printed adjacently
*
* Input :  cg_items to be printed adjacently
* Output : NONE
*
*/
void printItems(cg_item *x,cg_item *y);

/*
* getopString()
*
* Description : Returns the opcode as a string to be written to screen for debugging purposes
*
* Input :	The opcode
* Output :	The opcode in string format
*
*/
char* getOpString(enum opcodes op);

/*
* procPC()
*
* Description :  Returns the program counter
*
* Input : NONE
* Output : The current program counter
*
*/
unsigned long procPC();

/*
* procCall()
*
* Description :  Puts the branching instruction for a procedure call
*
* Input :	cg_item x
* Output : NONE
*
*/
void procCall(call_node *x);

/*
* recursiveCall()
*
* Description :	 Puts the branching instruction for a recursive procedure call
*
* Input : The current program counter
* Output : NONE
*
*/
void recursiveCall(int pc);

/*
* relation()
*
* Description : Handles relations of the form x := x ? y; stores the answer as mode
*
* Input :   opcode and cg_items x and y
* Output :  None
*
*/
void relation(enum opcodes op, cg_item *x, cg_item *y);


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
void CJump(cg_item *x);

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

void BJump(long L);

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
long FJump(long L);

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
void fix(long at, long with);

/*
* fixUp()
*
* Description :   Called by fixLink to perform the patching of link
*
* Input :  memory index where fix should occur
* Output : NONE
*
*/
void fixUp(long L);

/*
* FixWith()
*
* Description :  Called by parser to fix a do statement.
*
* Input :  memory to be fixed at, memory to compute displacement with
* Output : NONE
*
*/
void FixWith(long L0, long L1);

/*
* FixLink()
*
* Description :  Called by parser to fix memory at parameter index.
*
* Input :  memory to be fixed at, memory to compute displacement with
* Output : The new memory index
*
*/

long FixLink(long L);

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
unsigned long procEnter(int size);

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
long merged(long L0, long L1);

/*
* procReturn()
*
* Description :  Sets the instructions that always occurs when a procedure returns
*
* Input :  NONE
* Output : NONE
*
*/
void procReturn();


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
unsigned long resetLevel();


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
unsigned long changeLevel();

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
unsigned long rootLevel();
#endif 
