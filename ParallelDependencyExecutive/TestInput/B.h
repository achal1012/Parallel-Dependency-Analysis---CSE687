void globalFuncB(){
	std::cout<< "Global function of namespace B";		
}
typedef std::vector<std::string> stringvector;
struct BS{
	void printB(){
		std::cout<<"Member function of struct B";
	}
	int b;
	std::string bstring;
};
