#ifndef TASK_H
#define TASK_H
//////////////////////////////////////////////////////////////////////////
// Task.h - executes a specified callable object on a ThreadPool thread //
// Ver 1.0                                                              //
// Application: Project #3 Parallel Dependency Analysis                 //
// Language:    C++, Visual Studio 2015								    //
// Platform:    Dell Inspiron 5548, Core-i5, Windows 10				    //
// Author:      Achal Velani, SUID:292190387							//
// Reference:   Jim Fawcett, CST 4-187, Syracuse University				//
//              (315) 443-3948, jfawcett@twcny.rr.com				    //
//////////////////////////////////////////////////////////////////////////
/*
* Package Operations :
*== == == == == == == == == == == == == == == =
* This provides task class that executes a specified callable object on a ThreadPool thread, 
* using services of the ThreadPool package.
*
* Public Interface :
*== == == == == == == == == == == == == == ==
* void start(size_t threadCount) - start child thread that dequeus work items using Threadpool
* void doWork(workItem<Result, Parameter>* pWi) - enqueues work item in the threadsafe queue
* Result result() - retrieves results with blocking call to threadpool
* void wait() - wait for child threads to terminate
*
* Required Files :
*== == == == == == == == == == == == == ==
* Task.h, Task.cpp, Threadpool.h, ThreadPool.cpp
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


#include"../ThreadPool/ThreadPool.h"

template<typename Result, typename Parameter>
class Task
{
public:
	void start(size_t threadCount);
	void doWork(workItem<Result, Parameter>* pWi);
	Result result();
	void wait();
	~Task(){}
private:
	ThreadPool<bool, std::string> tp;
};

//----< wait for child thread to terminate >---------------------------
template<typename Result, typename Parameter>
void Task<Result, Parameter>::wait()
{
	tp.wait();
}
//----< enqueue work item >--------------------------------------------
template<typename Result, typename Parameter>
void Task<Result, Parameter>::doWork(workItem<Result, Parameter>* pWi)
{
	tp.doWork(pWi);
}
//----< retrieve results with blocking call >--------------------------

template<typename Result, typename Parameter>
Result Task<Result, Parameter>::result()
{
	return tp.result();
}
//----< start child thread that dequeus work items >-------------------

template<typename Result, typename Parameter>
void Task<Result, Parameter>::start(size_t threadCount)
{
	tp.start(threadCount);
}

#endif // !TASK_H
