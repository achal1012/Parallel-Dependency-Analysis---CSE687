#ifndef ACTIONSANDRULES_H
#define ACTIONSANDRULES_H
/////////////////////////////////////////////////////////////////////////////
// ActionsAndRules.h - declares new parsing rules and actions              //
// ver 2.2                                                                 //
// Language:    Visual C++                                                 //
// Platform:    Dell Inspiron 5548, Core-i5, Windows 10                    //
// Application: Project #3 Parallel Dependency Analysis                    //
// Author:      Achal Velani, SUID:292190387                               //
// Reference:   Jim Fawcett, CST 4-187, Syracuse University                //
//              (315) 443-3948, jfawcett@twcny.rr.com                      //
/////////////////////////////////////////////////////////////////////////////
/*
  Module Operations: 
  ==================
  This module defines several action classes.  Its classes provide 
  specialized services needed for specific applications.  The modules
  Parser, SemiExpression, and Tokenizer, are intended to be reusable
  without change.  This module provides a place to put extensions of
  these facilities and is not expected to be reusable. 

  Public Interface:
  =================
  Toker t(someFile);              // create tokenizer instance
  SemiExp se(&t);                 // create a SemiExp attached to tokenizer
  Parser parser(se);              // now we have a parser
  Rule1 r1;                       // create instance of a derived Rule class
  Action1 a1;                     // create a derived action
  r1.addAction(&a1);              // register action with the rule
  parser.addRule(&r1);            // register rule with parser
  while(se.getSemiExp())          // get semi-expression
  parser.parse();                 //   and parse it

  Build Process:
  ==============
  Required files
    - Parser.h, Parser.cpp, ScopeStack.h, ScopeStack.cpp,
      ActionsAndRules.h, ActionsAndRules.cpp, ConfigureParser.cpp,
      ItokCollection.h, SemiExpression.h, SemiExpression.cpp, tokenizer.h, tokenizer.cpp
  Build commands
    - devenv Project3.sln / rebuild debug

  Maintenance History:
  ====================
  ver 2.2 : 13 Mar 16
  - Final submiited version with added more action and rules.
  ver 2.1 : 15 Feb 16
  - small functional change to a few of the actions changes display strategy
  - preface the (new) Toker and SemiExp with Scanner namespace
  ver 2.0 : 01 Jun 11
  - added processing on way to building strong code analyzer
  ver 1.1 : 17 Jan 09
  - changed to accepst a pointer to interfaced ITokCollection instead
    of a SemiExpression
  ver 1.0 : 12 Jan 06
  - first release

*/
#include <queue>
#include <string>
#include <sstream>
#include <mutex>
#include "Parser.h"
#include "../ASTTree/ASTTree.h"
#include "../SemiExp/itokcollection.h"
#include "../ScopeStack/ScopeStack.h"
#include "../Tokenizer/Tokenizer.h"
#include "../SemiExp/SemiExp.h"
#include "../TypeTable/TypeTable.h"
///////////////////////////////////////////////////////////////
// ScopeStack element is application specific
///////////////////////////////////////////////////////////////
// Repository instance is used to share resources
// among all actions,.

class Repository
{
private:
  ScopeStack<element*> stack;
  Scanner::Toker* p_Toker;
  ASTTree *tree;
  static Repository* instance;
  std::string fileName;
  TypeTable *tp;
  static std::mutex mtx;

protected:
  static std::unordered_map<std::string, Repository*> setOfInstances;
  //static std::unordered_map<std::string, RecordData> finalTypeTable;
  static TypeTable finalTypeTable;
public:


  Repository(Scanner::Toker* pToker){
	instance = this;
    p_Toker = pToker;
	tree = new ASTTree();
	element *root = new element();
	root->type = "Namespace";
	root->name = "Global";
	root->startLine = 0;
	tree->setRoot(root);
	stack.push(root);
	tp = new TypeTable();
  }
  static TypeTable getFinalTable() {
	  return finalTypeTable; 
  }
  static void addInstance(std::string fileName, Repository *repo)
  {
	  std::lock_guard<std::mutex> lock(mtx);
	  setOfInstances[fileName] = repo;
  }

  TypeTable* getTypeTable()
  {	
	  return tp;
  }
  ASTTree * getAST(){
	  return tree;
  }
  static Repository* getInstance(std::string fileName){
	  if (setOfInstances.size() > 0)
		  return setOfInstances[fileName];
	  else
		  return nullptr;
  }
  ScopeStack<element*>& scopeStack(){
    return stack;
  }
  Scanner::Toker* Toker(){
    return p_Toker;
  }
  size_t lineCount(){
    return (size_t)(p_Toker->lineCount());
  }
  void setFileName(std::string fName){
	  fileName = fName;
  }
  std::string getFileName(){
	  return fileName;
  }
  static void mergeToTypeTable() {
	  std::unordered_map<std::string, Repository*>::iterator it;
	  for (it = setOfInstances.begin(); it != setOfInstances.end(); it++) {
		  TypeTable *interMediateTbale = (it->second)->getTypeTable();
		  finalTypeTable.insert(interMediateTbale);
	  }
  }
};

///////////////////////////////////////////////////////////////
// rule to detect beginning of anonymous scope

class BeginningOfScope : public IRule{
public:
  bool doTest(ITokCollection*& pTc){
	std::string a = (*pTc)[pTc->find("{") - 1];
    if(pTc->find("{") < pTc->length()){
      doActions(pTc);
      return true;
    }
    return true;
  }
};

///////////////////////////////////////////////////////////////
// action to handle scope stack at end of scope

class HandlePush : public IAction{
  Repository* p_Repos;
public:
  HandlePush(Repository* pRepos){
    p_Repos = pRepos;
  }
  void doAction(ITokCollection*& pTc){
    element * elem = new element; 
    elem->type = "unknown";
    elem->name = "anonymous";
	elem->startLine = p_Repos->lineCount();
	p_Repos->scopeStack().top()->addChild(elem);
    p_Repos->scopeStack().push(elem);
  }
};

///////////////////////////////////////////////////////////////
// rule to detect end of scope

class EndOfScope : public IRule{
public:
  bool doTest(ITokCollection*& pTc)
  {
    if(pTc->find("}") < pTc->length()){
      doActions(pTc);
      return true;
    }
    return true;
  }
};

///////////////////////////////////////////////////////////////
// action to handle scope stack at end of scope

class HandlePop : public IAction
{
  Repository* p_Repos;
public:
  HandlePop(Repository* pRepos){
    p_Repos = pRepos;
  }
  void doAction(ITokCollection*& pTc){
    if(p_Repos->scopeStack().size() == 0)
      return;
    element *elem = p_Repos->scopeStack().pop();
	elem->EndLine = p_Repos->lineCount();
	elem->lineCount = elem->EndLine - elem->startLine + 1;
	elem->complexity = p_Repos->getAST()->Complexity(elem);
  }
};

///////////////////////////////////////////////////////////////
// rule to detect preprocessor statements

class PreprocStatement : public IRule
{
public:
  bool doTest(ITokCollection*& pTc)
  {
    if(pTc->find("#") < pTc->length()){
      doActions(pTc);
      return true;
    }
    return true;
  }
};

///////////////////////////////////////////////////////////////
// action to print preprocessor statement to console

class PrintPreproc : public IAction
{
public:
  void doAction(ITokCollection*& pTc){
    std::cout << "\n\n  Preproc Stmt: " << pTc->show().c_str();
  }
};

///////////////////////////////////////////////////////////////
// rule to detect enum definitions
class EnumDefinition : public IRule {
public:
	bool doTest(ITokCollection*& pTc){
		if (pTc->find("enum") < pTc->length()) {
			doActions(pTc);
			return true;	
		}
		return true;
	}
};

///////////////////////////////////////////////////////////////
// action to push enum name onto ScopeStack
class PushEnum : public IAction
{
	Repository* p_Repos;
public:
	PushEnum(Repository* pRepos){
		p_Repos = pRepos;
	}
	bool ifNoName(const std::string& tok)
	{
		if (tok == "\n")
			return true;
		return false;
	}
	void doAction(ITokCollection*& pTc)
	{
		// pop anonymous scope
		element* deElem = p_Repos->scopeStack().pop();
		p_Repos->scopeStack().top()->deleteChild(p_Repos->scopeStack().top(), deElem);

		// push enum scope
		std::string name;
		element *elem = new element;
		if (pTc->find("class") < pTc->length()) {
			name = (*pTc)[pTc->find("enum") + 2];
			elem->type = "enum strong";
		}
		else {
			name = (*pTc)[pTc->find("enum") + 1];
			elem->type = "enum";
		}
		if (ifNoName(name))
			name = "noName";
		elem->name = name;
		
		element* check = p_Repos->scopeStack().top();
		std::string nspace = p_Repos->scopeStack().top()->name;
		p_Repos->getTypeTable()->constructRecord(name, elem->type, p_Repos->getFileName(), nspace);

		elem->startLine = p_Repos->lineCount();
		p_Repos->scopeStack().top()->addChild(elem);
		p_Repos->scopeStack().push(elem);

	}
};

///////////////////////////////////////////////////////////////
// rule to detect function definitions

class FunctionDefinition : public IRule{
public:
  bool isSpecialKeyWord	(const std::string& tok){
    const static std::string keys[]
      = { "for", "while", "switch", "if", "catch","else","try","do" };
    for(int i=0; i<8; ++i)
      if(tok == keys[i])
        return true;
    return false;
  }
  bool doTest(ITokCollection*& pTc){
    ITokCollection& tc = *pTc;
    if(tc[tc.length()-1] == "{"){
      size_t len = tc.find("(");
      if(len < tc.length() && !isSpecialKeyWord(tc[len-1])){
        doActions(pTc);
        return true;
      }
    }
    return true;
  }
};

///////////////////////////////////////////////////////////////
// action to push function name onto ScopeStack

class PushFunction : public IAction{
  Repository* p_Repos;
public:
  PushFunction(Repository* pRepos){
    p_Repos = pRepos;
  }
  void doAction(ITokCollection*& pTc){
	
    // pop anonymous scope
	element* deElem = p_Repos->scopeStack().pop();
	p_Repos->scopeStack().top()->deleteChild(p_Repos->scopeStack().top(), deElem);
    // push function scope
	ITokCollection& tc = *pTc;
    std::string name = (*pTc)[pTc->find("(") - 1];
	size_t findColon = tc.find("::");
	size_t findDist = tc.find("~");
	element *elem = new element;
	elem->type = "Function";
	if (findColon < tc.length())
	{
		if ((*pTc)[pTc->find("::") - 1] != "std")
			if ((*pTc)[pTc->find("::") - 1] == name && findDist >= tc.length())
			{
				name = (*pTc)[pTc->find("::") - 1]+"::" + name;
				elem->type = "Construcor";
			}
			else if ((*pTc)[pTc->find("::") - 1] == name && findDist < tc.length())
			{
				name = (*pTc)[pTc->find("::") - 1]+"::" + (*pTc)[pTc->find("::") + 1]+ name;
				elem->type = "Destructor";
			}
			else
				name = /*(*pTc)[pTc->find("::") - 1] + "::" +*/ name;		
	}
    
    elem->name = name;
	if (name != "main")
	{
		element* check = p_Repos->scopeStack().top();
			std::string nspace = p_Repos->scopeStack().top()->name;
			p_Repos->getTypeTable()->constructRecord(name, elem->type, p_Repos->getFileName(), nspace);
	}
    elem->startLine = p_Repos->lineCount();
	p_Repos->scopeStack().top()->addChild(elem);
    p_Repos->scopeStack().push(elem);
	
  }
};

///////////////////////////////////////////////////////////////
// action to send semi-expression that starts a function def
// to console

class PrintFunction : public IAction{
  Repository* p_Repos;
public:
  PrintFunction(Repository* pRepos){
    p_Repos = pRepos;
  }
  void doAction(ITokCollection*& pTc){
   // std::cout << "\n  FuncDef: " << pTc->show().c_str();
  }
};

///////////////////////////////////////////////////////////////
// action to send signature of a function def to console

class PrettyPrintFunction : public IAction
{
public:
  void doAction(ITokCollection*& pTc){
    pTc->remove("public");
    pTc->remove(":");
    pTc->trimFront();
    int len = pTc->find(")");
    for(int i=0; i<len+1; ++i)
      std::cout << (*pTc)[i] << " ";
  }
};

///////////////////////////////////////////////////////////////
// Rule to detect "using" aliases to TypeTable
class DetectUsing : public IRule
{
	bool doTest(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		if (tc.find("using")<tc.length() && tc.find("=")<tc.length()){
			doActions(pTc);
			return true;
		}
		return true;
	}
};

//////////////////////////////////////////////////////////////
// Action to push "using" aliases on the typetable
class PushUsingAlias : public IAction
{
	Repository* p_Repos;
public:
	PushUsingAlias(Repository* pRepos) {
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc) {
		ITokCollection& tc = *pTc;
		/////////////////////////
		element* check = p_Repos->scopeStack().top();
		std::string nspace = p_Repos->scopeStack().top()->name;
		std::string name = (*pTc)[pTc->find("=") - 1];
		p_Repos->getTypeTable()->constructRecord(name, "UsingAlias", p_Repos->getFileName(), nspace);
		
		/////////////////////////
	}

};



///////////////////////////////////////////////////////////////
// Rule to detect typeDefs to TypeTable
class DetecttypeDef : public IRule 
{
	bool doTest(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		if (tc.find("typedef")<tc.length())
		{
			doActions(pTc);
			return true;
		}
		return true;
	}
};

//////////////////////////////////////////////////////////////
// Action to push typedef on the typetable
class PushTypeDef : public IAction
{
	Repository* p_Repos;
public:
	PushTypeDef(Repository* pRepos) {
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc) {
		ITokCollection& tc = *pTc;
		/////////////////////////
		element* check = p_Repos->scopeStack().top();
		std::string nspace = p_Repos->scopeStack().top()->name;
		std::string name = (*pTc)[pTc->find(";") - 1];
		if (tc.find("[") < tc.length() && tc.find("]") < tc.length())
		{
			name = (*pTc)[pTc->find("[") - 1];
		}
		p_Repos->getTypeTable()->constructRecord(name, "typeDef", p_Repos->getFileName(), nspace);
	}

};


///////////////////////////////////////////////////////////////
// rule to detect declaration

class Declaration : public IRule          // declar ends in semicolon
{                                         // has type, name, modifiers &
public:                                   // initializers.  So eliminate
  bool isModifier(const std::string& tok) // modifiers and initializers.
  {                                       // If you have two things left
    const size_t numKeys = 22;            // its declar else executable.
    const static std::string keys[numKeys] = {
      "const", "extern", "friend", "mutable", "signed", "static",
      "typedef", "typename", "unsigned", "volatile", "&", "*", "std", "::",
      "public", "protected", "private", ":", "typename", "typedef", "++", "--"
    };
    for (int i = 0; i<numKeys; ++i)
    if (tok == keys[i])
      return true;
    return false;
  }
  void condenseTemplateTypes(ITokCollection& tc){
    size_t start = tc.find("<");
    size_t end = tc.find(">");
    if (start >= end || start == 0)
      return;
    else{
      if (end == tc.length())
        end = tc.find(">::");
      if (end == tc.length())
        return;
    }
    std::string save = tc[end];
    std::string tok = tc[start - 1];
    for (size_t i = start; i < end + 1; ++i)
      tok += tc[i];
    for (size_t i = start; i < end + 1; ++i)
      tc.remove(start);
    if (save == ">::"){
      tok += tc[start + 1];
      tc.remove(start);
    }
    tc[start - 1] = tok;
  }
  bool isSpecialKeyWord(const std::string& tok){
    const static std::string keys[]
      = { "for", "while", "switch", "if", "catch","else","try","do" };
    for (int i = 0; i<8; ++i)
    if (tok == keys[i])
      return true;
    return false;
  }
  void removeInvocationParens(ITokCollection& tc){
    size_t start = tc.find("(");
    size_t end = tc.find(")");
    if (start >= end || end == tc.length() || start == 0)
      return;
    if (isSpecialKeyWord(tc[start - 1]))
      return;
    for (size_t i = start; i < end + 1; ++i)
      tc.remove(start);
  }
  bool doTest(ITokCollection*& pTc){
    ITokCollection& in = *pTc;
    Scanner::SemiExp tc;
    for (size_t i = 0; i<in.length(); ++i)
      tc.push_back(in[i]);
    if (tc[tc.length() - 1] == ";" && tc.length() > 2){
      removeInvocationParens(tc);
      condenseTemplateTypes(tc);
      // remove modifiers, comments, newlines, returns, and initializers
	  ///////////////////////////////////////////////////////////////////////////////////////////////////// ISCOmment
      Scanner::SemiExp se;
      for (size_t i = 0; i < tc.length(); ++i){
        if (isModifier(tc[i])|| se.isComment(tc[i]) || tc[i] == "\n" || tc[i] == "return")
          continue;
        if (tc[i] == "=" || tc[i] == ";")
          break;
        else
          se.push_back(tc[i]);
      }
      if (se.length() == 2){
        doActions(pTc);
        return true;
      }
    }
    return true;
  }
};

///////////////////////////////////////////////////////////////
// action to show declaration

class ShowDeclaration : public IAction{
public:
  void doAction(ITokCollection*& pTc){
    ITokCollection& tc = *pTc;
    // remove comments
    Scanner::SemiExp se;
    for (size_t i = 0; i<tc.length(); ++i)
      if (!se.isComment(tc[i]))
        se.push_back(tc[i]);
    // show cleaned semiExp
  }
};

///////////////////////////////////////////////////////////////
// rule to detect executable

class Executable : public IRule           // declare ends in semicolon
{                                         // has type, name, modifiers &
public:                                   // initializers.  So eliminate
  bool isModifier(const std::string& tok) // modifiers and initializers.
  {                                       // If you have two things left
    const size_t numKeys = 22;            // its declar else executable.
    const static std::string keys[numKeys] = {
      "const", "extern", "friend", "mutable", "signed", "static",
      "typedef", "typename", "unsigned", "volatile", "&", "*", "std", "::",
      "public", "protected", "private", ":", "typename", "typedef", "++", "--"
    };
    for (int i = 0; i<numKeys; ++i)
    if (tok == keys[i])
      return true;
    return false;
  }
  void condenseTemplateTypes(ITokCollection& tc){
    size_t start = tc.find("<");
    size_t end = tc.find(">");
    if (start >= end || start == 0)
      return;
    else{
      if (end == tc.length())
        end = tc.find(">::");
      if (end == tc.length())
        return;
    }
    std::string save = tc[end];
    std::string tok = tc[start - 1];
    for (size_t i = start; i < end+1; ++i)
      tok += tc[i];
    for (size_t i = start; i < end+1; ++i)
      tc.remove(start);
    if (save == ">::"){
      tok += tc[start + 1];
      tc.remove(start);
    }
    tc[start - 1] = tok;
  }
  
  bool isSpecialKeyWord(const std::string& tok){
    const static std::string keys[]
      = { "for", "while", "switch", "if", "catch" };
    for (int i = 0; i<5; ++i)
    if (tok == keys[i])
      return true;
    return false;
  }
  void removeInvocationParens(ITokCollection& tc){
    size_t start = tc.find("(");
    size_t end = tc.find(")");
    if (start >= end || end == tc.length() || start == 0)
      return;
    if (isSpecialKeyWord(tc[start - 1]))
      return;
    for (size_t i = start; i < end + 1; ++i)
      tc.remove(start);
  }
  bool doTest(ITokCollection*& pTc){
    ITokCollection& in = *pTc;
    Scanner::SemiExp tc;
    for (size_t i = 0; i<in.length(); ++i)
      tc.push_back(in[i]);
    if (tc[tc.length() - 1] == ";" && tc.length() > 2){
      removeInvocationParens(tc);
      condenseTemplateTypes(tc);
      // remove modifiers, comments, newlines, returns, and initializers
      Scanner::SemiExp se;
      for (size_t i = 0; i < tc.length(); ++i){
        if (isModifier(tc[i])|| se.isComment(tc[i]) || tc[i] == "\n" || tc[i] == "return")
          continue;
        /*if (se.isComment(tc[i]) || tc[i] == "\n" || tc[i] == "return")
          continue;*/
        if (tc[i] == "=" || tc[i] == ";")
          break;
        else
          se.push_back(tc[i]);
      }
      //std::cout << "\n  ** " << se.show();
      if (se.length() != 2)  // not a declaration
      {
        doActions(pTc);
        return true;
      }
    }
    return true;
  }
};

///////////////////////////////////////////////////////////////
// action to show executable

class ShowExecutable : public IAction
{
public:
  void doAction(ITokCollection*& pTc)
  {
    ITokCollection& tc = *pTc;
    // remove comments
    Scanner::SemiExp se;
    for (size_t i = 0; i < tc.length(); ++i)
    {
      if (!se.isComment(tc[i]))
        se.push_back(tc[i]);
    }
    // show cleaned semiExp
    //std::cout << "\n  Executable: " << se.show();
  }
};

///////////////////////////////////////////////////////////////
// Rule to detect beginning of class
class BeginningOfClass: public IRule
{
public:
	bool doTest(ITokCollection*& pTc)
	{
		if (pTc->find("class") < pTc->length())
		{
			doActions(pTc);
			return true;
		}
		return true;
	}
};

///////////////////////////////////////////////////////////////
// action to print class statement to console

class PrintClass : public IAction
{
public:
	void doAction(ITokCollection*& pTc)
	{
		//std::cout << "\n\n  Class Stmt: " << pTc->show().c_str();
	}
};

///////////////////////////////////////////////////////////////
// action to push class statement to repo
class pushClass : public IAction
{
	Repository* p_Repos;
public:
	pushClass(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		//isglobal
	
		element* deElem=p_Repos->scopeStack().pop();
		p_Repos->scopeStack().top()->deleteChild(p_Repos->scopeStack().top(),deElem);
		
		std::string name = (*pTc)[pTc->find("class")+1];
		element *elem= new element;
		elem->type = "Class";
		elem->name = name;
		////
		element* check = p_Repos->scopeStack().top();
		std::string nspace = p_Repos->scopeStack().top()->name;
		p_Repos->getTypeTable()->constructRecord(name, "Class", p_Repos->getFileName(), nspace);
		elem->startLine = p_Repos->lineCount();
		p_Repos->scopeStack().top()->addChild(elem);
		p_Repos->scopeStack().push(elem);
	}
};

///////////////////////////////////////////////////////////////
// Rule to detect beginning of namespace
class BeginningOfNamespace : public IRule
{
public:
	bool doTest(ITokCollection*& pTc)
	{
		if (pTc->find("namespace") < pTc->length() && !(pTc->find("using")<pTc->length()))
		{
			doActions(pTc);
			return true;
		}
		return true;
	}
};

///////////////////////////////////////////////////////////////
// action to push namespace statement to repo
class pushNamespace : public IAction
{
	Repository* p_Repos;
public:
	pushNamespace(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		element* deElem = p_Repos->scopeStack().pop();
		p_Repos->scopeStack().top()->deleteChild(p_Repos->scopeStack().top(), deElem);

		std::string name = (*pTc)[pTc->find("namespace") + 1];
		element *elem = new element;
		elem->type = "Namespace"; 
		elem->name = name;

		std::string nspace = p_Repos->scopeStack().top()->name;
		p_Repos->getTypeTable()->constructRecord(name, "Namespace", p_Repos->getFileName(), nspace);

		elem->startLine = p_Repos->lineCount();
		p_Repos->scopeStack().top()->addChild(elem);
		p_Repos->scopeStack().push(elem);
	}
};

///////////////////////////////////////////////////////////////
// Rule to detect Structures
class DetectStructure : public IRule
{
public:
	bool doTest(ITokCollection*& pTc)
	{
		if (pTc->find("struct") < pTc->length()&& pTc->find("{") < pTc->length())
		{
			doActions(pTc);
			return true;
		}
		return true;
	}
};

///////////////////////////////////////////////////////////////
// action to print structure to console

class PrintStructure : public IAction
{
public:
	void doAction(ITokCollection*& pTc)
	{
		//std::cout << "\n\n  Structure: " << pTc->show().c_str();
	}
};

///////////////////////////////////////////////////////////////
// action to push class statement to repo
class pushStructure : public IAction
{
	Repository* p_Repos;
public:
	pushStructure(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		element* deElem = p_Repos->scopeStack().pop();
		p_Repos->scopeStack().top()->deleteChild(p_Repos->scopeStack().top(), deElem);

		std::string name = (*pTc)[pTc->find("struct") + 1];
		element *elem= new element;
		elem->type = "Structure";
		elem->name = name;
		/////////////////////////
		element* check = p_Repos->scopeStack().top();
		std::string nspace = p_Repos->scopeStack().top()->name;
		p_Repos->getTypeTable()->constructRecord(name, "Structure", p_Repos->getFileName(), nspace);
		
		elem->startLine = p_Repos->lineCount();
		p_Repos->scopeStack().top()->addChild(elem);
		p_Repos->scopeStack().push(elem);
		
	}
};



///////////////////////////////////////////////////////////////
// Rule to detect Structures
class DetectControl : public IRule
{
public:
	bool isSpecialKeyWord(const std::string& tok)
	{
		std::vector<std::string> keys
			= { "for", "while", "switch", "if", "catch","else","try","do"};
		for (int i = 0; i < (int)keys.size(); ++i)
			if (tok == keys[i])
				return true;
		return false;
	}
	bool doTest(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		if (tc[tc.length() - 1] == "{")
		{
			size_t len = tc.find("(");
			tc.trimFront();
			if (len < tc.length() && isSpecialKeyWord(tc[len - 1]))
			{
				doActions(pTc);
				return true;
			}
			else if (isSpecialKeyWord(tc[0]))
			{
				doActions(pTc);
				return true;
			}
		}
		return true;
	}
};

///////////////////////////////////////////////////////////////
// action to print ControlStatements

class PrintControl : public IAction
{
public:
	void doAction(ITokCollection*& pTc)
	{
		//std::cout << "\n\n  Control: " << pTc->show().c_str();
	}
};


///////////////////////////////////////////////////////////////
// action to push control  onto ScopeStack

class PushControl : public IAction
{
	Repository* p_Repos;
public:
	PushControl(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		// next statement is now  handled in PrintFunction
		// std::cout << "\n  FunctionDef: " << pTc->show();

		// pop anonymous scope
		element* deElem = p_Repos->scopeStack().pop();
		p_Repos->scopeStack().top()->deleteChild(p_Repos->scopeStack().top(), deElem);

		// push function scope
		std::string name;
		if (pTc->find("(")<pTc->length())
		{
			name = (*pTc)[pTc->find("(") - 1];
		}
		else if ((*pTc)[pTc->find("{") - 1] == "\n")
		{
			int count = 0;
			//std::cout << "New line found";
			do
			{
				count++;
			} while ((*pTc)[pTc->find("{") - count] == "\n");
			name = (*pTc)[pTc->length() - count - 1];
		}
		else
			name = (*pTc)[pTc->find("{") - 1];
		

			 
		element *elem= new element;
		elem->type = "control";
		elem->name = name;
		elem->startLine = p_Repos->lineCount();
		p_Repos->scopeStack().top()->addChild(elem);
		p_Repos->scopeStack().push(elem);
	}
};

#endif
