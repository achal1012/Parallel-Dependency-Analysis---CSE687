#ifndef SEMIEXPRESSION_H
#define SEMIEXPRESSION_H
///////////////////////////////////////////////////////////////
// SemiExpression.h - collect tokens for analysis            //
// Ver 1.0                                                   //
// Application: Project #3 Parallel Dependency Analysis      //
// Language:    C++, Visual Studio 2015                      //
// Platform:    Lenovo L420, Core-i3, Windows 10             //
// Author:      Achal Velani, SUID:292190387                 //
// Reference:   Jim Fawcett, CST 4-187, Syracuse University  //
//              (315) 443-3948, jfawcett@twcny.rr.com        //
///////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package provides a public SemiExp class that collects and makes
* available sequences of tokens.  SemiExp uses the services of a Toker
* class to acquire tokens.  Each call to SemiExp::get() returns a
* sequence of tokens that ends in {.  This will be extended to use the
* full set of terminators: {, }, ;, and '\n' if the line begins with #.
* This package also handles "for" loop semiexpressions, semiExpressions starting with # and also public:, private: and protected:.
* 
* Public Interface :
* ------------------
* bool get(bool clear = true) - This function will extract new semiExpressions from loaded file.
* bool handleFor(std::string token) -This function handles "for" semiExpression.
* bool handleColon(std::string token, std::string prevtoken) - This function handles colon terminated semiExpression.
* bool handleHash(std::string token)- This function handles semiExpression with # starting
* bool checkCase(std::string token, std::string prevtoken) - This is a sub-function of the get() which chcks for every specified case.
* std::string& operator[](int n) -This fuction is used to read _tokens using its position.
* size_t length() - This returns vector _token's length
* std::string show(bool showNewLines=false) -This returns a string which consists of the whole semiexpression.
* size_t find(const std::string& tok) -This function searches for strings in the vector _tokens.
* void push_back(const std::string& tok)- It is used to push new tokens to vector.
* bool merge(const std::string& firstTok, const std::string& secondTok) - merge is not implemented in current version!
* bool remove(const std::string& tok)- This function is used to remove perticular string from the vector.
* bool remove(size_t i) - This also removes specific string from vector but uses its position as parameter.
* void toLower() - Upon calling this function, it will convert all the strings in vector to lower case.
* void trimFront()- This function will remove all the blank and and newlines from start of the vector until a valid string is hit.
* void clear() - This will clear the whole _tokens vector.
* bool isComment(std::string token) - This function checks if the semi Exp is Comment
*
* Build Process:
* --------------
* Required Files:
*   SemiExpression.h, SemiExpression.cpp, Tokenizer.h, Tokenizer.cpp
*
* Build Command: devenv Project3.sln / rebuild debug
*
* Maintenance History:
* --------------------
* ver 1.0 : 09 Feb 2016
* -first release 
*
*/

#include <vector>
#include "itokcollection.h"
#include "../Tokenizer/Tokenizer.h"

namespace Scanner
{
	using Token = std::string;
	using namespace Scanner; // kem?
	class SemiExp  : public ITokCollection
	{
	public:
		SemiExp(Toker* pToker = nullptr);
		SemiExp(const SemiExp&) = delete;
		SemiExp& operator=(const SemiExp&) = delete;
		bool get(bool clear = true);
		bool handleFor(std::string token);
		bool handleColon(std::string token, std::string prevtoken);
		bool handleHash(std::string token);
		bool checkCase(std::string token, std::string prevtoken);
		std::string& operator[](int n);
		size_t length();
		std::string show(bool showNewLines=false);
		size_t find(const std::string& tok);
		void push_back(const std::string& tok);
	    bool merge(const std::string& firstTok, const std::string& secondTok);
		bool remove(const std::string& tok);
		bool remove(size_t i);
		void toLower();
		void trimFront();
		void clear();
		bool isComment(std::string token);
	private:
		std::vector<Token> _tokens;
		Toker* _pToker;
	};
}
#endif
