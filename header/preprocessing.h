#ifndef PREPROCESSING_H
#define PREPROCESSING_H

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

/* *
 * Struct com estruturas de controle para o 
 * processamento das diretivas de EQU e IF
 * */
typedef struct{
	std::map<std::string,int> equTable;
	std::vector<std::list<Operation>::iterator> equIndex;
	std::vector<std::list<Operation>::iterator> ifIndex;
} EquIf;

/* *
 * Struct com estruturas de controle para o
 * processamento e expanssão das macros
 * */
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

	/* *
	 * @brief Função responsavel por ler o codigo em arquivo texto
	 * e colocalo na memoria na estrutura que será trabalhada 
	 * até o final da montagem
	 * */
	void read_code(std::string strCode,std::list<Operation> &code,EquIf &equIf,Macro &macro);

	/* *
	 * @brief Função para abrir o arquivo texto onde esta o codigo 
	 * e colocar tudo em maiusculo, para que o montador não
	 * seja sensivel ao caso
	 * */
	void file2str(char *file, std::string &str);

	/* *
	 * @brief Função para codificar uma instrução no formato de string
	 * em uma enum que contem todas as instruçoes, dessa forma 
	 * facilita processamentos posteriores
	 * */
	MNEMONIC inst2cod(std::string str);
	
	/* *
	 * @brief Resolve as diretivas EQU e IF, retiram elas do codigo,
	 * e no caso do IF verifica se deve se retirar a proxima 
	 * instrução ou não
	 * */
	void equIfResolve(std::list<Operation> &code,EquIf &equIf);
	
	/* *
	 * @brief Função responsavel para expandir as macros onde as 
	 * mesmas forem chamadas, macro dentro de macro esta funcionando
	 * */
	void expandMacro(std::list<Operation> &code,Macro &macro);

	/* *
	 * @brief Função para verificar se um rotulo é um rotulo que pertence 
	 * a tabela de EQU se pertencer deve se trocar esse rotulo pelo valor
	 * que consta na tabela 
	 * */
	std::string equConverter(std::string str, const EquIf equIf);

}

#endif
