#ifndef WORKITEM_H
#define WORKITEM_H
///////////////////////////////////////////////////////////////////////////
// WorkItem.h - Provides custom template class for the workItem          //
// Ver 1.0                                                               //
// Application: Project #3 Parallel Dependency Analysis                  //
// Language:    C++, Visual Studio 2015								     //
// Platform:    Dell Inspiron 5548, Core-i5, Windows 10				     //
// Author:      Achal Velani, SUID:292190387							 //
// Reference:   Jim Fawcett, CST 4-187, Syracuse University				 //
//              (315) 443-3948, jfawcett@twcny.rr.com				     //
///////////////////////////////////////////////////////////////////////////
/*
* Package Operations :
*== == == == == == == == == == == == == == == =
* This package rovides custom template class for the workItem to be used by threadpool.
*
* Public Interface :
*== == == == == == == == == == == == == == ==
* returnType operator() () - it is callable object whcich binds a parameter with a function.
*
* Required Files :
*== == == == == == == == == == == == == ==
* WorkItem.h, ThreadPool.h, ThreadPool.cpp
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


#include<vector>
#include<iostream>
#include<functional>

template <typename returnType, typename Param1>
class workItem {
public:
	workItem(std::function<returnType(Param1)> *myfun, Param1 *param1) {
		param = *param1;
		myfunction = *myfun;
	}
	returnType operator() () {
		std::cout <<std::endl<< "Thread ID : "<< std::this_thread::get_id() << " File Name : "<< param<< std::endl;
		return myfunction(param);
	}
	~workItem() {}
private:
	std::function<returnType(Param1)> myfunction;
	Param1 param;
};

#endif