A::A(){
	std::cout << "Constructor of A";
}

A::~A(){
	std::cout << "Destructor of A";
}

void A::printA() {
	std::cout<<"Method of A";
}

int main(){
	A a;
	a.printA(); 
	return 0;
}