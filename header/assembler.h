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


/* Map da tabela de simbolos*/
typedef std::map<std::string,Symbol> SymbolTable;
/* Tabela de modificadore dos operandos */
typedef std::map<int,int> ModifierTable;

/* Construtros de um simbolo */
Symbol symbol(int value, bool defined, int list, TYPE_SECTION section, bool constant);

/* Namespace do montador*/
namespace _asm{
	/* ponteiro para função das funçoes de diretivas */
	typedef void (*directiveRoutine) (const std::list<Operation>::iterator op,
																		std::vector<int> &obj, int &pc,
																		SymbolTable &symTable,
																		ModifierTable &modTable);
	
	/* *
	 * @brief loop principal do montador de passagem unica, 
	 * aqui fica o loop onde se passa instrução a instrução 
	 * do codigo e o monta
	 * */
	void manager(std::list<Operation> &code, std::vector<int> &obj);

	/* Pesquisa na tabela de diretiva */
	bool searchDirectivesTable(MNEMONIC directive, directiveRoutine &routine );
	/* Pesquisa na tabela de instruçoes */
	bool searchInstrucTable(MNEMONIC directive, std::vector<int> &opRule);

	/* *
	 * Rotina para a diretiva COSNT
	 * @brief coloca o rotulo na tabela de symbolos, resolve as dependencias 
	 * e coloca o valor constante no codigo obj
	 * */
	void constRoutine(const std::list<Operation>::iterator op,
										std::vector<int> &obj, int &pc,
										SymbolTable &symTable,
										ModifierTable &modTable);

	/* *
	 * Rotina para a diretiva SPACE
	 * @brief coloca o rotulo na tabela de symbolos, resolve as dependencias
	 * e aloca a quantidade de espaço necessario no codigo obj
	 * */
	void spaceRoutine(const std::list<Operation>::iterator op,
										std::vector<int> &obj, int &pc,
										SymbolTable &symTable,
										ModifierTable &modTable);


	/* *
	 * Rotina para a diretiva SECTION
	 * @brief identifica a seção para qual a diretiva esta informando
	 * */
	TYPE_SECTION sectionRoutine(const std::list<Operation>::iterator op);


	/* Tabela de diretivas*/
	const std::map<MNEMONIC,directiveRoutine> directiveTable = {
		{CONST,&constRoutine},
		{SPACE,&spaceRoutine}
	};

	/* Tabela de instruções */
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

	/* *
	 * @brief chamada quando identificado um rotulo, é inserido o rotulo
	 * na tabela de simbolos e resolvido as dependincias para aquele rotulo
	 * */
	void resolveLable(const std::list<Operation>::iterator op, 
										std::vector<int> &obj, int pc,
									 	TYPE_SECTION section, 
										SymbolTable &SymbolTable,
										ModifierTable &modTable,
										bool isConst = false);
	/* *
	 * @brief função responsavel por resolver as dependencias para um simbolo,
	 * como o algoritimo é de passagem unica quando é usado um simbolo que nao é 
	 * definido é criado uma dependencia essa função resolve essas dependencias
	 * */
	void callBackSymbol(SymbolTable::iterator &itSymbol, 
											std::vector<int> &obj, 
											ModifierTable &modTable);
	/* *
	 * @brief chama toda vez que é passado por um operando 
	 * verifica na tabela de simbolo se ele ja foi definido 
	 * se nao cria a dependencia para ser resolvido posteriomente
	 * */
	void resolveOperand(std::string operand,std::string modifier,
											std::list<Operation>::iterator &op,
											std::vector<int> &obj ,int pc,
											SymbolTable &symTable,
											ModifierTable &modTable);
	/* *
	 * @brief idenftifica erros
	 * */
	void semanticErrorLable(const SymbolTable::iterator itSymbol, 
													const std::list<Operation>::iterator op,
													const std::vector<int> obj);

	/* *
	 * @brief idenftifica erros
	 * */
	void identifyError(std::list<Operation>::iterator op);
}

#endif

