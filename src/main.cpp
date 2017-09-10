#include <iostream>
#include <algorithm>
#include <regex>
#include <string>
#include <fstream>
#include <streambuf>

using namespace std;


int main (int argc, char *argv[]){	
	
	
	std::ifstream t(argv[2]);
	std::string str;

	t.seekg(0, std::ios::end);   
	str.reserve(t.tellg());
	t.seekg(0, std::ios::beg);

	str.assign((std::istreambuf_iterator<char>(t)),
			            std::istreambuf_iterator<char>());
	
	std::cout << str;
	
	std::cout << "\n========================================\n";

	const std::regex regex("^[ \\t]*(?:([a-zA-Z_]\\w*):\\s*)?(?:([a-zA-Z]+)\\s*)(?:(-?\\w+)(?:\\s*\\+\\s*(\\d))?)?(?:, (\\w+)(?:\\s*\\+\\s*(\\d))?[\\t ]*(?:;.*)?\\n|[\\t ]*(?:;.*)?\\n)");
	const std::regex rerror(".*\\n");
	const std::regex rline("^[ \\t]*(?:;.*)?\\s");
	smatch sm;
	
	int line = 0;
	
	string::const_iterator searchStart( str.cbegin() );

	while (searchStart != str.cend()){
		line++;
		regex_search( searchStart, str.cend(), sm, regex); 
		if(sm.empty()){
			regex_search( searchStart, str.cend(), sm, rline);
		 	if(sm.empty()){	
				regex_search(searchStart, str.cend(), sm, rerror);
				cout << "error in line: " << line << " - " << sm[0] << endl;
			}	
		}else{
			cout << "line: " << line << " " << sm[1] << " " << sm[2] << " " << sm[3] << " " << sm[4] << " " << sm[5]  << " " << sm[6] << endl;
			std::string str(sm[0]);
		//	cout << "line ant: " << line << endl;
			//cout << str;
			line += std::count(str.begin(), str.end(), '\n') - 1;
		//	cout << "line pos:" << line << endl;
		}
		searchStart += sm.position() + sm.length();
	}


}

