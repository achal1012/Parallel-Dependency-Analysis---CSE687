///////////////////////////////////////////////////////////////////////////
// Task.cpp - executes a specified callable object on a ThreadPool thread//
// Ver 1.0                                                               //
// Application: Project #3 Parallel Dependency Analysis                  //
// Language:    C++, Visual Studio 2015								     //
// Platform:    Dell Inspiron 5548, Core-i5, Windows 10				     //
// Author:      Achal Velani, SUID:292190387							 //
// Reference:   Jim Fawcett, CST 4-187, Syracuse University				 //
//              (315) 443-3948, jfawcett@twcny.rr.com				     //
///////////////////////////////////////////////////////////////////////////

#include "../Task/Task.h"
using WorkResult = bool;
using Parameter = std::string;
#ifdef TEST_TASK
int main()
{
	Task<WorkResult, Parameter > processor;

	processor.start(2);

	// define 1st work item
	std::string s = "a.txt";
	std::function<bool(std::string)> w = [&](std::string file) {
		std::cout << "\n Processing file :  " << file << std::this_thread::get_id();
		return false;
	};
	workItem<WorkResult, Parameter> *wi1 = new workItem<bool, std::string>(&w, &s);
	processor.doWork(wi1);

	// define 2nd work item
	std::string s1 = "b.txt";
	std::function<bool(std::string)> w1 = [&](std::string file) {
		std::cout << "\n Processing file :  " << file << std::this_thread::get_id();
		return false;
	};
	workItem<WorkResult, Parameter> *wi2 = new workItem<bool, std::string>(&w1, &s1);
	processor.doWork(wi2);

	// the following calls to result() block until results are enqueued

	std::cout << "\n  " << processor.result();
	std::cout << "\n  " << processor.result();
	processor.doWork(nullptr);

	// wait for child thread to complete

	processor.wait();
	std::cout << "\n\n";
	return 0;
}


#endif // TEST_TASK
