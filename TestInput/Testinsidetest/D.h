#include "C.h"
#include"A.h"

struct DS 
{
   Token _tok; // declared in C.h
   int id;
}; 

class D{
public:
	D();
	~D();
	void methodDC();
private:
	struct DC d;
};