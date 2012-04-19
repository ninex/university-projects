#ifndef CODEGEN_H
#define CODEGEN_H

void printReg();
int testRange(cg_item *x);
void initCodeGen();
void dumpToFile();
void Put(enum opcodes opc, unsigned long a, unsigned long b, unsigned long c);
int getReg();
void start(int disp);
void Close();
void load(cg_item *x);
void clearReg(int reg);
void op1(enum opcodes op, cg_item *x);
void op2(enum opcodes op, cg_item *x, cg_item *y);
void PutOp(enum opcodes cd, cg_item *x, cg_item *y);
void printItem(cg_item *x);
void printItems(cg_item *x,cg_item *y);
char* getOpString(enum opcodes op);
unsigned long procPC();
void procCall(call_node *x);
void recursiveCall(int pc);
unsigned long procEnter(int size);
void procReturn(int size);
unsigned long resetLevel();
unsigned long changeLevel();
unsigned long rootLevel();
#endif 
