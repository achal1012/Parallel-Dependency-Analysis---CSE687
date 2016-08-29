#ifndef DATASTORE_H
#define DATASTORE_H
///////////////////////////////////////////////////////////////
// DataStore.h - Store results of file search                //
// Ver 1.0                                                   //
// Application: Project #3 Parallel Dependency Analysis      //
// Language:    C++, Visual Studio 2015                      //
// Platform:    Dell Inspiron 5548, Core-i5, Windows 10      //
// Author:      Achal Velani, SUID:292190387                 //
// Reference:   Jim Fawcett, CST 4-187, Syracuse University  //
//              (315) 443-3948, jfawcett@twcny.rr.com        //
///////////////////////////////////////////////////////////////
/*
* Package Operations :
*== == == == == == == == == == == == == == == =
* This package will provides functionalites to build the Abstract Syntax tree..
*
* Public Interface :
*== == == == == == == == == == == == == == ==
* void save(const std::string& filespec) - stores the file to store vector
* void save(const std::string& filename, const std::string& path) - stores the file to store vector (overload)
* iterator begin() - returns begining element of the store.
* iterator end() - returns end of the element of the store.
*
* Required Files :
*== == == == == == == == == == == == == ==
* DataStore.h, DataStore.cpp
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

class DataStore
{
public:
  using iterator = std::vector<std::string>::iterator;
  void save(const std::string& filespec) { store.push_back(filespec);  }
  void save(const std::string& filename, const std::string& path) {}
  iterator begin() { return store.begin(); }
  iterator end() { return store.end(); }
private:
  std::vector<std::string> store;
};
#endif
