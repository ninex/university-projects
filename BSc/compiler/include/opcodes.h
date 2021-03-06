/****************************************************************
 * 	opcodes.h						*
 *								*
 * 	Part of Software design: Compiler project (2005)	*
 *								*
 * 	Group: Andrew Mori, Eduard Bergh, Mia Meyer Abrie Greef *
 ****************************************************************/
#ifndef OPCODES_H
#define OPCODES_H


enum opcodes {
	NO_CODE=0,
	// register instructions (F1)
	F1_ADD,
	F1_SUB,
	F1_MUL,
	F1_DIV,
	F1_MOD,
	F1_CMP,
	F1_CHK,
	F1_AND,
	F1_OR,
	F1_LSH,		//10
	F1_ASH,
	// memory reference instructions (F1)
	F1_LDW,
	F1_LDB,
	F1_STW,
	F1_STB,
	F1_POP,
	F1_PSH,
	// control flow instructions (F1)
	F1_BEQ,
	F1_BNE,
	F1_BLT,		//20
	F1_BGE,
	F1_BLE,
	F1_BGT,
	F1_BSR,
	F1_RD,
//	F3_JSR,
	//F2_RET,
	// register instructions (F2)
	F2_ADDI,
	F2_SUBI,
	F2_MULI,
	F2_DIVI,
	F2_MODI,
	F2_CMPI,	//30
	F2_CHKI,
	F2_ANDI,
	F2_ORI,
	F2_LSHI,
	F2_ASHI,
	F2_RET,
	F3_JSR,
	F3_UNARY,
	F3_WRD,
			//40
	F3_WRL,
	F3_WRH,
	F3_NOT
	};
#endif
