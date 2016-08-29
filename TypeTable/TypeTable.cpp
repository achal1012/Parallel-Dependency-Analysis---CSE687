////////////////////////////////////////////////////////////////
// TypeTable.cpp - Provides structure for TypeTable and       //
//                 TableRecord                                //
// Ver 1.0                                                    //
// Application: Project #3 Parallel Dependency Analysis       //
// Language:    C++, Visual Studio 2015                       //
// Platform:    Dell Inspiron 5548, Core-i5, Windows 10       //
// Author:      Achal Velani, SUID:292190387                  //
// Reference:   Jim Fawcett, CST 4-187, Syracuse University   //
//              (315) 443-3948, jfawcett@twcny.rr.com         //
////////////////////////////////////////////////////////////////

#include"TypeTable.h"

// This function helps in constructing a record in the type table.
void TypeTable::constructRecord(Name name, Type type, FileName fileName, Namespace nmspace) {
	std::unordered_map<std::string, RecordData>::const_iterator lookFor = tableRecord->find(name);
	//if not found create a new entry in the map
	if (lookFor == tableRecord->end()) {
		RecordData rd;
		rd.setType(type);
		rd.addfileNameSpace(fileName, nmspace);
		(*tableRecord)[name] = rd;
	}
	// if record found then fetch and addfileName
	else {
		RecordData exsitingr = (*tableRecord)[name];
		exsitingr.addfileNameSpace(fileName, nmspace);
		(*tableRecord)[name] = exsitingr;
	}
}

//This function prints typetable in proper fashion.
void TypeTable::showTypeTable(){
	std::cout << "\n-------------------------------------------------------------";
	std::cout << "\n FINAL TYPE TABLE:";
	std::cout << "\n-------------------------------------------------------------\n";
	std::cout << "\n " << std::left << std::setw(25) << "Name";
	std::cout << std::setw(15) << "Type" << std::setw(20) << "Namespace";
	std::cout << std::setw(15) << "FileName" << "\n";

	for (auto x : (*tableRecord)) {
		std::cout << std::endl<<" "<< std::setw(25) << x.first;
		RecordData rd = x.second;
		std::string type= rd.gettype();
		std::cout << std::setw(15)<< type;
		std::vector<std::pair<FileName,Namespace>> FileNamespace = x.second.getfileNameSpace();
		bool  first = true;
		for (auto it = FileNamespace.begin(); it != FileNamespace.end(); it++)
		{
			// To get hold of the class pointers:
			std::string _FileName = it->first;
			std::string _NameSpace = it->second;
			if (first == true) {
				std::cout << std::setw(20) << _NameSpace << std::setw(15) << _FileName << std::endl;
				first = false;
			}
			else {
				std::cout <<" "<< std::setw(25)<<"" << std::setw(15)<<"" << std::setw(20) << _NameSpace << std::setw(15) << _FileName << std::endl;
			}
		}
	}
}
// This function returns the unordered_map type tableRecord.
std::unordered_map<std::string, RecordData>* TypeTable::returntableRecord(){
	//std::cout << std::endl<<"Size : " << tableRecord->size() << std::endl;
	return tableRecord;
}

//This will insert typetable objects to create final typeTable.
void TypeTable::insert(TypeTable * tp){
	std::unordered_map<std::string, RecordData>::iterator it;
	for (it = tp->returntableRecord()->begin(); it != tp->returntableRecord()->end();it++) {
		/*check for previous entries if found then add to child else make an entry*/
		std::unordered_map<std::string, RecordData>::const_iterator lookFor = this->returntableRecord()->find(it->first);
		//Find for key if not found then add an entry. 
		if (lookFor == this->returntableRecord()->end()) 
			(*tableRecord)[it->first] = it->second;
		//If key found then push name and filespace to that key
		else {
			RecordData exsitingr = (*tableRecord)[it->first];
			std::vector<std::pair<FileName, Namespace>> exnameFile=it->second.getfileNameSpace();
			std::vector<std::pair<FileName, Namespace>>::iterator ex;
			for (ex = exnameFile.begin(); ex != exnameFile.end(); ex++){
				exsitingr.addfileNameSpace(ex->first, ex->second);
			}
			(*tableRecord)[it->first] = exsitingr;
		}
	}
}
#ifdef TEST_TYPETABLE 
int main()
{
	TypeTable t;
	RecordData rd;
	rd.setType("Function");
	rd.addfileNameSpace("Abc.cpp", "Global");
	t.constructRecord("A", "Function", "Abc.cpp", "Global");
	t.constructRecord("C", "Class", "As.cpp", "Scanner");
	t.showTypeTable();
	return 0;
}
#endif

