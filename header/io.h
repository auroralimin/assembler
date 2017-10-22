
#ifndef IO_H
#define IO_H

#include <stdlib.h>
#include <iostream>
#include <regex>


namespace io{

	template <typename Container> 
	inline void outCode(std::ostream &stream, const Container &container){
		for(const auto &op : container){
			if(!op.lable.empty())
				stream << op.lable << ":";
			stream << "\t";
			stream << op.instruction << "\t";
			stream << op.first_op << "\t";
			if(!op.first_mod.empty())
				stream << "+\t" << op.first_mod << "\t";
			if(!op.second_op.empty())
				stream << "," << op.second_op << "\t";
			if(!op.second_mod.empty())
				stream << "+\t" << op.second_mod << "\t";

			stream << "\n";
		
		}
	}

	
	inline void outObj(std::ostream &stream, const std::vector<int> &obj){
		for(int o : obj){
			stream << o << " "; 
		}
	}

	void outError(std::ostream &stream, std::string type, std::string msg,
								int line, std::string code, std::string extra = std::string(""));


	
	void catFileName(char *file, std::string &fileName);

}

#endif
