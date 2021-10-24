
#include <string>
#include <iostream>

std::string generatePing()
{
	char strrnd[13];
    srand(time(NULL));
    for(int i=0; i <= 11; i++)
        strrnd[i] = 33 + rand() % (126 - 33);
	strrnd[12] = '\0';
	return static_cast<std::string>(strrnd);
}



int main()
{
	std::cout << generatePing() << std::endl;
}