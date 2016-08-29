///////////////////////////////////////////////////////////////
// ASTTree.cpp - Provides functionality for AST              // 
// Ver 1.0                                                   //
// Application: Project #3 Parallel Dependency Analysis      //
// Language:    C++, Visual Studio 2015                      //
// Platform:    Dell Inspiron 5548, Core-i5, Windows 10      //
// Author:      Achal Velani, SUID:292190387                 //
// Reference:   Jim Fawcett, CST 4-187, Syracuse University  //
//              (315) 443-3948, jfawcett@twcny.rr.com        //
///////////////////////////////////////////////////////////////
#include"ASTTree.h"
#include"../ScopeStack/ScopeStack.h"

//This function counts complexity of an element node.
size_t ASTTree::Complexity(element *node)
{
	size_t count = 1;
	std::vector<element*> *v = node->getChildren();
	for (element * child : *v)
	{
		count += ASTTree::Complexity(child);
	}
	return count;

}
//This is a simple function to set an element as a Root node in AST
void ASTTree::setRoot(element *node)
{
	root = node;
}
// This function returns Root node of the AST.
element* ASTTree::getRoot()
{
	return root;
}
//This function returns vector containing all the function nodes in a tree.
std::vector<element> ASTTree::getFunctions(element* pItem)
{
	std::vector<element> funcNodes;
	treeWalkDFS(pItem, funcNodes);
	std::vector<element> fnds(funcNodes.begin(), funcNodes.end());
	return fnds;
}
//Treewalk DFS is used by the getfunctions to do recursion and add function nodes to the vector defined in the getfunctions function.
void ASTTree::treeWalkDFS(element * node, std::vector<element> &funcNodes)
{
	if (node != nullptr && node->type == "function")
		funcNodes.push_back(*node);
	std::vector<element*>::iterator cit;
	if (node != nullptr)
		for (cit = node->getChildren()->begin(); cit != node->getChildren()->end(); cit++)
			treeWalkDFS(*cit, funcNodes);
}
//This is simple trewalk, which takes root node as parameter and prints all the nodes in the tree with proper indentation.
void ASTTree::TreeWalk(element* pItem)
{
	static size_t indentLevel = 0;
	std::cout << "\n  " << std::string(2 * indentLevel, ' ') << pItem->show();
	auto iter = pItem->_children.begin();
	++indentLevel;
	while (iter != pItem->_children.end())
	{
		TreeWalk(*iter);
		++iter;
	}
	--indentLevel;
}

#ifdef TEST_ASTTREE
int main(){
	element *root = new element;//Created an example root for the testing.
	root->name = "Root";
	root->type = "rootType";
	root->startLine = 0;
	root->EndLine = 49;
	root->lineCount = 50;
	element *elem1 = new element;	// Elements  for testing.
	elem1->name = "elem1";
	elem1->type = "elemType";
	elem1->startLine = 3;
	elem1->EndLine = 6;
	elem1->lineCount = 7;
	element *elem2 = new element;
	elem2->name = "elem2";
	elem2->type = "elem2Type";
	elem2->startLine = 7;
	elem2->EndLine = 13;
	elem2->lineCount = 7;
	element *elem3 = new element;
	elem3->name = "elem3";
	elem3->type = "elem3Type";
	elem3->startLine = 14;
	elem3->EndLine = 21;
	elem3->lineCount = 8;
	element *elem4 = new element;
	elem4->name = "elem4";
	elem4->type = "function";
	elem4->startLine = 22;
	elem4->EndLine = 48;
	elem4->lineCount = 27;
	ASTTree *test_tree = new ASTTree;
	test_tree->setRoot(root);
	root->addChild(elem1);
	root->addChild(elem2);
	elem1->addChild(elem3);
	elem2->addChild(elem4);
	root->complexity = test_tree->Complexity(root); // setting complexity
	elem1->complexity = test_tree->Complexity(elem1);
	elem2->complexity = test_tree->Complexity(elem2);
	elem3->complexity = test_tree->Complexity(elem3);
	elem4->complexity = test_tree->Complexity(elem4);
	test_tree->TreeWalk(test_tree->getRoot()); // testing tree walk
	std::vector<element> funcs = test_tree->getFunctions(test_tree->getRoot());
	std::cout << "\nLineCount:" << "\tComplexity:" << "\tFunctionName:";
	for (auto func : funcs) {
		std::cout << "\n" << func.lineCount << " ";
		std::cout << "\t\t" << func.complexity;
		std::cout << "\t\t" << func.name << " ";
		return 0;
	}
	std::cout << "\n\n";
}

#endif // TEST_ASTTree

