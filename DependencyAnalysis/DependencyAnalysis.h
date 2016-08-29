#ifndef DEPENDENCYANALYSIS_H
#define DEPENDENCYANALYSIS_H
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
/*
* Package Operations :
*== == == == == == == == == == == == == == == =
* This package functionalites to check dependencies of given file using 
* existing type table.
*
* Public Interface :
*== == == == == == == == == == == == == == ==
*
* bool searchDependency(std::string filename) - main function which will search for all the dependencies.
* void displayDependency() - this function helps in printing the dependencies for the fiven file.
* std::unordered_map<std::string, std::set<std::string>> returnDependency() - this package helps higher layer packages to
																			  retrive dependencies for each files by returning 
																			  them the unordered_map od dependencies.
*
* Required Files :
*== == == == == == == == == == == == == ==
* ActionAndRules.h, ASTTree.h, ConfigureParser.h, DependencyAnalysis.h, FileSystem.h, Parser.h, SemiExp.h, Tokenizer.h,
* TypeAnalysis.h, TypeTable.h, ActionAndRules.cpp, ASTTree.cpp, ConfigureParser.cpp, DependencyAnalysis.cpp, FileSystem.cpp, 
* Parser.cpp, SemiExp.cpp, Tokenizer.cpp, TypeAnalysis.cpp, TypeTable.cpp.
*
* Build Command :
*== == == == == == == == == == == == == ==
* devenv Project3.sln / rebuild debug
*
* Maintenance History :
*== == == == == == == == == == == == == ==
* ver 1.0 : 05 Apr 16
* -first release
*/



#include<unordered_map>
#include<vector>
#include <iostream>
#include <cctype>
#include<string>
#include <iosfwd>
#include<sstream>
#include<fstream>
#include<set>
#include"../Tokenizer/Tokenizer.h"
#include"../ASTTree/ASTTree.h"
#include"../ScopeStack/ScopeStack.h"
#include"../TypeTable/TypeTable.h"
#include"../Parser/Parser.h"
#include"../Parser/ActionsAndRules.h"
#include"../Parser/ConfigureParser.h"
#include "../TypeAnalysis/TypeAnalysis.h"

class DependencyAnalysis
{
public:
	bool searchDependency(std::string filename);
	void displayDependency();
	std::unordered_map<std::string, std::set<std::string>> returnDependency();
private:
	std::unordered_map<std::string, std::set<std::string>> dependencies;
	
};

#endif