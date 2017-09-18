
#include "../header/preprocessing.h"

using namespace std;


int main (int argc, char *argv[]){	
	
	std::list<Operation> code;


	pp::manager(argv,code);

	for(Operation op : code){
	
		std::cout << op.line << " "  << op.instruction << " " << op.first_op << endl;
	}

}

