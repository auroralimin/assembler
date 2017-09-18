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

namespace tables{
	//map com os equ <Lable,valor>
	//map com os if <Lable,pos no vetor code>

	//pair de com inicio e fim de cada sessao,
	//fim como 0 quer dizer que é no final do arquivo
//	std::pair<int,int> text_section;
	//std::pair<int,int> data_section;
};




