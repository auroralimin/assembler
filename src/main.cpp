#include "../header/preprocessing.h"
#include "../header/assembler.h"
#include "../header/structures.h"

using namespace std;


int main (int argc, char *argv[]){	
	

	if(argc != 4){
		std::cout << "Parametros inválidos!\n";
		std::cout << "./montador -o myprog.asm myprog.o\n";
		std::cout << "-p: preprocessamento, avalia EQU IF\n";
		std::cout << "-m: macros, avalia EQU IF e expande macros\n";
		std::cout << "-o: monta o arquivo objeto\n";
		return -1;
	}

	std::string fileName;
	io::catFileName(argv[2],fileName);
	
	EquIf equIf; 
	Macro macro;
	
	std::list<Operation> code;
	std::vector<int> obj;

	std::string strCode;
	pp::file2str(argv[2],strCode);	
	
	pp::read_code(strCode,code,equIf,macro);
	if(!strcmp(argv[1],"-p") || !strcmp(argv[1],"-m") || !strcmp(argv[1],"-o")){
		pp::equIfResolve(code,equIf);

		std::filebuf fb;
		fb.open(fileName + ".pre", std::ios::out);
		std::ostream out (&fb);
		io::outCode(out,code);
		fb.close();

		if(!strcmp(argv[1],"-m") || !strcmp(argv[1],"-o")){
			pp::expandMacro(code,macro);

			fb.open(fileName + ".mcr", std::ios::out);
			std::ostream out (&fb);
			io::outCode(out,code);
			fb.close();

			if(!strcmp(argv[1],"-o")){
				_asm::manager(code,obj);

				fb.open(argv[3], std::ios::out);
				std::ostream out (&fb);
				io::outObj(out,obj);
				fb.close();
			}
		}
	}
	else{
		std::cout << "Parametros inválidos!\n";
		std::cout << "./montador -o myprog.asm myprog.o\n";
		std::cout << "-p: preprocessamento, avalia EQU IF\n";
		std::cout << "-m: macros, avalia EQU IF e expande macros\n";
		std::cout << "-o: monta o arquivo objeto\n";
		return -2;
	}

}

