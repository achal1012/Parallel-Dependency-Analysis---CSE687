D::D()
{
	std::cout << "Constructor of DC";
}
D::~D()
{
	std::cout << "Destructor of DC";
}
D::methodDC(){
	std::cout<< _d._tok << " : "<<_d.id;	
}
int main(){
	A a;
	a.printA();
	return 0;
}