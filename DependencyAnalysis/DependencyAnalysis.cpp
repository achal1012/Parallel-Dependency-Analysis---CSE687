////////////////////////////////////////////////////////////////
// DependencyAnalysis.h - Checks Dependenies for provided file// 
// Ver 1.0                                                    //
// Application: Project #3 Parallel Dependency Analysis       //
// Language:    C++, Visual Studio 2015                       //
// Platform:    Dell Inspiron 5548, Core-i5, Windows 10       //
// Author:      Achal Velani, SUID:292190387                  //
// Reference:   Jim Fawcett, CST 4-187, Syracuse University   //
//              (315) 443-3948, jfawcett@twcny.rr.com         //
////////////////////////////////////////////////////////////////



#include "DependencyAnalysis.h"

// This function will search for all the dependencies of the on typetable, by tokenizing the file.
bool DependencyAnalysis::searchDependency(std::string filename)
{
	std::string fileSpec = filename;
	std::ifstream in(fileSpec);
	if (!in.good()){
		std::cout << "\n  can't open " << fileSpec << "\n\n";
		return false;
	}
	Scanner::Toker toker;
	toker.toggleComments("false");
	toker.attach(&in);
	while (in.good()){
		std::string tok = toker.getTok();
		if (tok == "\n")
			tok = "newline";
		Repository *repo = Repository::getInstance(fileSpec);
		TypeTable ft = repo->getFinalTable();
		std::unordered_map<std::string, RecordData> *lookupMap= ft.returntableRecord();
		std::unordered_map<std::string, RecordData>::iterator foundtok = lookupMap->find(tok);
		if (!(foundtok == lookupMap->end())){
			std::vector<std::pair<std::string, std::string>> a = foundtok->second.getfileNameSpace();
			for (std::pair<std::string, std::string> i : a){
				if (!(fileSpec == i.first))
					dependencies[fileSpec].insert(i.first);// +" : " + tok);
			}
		}
	}
	return true;
}

// This function will show the dependencies of current file.
void DependencyAnalysis::displayDependency(){
	for (auto i : dependencies){
		std::cout << std::endl <<" "+ i.first << " Depends ON: ";
		for (auto j : i.second)
			std::cout << std::endl << "\t" <<"-"+j;
		std::cout << "\n\n";
	}
}

// This will return dependency vector to provide merging functionalities.
std::unordered_map<std::string, std::set<std::string>> DependencyAnalysis::returnDependency(){
	return dependencies;
}


#ifdef TEST_DEPENDENCYANALYSIS

int main()
{
	//std::cout << "hi";
	std::string fileSpec = "A.cpp";
	TypeAnalysis t;
	// small prototype showing the construction of typeTable.
	t.typeAnalysis("A.h");
	t.typeAnalysis("A.cpp");
	Repository::mergeToTypeTable();
	Repository::getFinalTable().showTypeTable();

	DependencyAnalysis da;
	da.searchDependency(fileSpec);
	std::cout << "\n--------------------Depndencies:--------------------\n";
	da.displayDependency();
}

#endif // TEST_DEPENDENCYANALYSIS
