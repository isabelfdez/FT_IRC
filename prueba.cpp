

#include <string>
#include <iostream>
#include <iomanip>

template <unsigned int n>
struct factorial {
	enum 
	{
		value = n * factorial<n - 1>::value
	};
};

template <>
struct factorial<0> {
	enum 
	{
		 value = 1 
	};
};

int main ()
{

	std::cout << factorial<5>::value << std::endl; 


}