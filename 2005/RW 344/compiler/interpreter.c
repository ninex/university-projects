/*
* R28 is used to keep track of which registers are currently being used.
* R31 is used to store the return adress of branch instructions
*
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//#include "include/scanner.h"
//#include "include/error.h"
//#include "include/symboltable.h"
//#include "include/parser.h"
#include "include/opcodes.h"
#include "include/asm.h"
#include "include/types.h"
#include "include/codegen.h"

#ifdef DEBUG_INTERP
	#define print(x, ...) printf(x, ## __VA_ARGS__)
#else
	#define print(x, ...)
#endif
#ifdef DEBUG_INTERP_INSTR
	#define printi(x, ...) printf(x, ## __VA_ARGS__)
#else
	#define printi(x, ...)
#endif

#define MEM_SIZE 1024*1024

#define delay(n)\
__asm__ volatile ("movl $1,%%eax\n\t"\
                  "L1_%=:\n\t"\
                  "cmp %%eax,%%ecx\n\t"\
                  "je L2_%=\n\t"\
                  "nop\n\t"\
                  "inc %%eax\n\t"\
                  "jmp L1_%=\n\t"\
                  "L2_%=:\n\t"\
                  :: "c" (n*70000)\
)

// control unit : program counter & instruction register
unsigned long PC, IR;
// arithmetic unit : registers 0 - 31
long R[32];
// memory
long memory[MEM_SIZE/4];
// buffer for reading strings from text file
unsigned long buffer, pc0;
// file to read from
FILE *fptr;
// index holder of current instruction in memory
//unsigned int next_instr;
int ms;

void printmem();

void loadFile() {

  fptr = fopen("outa.txt", "r");
  int i = 0;
  
  if (fptr == NULL) {
  	printf("\nError reading file received from code generator\n\n");
	exit(1);
  }
  fread(&R[30],sizeof(long),1,fptr);
  R[30] = R[30] / 4;
  fread(&R[29],sizeof(long),1,fptr);
  R[29] = R[29]/4;
  printf("FP = %ld\n",R[29]);
  
  //delay(40000);
  fread(&pc0,sizeof(long),1,fptr);
  //R[30] = R[30] / 4;
  printf("Stack pointer = %ld\n",R[30]);
  while ((fptr != NULL) && (feof(fptr) == 0)) {
  	fread(&memory[i],sizeof(buffer),1,fptr);
	i++;//printf("i=%d\n",i);
  }
  fclose(fptr);
}

void getValues(unsigned long *code, unsigned long *a, unsigned long *b, long *c) {

  // gets the opcode number  
  buffer = memory[PC];
  *code = (unsigned long) (buffer>>26);
  //print("buffer : %lu\n", buffer);
  print("*code : %lu\n", *code);
  
  
  if ((*code > NO_CODE)&&(*code <= F1_BSR)) {
  	print("code of type F1 = %s\n", getOpString(*code));
  	*a = (buffer<<6)>>27;
	*b = (buffer<<11)>>27;
	*c = (buffer<<16)>>16;
	if (bt(*c,15))
	{
		btr(*c,15);
		*c = -*c;
	}
	print("a = %lu  b = %lu  c = %ld\n", *a, *b, *c);
  }
  else if ((*code >= F2_ADDI)&&(*code <= F2_RET)) {
  	print("code of type F2 = %s\n", getOpString(*code));
  	*a = (buffer<<6)>>27;
	*b = (buffer<<11)>>27;
	*c = (buffer<<16)>>16;
	if (bt(*c,15)) {
		btr(*c,15);
		*c = -*c;
	}
	print("a = %lu  b = %lu  c = %ld\n", *a, *b, *c);
  }
  else if (*code >= F3_JSR) {
  	print("code of type F3 = %s\n",getOpString(*code));
	*c = (buffer<<6)>>6;
	if (bt(*c,25))
	{
		btr(*c,25);
		*c = -*c;
	}
	print("c = %ld\n", *c);
  }
  else {
  	print("Not a valid code constant\n");
  }
}

int getMemSize() {
  int i = 0;
  while (memory[i] != 0) {
  	i++;
  }
  print("memsize : %d\n", i);
  return i;
}

void print_reg() {

  print("proc print_reg\n");
  unsigned int i;
  for (i = 0; i < (sizeof(R)/4); i++) {
  	if (R[i] != 0) {
  		printf("R[%d] = %ld\n", i, R[i]);
	}
  }
}

void printStack() {

  print("proc printStack\n");
  int i;

  
  for (i = R[30]; i < MEM_SIZE/4; i++) {
  	printf("memory[%d] : %ld\n", i, memory[i]);
	
  }
}

void execute() {

  print("proc execute\n");

  unsigned long opcode;
  unsigned long a, b, next;
  long c;
  
  R[31] = 0;
  PC = pc0;
  int i = 0;
  ms = getMemSize();
  //print_reg();
  while (TRUE) {
  //printmem();
  
  	getValues(&opcode,&a,&b,&c);
	
  	R[0] = 0;
	next = PC + 1;
	IR = memory[PC];
	switch(opcode) {
		case F1_ADD : R[a] = R[b] + R[c];
			printi("ADD\n");
			break;
		case F1_SUB : R[a] = R[b] - R[c];
			printi("SUB\n");
			break;
		case F1_MUL : R[a] = R[b]*R[c];
			printi("MUL\n");
			break;
		case F1_DIV :
			printi("DIV\n");
			if (R[c] == 0) {
				printf("Runtime error: Division by zero\n");
				delay(12000);
				return;
			}
			R[a] = R[b]/R[c];
			break;
		case F1_MOD : R[a] = R[b]%R[c];
			printi("MOD\n");
			break;
		case F1_CMP : R[a] = R[b] - R[c];
			printi("CMP\n");
			break;
		case F1_CHK :printi("CHK\n");
			if ((R[a] < 0)||(R[a] >= R[c])) {
				printf("Runtime error: Array index out of bounds\n");
				return;
			}
			break;
		case F1_AND : R[a] = R[b]&R[c];
			printi("AND\n");
			break;
		case F1_OR : R[a] = R[b]|R[c];
			printi("OR\n");
			break;
		case F1_LSH :
			if (R[c] >= 0) {
				R[a] = R[b]<<R[c];
			}
			else R[a] = R[b]>>R[c];
			printi("LSH\n");
			break;
		case F1_ASH :
			// R[b] == 0 makes no difference
			if (R[c] >= 0) {
				R[a] = R[b]<<R[c];
			}
			else {
				R[a] = R[b]>>R[c];
				R[a] = R[a]*(-1);
			}
			printi("ASH\n");
			break;
		case F1_LDW : print("R[a] = %ld, R[b] = %ld, c = %ld %ld,%ld,%ld\n",R[a],R[b],c/4,a,b,c);
			R[a] = memory[R[b] + c/4];
			//print_reg();
			
			print("R[a] = %ld, R[b] = %ld, c = %ld\n",R[a],R[b],c/4);
			//printmem();
			//print_reg();
			printi("LDW\n");
			break;
		case F1_LDB : R[a] = memory[R[b] + c/4];
			break;
		case F1_STW : memory[R[b] + c/4] = R[a];
		print("R[a]=%ld mem[R[b] + c)/4]=%d (R[b] + c)/4=%d\n",R[a],memory[(R[b] + c)/4],(R[b] + c)/4);
		//print_reg();
		//printmem();
			
			//printmem();
			//print_reg();
			printi("STW\n");
			break;
		case F1_STB : memory[R[b] + c] = R[a];
			break;
		case F1_POP : R[a] = memory[R[b]];
			R[b] = R[b] + c/4;
			print("POP c = %ld\n",R[a]);
			//printmem();
			//print_reg();
			printi("POP\n");
			break;
		case F1_PSH :
		print("R[a]=%ld mem[R[b] + c/4]=%ld R[b] = %ld, a = %ld, b = %ld, c = %ld\n",R[a],memory[R[b] + c/4] ,R[b],a ,b, c);
		 //printmem();
			//print_reg();
		 	R[b] = R[b] - c/4;
			memory[R[b]] = R[a];
				//printmem();
			//print_reg();
			print("R[a]=%ld mem[R[b] + c/4]=%ld R[b] = %ld, a = %ld, b = %ld, c = %ld\n",R[a],memory[R[b] + c/4] ,R[b],a ,b, c);
			printi("PUSH\n");
			break;
		case F1_BEQ :
			if (R[a] == R[b]) {
				next = PC + c;
			}
			printi("BEQ\n");
			break;
		case F1_BNE :
			if (R[a] != R[b]) {
				next = PC + c;
			}
			printi("BNE\n");
			break;
		case F1_BLT :
			if (R[a] < R[b]) {
				next = PC + c;
			}
			printi("BLT\n");
			break;
		case F1_BGE :
			if (R[a] >= R[b]) {
				next = PC + c;
			}
			printi("BGE\n");
			break;
		case F1_BGT :
			if (R[a] > R[b]) {
				next = PC + c;
			}
			printi("BGT\n");
			break;
		case F1_BLE :
			if (R[a] <= R[b]) {
				next = PC + c;
			}
			printi("BLE\n");
			break;
		case F1_BSR :
			R[31] = PC+1;
			//printmem();
			
			//printmem();
			//print_reg();
			print("\t\tnext = %d\n",next);
			next = c;//PC + c;
			printi("BSR\n");
			break;
		case F2_ADDI : R[a] = R[b] + c;
			print("ADDI %d %d %d\n",R[a],R[b],c);
			//print_reg();
			printi("ADDI\n");
			break;
		case F2_SUBI : R[a] = R[b] - c;
			//print_reg();
			printi("SUBI\n");
			break;
		case F2_MULI : R[a] = R[b]*c;
				//printf("R[a] = %d\n", R[a]);
			printi("MULI\n");
			break;
		case F2_DIVI : R[a] = R[b]/c;
			printi("DIVI\n");
			break;
		case F2_MODI : R[a] = R[b]%c;
			printi("MODI\n");
			break;
		case F2_CMPI : R[a] = R[b] - c;
			printi("CMPI\n");
			break;
		case F2_CHKI : 
			if ((R[a] < 0)||(R[a] >= c)) {
				printf("Runtime error: Array index out of bounds\n");
				return;
			}
			printi("CHKI\n");
			break;
		case F2_ANDI : R[a] = R[b]&c;
			printi("ANDI\n");
			break;
		case F2_ORI : R[a] = R[b]||c;
			printi("ORI\n");
			break;
		case F2_LSHI :
			if (c > 0) {
				R[a] = R[b]<<c;
			}
			else R[a] = R[b]>>c;
			printi("LSHI\n");
			break;
		case F2_ASHI :
			// c == 0 makes no difference
			if (c >= 0) {
				R[a] = R[b]<<c;
			}
			else {
				R[a] = R[b]>>c;
				R[a] = R[a]*(-1);
			}
			printi("ASHI\n");
			break;
		case F2_RET :
			// jump to address R[c] (F2, absolute address)
			//printf("\n\nnext = %lu PC = %lu R[c] = %lu % ld\n\n",next,PC,R[c],R[c]);
			
			//printmem();
			//print_reg();
			next = R[c];
			//printf("\n\nnext = %lu PC = %lu R[c] = %lu % ld\n\n",next,PC,R[c],R[c]);
			//print_reg();
			if (R[c] == 0)
			{
				return ;
			}
				print("R[c] = %ld\n",R[c]);
			//print_reg();
			//delay(10000);
			printi("RET\n");
			break;
		case F3_JSR :
			// save PC in R31, then jump to c (F3, absolute address)
			R[31] = PC;
			next = PC + c/4;
			printi("JSR\n");
			break;
		case F3_WRD :
			printi("WRD\n");
			//printmem();
			print("PC = %d\n",PC);
			print("\t\t\t********WRD:value in register %ld = %ld\n", c, R[c]);
			printf("%ld\n",R[c]);
			delay(10000);
			break;
		case F1_RD:
			printi("RD\n");
			//print_reg();
			scanf("%ld",&R[a]);
			//print_reg();
			memory[R[b]+c/4] = R[a];
			print("R[a]=%ld mem[R[b] + c)/4]=%ld (R[b] + c)/4=%ld\tR[b] = %ld c = %ld\n",R[a],memory[(R[b] + c)],R[b]+c,R[b],c);
			break;
		default : print("not a valid opcode - interpreter.c\n");
			break;
		}
		i++;
		//printf("next = %d\n",next);
		PC = next;
		/*printf("_____________\n");
		print_reg();
		printf("=============\n");
		printStack();
		printf("=============\n");*/
		//delay(10000);
		
	}
	
}

void printmem() {

  print("proc printmem\n");
  int i;

  for (i = 0; i < ms; i++) {
  	if (memory[i] != 0) {
  		print("memory[%d] : %lu\n", i, memory[i]);
	}
  }
  
  for (i = ms; i < MEM_SIZE/4; i++) {
  	if (memory[i] != 0) {
  		printf("memory[%d] : %ld\n", i, memory[i]);
	}
  }
}


void init_mem() {
  int i;
  for (i = 0; i < (MEM_SIZE/4); i++) {
  	memory[i] = 0;
  }
}

int main() {

/*
  buffer = 144965632;
  FILE *f = fopen("outa.txt","w");
  fwrite(&buffer,sizeof(buffer),1,f);
  
  buffer = 2184577028;
  fwrite(&buffer,sizeof(buffer),1,f);
  
  buffer = 2483028138;
  fwrite(&buffer,sizeof(buffer),1,f);
  
  fclose(f);
*/
  init_mem();
  loadFile();
  execute();
  print_reg();
  printmem();
  return 0;
  
}