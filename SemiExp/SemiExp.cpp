///////////////////////////////////////////////////////////////
// SemiExpression.cpp - collect tokens for analysis          //
// Ver 1.0                                                   //
// Application: Project #3 Parallel Dependency Analysis      //
// Language:    C++, Visual Studio 2015                      //
// Platform:    Dell Inspiron 5548, Core-i5, Windows 10      //
// Author:      Achal Velani, SUID:292190387                 //
// Reference:   Jim Fawcett, CST 4-187, Syracuse University  //
//              (315) 443-3948, jfawcett@twcny.rr.com        //
///////////////////////////////////////////////////////////////

#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <exception>
#include "SemiExp.h"
#include "../Tokenizer/Tokenizer.h"


using namespace Scanner;

SemiExp::SemiExp(Toker* pToker) : _pToker(pToker) {}

// This function checks if the semi Exp is Comment 
bool SemiExp::isComment(std::string token)
{
	if ((token[0] == '/') && (token[1] == '/' || token[1] == '*'))
		return true;
	return false;
}


// This function handles "for" semiExpression.
bool SemiExp::handleFor(std::string token){
	int semiCln = 0;
	do {
		_tokens.push_back(token);
		token = _pToker->getTok();
		if (token == ":"){ // This handles c++11 type comments.
			do {
				_tokens.push_back(token);
				token = _pToker->getTok();
				if (token == ";") {
					semiCln++;
					_tokens.push_back(token);
					return true;
				}
			} while (token != "{" && semiCln != 1);
		}
		if (token == ";"){
			semiCln++;
		}
		if (semiCln == 3)
			break;
	} while (semiCln != 3 && token != "{");
	_tokens.push_back(token);
	return true;
}

// This function handles colon terminated lines. (Req 7)
bool SemiExp::handleColon(std::string token, std::string prevtoken) 
{
	if (prevtoken == "public" || prevtoken == "protected" || prevtoken == "private") {
		_tokens.push_back(token);
		return true;
	}
	return false;
}

// This function handles lines startingwith # (Req 7)
bool SemiExp::handleHash(std::string token)
{
	do {
		_tokens.push_back(token);
		token = _pToker->getTok();
	} while (token != "\n" && token != "");
	return true;
}

//This is a sub-function of the get() which chcks for every specified case.
bool SemiExp::checkCase(std::string token, std::string prevtoken)
{
	while (true) {
		prevtoken = token;
		token = _pToker->getTok();
		//std::cout << token << std::endl;
		if (token == "")
			break;
		if (token == ":" && handleColon(token, prevtoken)) {
			return true;
		}
		if (token == "#" && handleHash(token)) {
			return true;
		}
		if (token == "for" && handleFor(token)) {
			return true;
		}
		_tokens.push_back(token);
		if (token == "{")
			return true;
		if (token == "}")
			return true;
		if (token == ";")
			return true;
	}
	return false;
}

//This function will extract new semiExpressions from loaded file.
bool SemiExp::get(bool clear){
	std::string token, prevtoken;
	token = "";
	prevtoken = "";
	if (_pToker == nullptr)
		throw(std::logic_error("no Toker reference"));
	if(clear)
		_tokens.clear();
	return checkCase(token, prevtoken);// calls above function to check which semiExpression is found.
}

// This fuction is used to read _tokens using its position.
std::string& SemiExp::operator[](int n) 
{
	if (n < 0 || (size_t)n >= _tokens.size())
		throw(std::invalid_argument("index out of range"));
	return _tokens[n];
}

// This returns vector _token's length
size_t SemiExp::length() 
{
	return _tokens.size();
}

//This returns a string which consists of the whole semiexpression.
std::string SemiExp::show(bool showNewLines) 
{
	if (_tokens.size() == 0)
		return "";
	std::string retStr = "";
	if (showNewLines==false){
		for (auto token : _tokens) // Reads all the tokens from _tokens and appends to a string
			if (token != "\n")
				retStr.append(token+" ");
	}
	else // This shows new lines.
	{
		for (auto token : _tokens) 
			retStr.append(token+" ");
	}
	return "\n"+retStr+"\n";
}

// This function searches for strings in the vector _tokens.
size_t SemiExp::find(const std::string& tok)
{
	for (size_t i = 0; i < length(); i++)
		if (tok == _tokens[i])
			return i;
	return length();
}

//It is used to push new tokens to vector.
void SemiExp::push_back(const std::string& tok) 
{
	_tokens.push_back(tok);
}

// merge is not implemented in current version!
bool SemiExp::merge(const std::string& firstTok, const std::string& secondTok) 
{
	return true;
}

// This function is used to remove perticular string from the vector.
bool SemiExp :: remove(const std::string& tok) 
{
	std::vector<std::string>::iterator iterate;
	iterate = std::find(_tokens.begin(), _tokens.end(), tok);
	if (iterate!=_tokens.end())
	{
		_tokens.erase(iterate);
		return true;
	}
	return false;
}

// This also removes specific string from vector but uses its position as parameter.
bool SemiExp:: remove(size_t i)
{
	if (i >= length()||i<0)
		return false;
	else
	{
		_tokens.erase(_tokens.begin() + i);
		return true;
	}
}

// Upon calling this function, it will convert all the strings in vector to lower case.
void SemiExp::toLower()
{
	for (size_t i = 0; i < length(); i++)
	{
		std::string s;
		s = _tokens[i];
		int a = 0;
		while (s[a])
		{
			s[a] = tolower(s[a]);
			a++;
		}
		_tokens[i] = s;
	}
}

// This function will remove all the blank and and newlines from start of the vector until a valid string is hit.
void SemiExp::trimFront() 
{
	while (_tokens.size() > 0)
	{
		if (_tokens[0] == "\n" || _tokens[0] == "")
			remove(0);
		else
			break;
	}
}

// This will clear the whole _tokens vector.
void SemiExp::clear() 
{
	_tokens.clear();
}

//----< test stub >--------------------------------------------------
#ifdef TEST_SEMIEXP
int main()
{
	Toker toker;
	std::string fileSpec = "semi_test.txt";
	std::fstream in(fileSpec);
	if (!in.good())
	{
		std::cout << "\n  can't open file " << fileSpec << "\n\n";
		return 1;
	}
	toker.attach(&in);

	SemiExp semi(&toker);
	while (semi.get())
	{
		std::cout << "\n  -- semiExpression --";
		std::string sh;
		sh=semi.show();
		std::cout << "\n" << sh << "\n";
	}
	/*
	May have collected tokens, but reached end of stream
	before finding SemiExp terminator.
	*/
	if (semi.length() > 0)
	{
		std::cout << "\n  -- semiExpression --";
		std::string sh;
		sh = semi.show();
		std::cout << "\n" << sh << "\n";
		std::cout << "\n\n";
	}
	return 0;
}
#endif