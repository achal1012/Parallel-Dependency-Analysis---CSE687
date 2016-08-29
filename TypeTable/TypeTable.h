#ifndef TYPETABLE_H
#define TYPETABLE_H
////////////////////////////////////////////////////////////////
// TypeTable.cpp - Provides structure for TypeTable and       //
//                 TableRecord                                //
// Ver 1.0                                                    //
// Application: Project #3 Parallel Dependency Analysis       //
// Language:    C++, Visual Studio 2015                       //
// Platform:    Dell Inspiron 5548, Core-i5, Windows 10       //
// Author:      Achal Velani, SUID:292190387                  //
// Reference:   Jim Fawcett, CST 4-187, Syracuse University   //
//              (315) 443-3948, jfawcett@twcny.rr.com         //
////////////////////////////////////////////////////////////////
/*
* Package Operations :
*== == == == == == == == == == == == == == == =
* This package provides structure for TableRecord and TypeTable.
* It also provides helper function to print the final table.
*
* Public Interface :
*== == == == == == == == == == == == == == ==
* void constructRecord(Name name, Type type, FileName fileName, Namespace nmspace) - This function helps in constructing a record in the type table.
* void showTypeTable() - prints typetable in proper fashion.
* std::unordered_map<std::string, RecordData>* returntableRecord() - returns the unordered_map type tableRecord.
* void insert(TypeTable *tp) - inserts different tableRecords of different objects to create final typeTable.
*
*
* Required Files :
*== == == == == == == == == == == == == ==
* TypeTable.h, TypeTable.cpp
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

#include <string>
#include <vector>
#include<iostream>
#include <unordered_map>
#include<iomanip>

// This class is fordefining a single record in the table.
class RecordData
{
	using Namespace = std::string;
	using Name = std::string;
	using Type = std::string;
	using FileName = std::string;
	
public:
	void setName(Name nm) {
		_name = nm;
	}
	std::string getname() { 
		return _name; 
	}
	void setType(Type tp) {
		_type = tp;
	}
	Type gettype() { 
		return _type; 
	}
	void setFileName(FileName fn) {
		_whereDefined = fn;
	}
	FileName getfileName() {
		return _whereDefined;
	}
	void addfileNameSpace(FileName fileName, Namespace namespaces){
		fileNameSpace.push_back(std::make_pair(fileName, namespaces));
	}
	std::vector<std::pair<FileName, Namespace>> getfileNameSpace(){
		return fileNameSpace;
	}
private:
	Name _name;
	Type _type;
	FileName _whereDefined;
	std::vector<std::pair<FileName, Namespace>> fileNameSpace;
};

// This is a class for structure of the TypeTable, with helper functions.
class TypeTable
{
	using Namespace = std::string;
	using Name = std::string;
	using Type = std::string;
	using FileName = std::string;
	using iterator = std::vector<Namespace>::iterator;
public:
	
	TypeTable() {
		tableRecord = new std::unordered_map<std::string, RecordData>();
	}
	void constructRecord(Name name, Type type, FileName fileName, Namespace nmspace);
	void showTypeTable();
	std::unordered_map<std::string, RecordData>* returntableRecord();
	void insert(TypeTable *tp);
private:
	std::unordered_map<std::string, RecordData> *tableRecord;

};

#endif