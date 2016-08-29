#ifndef WORKITEM_H
#define WORKITEM_H

#include<vector>
#include<iostream>
#include<functional>

template <typename returnType, typename Param1>
class workItem {
public:
	workItem(std::function<returnType(Param1)> *myfun, Param1 *param1) {
		_param1 = *param1;
		_myfunction = *myfun;
	}
	returnType operator() () {
		std::cout <<std::endl<< "Thread ID : "<< std::this_thread::get_id() << " File Name : "<< _param1<< std::endl;
		return _myfunction(_param1);
	}
	~workItem() {}
private:
	std::function<returnType(Param1)> _myfunction;
	Param1 _param1;
};

#endif