#include "include/types.h"
#include "include/opcodes.h"
#include "include/codegen.h"
#include "include/asm.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MEM_SIZE 1024*1024
#define REG_MAX 32
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
static long R[32];
static unsigned long inUse[32];
static unsigned long baseAddr=FP;
static int size,root;
static long pc0;
long memory[MEM_SIZE/4];
FILE *f= NULL;
FILE *d= NULL;

void printReg() {

  print("print_reg\n");
  unsigned int i;
  for (i = 0; i < (sizeof(R)/4); i++) {
  	if (R[i] != 0) {
  		printf("R[%d] = %ld\n", i, R[i]);
	}
  }
}


//Initilizes all registers to 0 (empy). Register 0 is reserved to stay 0.
void initCodeGen(){
	int i;
	i=0;
	count = 0;
	pc = 0;
	system("rm -f outa.txt");
	system("rm -f outaD.txt");
	f=fopen("outa.txt","w");
	d=fopen("outaD.txt","w");
	
	while (i < REG_MAX){
		R[i] = 0;
		inUse[i] = 0;
		i++;
	}
	
	root = FALSE;
}

void start(int disp){
	
	pc0 = pc;
	//Put(F1_ADD, FP, 0, SP);
	Put(F1_PSH, LNK, SP, 4);   
 	//Put(F1_ADD, FP, 0, SP);    //<--- init die interpreter FP sodat FP=SP
	Put(F1_ADD, GFP, 0, FP);
	size = disp;
}

void Close(){
	
	Put(F1_POP, LNK, SP, 4);   
 	Put(F2_RET, 0, 0, LNK);    //<--- init die interpreter FP sodat FP=SP
	
}

int testRange(cg_item *x){
	if ((x->c >= 0x10000) || (x->c < -0x10000) ){
		return FALSE;
		printf("value too large\n");
	}else
	{
		return TRUE;
	}

}

void clearReg(int reg)
{
	R[reg] = 0;
}

void dumpToFile()
{
	
	long int mem_size = MEM_SIZE-size;
	long int full = MEM_SIZE;
	print("mem_size = %d\n",mem_size);
	print("full = %d\n",full);
	fwrite(&mem_size,sizeof(long),1,f);
	fwrite(&full,sizeof(long),1,f);
	fwrite(&pc0,sizeof(long),1,f);
	fwrite(&memory,sizeof(long),count,f);
	fclose(f);
}

/*
 *   This procedure returns an int to be used as index for free register. If no registers are available a negative 
 *   value (-1) is returned.
 *
 */
int getReg(){
	unsigned int i;
	i=1;
	while (i < 28){		//32 == REGMAX rather use const
		if ( R[i] != 0 ){
			i++;
		}
		else
		{
			R[i] = 1;
			return i;
		}
	
	}
	return -1;
}



	
/*  
 *  This function writes the opcodes to a file as an unsigned int 
 *  
 */
void Put(enum opcodes opc, unsigned long a, unsigned long b,unsigned long c){
	unsigned long buf;
	
	unsigned long op = (unsigned long) opc;
	op = op << 26;
	
	//            6     5   5   16
	//case F1: opcode | a | b | - 
	if (opc < F2_ADDI ) {
		printf("%s\t%lu,%lu,%d\n",getOpString(opc),a,b,(int)c);
		fprintf(d,"%s\t%lu,%lu,%d\n",getOpString(opc),a,b,(int)c);
		a = a << 21;
		b = b << 16;
		
		if ((int)c < 0)
		{
			c = abs((int)c);
			bts(c,15);
		}
		
		c = (c << 16) >> 16;
		buf = op | a | b | c; 
	}
	//	      6	    5	5   11   5
	//case F2: opcode | a | b | -- | c
	else if (opc < F3_JSR){
		printf("%s\t%lu,%lu,%d\n",getOpString(opc),a,b,(int)c);
		fprintf(d,"%s\t%lu,%lu,%d\n",getOpString(opc),a,b,(int)c);
		a = a << 21;
		b = b << 16;
		if ((int)c < 0)
		{
			c = abs((int)c);
			bts(c,15);
		}
		buf = op | a | b | c;
		//printf("op = %lu buf = %lu\n",(unsigned long)opc,buf);
	}
	else{	
	//            6     26
	//case F3: opcode | c
		printf("%s\t%lu\n",getOpString(opc),c);
		fprintf(d,"%s\t%lu\n",getOpString(opc),c);
		
		if ((int)c < 0)
		{
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
 *   This function services requests to store a value in a variable 
 *   x := y
 */
void Store(struct cg_item *x,struct cg_item *y){
	int r;
	if (y->mode == cons){
	//====================================================================================================
		
		//HOEKOM TOETS ONS NIE VIR REGISTERS NIE, KODE IS VERANDER
		
	//====================================================================================================
		if (x->mode != cons){
			//printf("single assignment\n");
			r = getReg();
// 			Put(F2_ADDI, r,0,y->c);
// 			Put(F1_STW,r,baseAddr,x->a);
			
			
			if (testRange(y))
			{
				//x->r = getReg();
				Put(F2_ADDI, r,0,y->c);
				
				
				//Put(F1_STW,r,x->baseAddr,x->a);
				
				//Put(F2_ADDI, x->r, 0, x->c);
			}else
			{
				print("splitting value\n");
				int r2 = getReg();
				Put(F2_ADDI,r,0,(y->c)>> 16);
				Put(F2_LSHI,r,r,16);
				Put(F2_ADDI,r,r,(y->c)& 0x00007FFF);
				Put(F2_ADDI,r2,0,((y->c)& 0x00008000)>>2);
				Put(F2_LSHI,r2,r2,2);
				Put(F1_ADD,r,r,r2);
				
				print("higher = %u\n",((y->c))>>16);
				print("%u lower = %u\n",((y->c)<< 17)>>17,((y->c) & 0x0000FFFF));
				print("lower = %u\n",((y->c)<< 31)>>31);
				//Put(F1_STW,r,x->baseAddr,x->a);
				clearReg(r2);
			}
			if (x->isArray)
			{
				print("\t\t\tstoring array in store\n");
				Put(F1_STW,r,x->r,0);
			}else
			{
				Put(F1_STW,r,x->baseAddr,x->a);
			}
			
			clearReg(r);
			clearReg(y->r);
			
		}
	}
	else if (x->type == y->type) {
		if (y->mode != reg){
			load(y);
		}
		if (y->isArray)
		{
			print("\t\t\tloading y array in store\n");
			Put(F1_LDW,y->r,y->r,0);
		}
		if (x->isArray)
		{
			print("\t\t\tloading array in store\n");
			Put(F1_STW,y->r,x->r,0);
		}
		if (x->mode == var){
			Put(F1_STW,y->r,x->baseAddr,x->a);
		}
 		clearReg(x->r);
 		clearReg(y->r);
	}
	else{
		printf("Store: incompatible assignment\n");
	}
} 

unsigned long rootLevel(){
	int r;
	int c;
	long int full = MEM_SIZE/4;
	
	if (baseAddr == FP){
		/*r = getReg();
		c = 0;
		
		print("rootLevel\n");
		//Put(F2_ADDI, r,0, full);
		
		int r2 = getReg();

					
		int r3 = getReg();

		Put(F2_ADDI,r2,0,full>> 16);
		Put(F2_LSHI,r2,r2,16);
		Put(F2_ADDI,r2,r2,full& 0x00007FFF);
		Put(F2_ADDI,r3,0,(full& 0x00008000)>>2);
		Put(F2_LSHI,r3,r3,2);
		Put(F1_ADD,r2,r2,r3);
		
		Put(F1_ADD,r,0,r2);
		
		clearReg(r2);
		clearReg(r3);
		//clearReg();
		//Put(F1_LDW,r,baseAddr,c);*/
		baseAddr = GFP;
		//root = TRUE;
	}
	return baseAddr;
}

unsigned long changeLevel(){
	int r;
	int c;
	print("Level up\n");
	if (baseAddr == FP){
		r = getReg();
		c = 0;
		
		Put(F1_LDW,r,baseAddr,c);
		baseAddr = GFP;
		//root = FALSE;
	}
	else{
		//printf("error with changeLevel\n");
	}
	return baseAddr;
}

unsigned long resetLevel(){

	if (baseAddr != FP){
		//clearReg(baseAddr);
		//normal = TRUE;
		print("normal framepointer\n");
		baseAddr = FP;
		//root = FALSE;
	}
	else{
		//printf("error with resetLevel\n");
	}
	return baseAddr;
}

long negated(long c){
	if (c == 1){
		return 0;
	}else {
		return 1;
	}
}

/*
 *   This procedure loads an operand x into a register
 *
 */
void load(cg_item *x){
	int r;
	//if item is a variable
	if (x->mode == var){
		r = getReg();
		Put(F1_LDW, r, baseAddr , x->a);
		//clearReg(x->r);  
		x->r = r;
		//printf("Allocated register %ld\n",x->r);
	}	
	// if item is a const
	else
	{
		if ( x->mode == cons)
		{
			if (x->c == 0)
			{
 				x->r = 0;
 			}		
 			else
			{
				if (testRange(x))
				{
					x->r = getReg();
					Put(F2_ADDI, x->r, 0, x->c);
				}else
				{
					printf("splitting value\n");
					int r = getReg();
	// 				Put(F2_ADDI,x->r,0,(x->c)>> 16);
	// 				printf("higher = %d\n",((x->c))>>16);
	// 				Put(F2_LSHI,x->r,x->r,16);
	// 				Put(F2_ADDI,x->r,0,((x->c)<< 16)>>16);
	// 				printf("lower = %d\n",((x->c)<< 16)>>16);
					
					int r2 = getReg();
	
					Put(F2_ADDI,r,0,(x->c)>> 16);
					Put(F2_LSHI,r,r,16);
					Put(F2_ADDI,r,r,(x->c)& 0x00007FFF);
					Put(F2_ADDI,r2,0,((x->c)& 0x00008000)>>2);
					Put(F2_LSHI,r2,r2,2);
					Put(F1_ADD,r,r,r2);
				}
			}
		}
	}
	x->mode = reg;  // item is now stored in a register	
}

void loadBool(cg_item *x){
	if ((x->type != t_bool) && (x->type != a_bool) && (x->type != c_bool)){
		printf("[codegen] item does not have a boolean type");
	}	
	load(x);
	x->mode = cond;
	//x->a = 0;
	//x->b = 0;
	x->c = 1;
}

/*
 *  This operation receives an instruction and reformats the instruction 
 *  into a format readable by the RISC architecture
 */
void PutOp(enum opcodes cd, cg_item *x, cg_item *y){
	unsigned long r;
	//printItems(x,y);
	if (x->mode != reg ){	
		load(x);
	}
	if (x->r == 0){
		x->r = getReg();
		r = 0;
	}
	else {
		r = x->r;
		print("\t\t\there %d\n",x->isArray);
		if (x->isArray)
		{
			print("\t\t\tloading array in putop\n");
			Put(F1_LDW,x->r,x->r,0);
			//Put(F3_WRD,x->r,x->baseAddr,x->r);
			x->isArray = FALSE;
		}
		
	}
	if (y->mode == cons){
		if (testRange(y))
		{   			
			Put(cd, r, x->r, y->c);
		}else
		{
			print("splitting value\n");
			int r = getReg();
			int r2 = getReg();

			Put(F2_ADDI,r,0,(y->c)>> 16);
			Put(F2_LSHI,r,r,16);
			Put(F2_ADDI,r,r,(y->c)& 0x00007FFF);
			Put(F2_ADDI,r2,0,((y->c)& 0x00008000)>>2);
			Put(F2_LSHI,r2,r2,2);
			Put(F1_ADD,r,r,r2);
		
		}
		//R[y->r] = 0;
	}
	else{
		if (y->mode != reg){
			load(y);
		}else
		{
			printf("Ek is Abrie en ek suck");			
			if (y->isArray)
			{
				print("\t\t\tloading array in putop\n");
				Put(F1_LDW,y->r,y->r,0);
			}
		}
		
		Put(cd, x->r, r, y->r);
		//printf("%lu, %d, %d, %d\n",cd, x->r, r, y->r);
		clearReg(y->r);
		//printf("%lu, %d, %d, %d\n",cd, x->r, r, y->r);
		
	}
}


/*
 *   This procedure is called if only one item is used in the operation. op1() calls
 *   Put the operation to the output file.
 */
void op1(enum opcodes op, cg_item *x){

	long t;
	
	if (op == F3_UNARY){ //F3_UNARYMINUS){
		///om te onderskei tussen konstantes en veranderlikes (konst is < 0)
		if ((x->type != t_int ) && (x->type != c_int) && (x->type != a_int )){
			printf("Cannot perform unary minus operation on a boolean variable!!\n"  );
		}
		// item is a constant
		if (x->mode == cons){
			x->c = -x->c;   
		}
		// item is a variable
		else{
			if (x->mode == var){
				//printf("load\n");
				load(x);
			}
			if (x->isArray)
			{
				Put(F1_LDW,x->r,x->r,0);
				x->isArray = FALSE;
			}
			print("\tunary\n");
			Put(F1_SUB,x->r,0,x->r); 
		}
	}
	else if (op == F3_NOT){
		//all the rest here...
		printf("F3_NOT\n");
		if (x->mode != cond){
			loadBool(x);
		}
		x->c = negated(x->c);
		t = x->a;
		x->a = x->b;
		x->b = t;
	}
	else if (op == F3_WRD){
		
		if (x->mode != reg){
			load(x);
		}
		Put(F3_WRD,x->r,x->baseAddr,x->r);
	}
	else if (op == F1_BLT)
	{
		if (x->mode != reg){
			load(x);
		}
		Put(F1_BLT,x->r,x->baseAddr,2);
	}
	else if (op == F1_RD)
	{
		if (x->mode != reg)
		{
			load(x);
		}
		Put(F1_RD,x->r,x->baseAddr,x->a);
	}
}

/*
 * For opcodes of the form y := y op z
 */

void op2(enum opcodes op, cg_item *x, cg_item *y){

	switch (op)
		{
			case F1_STW:
				print("\t\t\tbefore some here %d\n",x->isArray);
				Store(x,y);
				break;
			default:
				print("\t\t\tbefore some here %d y = %d\n",x->isArray,y->isArray);
				PutOp(op,x,y);
		}
	
// 	else if   ( ( (x->type == t_bool) && (y->type == t_bool) ) || 
//   		((x->type == a_bool) && (y->type == a_bool) ) || 
//   		((x->type == c_bool) && (y->type == c_bool) ) )
//  	{	 //x and y = boolean
//  		if (y->mode != cond){
//  			loadBool(y);
//  		}
//  		if (op == F1_OR){
// 			Put(F1_OR, x->c
//  		}
// // 		else if ( op == F1_AND){
// // 			//x->a = merged(y->a,y->b);
// // 			//x->b = y->b;
// // 			x->c = y->c;
// // 		}
// // 		else{
// // 			printf("[codegen] item x an y doesn't have boolean mode!!");
// // 		}
// 	//}
}

void fix(long at, long with){
	memory[at] = memory[at] % MEM_SIZE * MEM_SIZE + (with % MEM_SIZE);
}

void FixWith(long L0, long L1){
	long L2;
	while (L0 != 0){
		L2 = memory[L0] % MEM_SIZE;
		fix(L0,L1-L0);
		L0 = L2;
	}
}


void FixLink(long L){
	long L1;
	while (L != 0){
		L1 = memory[L] % MEM_SIZE;
		fix(L,pc-L);
		L = L1;
	}

}

void cJump(cg_item *x){
	if ((x->type == t_bool) || (x->type == a_bool) || (x->type == c_bool)){
		if (x->mode != cond)
			loadBool(x);
		Put(F1_BEQ + negated(x->c), x->r, 0, x->a);
		clearReg(x->r);
		FixLink(x->b);
		x->a = pc-1;
	}
	else {
		printf("Cant perform CJump on non-boolean.\n");
	}
}

void BJump(long L){
	Put(F1_BEQ, 0, 0,  L-pc);
}

void FJump(long L){
 	Put(F1_BEQ, 0, 0, L);
	L = pc-1;
}


unsigned long procEnter(int size){
	unsigned long x=pc;
	Put(F1_PSH,LNK,SP,4);
	Put(F1_PSH, FP, SP, 4);
	Put(F1_ADD, FP, 0, SP);
	if (size > 0)
	{
		Put(F2_SUBI, SP, SP, size/4);
	}
	return x;
}

void procReturn(int size){
	Put(F1_ADD, SP, 0, FP);
	Put(F1_POP, FP, SP, 4);
	//===========================================================================================
	//hoekom was dit nodig om size weg te vat
	Put(F1_POP, LNK, SP, 4); 
	Put(F2_RET,0,0,LNK);
}

void procCall(call_node *x){
	Put(F1_BSR, 0, 0, x->pc );
}

void recursiveCall(int pc){
	Put(F1_BSR, 0, 0, pc );
}

unsigned long procPC(){
	return pc;
}

void printItem(cg_item *x)
{
	printf("Item contents\n");
	switch (x->mode)
	{
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
	switch (x->type)
	{
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
	printf("\ta = %ld, r = %ld, c = %ld\n",x->a,x->r,x->c);
	if (x->isInt)
	{
		printf("\tisInt = TRUE\n");
	}else
	{
		printf("\tisInt = FALSE\n");
	}
	if (x->isArray)
	{
		printf("\tisArray = TRUE\n");
	}else
	{
		printf("\tisArray = FALSE\n");
	}
	if (x->varIndex)
	{
		printf("\tvarIndex = TRUE\n\n");
	}else
	{
		printf("\tvarIndex = FALSE\n\n");
	}
}

void printItems(cg_item *x,cg_item *y)
{
	printf("Item contents\n");
	switch (x->mode)
	{
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
		printf("\tMode: reg\n");
			break;
	}
	switch (y->mode)
	{
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
			printf("\tMode: reg\n");
			break;
	}
	switch (x->type)
	{
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
	switch (y->type)
	{
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
	if (x->isInt)
	{
		printf("\tisInt = TRUE\t");
	}else
	{
		printf("\tisInt = FALSE\t");
	}
	if (y->isInt)
	{
		printf("\tisInt = TRUE\n");
	}else
	{
		printf("\tisInt = FALSE\n");
	}
	if (x->isArray)
	{
		printf("\tisArray = TRUE\t");
	}else
	{
		printf("\tisArray = FALSE\t");
	}
	if (y->isArray)
	{
		printf("\tisArray = TRUE\n\n");
	}else
	{
		printf("\tisArray = FALSE\n\n");
	}
}

char* getOpString(enum opcodes op)
{

	switch (op)
	{
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