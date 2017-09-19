#include <stdlib.h>
#include <iostream>


namespace io{

	//TODO melhorar essa saida para nao ter coisas desnecessarias 
	template <typename Container> 
	inline void outCode(std::ostream &stream, const Container &container){
		for(const auto &op : container)
			stream << op.complete;
	}

	void outError(std::ostream &stream, std::string type, std::string msg,
								int line, std::string code, std::string extra = std::string(""));



}
