#ifndef STRUCTURES_H
#define STRUCTURES_H
#include <stdlib.h>
#include <string>
#include <map>



typedef enum{
	ADD,
	SUB,
	MULT,
	DIV,
	JMP,
	JMPN,
	JMPP,
	JMPZ,
	COPY,
	LOAD,
	STORE,
	INPUT,
	OUTPUT,
	STOP,

	SECTION,
	SPACE,
	CONST,
	EQU,
	IF,
	MACRO,
	END,

	NON_DEFINED

} MNEMONIC;




typedef struct{
	int line;
	std::string complete;
	std::string lable;
	std::string instruction;
	MNEMONIC instCod;
	std::string first_op;
	std::string first_mod;
	std::string second_op;
	std::string second_mod;

	bool error;
} Operation;


#endif


