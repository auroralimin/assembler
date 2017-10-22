#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <stdlib.h>
#include <list>
#include <vector>

#include "structures.h"
#include "io.h"


typedef enum{
	DATA,
	TEXT,
	NOT_DEFINED
} TYPE_SECTION;

typedef struct{
	int value;
	bool defined;
	int list;
	TYPE_SECTION section;
	std::list<std::list<Operation>::iterator> lOp;
	bool constant;
} Symbol;



typedef std::map<std::string,Symbol> SymbolTable;
typedef std::map<int,int> ModifierTable;

Symbol symbol(int value, bool defined, int list, TYPE_SECTION section, bool constant);


namespace _asm{
	typedef void (*directiveRoutine) (const std::list<Operation>::iterator op,
																		std::vector<int> &obj, int &pc,
																		SymbolTable &symTable,
																		ModifierTable &modTable);
	
	void manager(std::list<Operation> &code, std::vector<int> &obj);

	bool searchDirectivesTable(MNEMONIC directive, directiveRoutine &routine );
	bool searchInstrucTable(MNEMONIC directive, std::vector<int> &opRule);

	void constRoutine(const std::list<Operation>::iterator op,
										std::vector<int> &obj, int &pc,
										SymbolTable &symTable,
										ModifierTable &modTable);

	void spaceRoutine(const std::list<Operation>::iterator op,
										std::vector<int> &obj, int &pc,
										SymbolTable &symTable,
										ModifierTable &modTable);


	TYPE_SECTION sectionRoutine(const std::list<Operation>::iterator op);


	const std::map<MNEMONIC,directiveRoutine> directiveTable = {
		{CONST,&constRoutine},
		{SPACE,&spaceRoutine}
	};

	const std::map<MNEMONIC,const std::vector<int>> instructionTable = {
		{ADD,{1,1,2}},
		{SUB,{1,2,2}},
		{MULT,{1,3,2}},
		{DIV,{1,4,2}},
		{JMP,{1,5,2}},
		{JMPN,{1,6,2}},
		{JMPP,{1,7,2}},				
		{JMPZ,{1,8,2}},
		{COPY,{2,9,3}},
		{LOAD,{1,10,2}},
		{STORE,{1,11,2}},
		{INPUT,{1,12,2}},
		{OUTPUT,{1,13,2}},
		{STOP,{0,14,1}}
	};


	void resolveLable(const std::list<Operation>::iterator op, 
										std::vector<int> &obj, int pc,
									 	TYPE_SECTION section, 
										SymbolTable &SymbolTable,
										ModifierTable &modTable,
										bool isConst = false);

	void callBackSymbol(SymbolTable::iterator &itSymbol, 
											std::vector<int> &obj, 
											ModifierTable &modTable);
	
	void resolveOperand(std::string operand,std::string modifier,
											std::list<Operation>::iterator &op,
											std::vector<int> &obj ,int pc,
											SymbolTable &symTable,
											ModifierTable &modTable);

	void semanticErrorLable(const SymbolTable::iterator itSymbol, 
													const std::list<Operation>::iterator op,
													const std::vector<int> obj);

	void identifyError(std::list<Operation>::iterator op);
}

#endif

