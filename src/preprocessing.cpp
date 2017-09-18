#include "../header/preprocessing.h"


void pp::control_tables(Operation op, std::list<Operation>::iterator it, EquIf &equIf){

	switch(op.instCod){
		case MNEMONIC::EQU :
			if(equIf.equTable.count(op.lable)){
				std::cout << "ERRO semântico: declaração de rótulos repetidos" << std::endl;
				std::cout << "Linha: " << op.line << "\tRótulo: " << op.lable << std::endl;
				std::cout << "Operação:\t" << op.complete << std::endl;			
			}
			equIf.equTable.insert(std::pair<std::string,bool>(op.lable,std::stoi(op.first_op)));
			equIf.equIndex.push_back(it);
			break;
		case MNEMONIC::IF :
			equIf.ifIndex.push_back(std::pair<std::string,std::list<Operation>::iterator>(op.first_op,it));
	
	}

}

void pp::manager(char* argv[], std::list<Operation> &code){
	
	std::string strCode;
	pp::file2str(argv[2],strCode);	
	
	EquIf equIf; 
	pp::read_code(strCode,code,equIf);

	pp::equIfResolve(code,equIf);
	
}


void pp::file2str(char *file, std::string &str){

	std::ifstream t(file);
	t.seekg(0, std::ios::end);   
	str.reserve(t.tellg());
	t.seekg(0, std::ios::beg);

	str.assign((std::istreambuf_iterator<char>(t)),
			            std::istreambuf_iterator<char>());

	std::transform(str.begin(), str.end(),str.begin(), ::toupper);

}

void pp::read_code(std::string strCode,std::list<Operation> &code,EquIf &equIf){

	
	const std::regex regex("^[ \\t]*(?:([a-zA-Z_]\\w*):\\s*)?(?:([a-zA-Z]+)\\s*)"
												 "(?:(-?\\w+)(?:\\s*\\+\\s*(\\d))?)?(?:, (\\w+)(?:\\s*"
												 "\\+\\s*(\\d))?[\\t ]*(?:;.*)?\\n|[\\t ]*(?:;.*)?\\n)");

	const std::regex rerror(".*\\n");
	const std::regex rline("^[ \\t]*(?:;.*)?\\s");
	std::smatch sm;
	
	int line = 0;
	
	std::string::const_iterator searchStart( strCode.cbegin() );

	//passa o regex em todo o arquivo de entrada
	while (searchStart != strCode.cend()){
		line++;
		//casa o padrao caso seja tokens validos
		regex_search( searchStart, strCode.cend(), sm, regex); 
		if(sm.empty()){
			//caso não seja pode ser linha em branco ou linha de comentario 
			regex_search( searchStart, strCode.cend(), sm, rline);
		 	if(sm.empty()){	
				//caso nao seja é uma linha que esta errada, ou seja uma linha que
				//não foi possivel tokear 
				regex_search(searchStart, strCode.cend(), sm, rerror);
				Operation op;
				op.line = line;
				op.complete = sm[0];
				op.error = true;
				
				code.push_back(op);
			}	
		}else{
			Operation op;
			op.line = line;
			op.complete = sm[0];
			op.lable = sm[1];
			op.instruction = sm[2];
			op.instCod = pp::inst2cod(sm[2].str());
			op.first_op = sm[3];
			op.first_mod = sm[4].str().empty() ? 0 : stoi(sm[4]);
			op.second_op = sm[5];
			op.second_mod = sm[6].str().empty() ? 0 : stoi(sm[6]);
			op.error = false;
			
			code.push_back(op);
	
			//--code.end -> iterator do ultimo elemento inserido
			pp::control_tables(op,--code.end(),equIf);

			std::string str(sm[0]);
			line += std::count(str.begin(), str.end(), '\n') - 1;
		}
		searchStart += sm.position() + sm.length();
	}

}


MNEMONIC pp::inst2cod(std::string str){
	const std::map<std::string,MNEMONIC> inst = {
		{"ADD",ADD},
		{"SUB",SUB},
		{"MULT",MULT},
		{"DIV",DIV},
		{"JMP",JMP},
		{"JMPN",JMPN},
		{"JMPP",JMPP},
		{"JMPZ",JMPZ},
		{"COPY",COPY},
		{"LOAD",LOAD},
		{"STORE",STORE},
		{"INPUT",INPUT},
		{"OUTPUT",OUTPUT},
		{"STOP",STOP},

		{"SECTION",SECTION},
		{"SPACE",SPACE},
		{"CONST",CONST},
		{"EQU",EQU},
		{"IF",IF},
		{"MACRO",MACRO},
		{"END",END}
	};

	return inst.count(str) ? inst.find(str)->second : MNEMONIC::NON_DEFINED;
}


void pp::equIfResolve(std::list<Operation> &code,EquIf &equIf){
	for(std::pair<std::string,std::list<Operation>::iterator> ifI : equIf.ifIndex){
		
		if(!equIf.equTable.count(ifI.first)){
			std::cout << "ERRO semântico: declaração de rótulos ausentes" << std::endl;
			std::cout << "Linha: " << ifI.second->line  << "\tRótulo: " << ifI.second->lable << std::endl;
			std::cout << "Operação:\t" << ifI.second->complete << std::endl;

		}else if(!equIf.equTable[ifI.first]){
			ifI.second = --code.erase(++ifI.second);
		}
		code.erase(ifI.second);
	}
	
	
	for(std::list<Operation>::iterator equI : equIf.equIndex){
		code.erase(equI);
	}

	

}
