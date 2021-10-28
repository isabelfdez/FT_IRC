

#include <string>
#include <iostream>
#include <iomanip>

void displayLog( std::string const & log )
{
	int len = 15;
	std::cout << std::setfill('.')  << std::setw(30) << log.substr(0,25)  ;
	std::cout  << " IP: " <<  log.length();
	std::cout << " Socket: " << std::endl;
}


int main ()
{
	std::string prueba = "hola mundo colombia madrid";

	displayLog(prueba);

	std::string prueba2 = "hola mundo";
	displayLog(prueba2);


}