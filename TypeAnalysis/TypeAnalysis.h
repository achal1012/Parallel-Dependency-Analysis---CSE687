#ifndef TYPEANALYSIS_H
#define TYPEANALYSIS_H
////////////////////////////////////////////////////////////////
// TypeAnalysis.h - Finds all the types defined in each of a  // 
//                  collection of C++ source files            //
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
* Finds all the types defined in each of a collection of C++ source files. 
* It does this by building rules to detect type definitions: classes, structs, enums, 
* typedefs, and aliases, and capture their fully qualified names and files where they 
* are defined.
*
* Public Interface :
*== == == == == == == == == == == == == == ==
* bool typeAnalysis(std::string fileName) - this is the main function which will be called for each file in files vector.
*											to perform type analysis.
*
* Required Files :
*== == == == == == == == == == == == == ==
* ActionAndRules.h, ASTTree.h, ConfigureParser.h, FileMgr.h, FileSystem.h, Parser.h, SemiExp.h, Tokenizer.h, Utilities.h
* TypeAnalysis.h, TypeTable.h, DataStore.h, ActionAndRules.cpp, ASTTree.cpp, ConfigureParser.cpp, FileSystem.cpp,
* Parser.cpp, SemiExp.cpp, Tokenizer.cpp, TypeAnalysis.cpp, TypeTable.cpp , DataStore.cpp, FileMgr.cpp, Utilities.cpp
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

#include<string>
#include <iostream>
#include <sstream>
#include <iterator>
#include"../ASTTree/ASTTree.h"
#include"../ScopeStack/ScopeStack.h"
#include"../TypeTable/TypeTable.h"
#include"../Parser/Parser.h"
#include"../Parser/ActionsAndRules.h"
#include"../Parser/ConfigureParser.h"
#include"../FileMgr/FileMgr.h"
#include"../FileSystem-Windows/FileSystemDemo/FileSystem.h"
#include"../DataStore/DataStore.h"
#include"../ThreadPool/ThreadPool.h"

class TypeAnalysis{
public:
	bool typeAnalysis(std::string fileName);
};

#endif