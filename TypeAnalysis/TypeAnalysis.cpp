////////////////////////////////////////////////////////////////
// TypeAnalysis.cpp - Finds all the types defined in each of a// 
//                    collection of C++ source files          //
// Ver 1.0                                                    //
// Application: Project #3 Parallel Dependency Analysis       //
// Language:    C++, Visual Studio 2015                       //
// Platform:    Dell Inspiron 5548, Core-i5, Windows 10       //
// Author:      Achal Velani, SUID:292190387                  //
// Reference:   Jim Fawcett, CST 4-187, Syracuse University   //
//              (315) 443-3948, jfawcett@twcny.rr.com         //
////////////////////////////////////////////////////////////////


#include"TypeAnalysis.h"


bool TypeAnalysis::typeAnalysis(std::string fileName)
{
	// Upoon call of this function typeTable would be created in the Repository.
	// This function will be called for each file in files to create final type table in repository.
	std::string fileSpec = FileSystem::Path::getFullFileSpec(fileName);	
	ConfigParseToConsole configure;
	Parser* pParser = configure.Build();
	try {
		if (pParser) {
			if (!configure.Attach(fileSpec)) {
				std::cout << "\n  could not open file " << fileSpec << std::endl;
				return false;
			}
		}
		else {
			std::cout << "\n\n  Parser not built\n\n";
			return false;
		}
		// now that parser is built, we use it
		while (pParser->next())
			pParser->parse();
		return false;
	}
	catch (std::exception& ex) {
		std::cout << "\n\n    " << ex.what() << "\n\n";
	}
	std::cout << "\n";
	return true;
}

#ifdef TEST_TYPEANALYSIS
int main()
{
	TypeAnalysis t;
	std::string fileSpec = FileSystem::Path::getFullFileSpec("test.cpp");
	t.typeAnalysis("test.cpp");
	Repository* repo = Repository::getInstance(fileSpec);
	std::cout << "\n------- TypeTable For a Singe file:\n" << fileSpec;
	repo->getTypeTable()->showTypeTable();
	return 0;
}

#endif // TEST_TYPEANALYSIS
