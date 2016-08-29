///////////////////////////////////////////////////////////////////////////
// ParrallelDependencyAnalysis.cpp - Checks Dependenies for provided file// 
// Ver 1.0                                                               //
// Application: Project #3 Parallel Dependency Analysis                  //
// Language:    C++, Visual Studio 2015                                  //
// Platform:    Dell Inspiron 5548, Core-i5, Windows 10                  //
// Author:      Achal Velani, SUID:292190387                             //
// Reference:   Jim Fawcett, CST 4-187, Syracuse University	             //
//              (315) 443-3948, jfawcett@twcny.rr.com			  	     //
///////////////////////////////////////////////////////////////////////////


#include"ParallelDependencyAnalysis.h"


std::unordered_map<std::string, std::set<std::string>> ParallelDependencyAnalysis::finalDependencies;
std::vector<DependencyAnalysis> ParallelDependencyAnalysis::setOfDependencyObjects;
std::mutex ParallelDependencyAnalysis::mtx;

//This function merges all the dependencies in one unordered map.
void ParallelDependencyAnalysis::meregeDependencies(){
	for (DependencyAnalysis i : setOfDependencyObjects){
		auto objectDependency = i.returnDependency();
		finalDependencies = merge(objectDependency, finalDependencies);
	}
}

// This is a simple merge function for two unordered_map
std::unordered_map<std::string, std::set<std::string>> ParallelDependencyAnalysis::merge(std::unordered_map<std::string, std::set<std::string>> objectDependency, std::unordered_map<std::string, std::set<std::string>> finalDependencies){
	std::unordered_map<std::string, std::set<std::string>> temp(finalDependencies);
	temp.insert(objectDependency.begin(), objectDependency.end()); 
	return temp;
}

//This function will print the final dependency table.
void ParallelDependencyAnalysis::displayFinalDependency(){
	ParallelDependencyAnalysis::meregeDependencies();
	std::cout << "\n-------------------------------------------------------------";
	std::cout << "\n FINAL DEPENDENCY TABLE:";
	std::cout << "\n-------------------------------------------------------------\n";
	for (auto i : finalDependencies)
	{
		std::cout << std::endl <<"FILE NAME:"<< i.first;
		std::cout << "\n\tDepends On:";
		for (auto j : i.second)
			std::cout << std::endl << "\t\t --" + j;
		std::cout << std::endl;

	}
	std::cout << std::endl;
}

// This function adds all the used instances of the DependencyAnalysis to a vector.
void ParallelDependencyAnalysis::addInstances(DependencyAnalysis depanal){
	std::lock_guard<std::mutex> lock(mtx);
	setOfDependencyObjects.push_back(depanal);
}

// This function calls TypeAnalysis for each file in files vector.
bool ParallelDependencyAnalysis::firstPass(std::vector<std::string> files, size_t threadCount)
{
	Task<bool, std::string> taskTypeAnal;
	TypeAnalysis typeanal;
	taskTypeAnal.start(threadCount);
	std::cout << "\n****************************************************************************************************************************\n";
	std::cout << "FIRST PASS:\n";
	std::cout << "****************************************************************************************************************************\n\n";
	try {
		if (files.size() > 0) {
			for (size_t i = 0; i < files.size(); i++) {
				std::function<bool(std::string)> w = [&](std::string file) { return typeanal.typeAnalysis(files[i]); };
				workItem<bool, std::string> *wi1 = new workItem<bool, std::string>(&w, &files[i]);
				taskTypeAnal.doWork(wi1);
				taskTypeAnal.result();
			}
		}
	}
	catch (std::exception ex) {
		std::cout << ex.what();
		return false;
	}
	taskTypeAnal.doWork(nullptr);
	taskTypeAnal.wait();
	//This function will merge all the partial type tables.
	Repository::mergeToTypeTable();
	// This function will print the final typeTable on the console.
	Repository::getFinalTable().showTypeTable();
	return true;
}

//This function will call dependency analysis for all the files in the files vector.
bool ParallelDependencyAnalysis::secondPass(std::vector<std::string> files, size_t threadCount){
	std::cout << "\n****************************************************************************************************************************\n";
	std::cout << "SECOND PASS:\n";
	std::cout << "****************************************************************************************************************************\n\n";

	Task<bool, std::string> taskDepAnal;
	taskDepAnal.start(threadCount);
	try {
		if (files.size() > 0) {
			for (size_t i = 0; i < files.size(); i++) {
				DependencyAnalysis depanal;

				std::function<bool(std::string)> w = [&](std::string file) { return depanal.searchDependency
					(files[i]); };
				workItem<bool, std::string> *wi1 = new workItem<bool, std::string>(&w, &files[i]);
				taskDepAnal.doWork(wi1);
				taskDepAnal.result();
				ParallelDependencyAnalysis::addInstances(depanal);
			}
		}
	}
	catch (std::exception ex) {
		std::cout << ex.what();
		return false;
	}
	
	taskDepAnal.doWork(nullptr);
	taskDepAnal.wait();
	return true;
}


#ifdef TEST_PARALLELDEPENDENCYANALYSIS
int main()
{
	// Prototype for displaying of the working of ParallelDependencyAnalysis.
	std::vector<std::string> files;
	files.push_back("A.h");
	files.push_back("A.cpp");
	files.push_back("B.h");
	files.push_back("B.cpp");
	ParallelDependencyAnalysis pda;
	pda.firstPass(files, 2);
	pda.secondPass(files, 4);
	ParallelDependencyAnalysis::displayFinalDependency();
	return 0;
}
#endif // 




