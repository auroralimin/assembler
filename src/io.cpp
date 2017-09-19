#include "../header/io.h"


void io::outError(std::ostream &stream, std::string type, std::string msg,
									int line, std::string code, std::string extra){
	stream << "ERRO " << type << ":\t" << msg << std::endl;
	stream << extra << std::endl;
	stream << "LINHA: " << line << "\t"  << code << std::endl;	
	

}

