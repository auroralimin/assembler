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


typedef struct{
	std::map<std::string,bool> equTable;
	std::vector<std::list<Operation>::iterator> equIndex;
	std::vector<std::pair<std::string,std::list<Operation>::iterator>> ifIndex;
} EquIf;

namespace pp{

	/* *	
	 * 	@brief Função que capta e insere informações
	 * 	para as tabelas de controle
	 *
	 *	@param Operation operação que vai ser analisada
	 * */
	void control_tables(Operation op, std::list<Operation>::iterator it, EquIf &equIf);


	void read_code(std::string strCode,std::list<Operation> &code,EquIf &equIf);
	void manager(char* argv[], std::list<Operation> &code);
	void file2str(char *file, std::string &str);
	MNEMONIC inst2cod(std::string str);

	void equIfResolve(std::list<Operation> &code,EquIf &equIf);


}

