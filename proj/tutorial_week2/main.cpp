#include "MyFirstDriver.h"

int main()
{
	int a=1;
	int* b=&a;
	int& c=a;
	std::shared_ptr<int> d(&a);
	a=2;
	std::cout<<a<<", "<<*b<<", "<<c<<", "<<*d<<std::endl;

	MyFirstDriver<3> driver;
	driver.Initialize();
	driver.Run();

	return 0;
}
