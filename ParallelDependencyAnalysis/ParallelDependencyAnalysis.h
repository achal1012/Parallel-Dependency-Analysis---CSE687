#ifndef PARALLELDEPENDENCYANALYSIS_H
#define PARALLELDEPENDENCYANALYSIS_H
//////////////////////////////////////////////////////////////////////////
// ParrallelDependencyAnalysis.h - Checks Dependenies for provided files// 
// Ver 1.0                                                              //
// Application: Project #3 Parallel Dependency Analysis                 //
// Language:    C++, Visual Studio 2015                                 //
// Platform:    Dell Inspiron 5548, Core-i5, Windows 10                 //
// Author:      Achal Velani, SUID:292190387                            //
// Reference:   Jim Fawcett, CST 4-187, Syracuse University			    //
//              (315) 443-3948, jfawcett@twcny.rr.com				    //
/////////////////////////////////////////////////////////////////////////
/*
* Package Operations :
*== == == == == == == == == == == == == == == =
* This package provides functionalites to build final dependency analysis for more than one file.
*
* Public Interface :
*== == == == == == == == == == == == == == ==
* static void meregeDependencies() - this function provides functionalities to merge dependencies from all partial dependencies.
* static std::unordered_map<std::string, std::set<std::string>> merge(std::unordered_map<std::string, std::set<std::string>>, std::unordered_map<std::string, std::set<std::string>>) 
*								   - This function simply merges two unordered maps. 
* static void displayFinalDependency() - This function displays final dependency table in proper fashion.
* static void addInstances(DependencyAnalysis depanal) - This function will store all the DependencyAnalysis objects for retriving of dependency purpose.
* bool firstPass(std::vector<std::string> files, size_t threadCount) - This function will call TypeAnalysis for all the files.
* bool secondPass(std::vector<std::string> files, size_t threadCount)- This function will call Dependency Analysis for all the files.
*
*
* Required Files :
*== == == == == == == == == == == == == ==
* ActionAndRules.h, ASTTree.h, ConfigureParser.h, DependencyAnalysis.h, FileSystem.h, Parser.h, SemiExp.h, Tokenizer.h,
* TypeAnalysis.h, TypeTable.h, Datastore.h, FileMagr.h, itokcollection.h, ParrallelDependencyAnalysis.h, ThreadPool.h
* Utilities.h, WorkItem.h, Cpp11-BlockingQueue.h, ActionAndRules.cpp, ASTTree.cpp, ConfigureParser.cpp, DependencyAnalysis.cpp, FileSystem.cpp,
* Parser.cpp, SemiExp.cpp, Tokenizer.cpp, TypeAnalysis.cpp, TypeTable.cpp, Datastore.cpp, FileMagr.cpp, itokcollection.cpp, ParrallelDependencyAnalysis.cpp,
* ThreadPool.cpp, Utilities.cpp, WorkItem.cpp, Cpp11-BlockingQueue.cpp
*
* Build Command :
*== == == == == == == == == == == == == ==
* devenv Project3.sln / rebuild debug
*
* Maintenance History :
*== == == == == == == == == == == == == ==
* ver 1.0 : 13 Mar 16
* -first release
*/

#include<vector>
#include <iostream>
#include<string>
#include"../FileMgr/FileMgr.h"
#include"../DataStore/DataStore.h"
#include"../ThreadPool/ThreadPool.h"
#include <sstream>
#include <iterator>
#include "../WorkItem/WorkItem.h"
#include"../Parser/Parser.h"
#include"../Parser/ActionsAndRules.h"
#include"../Parser/ConfigureParser.h"
#include"../TypeAnalysis/TypeAnalysis.h"
#include"../DependencyAnalysis/DependencyAnalysis.h"
#include<mutex>
#include <iomanip>
#include"../Task/Task.h"

class ParallelDependencyAnalysis{
private:
	static std::mutex mtx;
protected:
	static std::unordered_map<std::string, std::set<std::string>> finalDependencies;
	static std::vector<DependencyAnalysis> setOfDependencyObjects;
public:
	static void meregeDependencies();
	static std::unordered_map<std::string, std::set<std::string>> merge(std::unordered_map<std::string, std::set<std::string>>, std::unordered_map<std::string, std::set<std::string>>);
	static void displayFinalDependency();
	static void addInstances(DependencyAnalysis depanal);
	bool firstPass(std::vector<std::string> files, size_t threadCount);
	bool secondPass(std::vector<std::string> files, size_t threadCount);
};

#endif

