#ifndef ASTTREE_H
#define ASTTREE_H

///////////////////////////////////////////////////////////////
// ASTTree.h - Provides functionality for AST                // 
// Ver 1.0                                                   //
// Application: Project #3 Parallel Dependency Analysis      //
// Language:    C++, Visual Studio 2015                      //
// Platform:    Dell Inspiron 5548, Core-i5, Windows 10      //
// Author:      Achal Velani, SUID:292190387                 //
// Reference:   Jim Fawcett, CST 4-187, Syracuse University  //
//              (315) 443-3948, jfawcett@twcny.rr.com        //
///////////////////////////////////////////////////////////////

/*
* Package Operations :
*== == == == == == == == == == == == == == == =
* This package provides functionalites to build the Abstract Syntax tree.
*
* Public Interface :
*== == == == == == == == == == == == == == ==
*
* ASTTree() - simple void constructor.
* void setRoot(element *node) - sets the node as a root node.
* element* getRoot() - returns the root node.
* void TreeWalk(element* pItem) - provides tree walk function and prints the tree.
* size_t Complexity(element *node) - Returns complexity count for a node.
* std::vector<element> getFunctions(element* pItem) - returns vector containing all the nodes containing functions
* void treeWalkDFS(element * node, std::vector<element> &functionNodes) - Provides treewalk dfs while adding nodes to the functionNodes vector.
*
* Required Files :
*== == == == == == == == == == == == == ==
* ASTTree.h, ASTTRee.cpp
* Build Command :
*== == == == == == == == == == == == == ==
* devenv Project3.sln / rebuild debug
*
* Maintenance History :
*== == == == == == == == == == == == == ==
* ver 1.0 : 13 Mar 16
* -first release
*/

#include <string>
#include<vector>
#include<iostream>
#include <sstream>
//Element structure is the node in the AST.
struct element
{
	std::string type;
	std::string name;
	size_t lineCount;
	size_t startLine;
	size_t EndLine;
	std::vector<element*> _children;
	size_t complexity;
	//simple show function to display an element in the AST
	std::string show()
	{
		std::ostringstream temp;
		temp << "(";
		temp << type;
		temp << ", ";
		temp << name;
		temp << ", ";
		temp << lineCount;
		temp << ", ";
		temp << complexity;
		temp << ")";
		return temp.str();
	}
	// Adds child to the vector of the element.
	void addChild(element *node) {
		_children.push_back(node);
	}
	// Retrives the children of a node.
	std::vector<element*>* getChildren(){
		return &_children;
	}
	// Used to delete a child node from the vector children
	void element::deleteChild(element *node, element *deleteNode)
	{
		if (node == nullptr)
			return;
		std::vector<element*>::iterator it;
		std::vector<element*> *children = node->getChildren();
		for (it = children->begin(); it != children->end();)
		{
			if ((*it) == deleteNode)
			{
				delete * it;
				it = children->erase(it);
			}
			else
			{
				deleteChild((*it), deleteNode);
				++it;
			}
		}
	}
};


// AST Class
class ASTTree
{
public:
	ASTTree() {}
	void setRoot(element *node);
	element* getRoot();
	void TreeWalk(element* pItem);
	size_t Complexity(element *node);
	std::vector<element> getFunctions(element* pItem);
	void treeWalkDFS(element * node, std::vector<element> &functionNodes);

private:
	element *root;
};

#endif