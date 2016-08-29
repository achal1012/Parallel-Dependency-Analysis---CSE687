
///////////////////////////////////////////////////////////////
// Tokenizer.cpp - read words from a std::stream             //
// Ver 1.0                                                   //
// Application: Project #2 Code Parser with AST              //
// Language:    C++, Visual Studio 2015                      //
// Platform:    Dell Inspiron 5548, Core-i5, Windows 10      //
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
  struct Context
  {
	  Context();
	  ~Context();
	  std::string token;
	  std::istream* _pIn;
	  //std::map<std::string, int> specialChar = { { "[",1 },{ "]",2 },{ "<",3 },{ ">",4 },{ "(",5 },{ ")",6 },{ ":",7 },{ "=",8 },
	//											{ "+",9 },{ "-",10 },{ "*",11 },{ "\\n",12 },{ "<<",13 },{ ">>",14 },{ "::",15 },
		//										{ "++",16 },{ "--",17 },{ "==",18 },{ "+=",19 },{ "-=",20 },{ "*=",21 },{ "/=",22 },
		//										{ "{",23 },{ "}",24 } };
	  int prevChar;
	  int currChar;
	  bool showComment; // This bool is used to turn comments on and off.
	  size_t linecount;
	  ConsumeState* _pState;
	  ConsumeState* _pEatCppComment;
	  ConsumeState* _pEatCComment;
	  ConsumeState* _pEatWhitespace;
	  ConsumeState* _pEatPunctuator;
	  ConsumeState* _pEatAlphanum;
	  ConsumeState* _pEatNewline;
	  ConsumeState* _pEatSpecialChar;
	  ConsumeState* _pEatString;
	  ConsumeState* _pEatCh;
  };

  class ConsumeState
  {
  public:
    ConsumeState();
    virtual ~ConsumeState();
    void attach(std::istream* pIn) { _pContext->_pIn = pIn; }
    virtual void eatChars() = 0;
    void consumeChars() {
	  _pContext->_pState->eatChars();
	  _pContext->_pState = nextState();
    }
    bool canRead() { return _pContext->_pIn->good(); }
    std::string getTok() { return _pContext->token; }
    ConsumeState* nextState();
    bool hasTok() { return _pContext->token.size() > 0; }
	void setSpecialSingleChars(std::string ssc);
	void setSpecialCharPairs(std::string scp);
	ConsumeState* stateQuote(int currChar, int chNext);
	ConsumeState* stateComment(int currChar, int chNext);
	ConsumeState* statePunct(int currChar, int chNext, std::string check);
	void displayComment(bool commentState);
	//void reset();
	void setContext(Context* pContext);
	size_t lineNumber();
  protected:
	Context* _pContext; 
  };
}

using namespace Scanner;

//std::string ConsumeState::token;
//std::istream* ConsumeState::_pIn;
//size_t ConsumeState::linecount = 1;
//int ConsumeState::prevChar;
//int ConsumeState::currChar;
//bool ConsumeState::first = true;
//bool ConsumeState::showComment = false;
//ConsumeState* ConsumeState::_pState = nullptr;
//ConsumeState* ConsumeState::_pEatCppComment = nullptr;
//ConsumeState* ConsumeState::_pEatCComment = nullptr;
//ConsumeState* ConsumeState::_pEatWhitespace = nullptr;
//ConsumeState* ConsumeState::_pEatPunctuator = nullptr;
//ConsumeState* ConsumeState::_pEatAlphanum = nullptr;
//ConsumeState* ConsumeState::_pEatNewline;
//ConsumeState* ConsumeState::_pEatSpecialChar = nullptr;
////This hash map will save all the single and double special characters as keys.
//std::map<std::string, int> ConsumeState::specialChar = 
//ConsumeState* ConsumeState::_pEatString = nullptr;
//ConsumeState* ConsumeState::_pEatCh = nullptr;


void testLog(const std::string& msg);

////This function will make bool first true;
//void ConsumeState::reset()
//{
//	first = true;
//}

//----< used by Toker to provide Consumer with Context ptr >---------

void ConsumeState::setContext(Context* pContext)
{
	_pContext = pContext;
}


//This function is used to toggle comments on and off
void Toker::toggleComments(bool value)
{
	if (value == true)
		pConsumer->displayComment(true);
	if (value == false)
		pConsumer->displayComment(false);
}

size_t Toker::lineCount()
{
	return pConsumer->lineNumber();
}
size_t ConsumeState::lineNumber()
{
	return _pContext->linecount;
}
//This function will set the bool showCommment.
void ConsumeState::displayComment(bool commentStae)
{
	_pContext->showComment = commentStae;
}

// This function will take string as input and parse all the single characters and 
// add them to the predefined special character set.
void ConsumeState::setSpecialSingleChars(std::string ssc)
{
	std::string::size_type i = 0;
	std::string::size_type j = ssc.find(',');
	if (j == std::string::npos)
	{
		_pContext->specialChar[ssc]= _pContext->specialChar.size() + 1;
	}
	while (j != std::string::npos) {
		_pContext->specialChar[ssc.substr(i, j - i)] = _pContext->specialChar.size()+1;
		i = ++j;
		j = ssc.find(',', j);
		if (j == std::string::npos)
			_pContext->specialChar[ssc.substr(i, ssc.length())] = _pContext->specialChar.size()+1;
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
		_pContext->specialChar[scp] = _pContext->specialChar.size() + 1;
	}
	
	while (j != std::string::npos) {
		_pContext->specialChar[scp.substr(i, j - i)] = _pContext->specialChar.size()+1;
		i = ++j;
		j = scp.find(',', j);
		if (j == std::string::npos)
			_pContext->specialChar[scp.substr(i, scp.length())] = _pContext->specialChar.size()+1;
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
		return _pContext->_pEatCh;
	}
	if (currChar == '"') {
		testLog("state: EatString");
		return _pContext->_pEatString;
	}
	return nullptr;
}

// This is a sub nextState() function which handles all C and C++ style comments
ConsumeState* ConsumeState::stateComment(int currChar, int chNext)
{
	if (currChar == '/' && chNext == '/') {
		testLog("state: eatCppComment");
		return _pContext->_pEatCppComment;
	}
	if (currChar == '/' && chNext == '*') {
		testLog("state: eatCComment");
		return _pContext->_pEatCComment;
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
	if (_pContext->specialChar.find(check) != _pContext->specialChar.end()|| _pContext->specialChar.find(sptwo)!= _pContext->specialChar.end()) {
		testLog("state: EatSpecialChar");
		return _pContext->_pEatSpecialChar;
	}
	if (ispunct(currChar)&& _pContext->specialChar.find(check) == _pContext->specialChar.end() && _pContext->specialChar.find(sptwo) == _pContext->specialChar.end() /*&& currChar != '\''&& currChar != '\"'*/) {
		testLog("state: eatPunctuator");
		return _pContext->_pEatPunctuator;
	}

	return nullptr;
}

// This  function decides which type of token is going to come next and will call appropriate state functions.
ConsumeState* ConsumeState::nextState(){
	if (!(_pContext->_pIn->good()))
		return nullptr;
	int chNext = _pContext->_pIn->peek();
	if (chNext == EOF)
		_pContext->_pIn->clear();
	// if peek() reads end of file character, EOF, then eofbit is set and _pIn->good() will return false.
	// clear() restores state to good
	std::string check;
	check = _pContext->currChar;
		ConsumeState* detectComment = stateComment(_pContext->currChar, chNext);
	if (detectComment != nullptr)
		return detectComment;
	ConsumeState* detectQuote = stateQuote(_pContext->currChar, chNext);
	if (detectQuote != nullptr)
		return detectQuote;
	
	if (std::isspace(_pContext->currChar) && _pContext->currChar != '\n') {
		testLog("state: eatWhitespace");
		return _pContext->_pEatWhitespace;
	}
	if (_pContext->currChar == '\n') {
		testLog("state: eatNewLine");
		return _pContext->_pEatNewline;
	}
	////////////////////////////////////////////////////////////////////////////////////////
	if (std::isalnum(_pContext->currChar) || (_pContext->currChar == '_' && (std::isalnum(chNext) || chNext == '_'))) {
		testLog("state: eatAlphanum");
		return _pContext->_pEatAlphanum;
	}
	ConsumeState* detectPunct = statePunct(_pContext->currChar, chNext, check);
	if (detectPunct != nullptr)
		return detectPunct;
	if (!_pContext->_pIn->good())
		return _pContext->_pEatWhitespace;
	throw(std::logic_error("invalid type"));
}

// This class has  eatChars() function which handles and collects all the single characters.
class EatCh : public ConsumeState
{
public:
	EatCh(Context* pContext)
	{
		_pContext = pContext;
	}
	virtual void eatChars()
	{
		_pContext->token.clear();
		if (!_pContext->_pIn->good())
			return;
		_pContext->currChar = _pContext->_pIn->get();
		if (_pContext->currChar == '\'')
		{
			_pContext->currChar= _pContext->_pIn->get();
			return;
		}
		if (_pContext->currChar == '\\')
		{
			if(_pContext->_pIn->peek()!='\''&&_pContext->_pIn->peek() != '\"')
				_pContext->token += _pContext->currChar;
			_pContext->currChar = _pContext->_pIn->get();
			_pContext->token += _pContext->currChar;
			_pContext->currChar = _pContext->_pIn->get();
			_pContext->currChar = _pContext->_pIn->get();
			return;
		}
		_pContext->token += _pContext->currChar;
		_pContext->currChar = _pContext->_pIn->get();
		_pContext->currChar = _pContext->_pIn->get();
	}
};

// This class has  eatChars() function which handles and collects all the strings in a token.
class EatString : public ConsumeState
{
public:
	EatString(Context* pContext)
	{
		_pContext = pContext;
	}
	virtual void eatChars()
	{
		_pContext->token.clear();
		if (!_pContext->_pIn->good())  // end of stream
			return;
		_pContext->token+= _pContext->currChar;
		_pContext->currChar = _pContext->_pIn->get();
		//char a = (char)_pContext->currChar;
		if (_pContext->currChar == '"')
		{
			_pContext->token += _pContext->currChar;
			_pContext->currChar = _pContext->_pIn->get();
			return;
		}
		do {
			if (_pContext->currChar == '\\' && _pContext->_pIn->peek() == '"')
			{
				_pContext->currChar = _pContext->_pIn->get();
				_pContext->token += _pContext->currChar;
				_pContext->currChar = _pContext->_pIn->get();
				continue;
			}
			if (_pContext->currChar == '\\' && _pContext->_pIn->peek() == '\\')
			{
				_pContext->token += _pContext->currChar;
				_pContext->currChar = _pContext->_pIn->get();
			}
			if (_pContext->currChar == '\\' && _pContext->_pIn->peek() == '\'')
			{
				_pContext->currChar = _pContext->_pIn->get();
				_pContext->token += _pContext->currChar;
				_pContext->currChar = _pContext->_pIn->get();
				continue;
			}
			_pContext->token += _pContext->currChar;
			if (!_pContext->_pIn->good())  // end of stream
				return;
			_pContext->currChar = _pContext->_pIn->get();
		} while (_pContext->currChar!='"');
		_pContext->token += _pContext->currChar;
		_pContext->currChar = _pContext->_pIn->get();
	}
};

// This class has  eatChars() function which handles and collects all the single and double special characters in a token.
class EatSpecialChar : public ConsumeState
{
public:
	EatSpecialChar(Context* pContext)
	{
		_pContext = pContext;
	}
	virtual void eatChars()
	{
		_pContext->token.clear();
		if (!_pContext->_pIn->good())  // end of stream
			return;
		std::string check;
		check = _pContext->currChar;
		int nxtchar = _pContext->_pIn->peek();
		std::string check1;
		check1 = nxtchar;
		std::string special2char;
		special2char = check + check1;
		if (_pContext->specialChar.find(special2char) != _pContext->specialChar.end())
		{
			_pContext->token += _pContext->currChar;
			_pContext->currChar = _pContext->_pIn->get();
			_pContext->token += _pContext->currChar;
		}
		else
		{
			if(_pContext->specialChar.find(check) != _pContext->specialChar.end())
				_pContext->token += _pContext->currChar;
		} 
		_pContext->currChar = _pContext->_pIn->get();
	}
};

// This class has  eatChars() function which handles and collects all the white space and discards it.
class EatWhitespace : public ConsumeState
{
public:
	EatWhitespace(Context* pContext)
	{
		_pContext = pContext;
	}
  virtual void eatChars()
  {
	_pContext->token.clear();
    do {
      if (!_pContext->_pIn->good())  // end of stream
        return;
	  _pContext->currChar = _pContext->_pIn->get();
    } while (std::isspace(_pContext->currChar) && _pContext->currChar != '\n');
  }
};

//// This is a wrapper to function reset() used to reset the "first" flag which is used to consruct and destruct Consumestate.
//void Toker::toggleFirstState()
//{
//	pConsumer->reset();
//	pConsumer = new EatWhitespace();
//}

// This class has  eatChars() function which handles and collects all the c++ style comments in a token.
class EatCppComment : public ConsumeState
{
public:
	EatCppComment(Context* pContext)
	{
		_pContext = pContext;
	}
  virtual void eatChars()
  {
	  _pContext->token.clear();
    do {
		_pContext->token += _pContext->currChar;
      if (!_pContext->_pIn->good())  // end of stream
        return;
	  _pContext->currChar = _pContext->_pIn->get();
    } while (_pContext->currChar != '\n');
	if (_pContext->showComment == false)
		_pContext->token.clear();
  }
};

// This class has  eatChars() function which handles and collects all the C type comments in a token.

class EatCComment : public ConsumeState
{
public:
	EatCComment(Context* pContext)
	{
		_pContext = pContext;
	}
  virtual void eatChars()
  {
	_pContext->token.clear();
    do {
		_pContext->token += _pContext->currChar;
		if (_pContext->currChar == '\n')
			_pContext->linecount++;
		if (!_pContext->_pIn->good())  // end of stream
			return;
		_pContext->currChar = _pContext->_pIn->get();
    } while (_pContext->currChar != '*' || _pContext->_pIn->peek() != '/');
	if (_pContext->currChar == '*' && _pContext->_pIn->peek() == '/')
	{
		_pContext->token += _pContext->currChar;
		_pContext->token+= _pContext->_pIn->peek();
		if (!_pContext->_pIn->good())  // end of stream
			return;
		_pContext->currChar = _pContext->_pIn->get();
	}
	if (_pContext->showComment == false)
		_pContext->token.clear();
	_pContext->_pIn->get();
	_pContext->currChar = _pContext->_pIn->get();
  }
};

// This class has  eatChars() function which handles and collects all the punctuators in a token.
class EatPunctuator : public ConsumeState
{
public:
	EatPunctuator(Context* pContext)
	{
		_pContext = pContext;
	}
  virtual void eatChars()
  {
	_pContext->token.clear();
	std::string check;
	std::string check1;
	int nxtchar;
	std::string special2char;
    do {
		_pContext->token += _pContext->currChar;
      if (!_pContext->_pIn->good())  // end of stream
        return;
	  _pContext->currChar = _pContext->_pIn->get();
	  nxtchar = _pContext->_pIn->peek();
	  check = _pContext->currChar;
	  check1 = nxtchar;
	  special2char = check + check1;
    } while (ispunct(_pContext->currChar) && _pContext->specialChar.find(special2char)== _pContext->specialChar.end() && _pContext->specialChar.find(check) == _pContext->specialChar.end()&& special2char!="//" && special2char!="/*" && _pContext->currChar!='\"' && _pContext->currChar != '\'' && _pContext->currChar!='/');
  }
};

// This class has  eatChars() function which handles and collects all the AlphaNums in a token.
class EatAlphanum : public ConsumeState
{
public:
	EatAlphanum(Context* pContext)
	{
		_pContext = pContext;
	}
  virtual void eatChars()
  {
	  _pContext->token.clear();
    do {
		_pContext->token += _pContext->currChar;
      if (!_pContext->_pIn->good())  // end of stream
        return;
	  _pContext->currChar = _pContext->_pIn->get();
    } while (isalnum(_pContext->currChar) || _pContext->currChar == '_'); // /////////////////////////////////////////////////////////////////////////////////////
  }
};

// This class has  eatChars() function which handles and collects all the newLines in a token.
class EatNewline : public ConsumeState
{
public:
	EatNewline(Context* pContext)
	{
		_pContext = pContext;
	}
  virtual void eatChars()
  {
	  _pContext->token.clear();
	  _pContext->token += _pContext->currChar;
	  _pContext->linecount++;
    if (!_pContext->_pIn->good())  // end of stream
      return;
	_pContext->currChar = _pContext->_pIn->get();
  }
};


Context::Context()
{
	linecount = 1;
	_pEatAlphanum = new EatAlphanum(this);
	_pEatCComment = new EatCComment(this);
	_pEatCppComment = new EatCppComment(this);
	_pEatPunctuator = new EatPunctuator(this);
	_pEatWhitespace = new EatWhitespace(this);
	_pEatNewline = new EatNewline(this);
	_pEatSpecialChar = new EatSpecialChar(this);
	_pEatString = new EatString(this);
	_pEatCh = new EatCh(this);
	_pState = _pEatWhitespace;
	showComment = false;
}

Context::~Context()
{
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
ConsumeState::ConsumeState() {}
//{
//  if (first)
//  {
//    first = false;
//	linecount = 1;
//    _pEatAlphanum = new EatAlphanum();
//    _pEatCComment = new EatCComment();
//    _pEatCppComment = new EatCppComment();
//    _pEatPunctuator = new EatPunctuator();
//    _pEatWhitespace = new EatWhitespace();
//    _pEatNewline = new EatNewline();
//	_pEatSpecialChar = new EatSpecialChar();
//	_pEatString = new EatString();
//	_pEatCh = new EatCh();
//    _pState = _pEatWhitespace;
//  }
//}

ConsumeState::~ConsumeState() {}
//{
//  if (first)
//  {
//    first = false;
//    delete _pEatAlphanum;
//    delete _pEatCComment;
//    delete _pEatCppComment;
//    delete _pEatPunctuator;
//    delete _pEatWhitespace;
//    delete _pEatNewline;
//	delete _pEatSpecialChar;
//	delete _pEatString;
//	delete _pEatCh;
//  }
//}

Toker::Toker()
{
	_pContext = new Context();
	pConsumer = _pContext->_pEatWhitespace;
	pConsumer->setContext(_pContext);
}

Toker::~Toker() 
{ 
	delete _pContext;
	_pContext = nullptr;
}

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
	//std::string fileSpec = "tokenizer_test.txt";
	std::string fileSpec = "new.txt";

	std::ifstream in(fileSpec);
	if (!in.good())
	{
		std::cout << "\n  can't open " << fileSpec << "\n\n";
		return 1;
	}
	Toker toker;
	toker.toggleComments("true");
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
