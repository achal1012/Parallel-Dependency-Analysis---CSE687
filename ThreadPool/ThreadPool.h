#ifndef THREADPOOL_H
#define THREADPOOL_H
/////////////////////////////////////////////////////////////////////////////////////
// ThreadPool.h - facilities to enqueue work items and execute them asynchronously //
// Ver 1.0																		   //
// Application: Project #3 Parallel Dependency Analysis                            //
// Language:    C++, Visual Studio 2015								               //
// Platform:    Dell Inspiron 5548, Core-i5, Windows 10				               //
// Author:      Achal Velani, SUID:292190387							           //
// Reference:   Jim Fawcett, CST 4-187, Syracuse University			  	           //
//              (315) 443-3948, jfawcett@twcny.rr.com				               //
/////////////////////////////////////////////////////////////////////////////////////
/*
* Package Operations :
*== == == == == == == == == == == == == == == =
* Provides facilities to enqueue work items and execute them asynchronously. 
* Each work item will be a callable object which is supplied by application code.
*
* Public Interface :
*== == == == == == == == == == == == == == ==
* void start(size_t threadCount) - starts number of child threads that dequeus work items.
* void doWork(workItem<Result, Parameter>* pWi) - enqueues work item in the threadsafe queue
* Result result() - retrieves results with blocking call 
* void wait() - wait for child threads to terminate
* BlockingQueue<Result> returnQueue() - this will return the blocking queue containing the results.
* ~ThreadPool() - destructor to clean up heap. 
*
* Required Files :
*== == == == == == == == == == == == == ==
* ThreadPool.h, ThreadPool.cpp, Cpp11-BlockingQueue.h, Cpp11-BlockingQueue.cpp,
* WorkItem.h, Utilities.h, Utilities.cpp
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

#include<thread>
#include <functional>
#include "../Cpp11-BlockingQueue/Cpp11-BlockingQueue.h"
#include"WorkItem.h"

///////////////////////////////////////////////////////////////////////
// class to process work items
// - each work item is processed sequentially on a single child thread

template<typename Result, typename Parameter>
class ThreadPool
{
public:
	void start(size_t threadCount);
	void doWork(workItem<Result, Parameter>* pWi);
	Result result();
	void wait();
	~ThreadPool();
	BlockingQueue<Result> returnQueue();
private:
	std::vector<std::thread*> threadsVec; // holds all the initiated threads.
	std::thread* _pThread; 
	BlockingQueue<workItem<Result, Parameter>*> _workItemQueue; // workitem blocking queue
	BlockingQueue<Result> _resultsQueue; // blocking queue containing results.
};

//----< wait for child thread to terminate >---------------------------
template<typename Result, typename Parameter>
void ThreadPool<Result, Parameter>::wait()
{
	std::cout << std::endl << "\n  Thread Count (Wait) : " << threadsVec.size() << std::endl;
	try {
		for (auto thread : threadsVec)
			thread->join();
	}
	catch (std::exception& ex)
	{
		std::cout << "\n\n    " << ex.what() << "\n\n";
	}
}
template<typename Result, typename Parameter>
BlockingQueue<Result> ThreadPool<Result, Parameter>::returnQueue()
{
	return _resultsQueue;
}
//----< enqueue work item >--------------------------------------------

template<typename Result, typename Parameter>
void ThreadPool<Result, Parameter>::doWork(workItem<Result, Parameter>* pWi)
{
	_workItemQueue.enQ(pWi);
}
//----< retrieve results with blocking call >--------------------------

template<typename Result, typename Parameter>
Result ThreadPool<Result, Parameter>::result()
{
	return _resultsQueue.deQ();
}
//----< start child thread that dequeus work items >-------------------

template<typename Result, typename Parameter>
void ThreadPool<Result, Parameter>::start(size_t threadCount)
{
	size_t count = 0;
	while (count < threadCount) {
		std::function<void()> threadProc =
			[&]() {
			while (true){
				workItem<Result, Parameter>* pWi = _workItemQueue.deQ();
				if (pWi == nullptr)// workitem is null so the thread will enter this loop and shut down.
				{
					std::cout << "\n  shutting down work item processing"<<" : "<<std::this_thread::get_id()<<"\n";
					_workItemQueue.enQ(nullptr); // This is turn off other initiated threads.
					return;
				}
				Parameter p;
				Result result = (*pWi)();
				_resultsQueue.enQ(result);
			}
		};
		_pThread = new std::thread(threadProc);
		threadsVec.push_back(_pThread);
		count++;
	}
}
//----< clean up heap >------------------------------------------------
template<typename Result, typename Parameter>
ThreadPool<Result, Parameter>::~ThreadPool(){
	delete _pThread;
}

#endif