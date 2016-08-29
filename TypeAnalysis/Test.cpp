
///////////////////////////////////////////////////////////////
// Tokenizer.cpp - read words from a std::stream             //
// Ver 1.0                                                   //
// Application: Project #1                                   //
// Language:    C++, Visual Studio 2015                      //
// Platform:    Lenovo L420, Core-i3, Windows 10             //
// Author:      Achal Velani, SUID:292190387                 //
// Reference:   Jim Fawcett, CST 4-187, Syracuse University  //
//              (315) 443-3948, jfawcett@twcny.rr.com        //
///////////////////////////////////////////////////////////////

#include "Tokenizer.h"
#include<string>
#include <iostream>
#include <cctype>
#include <string>
#include<map>

namespace Scanner
{

  class ConsumeState
  {
  public:
    ConsumeState();
    virtual ~ConsumeState();
    void attach(std::istream* pIn) { _pIn = pIn; }
    virtual void eatChars() = 0;
    void consumeChars() {
      _pState->eatChars();
      _pState = nextState();
    }
    bool canRead() { return _pIn->good(); }
    std::string getTok() { return token; }
    bool hasTok() { return token.size() > 0; }
    ConsumeState* nextState();
	void setSpecialSingleChars(std::string ssc);
	void setSpecialCharPairs(std::string scp);
	ConsumeState* stateQuote(int currChar, int chNext);
	ConsumeState* stateComment(int currChar, int chNext);
	ConsumeState* statePunct(int currChar, int chNext, std::string check);
	void displayComment(bool commentState);
	void reset();
  protected:
    static std::string token;
    static std::istream* _pIn;
    static int prevChar;
    static int currChar;
    static ConsumeState* _pState;
    static ConsumeState* _pEatCppComment;
    static ConsumeState* _pEatCComment;
    static ConsumeState* _pEatWhitespace;
    static ConsumeState* _pEatPunctuator;
    static ConsumeState* _pEatAlphanum;
    static ConsumeState* _pEatNewline;
	static ConsumeState* _pEatSpecialChar;
	static ConsumeState* _pEatString;
	static ConsumeState* _pEatCh;
	static std::map<std::string, int> specialChar;
	static bool showComment; // This bool is used to turn comments on and off.
	static bool first;
  };
}

using namespace Scanner;

std::string ConsumeState::token;
std::istream* ConsumeState::_pIn;
int ConsumeState::prevChar;
int ConsumeState::currChar;
bool ConsumeState::first = true;
bool ConsumeState::showComment = false;
ConsumeState* ConsumeState::_pState = nullptr;
ConsumeState* ConsumeState::_pEatCppComment = nullptr;
ConsumeState* ConsumeState::_pEatCComment = nullptr;
ConsumeState* ConsumeState::_pEatWhitespace = nullptr;
ConsumeState* ConsumeState::_pEatPunctuator = nullptr;
ConsumeState* ConsumeState::_pEatAlphanum = nullptr;
ConsumeState* ConsumeState::_pEatNewline;
ConsumeState* ConsumeState::_pEatSpecialChar = nullptr;
//This hash map will save all the single and double special characters as keys.
ConsumeState* ConsumeState::_pEatString = nullptr;
ConsumeState* ConsumeState::_pEatCh = nullptr;


void testLog(const std::string& msg);
//This function will make bool first true;
void ConsumeState::reset()
{
	first = true;
}

//This function is used to toggle comments on and off
void Toker::toggleComments(const std::string value)
{
	if (value == "true")
		pConsumer->displayComment(true);
	if (value == "false")
		pConsumer->displayComment(false);
}

//This function will set the bool showCommment.
void ConsumeState::displayComment(bool commentStae)
{
	showComment = commentStae;
}

// This function will take string as input and parse all the single characters and 
// add them to the predefined special character set.
void ConsumeState::setSpecialSingleChars(std::string ssc)
{
	std::string::size_type i = 0;
	std::string::size_type j = ssc.find(',');
	if (j == std::string::npos)
	{
		specialChar[ssc] = specialChar.size() + 1;
	}
	while (j != std::string::npos) {
		{
			int a;

			specialChar[ssc.substr(i, j - i)] = specialChar.size() + 1;
			i = ++j;
			j = ssc.find(',', j);
			if (j == std::string::npos)
				specialChar[ssc.substr(i, ssc.length())] = specialChar.size() + 1;

		}
	}
}

//Just a toker function which works as a wrapper for setSpecialSingleChars
void Toker::setSpecialSingleChars(std::string ssc)
{
	pConsumer->setSpecialSingleChars(ssc);
}

// This function will take string as input and parse all the double  characters and 
// add them to the predefined special character set.
void ConsumeState::setSpecialCharPairs(std::string scp)
{
	std::string::size_type i = 0;
	std::string::size_type j = scp.find(',');
	if (j == std::string::npos)
	{
		specialChar[scp] = specialChar.size() + 1;
	}
	
	while (j != std::string::npos) {
		specialChar[scp.substr(i, j - i)] = specialChar.size()+1;
		i = ++j;
		j = scp.find(',', j);
		if (j == std::string::npos)
			specialChar[scp.substr(i, scp.length())] = specialChar.size()+1;
	}
}

//Just a toker function which works as a wrapper for setSpecialCharPairs
void Toker::setSpecialCharPairs(std::string scp)
{
	pConsumer->setSpecialCharPairs(scp);
}

// This is a sub nextState() function which handles all the Quoted strings and also single quotes.
ConsumeState* ConsumeState::stateQuote(int currChar, int chNext)
{
	if (currChar == '\'') {
		testLog("state: EatChar");
		return _pEatCh;
	}
	if (currChar == '"') {
		testLog("state: EatString");
		return _pEatString;
	}
	return nullptr;
}

// This is a sub nextState() function which handles all C and C++ style comments
ConsumeState* ConsumeState::stateComment(int currChar, int chNext)
{
	if (currChar == '/' && chNext == '/') {
		testLog("state: eatCppComment");
		return _pEatCppComment;
	}
	if (currChar == '/' && chNext == '*') {
		testLog("state: eatCComment");
		return _pEatCComment;
	}
	return nullptr;
}

//This is a sub nextState() function which handles all cases for punctuators
ConsumeState* ConsumeState::statePunct(int currChar, int chNext,std::string check)
{
	std::string sp1, sp2, sptwo;
	sp1 = currChar;
	sp2 = chNext;
	sptwo = sp1 + sp2;
	if (specialChar.find(check) != specialChar.end()||specialChar.find(sptwo)!=specialChar.end()) {
		testLog("state: EatSpecialChar");
		return _pEatSpecialChar;
	}
	if (ispunct(currChar)&& specialChar.find(check) == specialChar.end() && specialChar.find(sptwo) == specialChar.end() /*&& currChar != '\''&& currChar != '\"'*/) {
		testLog("state: eatPunctuator");
		return _pEatPunctuator;
	}

	return nullptr;
}

// This  function decides which type of token is going to come next and will call appropriate state functions.
ConsumeState* ConsumeState::nextState(){
	if (!(_pIn->good()))
		return nullptr;
	int chNext = _pIn->peek();
	if (chNext == EOF)
		_pIn->clear();
	// if peek() reads end of file character, EOF, then eofbit is set and _pIn->good() will return false.
	// clear() restores state to good
	std::string check;
	check = currChar;
		ConsumeState* detectComment = stateComment(currChar, chNext);
	if (detectComment != nullptr)
		return detectComment;
	ConsumeState* detectQuote = stateQuote(currChar, chNext);
	if (detectQuote != nullptr)
		return detectQuote;
	
	if (std::isspace(currChar) && currChar != '\n') {
		testLog("state: eatWhitespace");
		return _pEatWhitespace;
	}
	if (currChar == '\n') {
		testLog("state: eatNewLine");
		return _pEatNewline;
	}
	if (std::isalnum(currChar)) {
		testLog("state: eatAlphanum");
		return _pEatAlphanum;
	}
	ConsumeState* detectPunct = statePunct(currChar, chNext, check);
	if (detectPunct != nullptr)
		return detectPunct;
	if (!_pIn->good())
		return _pEatWhitespace;
	throw(std::logic_error("invalid type"));
}

// This class has  eatChars() function which handles and collects all the single characters.
class EatCh : public ConsumeState
{
public:
	virtual void eatChars()
	{
		token.clear();
		if (!_pIn->good())
			return;
		currChar = _pIn->get();
		if (currChar == '\'')
		{
			currChar=_pIn->get();
			return;
		}
		if (currChar == '\\')
		{
			if(_pIn->peek()!='\''&&_pIn->peek() != '\"')
				token += currChar;
			currChar = _pIn->get();
			token += currChar;
			currChar = _pIn->get();
			currChar = _pIn->get();
			return;
		}
		token += currChar;
		currChar = _pIn->get();
		currChar = _pIn->get();
	}
};

// This class has  eatChars() function which handles and collects all the strings in a token.
class EatString : public ConsumeState
{
public:
	virtual void eatChars()
	{
		token.clear();
		if (!_pIn->good())  // end of stream
			return;

		currChar = _pIn->get();
		if (currChar == '"')
		{
			currChar = _pIn->get();
			return;
		}
		do {
			if (currChar == '\\' && _pIn->peek() == '"')
			{
				currChar = _pIn->get();
				token += currChar;
				currChar = _pIn->get();
				continue;
			}
			if (currChar == '\\' && _pIn->peek() == '\\')
			{
				token += currChar;
				currChar = _pIn->get();
			}
			if (currChar == '\\' && _pIn->peek() == '\'')
			{
				currChar = _pIn->get();
				token += currChar;
				currChar = _pIn->get();
				continue;
			}
			token += currChar;
			if (!_pIn->good())  // end of stream
				return;
			currChar = _pIn->get();
		} while (currChar!='"');
		currChar = _pIn->get();
	}
};

// This class has  eatChars() function which handles and collects all the single and double special characters in a token.
class EatSpecialChar : public ConsumeState
{
public:
	virtual void eatChars()
	{
		token.clear();
		if (!_pIn->good())  // end of stream
			return;
		std::string check;
		check = currChar;
		int nxtchar = _pIn->peek();
		std::string check1;
		check1 = nxtchar;
		std::string special2char;
		special2char = check + check1;
		if (specialChar.find(special2char) != specialChar.end())
		{
			token += currChar;
			currChar = _pIn->get();
			token += currChar;
		}
		else
		{
			if(specialChar.find(check) != specialChar.end())
				token += currChar;
		} 
		currChar = _pIn->get();
	}
};

// This class has  eatChars() function which handles and collects all the white space and discards it.
class EatWhitespace : public ConsumeState
{
public:
  virtual void eatChars()
  {
    token.clear();
    do {
      if (!_pIn->good())  // end of stream
        return;
      currChar = _pIn->get();
    } while (std::isspace(currChar) && currChar != '\n');
  }
};

// This is a wrapper to function reset() used to reset the "first" flag which is used to consruct and destruct Consumestate.
void Toker::toggleFirstState()
{
	pConsumer->reset();
	pConsumer = new EatWhitespace();
}

// This class has  eatChars() function which handles and collects all the c++ style comments in a token.
class EatCppComment : public ConsumeState
{
public:
  virtual void eatChars()
  {
    token.clear();
    do {
		token += currChar;
      if (!_pIn->good())  // end of stream
        return;
      currChar = _pIn->get();
    } while (currChar != '\n');
	if (showComment == false)
		token.clear();
  }
};

// This class has  eatChars() function which handles and collects all the C type comments in a token.

class EatCComment : public ConsumeState
{
public:
  virtual void eatChars()
  {
    token.clear();
    do {
		token += currChar;
      if (!_pIn->good())  // end of stream
        return;
      currChar = _pIn->get();
    } while (currChar != '*' || _pIn->peek() != '/');
	if (currChar == '*' && _pIn->peek() == '/')
	{
		token += currChar;
		token+=_pIn->peek();
		if (!_pIn->good())  // end of stream
			return;
		currChar = _pIn->get();
	}
	if (showComment == false)
		token.clear();
    _pIn->get();
    currChar = _pIn->get();
  }
};

// This class has  eatChars() function which handles and collects all the punctuators in a token.
class EatPunctuator : public ConsumeState
{
public:
  virtual void eatChars()
  {
    token.clear();
	std::string check;
	std::string check1;
	int nxtchar;
	std::string special2char;
    do {
      token += currChar;
      if (!_pIn->good())  // end of stream
        return;
      currChar = _pIn->get();
	  nxtchar = _pIn->peek();
	  check = currChar;
	  check1 = nxtchar;
	  special2char = check + check1;
    } while (ispunct(currChar) && specialChar.find(special2char)== specialChar.end() &&  specialChar.find(check) == specialChar.end()&& special2char!="//" && special2char!="/*" && currChar!='\"' && currChar != '\'' && currChar!='/');
  }
};

// This class has  eatChars() function which handles and collects all the AlphaNums in a token.
class EatAlphanum : public ConsumeState
{
public:
  virtual void eatChars()
  {
    token.clear();
    do {
      token += currChar;
      if (!_pIn->good())  // end of stream
        return;
      currChar = _pIn->get();
    } while (isalnum(currChar));
  }
};

// This class has  eatChars() function which handles and collects all the newLines in a token.
class EatNewline : public ConsumeState
{
public:
  virtual void eatChars()
  {
    token.clear();
    token += currChar;
    if (!_pIn->good())  // end of stream
      return;
    currChar = _pIn->get();
  }
};

ConsumeState::ConsumeState()
{
  if (first)
  {
    first = false;
    _pEatAlphanum = new EatAlphanum();
    _pEatCComment = new EatCComment();
    _pEatCppComment = new EatCppComment();
    _pEatPunctuator = new EatPunctuator();
    _pEatWhitespace = new EatWhitespace();
    _pEatNewline = new EatNewline();
	_pEatSpecialChar = new EatSpecialChar();
	_pEatString = new EatString();
	_pEatCh = new EatCh();
    _pState = _pEatWhitespace;
  }
}

ConsumeState::~ConsumeState()
{
  if (first)
  {
    first = false;
    delete _pEatAlphanum;
    delete _pEatCComment;
    delete _pEatCppComment;
    delete _pEatPunctuator;
    delete _pEatWhitespace;
    delete _pEatNewline;
	delete _pEatSpecialChar;
	delete _pEatString;
	delete _pEatCh;
  }
}

Toker::Toker()
{
	pConsumer = new EatWhitespace();
}

Toker::~Toker() { delete pConsumer; }

bool Toker::attach(std::istream* pIn)
{
  if (pIn != nullptr && pIn->good())
  {
    pConsumer->attach(pIn);
    return true;
  }
  return false;
}

// This function produce one token for each call.
std::string Toker::getTok()
{
  while(true) 
  {
    if (!pConsumer->canRead())
      return "";
    pConsumer->consumeChars();
    if (pConsumer->hasTok())
      break;
  }
  {
	  for (int i = 0; i < 5; i++)
	  {
		  std::cout << "Example of anonymous scope.";
	  }
  }
  return pConsumer->getTok();
}

bool Toker::canRead() { return pConsumer->canRead(); }

//Upon turned on it will display state of the token.
void testLog(const std::string& msg)
{
#ifdef TEST_LOG
  std::cout << "\n  " << msg;
#endif
}

//----< test stub >--------------------------------------------------

#ifdef TEST_TOKENIZER

#include <fstream>

int main()
{
	std::string fileSpec = "req4.txt";

	std::ifstream in(fileSpec);
	if (!in.good())
	{
		std::cout << "\n  can't open " << fileSpec << "\n\n";
		return 1;
	}
	Toker toker;
	toker.attach(&in);
	while (in.good())
	{
		std::string tok = toker.getTok();
		if (tok == "\n")
			tok = "newline";
		std::cout << "\n -- " << tok;
	}
	std::cout << "\n\n";
}
#endif
