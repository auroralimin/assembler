#include "../header/preprocessing.h"

void pp::control_tables(Operation op, std::list<Operation>::iterator it, EquIf &equIf, Macro &macro){

	switch(op.instCod){
		case MNEMONIC::EQU:
			if(equIf.equTable.count(op.lable)){
				io::outError(std::cerr,"semântico","declaração de rótulos repetidos",
										op.line,op.complete,"rótulo "+op.lable+" já declarado anteriormente");	
			}
			try{
				equIf.equTable.insert(std::make_pair(op.lable,std::stoi(op.first_op,nullptr,0)));
			}catch(std::invalid_argument){
				io::outError(std::cerr,"semântico","tipo de argumento invalido",
										op.line,op.complete,"Note que a diretiva EQU espera um numero como argumento");	
			}
			equIf.equIndex.push_back(it);
			break;

		case MNEMONIC::IF:
			equIf.ifIndex.push_back(it);
			break;

		//TODO verificar erros na macro
		case MNEMONIC::MACRO:
			macro.currMacro = std::make_pair(op.lable,it);
			break;

		case MNEMONIC::END:
			macro.mnt.insert(std::make_pair(macro.currMacro.first,std::make_pair(macro.currMacro.second,it)));
			break;
	
	}

}

void pp::manager(char* argv[], std::list<Operation> &code){
	
	std::string strCode;
	pp::file2str(argv[2],strCode);	
	
	EquIf equIf; 
	Macro macro;
	pp::read_code(strCode,code,equIf,macro);
	
	pp::equIfResolve(code,equIf);
	pp::expandMacro(code,macro);
	
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

void pp::read_code(std::string strCode,std::list<Operation> &code,EquIf &equIf,Macro &macro){

	const std::regex regex("^[ \\t]*(?:([a-zA-Z_]\\w*):\\s*)?(?:([a-zA-Z]\\w*)[ \\t]*)"
												 "(?:(-?\\w+)(?:\\s*\\+\\s*([a-zA-Z]\\w*|\\d|0[xX][a-fA-F]+))?)?(?:, (\\w+)(?:\\s*"
												 "\\+\\s*([a-zA-z]\\w*|\\d|0[xX][a-fA-F]+))?[\\t ]*(?:;.*)?(?:\\n)|[\\t ]*(?:;.*)?(?:\\n))");
	
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
				//TODO separar tipos de erros lexico, ex duas lables na mesma linha
				//io::outError(std::cerr,"léxico","tokens inválidos",line,sm[0]);
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
			op.lable = pp::equConverter(sm[1],equIf);
			op.instruction = sm[2];
			op.instCod = pp::inst2cod(sm[2].str());
			op.first_op = pp::equConverter(sm[3],equIf); 
			op.first_mod = pp::equConverter(sm[4],equIf); 
			op.second_op = pp::equConverter(sm[5],equIf); 
			op.second_mod = pp::equConverter(sm[6],equIf); 
			op.error = false;
			
			code.push_back(op);
	
			//--code.end -> iterator do ultimo elemento inserido
			pp::control_tables(op,--code.end(),equIf,macro);

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
	for(std::list<Operation>::iterator it : equIf.ifIndex){
		
		try{
			if(!std::stoi(it->first_op,nullptr,0))
				it = --code.erase(++it);
		}catch(std::invalid_argument){
			io::outError(std::cerr,"semântico","tipo de argumento invalido",
			                      it->line,it->complete,"Note: O argumento da diretiva IF tem que ser anteriomente declarado como um rotulo de EQU "+
																									it->first_op + " não é nem um numero nem foi não foi declarado anteriomente");		
		}
		code.erase(it);
	}
	
	
	for(std::list<Operation>::iterator equI : equIf.equIndex){
		code.erase(equI);
	}

}


void pp::expandMacro(std::list<Operation> &code, Macro &macro){

	std::list<Operation>::iterator it;	
	for(it = code.begin(); it != code.end(); it++ ){
		if(macro.mnt.count(it->instruction)){
			code.insert(it,++macro.mnt[it->instruction].first,macro.mnt[it->instruction].second);	
			macro.mnt[it->instruction].first--;
			it = code.erase(it);
			it--;
		}
	}

	for(auto m : macro.mnt){
		code.erase(m.second.first,++m.second.second);
	}


}



std::string pp::equConverter(std::string str,const EquIf equIf){

	auto it = equIf.equTable.find(str); 
	if(it != equIf.equTable.end())
		return std::to_string(it->second);
	else
		return str;

}



