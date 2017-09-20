#include <map>
#include <iostream>
#include <algorithm>
#include <regex>
#include <string>
#include <fstream>
#include <streambuf>
#include <vector>
#include <list>

#include "../header/structures.h"
#include "../header/io.h" 


typedef struct{
	std::map<std::string,bool> equTable;
	std::vector<std::list<Operation>::iterator> equIndex;
	std::vector<std::pair<std::string,std::list<Operation>::iterator>> ifIndex;
} EquIf;

typedef struct{
	//<Nome Macro,it inicio, it fim>
	std::map<std::string,std::pair<std::list<Operation>::iterator,std::list<Operation>::iterator>> mnt;
	std::pair<std::string,std::list<Operation>::iterator> currMacro;

} Macro;

namespace pp{

	/* *	
	 * 	@brief Função que capta e insere informações
	 * 	para as tabelas de controle
	 *
	 *	@param Operation operação que vai ser analisada
	 * */
	void control_tables(Operation op, std::list<Operation>::iterator it, EquIf &equIf,Macro &macro);


	void read_code(std::string strCode,std::list<Operation> &code,EquIf &equIf,Macro &macro);
	void manager(char* argv[], std::list<Operation> &code);
	void file2str(char *file, std::string &str);
	MNEMONIC inst2cod(std::string str);

	void equIfResolve(std::list<Operation> &code,EquIf &equIf);

	void expandMacro(std::list<Operation> &code,Macro &macro);


}

