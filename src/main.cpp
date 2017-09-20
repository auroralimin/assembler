#include "../header/preprocessing.h"
using namespace std;


int main (int argc, char *argv[]){	
	
	std::list<Operation> code;

	pp::manager(argv,code);

 	io::outCode(std::cout,code);
}

