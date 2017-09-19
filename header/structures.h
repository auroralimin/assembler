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
	int first_mod;
	std::string second_op;
	int second_mod;
	bool error;

} Operation;





