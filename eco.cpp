#include <string>
#include <iostream>
#include <vector>

template<class T>
static typename std::string::iterator delimiterAdvance(T begin, T end, char c)
{
	for( ; *begin == c && begin != end; begin++ );
	return ( begin );
}

std::vector<std::string> split( std::string buffer, char c )
{
	std::vector<std::string>	matrix;
	size_t						pos;
	std::string					tmp;

	while ( ( pos = buffer.find(c) ) != std::string::npos || buffer.length() > 0 )
	{
		tmp = buffer.substr(0, pos);
		if ( tmp.length() > 0 )
			matrix.push_back(tmp);
		buffer.erase(0, pos);
		buffer = &( *delimiterAdvance<std::string::iterator>(buffer.begin(), buffer.end(), c) );
		if ( pos == std::string::npos )
			break ;
	}
	
	return matrix;
}

int main ()
{
	std::vector<std::string> tmp = split("hola,,,mundo,,,,madrid,,,    ", ',');

	for (size_t i = 0; i < tmp.size(); i++)
	{
		std::cout << tmp[i] << std::endl;
	}
	
}