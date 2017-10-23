#include "../header/assembler.h"



void _asm::manager(std::list<Operation> &code,std::vector<int> &obj){
	
	SymbolTable symTable;
	ModifierTable modTable; 
	
	bool existTextSec = false;
	int pc = 0;

	TYPE_SECTION section = NOT_DEFINED;
	directiveRoutine routine;
	std::vector<int> opRule;

	std::list<Operation>::iterator it;
	for(it = code.begin() ; it != code.cend(); it++){

		if(it->error){
			identifyError(it);
			continue;
		}	
		else if(searchInstrucTable(it->instCod,opRule)){
			if(section != TEXT){
				io::outError(std::cerr,"semântico","Instrução na seção errada",
						it->line,it->complete);
				continue;
			}

			if(!it->lable.empty()){

				resolveLable(it,obj,pc,TEXT,symTable,modTable);
			}		
			//Codigo da instrução
			obj.push_back(opRule[1]);
			
			//Verificação dos numeros de operandos
			int nOperand = 0;
			if(!(it->first_op.empty()))
				nOperand = 1;
			if(!(it->second_op.empty()))
				nOperand = 2;
			
			if(opRule[0] != nOperand)
				io::outError(std::cerr,"sintático","instruções com a quantidade de operando inválida",
										it->line,it->complete, "Note: a instrução "+it->instruction+
																					 " aceita "+std::to_string(opRule[0])+ 
																					 " e esta sendo usado "+std::to_string(nOperand)+
																					 " operando(s)");
			

			//Operandos
			if(opRule[0] >= 1)
				resolveOperand(it->first_op,it->first_mod,it,obj,pc+1,symTable,modTable);
			if(opRule[0] == 2)
				resolveOperand(it->second_op,it->second_mod,it,obj,pc+2,symTable,modTable);
			
	
			//pc = pc + tamanho da op
			pc += opRule[2];

		}
		else if(searchDirectivesTable(it->instCod,routine)){
			if(section != DATA){
				io::outError(std::cerr,"semântico","Diretiva na seção errada",
						it->line,it->complete);
				continue;
			}

			routine(it,obj,pc,symTable,modTable);
		}else if(it->instCod == SECTION){
			section = sectionRoutine(it);
			if(section == TEXT)
				existTextSec = true;
		}
		else
			io::outError(std::cerr,"sintático","Instrução/Diretiva invalida",it->line,
						it->complete,"NOTE "+it->instruction+" não é uma instrução reconhecida");
	}


	if(!existTextSec){
		io::outError(std::cerr,"semântico","seção TEXT faltante",0,"",
								"Note: o código precisa de pelo menos uma seção TEXT");
	}


	for(auto &sym : symTable){
		if(!(sym.second.defined)){
			for(std::list<Operation>::iterator op : sym.second.lOp){
				io::outError(std::cerr,"semântico","declarações e rótulos ausentes",op->line,
										op->complete,"Note: o rótulo "+sym.first+" não foi declarada");

				switch(op->instCod){
					case JMP:
					case JMPZ:
					case JMPP:
					case JMPN:
						io::outError(std::cerr,"semântico","pulo para rótulos inválidos",
												op->line,op->complete,
												"Note: Pulo para um rótulo não declarado");
				}
			}
		}
	}

}

bool _asm::searchDirectivesTable(MNEMONIC directive, directiveRoutine &routine ){
	auto it = _asm::directiveTable.find(directive);
	if(it != _asm::directiveTable.end()){
		routine = it->second;
		return true;
	}
	return false;
}


bool _asm::searchInstrucTable(MNEMONIC directive, std::vector<int> &opRule){
	auto it = _asm::instructionTable.find(directive);
	if(it != _asm::instructionTable.end()){
		opRule = std::vector<int>(it->second);
		return true;
	}
	return false;
}



void _asm::constRoutine(const std::list<Operation>::iterator op, 
												std::vector<int> &obj, int &pc,
								  			SymbolTable &symTable, ModifierTable &modTable){

	if(!op->lable.empty())
		resolveLable(op,obj,pc,DATA,symTable,modTable,true);
	
	if(!op->first_op.empty())
		obj.push_back(std::stoi(op->first_op,nullptr,0));
	
	pc++;	
	
}

void _asm::spaceRoutine(const std::list<Operation>::iterator op, 
												std::vector<int> &obj, int &pc,
								  			SymbolTable &symTable, ModifierTable &modTable){

	if(!op->lable.empty())
		resolveLable(op,obj,pc,DATA,symTable,modTable);
	
	int alloc = 1;
	if(!op->first_op.empty())
		alloc = std::stoi(op->first_op,nullptr,0);
	
	for(int i = 0; i < alloc; i++){
		obj.push_back(0);
	}	
	
	pc += alloc;	
}


TYPE_SECTION _asm::sectionRoutine(const std::list<Operation>::iterator op){

	if(!op->first_op.compare("TEXT"))
		return TEXT;
	else if(!op->first_op.compare("DATA"))
		return DATA;
	else
		io::outError(std::cerr,"sintático","Seção inválida",op->line,op->complete,
								"NOTE: " +op->first_op+ " não representa uma seção do tipo TEXT ou DATA");

	return TYPE_SECTION::NOT_DEFINED;
}



void _asm::resolveLable(const std::list<Operation>::iterator op, 
												std::vector<int> &obj, int pc,
											 	TYPE_SECTION section,
												SymbolTable &symTable, 
												ModifierTable &modTable,
												bool isConst){

	SymbolTable::iterator it = symTable.find(op->lable);
	if(it != symTable.end()){
		if(!it->second.defined){
			it->second.value = pc;
			it->second.defined = true;
			it->second.section = section;
			it->second.constant = isConst;
			callBackSymbol(it,obj,modTable);
		}
		else{
			io::outError(std::cerr,"semântico","declarações e rótulos repetidos",op->line,
									op->complete,"Note: "+op->lable+" já foi definido anteriomente");
		}		
	}else{
		symTable.insert(std::make_pair(op->lable,symbol(pc,true,-1,section,isConst)));
	}
}


void _asm::callBackSymbol(SymbolTable::iterator &itSymbol, std::vector<int> &obj, ModifierTable &modTable){
	
	for(std::list<Operation>::iterator op : itSymbol->second.lOp){
		semanticErrorLable(itSymbol,op,obj);	
	}	

	int aux;
	
	do{
		aux = obj[itSymbol->second.list];
		obj[itSymbol->second.list] = itSymbol->second.value + modTable[itSymbol->second.list];
		itSymbol->second.list = aux;
	}while(itSymbol->second.list != -1);	

}


void _asm::resolveOperand(std::string operand,std::string op_mod,
													std::list<Operation>::iterator &op,
													std::vector<int> &obj ,int pc,
													SymbolTable &symTable,
													ModifierTable &modTable){	

	if(operand.empty()){
		obj.push_back(-1);
		return;
	}

	try{
		std::stoi(operand,nullptr,0);
		//o operando é um numero inteiro, possivel quando usado
		//com o EQU
		io::outError(std::cerr,"semântico","tipo de argumento inválido",op->line,
													 op->complete,"O operando "+operand+
													 " é inválido, verifica se esse rótulo não é um rótulo de EQU");

	}catch(std::invalid_argument){}

	int modifier = 0;
	if(!op_mod.empty()){
		try{
			modifier = std::stoi(op_mod,nullptr,0);
		}catch(std::invalid_argument){
			io::outError(std::cerr,"semântico","tipo de argumento inválido",op->line,
														op->complete,"O modificador "+op_mod+
														" é inválido, deveria ser um valor numerico");
		}
	}


	auto it = symTable.find(operand);
	if(it != symTable.end()){
		if(it->second.defined){
			semanticErrorLable(it,op,obj);	
			obj.push_back(it->second.value + modifier);
		}
		else{
			it->second.lOp.push_back(op);

			obj.push_back(it->second.list);
			it->second.list = pc;	
			
			modTable.insert(std::make_pair(pc,modifier));
		}
	}
	else{
		Symbol s;
		s.defined = false;
		s.list = pc;
		s.lOp.push_back(op);
		obj.push_back(-1);
		symTable.insert(std::make_pair(operand,s));
		modTable.insert(std::make_pair(pc,modifier));
	}
	

}

Symbol symbol(int value, bool defined, int list, TYPE_SECTION section, bool constant){
	Symbol s;
	s.value = value;
	s.defined = defined;
	s.list = list;
	s.section = section;
	s.constant = constant;
	return s;
};

void _asm::semanticErrorLable(const SymbolTable::iterator itSymbol,
	 														const std::list<Operation>::iterator op,
															const std::vector<int> obj){
	switch(op->instCod){
		case JMP:
		case JMPN:
		case JMPP:
		case JMPZ:
			if(itSymbol->second.section != TEXT){
				io::outError(std::cerr,"semântico","pulo para seção errada",op->line,
										op->complete,"Note: A instrução jump tem que ser feita para um rotulo na seção TEXT");

				io::outError(std::cerr,"semântico","pulo para rótulo invalido",op->line,op->complete);
			}
			break;
		case DIV:
			if(itSymbol->second.constant){
				if(obj[itSymbol->second.value] == 0)
					io::outError(std::cerr,"semântico","divisão por zero",op->line,op->complete,
											"Note: não se pode fazer divizar por uma constate de valor 0");
			}
			break;
		case COPY:
			//se for COPY e o operador 1 nao for igual 
			//ao simbolo sai 
			//Logo
			//se for COPY e o primeiro operador for o simbolo eu continuo 
			//no switche assim ali na frente se o simbolo for 
			//constante da erro 
			if(op->first_op.compare(itSymbol->first))
				break;	
			
		case STORE:
		case INPUT:
			if(itSymbol->second.constant){
				io::outError(std::cerr,"semântico","modificação de um valor constante",op->line,op->complete,
										"Note: Essa intrução esta modificando um endereço declarado como constante");
			}
			break;
	}
}


void _asm::identifyError(std::list<Operation>::iterator op){
	const std::regex doubleLable ("[ \t]*(?:([a-zA-Z_]\\w*):\\s*)(?:([a-zA-Z_]\\w*):\\s*).*");
	std::smatch sm;

	
	std::string::const_iterator searchStart( op->complete.cbegin());
	regex_search( searchStart, op->complete.cend(), sm, doubleLable);
	
	if(!sm.empty()){
		io::outError(std::cerr,"sintático","dois rótulos na mesma linha",op->line,op->complete);
	}else{
		io::outError(std::cerr,"léxico","tokens inválidos",op->line,op->complete);
	}

}


