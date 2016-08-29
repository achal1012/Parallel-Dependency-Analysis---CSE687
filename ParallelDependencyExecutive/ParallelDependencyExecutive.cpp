////////////////////////////////////////////////////////////////////////////
// ParallelDependencyExecutive.cpp - Provides functionality to display    // 
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

#include"ParallelDependencyExecutive.h"

//this is the main function which calls the parallel dependency analysis for all the files.
bool ParallelDependencyExecutive::runAnalysis(std::vector<std::string> files, size_t threadCount){
	ParallelDependencyAnalysis pda;
	pda.firstPass(files, threadCount);
	pda.secondPass(files, threadCount);
	return false;
}

//Fetches all the files from file path specified
void ParallelDependencyExecutive::fetchFiles(std::string path, std::vector<std::string> patterns){
	DataStore ds;
	FileMgr fm(path, ds);

	for (std::string pat : patterns){
		fm.addPattern(pat);
	}
	fm.search();
	for (auto fs : ds){
		files.push_back(fs);
	}
}

int ParallelDependencyExecutive::getThreadCount(size_t fileSize){
	if (fileSize > MAX_THREAD_COUNT)
		return MAX_THREAD_COUNT;
	else
		return fileSize;
}


//This function will parse all the patterns from command line
std::vector<std::string> ParallelDependencyExecutive::parseFilePatterns(std::string patterns){
	std::istringstream iss(patterns);
	// splitting with space and add in vector.
	std::vector<std::string> tokens{ std::istream_iterator<std::string>{iss},
		std::istream_iterator<std::string>{} };
	return tokens;
}
void ParallelDependencyExecutive::R1_R2()

{
	std::cout << "\n****************************************************************************************************************************\n";
	std::cout << "REQUIREMENT 1 & 2:\n";
	std::cout << "****************************************************************************************************************************\n\n";
	std::cout << "I have used Visual Studio 2015 and its C++ Windows Console Projects\n";
	std::cout << "I have used C++ standard library's streams for all I/O and new and delete for all heap-based memory management\n\n\n";
}

void ParallelDependencyExecutive::R3()
{
	std::cout << "****************************************************************************************************************************\n";
	std::cout << "REQUIREMENT 3:\n";
	std::cout << "****************************************************************************************************************************\n\n";
	std::cout << " I have made ThreadPool, Tasks, DependencyAnalysis, ParallelDependencyAnalysis and ParallelDependencyExecutive packages.\n";
	std::cout << "And \"Single Responsiblity Principle\" is satisfied during doing so.\n";
}

void ParallelDependencyExecutive::R4_R5()
{
	std::cout << "\n****************************************************************************************************************************\n";
	std::cout << "REQUIREMENT 4 & 5:\n";
	std::cout << "****************************************************************************************************************************\n\n";
	std::cout << "I have provided ThreadPool class that executes enqueued work items asynchronously, and Task class executes callable object\n on threadpool thread.";

}
void ParallelDependencyExecutive::R6_7_8_9()
{
	std::cout << "\n****************************************************************************************************************************\n";
	std::cout << "REQUIREMENT 6, 7, 8 & 9:\n";
	std::cout << "****************************************************************************************************************************\n\n";
	std::cout << "I have provided \n\t-TypeAnalysis package to get typeTable for a single file."
			  << "\n\t-DependencyAnalysis package to find dependecie of a file on other files using TypeTable."
			  << "\n\t-ParallelDependencyAnalysis package which will in first pass find all the types, and in second pass will create dependecy table."
			  << "\n\t The following presentation is done by executing ParallelDependencyExecutive:\n\n";
}
void ParallelDependencyExecutive::R10()
{
	std::cout << "\n****************************************************************************************************************************\n";
	std::cout << "REQUIREMENT 10:\n";
	std::cout << "****************************************************************************************************************************\n\n";
	std::cout << "This is the end of automated test suit.\n\n";
}
using WorkResult = std::string;

#ifdef TEST_PARALLELDEPENDENCYEXECUTIVE
int main(int argc, char* argv[]) {
	
	ParallelDependencyExecutive pde;
	pde.R1_R2();
	pde.R3();
	pde.R4_R5();
	pde.R6_7_8_9();
	std::vector<std::string> patterns;
	std::string path;
	if (argc > 3) {
		std::cout << "\n-------------------------------------------------------------";
		std::cout << "\nInvalid arguments passed! Program is Terminated!\n";
		std::cout << "\n-------------------------------------------------------------";
		return 0;
	}
	if (argc < 2) {
		path = "./Testinput";
		std::cout << "\n-------------------------------------------------------------";
		std::cout << "\nPath not entered default path is being used: " << path;
		std::cout << "\n-------------------------------------------------------------";
	}
	else {
		path = argv[1];
	}
	if (argc == 3)
		patterns = pde.parseFilePatterns(argv[2]);// parse arguments
	else {
		// set default patterns .cpp and .h
		patterns.push_back("*.cpp");
		patterns.push_back("*.h");
		std::cout << "\n-------------------------------------------------------------";
		std::cout << "\n Patterns to get files not found .cpp and .h are set as default patterns.";
		std::cout << "\n-------------------------------------------------------------";
	}
	std::cout << "\n-------------------------------------------------------------";
	std::cout << "\nFollowing files are found from derectory: " << path;
	std::cout << "\n-------------------------------------------------------------";
	pde.fetchFiles(path, patterns);
	int threadCount = pde.getThreadCount(pde.getFiles().size());
	std::vector<std::string> files = pde.getFiles();
	if (files.size() == 0)
	{
		std::cout << "\nNo files Found\n";
		return 0;
	}
	pde.runAnalysis(files, threadCount);
	ParallelDependencyAnalysis::displayFinalDependency();
	pde.R10();
	return 0;
}
#endif