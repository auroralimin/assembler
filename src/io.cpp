#include "../header/io.h"


void io::outError(std::ostream &stream, std::string type, std::string msg,
									int line, std::string code, std::string extra){
	stream << "ERRO " << type << ":\t" << msg << std::endl;
	if(!extra.empty())
		stream << extra << std::endl;
	stream << "LINHA: " << line << "\t"  << code << std::endl;	

}

void io::catFileName(char *file, std::string &fileName){
	//Regex para pegar o nome do arquivo sem extensão
	std::string strFile (file);
	const std::regex regex("(.*)\\.asm$");
	std::smatch sm;
	
	std::string::const_iterator searchStart( strFile.cbegin() );
	regex_search( searchStart, strFile.cend(), sm, regex);
	if(!sm.empty()){
		fileName = sm[1];
	}else{
		std::cout << "O aquivo de entrada tem que ser um arquivo .asm\n";
		exit(-2);
	}

}
