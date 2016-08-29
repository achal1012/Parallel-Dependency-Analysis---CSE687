#ifndef TOKENIZER_H
#define TOKENIZER_H
///////////////////////////////////////////////////////////////
// Tokenizer.h - read words from a std::stream               //
// Ver 1.0                                                   //
// Application: Project #3 Parallel Dependency Analysis      //
// Language:    C++, Visual Studio 2015                      //
// Platform:    Dell Inspiron 5548, Core-i5, Windows 10      //
// Author:      Achal Velani, SUID:292190387                 //
// Reference:   Jim Fawcett, CST 4-187, Syracuse University  //
//              (315) 443-3948, jfawcett@twcny.rr.com        //
///////////////////////////////////////////////////////////////
/*
 * Package Operations:
 * -------------------
 * This package provides a public Toker class and private ConsumeState class.
 * Toker reads words from a std::stream, throws away whitespace and comments
 * and returns words from the stream in the order encountered.  Quoted
 * strings and certain punctuators and newlines are returned as single tokens.
 *
 * This version is based on the State Design Pattern.
 *
 * This package will parse all the tokens seperately and handles all the special single and double tokens as per requirements.
 * Special one character tokens: <, >, [, ], (, ), {, }, :, =, +, -, *, \n
 * Special two character tokens: <<, >>, ::, ++, --, ==, +=, -=, *=, /=
 *
 * Public Interface :
 * ------------------
 * bool attach(std::istream* pIn) - This function attaches input stream to pConsumer.
 * std::string getTok() - This function produce one token for each call.
 * bool canRead() - checks if pIn is good or not.
 * void setSpecialSingleChars(std::string ssc) -This is a toker function which works as a wrapper for ConsumeState::setSpecialSingleChars
 * void setSpecialCharPairs(std::string scp) - This is a toker function which works as a wrapper for ConsumeState::setSpecialCharPairs
 * void toggleComments(bool value) - This function is used to toggle comments on and off
 * size_t lineCount() - This function returns linecount.
 * 
 * Build Process:
 * --------------
 * Required Files: Tokenizer.h, Tokenizer.cpp
 *
 * Build Command: 
 * devenv Project3.sln /rebuild debug
 *
 * Maintenance History:
 * --------------------
 * ver 1.0 : 09 Feb 2016
 * - first release
 */
#include <iosfwd>
#include <string>

namespace Scanner
{
  class ConsumeState;
  struct Context;
  class Toker
  {
  public:
	Toker();
	Toker(const Toker&) = delete;
	~Toker();
	Toker& operator=(const Toker&) = delete;
    bool attach(std::istream* pIn);
    std::string getTok();
    bool canRead();
	void setSpecialSingleChars(std::string ssc);
	void setSpecialCharPairs(std::string scp);
	void toggleComments(bool value);
	size_t lineCount();
  private:
    ConsumeState* pConsumer;
	Context* _pContext;
  };
}
#endif

