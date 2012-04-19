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

#define MEM_SIZE 1024*1024

// control unit : program counter & instruction register
unsigned long PC, IR;
// arithmetic unit : registers 0 - 31
long R[32];
// memory
long memory[MEM_SIZE/4];
// buffer for reading strings from text file
unsigned long buffer;
// file to read from
FILE *fptr;
// index holder of current instruction in memory
unsigned int next_instr;

void printmem();

void loadFile() {

  int i = 0;
  fptr = fopen("outa.txt", "r");
  
  if (fptr == NULL) {
  	printf("Error reading file received from code generator\n");
	return;
  }
  
  while ((fptr != NULL) && (feof(fptr) == 0)) {
  	fread(&memory[i],sizeof(buffer),1,fptr);	
	i++;
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
  	print("*code of type F1 = %s\n", getOpString(*code));
  	*a = (buffer<<6)>>27;
	print("a = %lu\n", *a);
	*b = (buffer<<11)>>27;
	print("b = %lu\n", *b);
	*c = (buffer<<16)>>16;
	print("c = %ld\n", *c);
  }
  else if ((*code >= F2_ADDI)&&(*code <= F2_RET)) {
  	print("*code of type F2 = %s\n", getOpString(*code));
  	*a = (buffer<<6)>>27;
	print("a = %lu\n", *a);
	*b = (buffer<<11)>>27;
	print("b = %lu\n", *b);
	*c = (buffer<<27)>>27;
	print("c = %ld\n", *c);
  }
  else if (*code >= F3_JSR) {
  	print("*code of type F3 = %s\n",getOpString(*code));
	*c = (buffer<<6)>>6;
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

void execute(long pc0) {

  print("proc execute\n");

  unsigned long opcode;
  unsigned long a, b, next;
  long c;
  
  R[31] = 0;
  PC = pc0/4;
  int i = 0;
  int ms = getMemSize();
  
  while (i < ms) {
  //printmem();
  
  	getValues(&opcode,&a,&b,&c);
	
  	R[0] = 0;
	next = PC + 1;
	IR = memory[PC];
	// Hakies van hierdie 3 bewerkings moet miskien andersom wees - vanaf boek gecheck
	switch(opcode) {
		case F1_ADD : R[a] = R[b] + R[c];
			break;
		case F1_SUB : R[a] = R[b] - R[c];
			break;
		case F1_MUL : R[a] = R[b]*R[c];
			break;
		case F1_DIV : R[a] = R[b]/R[c];		 // ZERO DIVISION?
			break;
		case F1_MOD : R[a] = R[b]%R[c];
			break;
		case F1_CMP : R[a] = R[b] - R[c];
			break;
		case F1_CHK :
			if ((R[a] > 0)||(R[a] >= R[c])) {
				printf("error in F1_check - interpreter.c\n");
				return;
			}
			break;
		case F1_AND : R[a] = R[b]&R[c];
			break;
		case F1_OR : R[a] = R[b]|R[c];
			break;
		case F1_LSH :
			if (R[c] >= 0) {
				R[a] = R[b]<<R[c];
			}
			else R[a] = R[b]>>R[c];
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
			break;
		case F1_LDW : R[a] = memory[R[b] + c];
			break;
		case F1_LDB : R[a] = memory[R[b] + c];
			break;
		case F1_STW : memory[R[b] + c] = R[a];
			break;
		case F1_STB : memory[R[b] + c] = R[a];
			break;
		case F1_POP : R[a] = memory[R[b]];
			R[b] = R[b] + c;
			break;
		case F1_PSH : R[b] = R[b] - c;
			memory[R[b]] = R[a];
			break;
		case F1_BEQ :
			if (R[a] == R[b]) {
				next = PC + c;
			}
			break;
		case F1_BNE :
			if (R[a] != R[b]) {
				next = PC + c;
			}
			break;
		case F1_BLT :
			if (R[a] < R[b]) {
				next = PC + c;
			}
			break;
		case F1_BGE :
			if (R[a] >= R[b]) {
				next = PC + c;
			}
			break;
		case F1_BGT :
			if (R[a] > R[b]) {
				next = PC + c;
			}
			break;
		case F1_BLE :
			if (R[a] <= R[b]) {
				next = PC + c;
			}
			break;
		case F1_BSR :
			R[31] = PC;
			next = PC + c;
			break;
		case F2_ADDI : R[a] = R[b] + c;
			break;
		case F2_SUBI : R[a] = R[b] - c;
			break;
		case F2_MULI : R[a] = R[b]*c;
			break;
		case F2_DIVI : R[a] = R[b]/c;
			break;
		case F2_MODI : R[a] = R[b]%c;
			break;
		case F2_CMPI : R[a] = R[b] - c;
			break;
		case F2_CHKI : 
			if ((R[a] > 0)||(R[a] >= c)) {
				printf("error in F2_check - interpreter.c\n");
				return;
			}
			break;
		case F2_ANDI : R[a] = R[b]&c;
			break;
		case F2_ORI : R[a] = R[b]||c;
			break;
		case F2_LSHI :
			if (c > 0) {
				R[a] = R[b]<<c;
			}
			else R[a] = R[b]>>c;
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
			break;
		case F2_RET :
			// jump to address R[c] (F2, absolute address)
			next = PC + R[c];
		case F3_JSR :
			// save PC in R31, then jump to c (F3, absolute address)
			R[31] = PC;
			next = PC + c;
			break;
		default : printf("not a valid opcoderhkjsfhd - interpreter.c\n");
			break;
		}
		i++;
		PC = next;
	}
}

void printmem() {

  print("proc printmem\n");
  int i;

  for (i = 0; i < (MEM_SIZE/4); i++) {
  	if (memory[i] != 0) {
  		print("memory[%d] : %lu\n", i, memory[i]);
	}
  }
}

void print_reg() {

  print("proc print_reg\n");
  unsigned int i;
  for (i = 0; i < (sizeof(R)/4); i++) {
  	if (R[i] != 0) {
  		print("R[%d] = %ld\n", i, R[i]);
	}
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
  
  
  loadFile();
  execute(0);
  print_reg();
  printmem();
  return 0;
  
}
