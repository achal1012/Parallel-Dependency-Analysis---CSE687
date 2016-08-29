/////////////////////////////////////////////////////////////////////////////
//  ActionsAndRules.cpp - implements new parsing rules and actions         //
//  ver 2.1                                                                //
//  Language:    Visual C++                                                //
//  Platform:    Dell Inspiron 5548, Core-i5, Windows 10                   //
//  Application: Project #3 Parallel Dependency Analysis                   //
//  Author:      Achal Velani, SUID:292190387                              //
//  Reference:   Jim Fawcett, CST 4-187, Syracuse University               //
//              (315) 443-3948, jfawcett@twcny.rr.com                      //
/////////////////////////////////////////////////////////////////////////////

#include"ActionsAndRules.h"
Repository* Repository::instance = nullptr;
std::unordered_map<std::string, Repository*> Repository::setOfInstances;
std::mutex Repository::mtx;
//std::unordered_map<std::string, RecordData> Repository::finalTypeTable;
TypeTable Repository::finalTypeTable;
#ifdef TEST_ACTIONSANDRULES

#include <iostream>
#include "ActionsAndRules.h"
#include "Tokenizer.h"
#include "SemiExpression.h"

int main(int argc, char* argv[])
{
  std::cout << "\n  Testing ActionsAndRules class\n "
            << std::string(30,'=') << std::endl;

  try
  {
    std::queue<std::string> resultsQ;
    PreprocToQ ppq(resultsQ);
    PreprocStatement pps;
    pps.addAction(&ppq);

    FunctionDefinition fnd;
    PrettyPrintToQ pprtQ(resultsQ);
    fnd.addAction(&pprtQ);

    Toker toker("../ActionsAndRules.h");
    SemiExp se(&toker);
    Parser parser(&se);
    parser.addRule(&pps);
    parser.addRule(&fnd);
    while(se.get())
      parser.parse();
    size_t len = resultsQ.size();
    for(size_t i=0; i<len; ++i)
    {
      std::cout << "\n  " << resultsQ.front().c_str();
      resultsQ.pop();
    }
    std::cout << "\n\n";
  }
  catch(std::exception& ex)
  {
    std::cout << "\n\n  " << ex.what() << "\n\n";
  }
}
#endif
