#ifndef PARALLELDEPENDENCYEXECUTIVE_H
#define PARALLELDEPENDENCYEXECUTIVE_H
////////////////////////////////////////////////////////////////////////////
// ParallelDependencyExecutive.h - Provides functionality to display      // 
//									 multiple Parallel Analysis and       // 
//						             Project Requirements                 // 
// Ver 1.0                                                                //
// Application: Project #3 Parallel Dependency Analysis                   //
// Language:    C++, Visual Studio 2015									  //
// Platform:    Dell Inspiron 5548, Core-i5, Windows 10					  //
// Author:      Achal Velani, SUID:292190387							  //
// Reference:   Jim Fawcett, CST 4-187, Syracuse University				  //
//              (315) 443-3948, jfawcett@twcny.rr.com					  //
////////////////////////////////////////////////////////////////////////////
/*
* Package Operations :
*== == == == == == == == == == == == == == == =
* This package acts as the main executive for the whole project. 
* it will use parallelDependencyAnalysis to do two types of analysis: type and dependency analysis.
* This package will pares the required files from the given directory.
*
* Public Interface :
*== == == == == == == == == == == == == == ==
*	void fetchFiles(std::string path, std::vector<std::string> patterns)- Fetches all the files from file path specified 
*	int getThreadCount(size_t fileSize) - This function helps deciding the number of threads to be initialized. 
*   std::vector<std::string> getFiles() - this function simply returns the files vector.
*	std::vector<std::string> ParallelDependencyExecutive::parseFilePatterns(std::string patterns)- This function helps in discovering patterns
*	bool runAnalysis(std::vector<std::string>,size_t threadCount) - this is the main function which calls the parallel dependency analysis for all the files.
*	void R1_R2() - displays Requirement 1 and 2.
*	void R3() - displays Requirement 3.
*	void R4_R5() - displays Requirement 4 and 5.
*	void R6_7_8_9() - displays Requirement 6,7,8 and 9.
*	void R10() - displays Requirement 10
*
* Required Files :
*== == == == == == == == == == == == == ==
* ActionAndRules.h, ASTTree.h, ConfigureParser.h, DependencyAnalysis.h, FileSystem.h, Parser.h, SemiExp.h, Tokenizer.h,
* TypeAnalysis.h, TypeTable.h, Datastore.h, FileMagr.h, itokcollection.h, ParrallelDependency.h, ThreadPool.h
* Utilities.h, WorkItem.h, Cpp11-BlockingQueue.h, ActionAndRules.cpp, ASTTree.cpp, ConfigureParser.cpp, DependencyAnalysis.cpp, FileSystem.cpp,
* Parser.cpp, SemiExp.cpp, Tokenizer.cpp, TypeAnalysis.cpp, TypeTable.cpp, Datastore.cpp, FileMagr.cpp, itokcollection.cpp, ParrallelDependency.cpp,
* ThreadPool.cpp, Utilities.cpp, WorkItem.cpp, Cpp11-BlockingQueue.cpp, ParallelDependencyAnalysis.h, ParallelDependencyAnalysis.cpp
* ParallelDependencyExecutive.h, ParallelDependencyExecutive.cpp
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


#include <vector>
#include <iostream>
#include"../FileMgr/FileMgr.h"
#include"../DataStore/DataStore.h"
#include"../ThreadPool/ThreadPool.h"
#include"../ParallelDependencyAnalysis/ParallelDependencyAnalysis.h"
#include"../Utilities/Utilities.h"
#include <sstream>
#include <iterator>
#include "../WorkItem/WorkItem.h"
#include"../Parser/Parser.h"
#include"../Parser/ActionsAndRules.h"
#include"../Parser/ConfigureParser.h"

#define MAX_THREAD_COUNT 10

class ParallelDependencyExecutive
{
private:
	std::vector<std::string> files;// this vector will store all the file paths
public:
	void fetchFiles(std::string path, std::vector<std::string> patterns);
	int getThreadCount(size_t fileSize);
	std::vector<std::string> getFiles() {
		return files;
	}
	std::vector<std::string> ParallelDependencyExecutive::parseFilePatterns(std::string patterns);
	bool runAnalysis(std::vector<std::string>,size_t threadCount);
	void R1_R2();
	void R3();
	void R4_R5();
	void R6_7_8_9();
	void R10();	
};

#endif
