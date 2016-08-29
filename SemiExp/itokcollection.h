#ifndef ITOKCOLLECTION_H
#define ITOKCOLLECTION_H
/////////////////////////////////////////////////////////////////
// ITokCollection.h - package for the ITokCollection interface //
// Ver 1.0                                                     //
// Application: Project #3 Parallel Dependency Analysis        //
// Language:    Visual C++, Visual Studio 2015                 //
// Platform:    Dell Inspiron 5548, Core-i5, Windows 10        //
// Author:      Achal Velani, SUID:292190387                   //
// Reference:   Jim Fawcett, CST 4-187, Syracuse University    //
//              (315) 443-3948, jfawcett@twcny.rr.com          //
/////////////////////////////////////////////////////////////////
/*
  Module Purpose:
  ===============
  ITokCollection is an interface that supports substitution of different
  types of scanners for parsing.  In this solution, we illustrate that
  by binding two different types of collections, SemiExp and XmlParts,
  to this interface.  This is illustrated in the test stubs for the
  SemiExpression and XmlElementParts modules.

  Maintenance History:
  ====================
  ver 1.0 : 09 Feb 16
  - first release
*/

#include<string>

	struct ITokCollection
	{
		virtual bool get(bool clear = true) = 0;
		virtual size_t length() = 0;
		virtual std::string& operator[](int n) = 0;
		virtual size_t find(const std::string& tok) = 0;
		virtual void push_back(const std::string& tok) = 0;
		virtual bool merge(const std::string& firstTok, const std::string& secondTok) = 0;// no implementation
		virtual bool remove(const std::string& tok) = 0;
		virtual bool remove(size_t i) = 0;
		virtual void toLower() = 0; //convert all tokens to lower case
		virtual void trimFront() = 0;
		virtual void clear() = 0;
		virtual std::string show(bool showNewLines = false) = 0;
		virtual ~ITokCollection() {};
	};

#endif
