/////////////////////////////////////////////////////////////////////////////
// ConfigureParser.cpp - builds and configures parsers                     //
// ver 2.1                                                                 //
// Language:    Visual C++                                                 //
// Platform:    Dell Inspiron 5548, Core-i5, Windows 10                    //
// Application: Project #3 Parallel Dependency Analysis                    //
// Author:      Achal Velani, SUID:292190387                               //
// Reference:   Jim Fawcett, CST 4-187, Syracuse University                //
//              (315) 443-3948, jfawcett@twcny.rr.com                      //
/////////////////////////////////////////////////////////////////////////////

#include <fstream>
#include "Parser.h"
#include "../SemiExp/SemiExp.h"
#include "../Tokenizer/Tokenizer.h"
#include "ActionsAndRules.h"
#include "ConfigureParser.h"

using namespace Scanner;
//----< destructor releases all parts >------------------------------

ConfigParseToConsole::~ConfigParseToConsole()
{
  // when Builder goes out of scope, everything must be deallocated
  delete pEnumDefinition;
  delete pPushEnum;
  delete pBeginningOfNameSpace;
  delete pPushNamespace;
  delete pBeginningOfClass;
  delete pPushClass;
  delete pDetectStructure;
  delete pPushStructure;
  delete pDetectControl;
  delete pPushControl;
  delete pDetecttypeDef;
  delete pPushTypeDef;
  delete pDetectUsing;
  delete pPushUsingAlias;
  delete pHandlePush;
  delete pBeginningOfScope;
  delete pHandlePop;
  delete pEndOfScope;
  delete pPushFunction;
  delete pFunctionDefinition;
  delete pDeclaration;
  delete pShowDeclaration;
  delete pExecutable;
  delete pShowExecutable;
  delete pParser;
  delete pSemi;
  delete pToker;
  pIn->close();
  delete pIn;
}
//----< attach toker to a file stream or stringstream >------------

bool ConfigParseToConsole::Attach(const std::string& name, bool isFile){
  if(pToker == 0)
    return false;
  pIn = new std::ifstream(name);
  if (!pIn->good())
    return false;
  pRepo->setFileName(name);
  Repository::addInstance(name,pRepo);
  return pToker->attach(pIn);
}
//----< Here's where alll the parts get assembled >----------------

Parser* ConfigParseToConsole::Build(){
  try{
    pToker = new Toker;
    pToker->toggleComments(false);
    pSemi = new SemiExp(pToker);					pParser = new Parser(pSemi);
    pRepo = new Repository(pToker);
    pBeginningOfScope = new BeginningOfScope();// configure to manage scope
    pHandlePush = new HandlePush(pRepo);
    pBeginningOfScope->addAction(pHandlePush);		pParser->addRule(pBeginningOfScope);
    pEndOfScope = new EndOfScope();
    pHandlePop = new HandlePop(pRepo);
    pEndOfScope->addAction(pHandlePop);				pParser->addRule(pEndOfScope);
	pBeginningOfNameSpace = new BeginningOfNamespace();// configure to detect beginning of Namespace
	pPushNamespace = new pushNamespace(pRepo);
	pBeginningOfNameSpace->addAction(pPushNamespace);pParser->addRule(pBeginningOfNameSpace);
	pBeginningOfClass = new BeginningOfClass();// configure to detect beginnig of class
	pPushClass = new pushClass(pRepo);
	pBeginningOfClass->addAction(pPushClass);		pParser->addRule(pBeginningOfClass);
	pDetectStructure = new DetectStructure();// configure to detect Strucutre
	pPushStructure = new pushStructure(pRepo);
	pDetectStructure->addAction(pPushStructure);	pParser->addRule(pDetectStructure);
	// configure to detect and act on function definitions these will stop further rule checking by returning false
	pFunctionDefinition = new FunctionDefinition;
	pPushFunction = new PushFunction(pRepo);  // no action
	pFunctionDefinition->addAction(pPushFunction);  pParser->addRule(pFunctionDefinition);
	pDetectControl = new DetectControl();// Detect Control
	pPushControl = new PushControl(pRepo);
	pDetectControl->addAction(pPushControl);		pParser->addRule(pDetectControl);
    pDeclaration = new Declaration;             	pParser->addRule(pDeclaration);
    pExecutable = new Executable;					pParser->addRule(pExecutable);
	pEnumDefinition = new EnumDefinition; // Detect Enum
	pPushEnum = new PushEnum(pRepo);
	pEnumDefinition->addAction(pPushEnum);          pParser->addRule(pEnumDefinition);
	pDetecttypeDef = new DetecttypeDef; // Detect typeDef
	pPushTypeDef = new PushTypeDef(pRepo);
	pDetecttypeDef->addAction(pPushTypeDef);		pParser->addRule(pDetecttypeDef);
	pDetectUsing = new DetectUsing;  // Detect Alias
	pPushUsingAlias = new PushUsingAlias(pRepo);
	pDetectUsing->addAction(pPushUsingAlias);		 pParser->addRule(pDetectUsing);
    return pParser;
  }
  catch(std::exception& ex){
    std::cout << "\n\n  " << ex.what() << "\n\n";
    return 0;
  }
}

#ifdef TEST_CONFIGUREPARSER

#include <queue>
#include <string>

int main(int argc, char* argv[])
{
  std::cout << "\n  Testing ConfigureParser module\n "
            << std::string(32,'=') << std::endl;

  // collecting tokens from files, named on the command line

  if(argc < 2)
  {
    std::cout 
      << "\n  please enter name of file to process on command line\n\n";
    return 1;
  }

  for(int i=1; i<argc; ++i)
  {
    std::cout << "\n  Processing file " << argv[i];
    std::cout << "\n  " << std::string(16 + strlen(argv[i]),'-');

    ConfigParseToConsole configure;
    Parser* pParser = configure.Build();
    try
    {
      if(pParser)
      {
        if(!configure.Attach(argv[i]))
        {
          std::cout << "\n  could not open file " << argv[i] << std::endl;
          continue;
        }
      }
      else
      {
        std::cout << "\n\n  Parser not built\n\n";
        return 1;
      }
      // now that parser is built, use it

      while(pParser->next())
        pParser->parse();
      std::cout << "\n\n";
    }
    catch(std::exception& ex)
    {
      std::cout << "\n\n    " << ex.what() << "\n\n";
    }
    std::cout << "\n\n";
  }
}

#endif
