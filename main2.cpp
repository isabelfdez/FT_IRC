#include <vector>
#include <iostream>
#include <string>


template<class T> 
typename std::string::iterator delimiterAdvance(T begin, T end, char c)
{
	for( ; *begin == c && begin != end; begin++ );

	return ( begin );
}

std::vector<std::string> split(std::string buffer, char c)
{
	size_t						pos;
	std::string					tmp;
	std::vector<std::string>	matrix;

	std::string::iterator begin = buffer.begin();
	std::string::iterator end = buffer.end();

	buffer = &( *delimiterAdvance<std::string::iterator>(begin, end, c) );
	while ( (pos = buffer.find(c) ) != std::string::npos )
	{
		tmp = buffer.substr(0, pos);
		if ( tmp.length() > 0)
			matrix.push_back(tmp);
		buffer.erase(0, pos);
		buffer = &( *delimiterAdvance<std::string::iterator>(begin, end, c) );
	}
	return matrix;
}



int main()
{
	std::vector<std::string> matrix = split("hola;mundo;colombia;jejeje;lol;a", ';');

	std::vector<std::string>::iterator it = matrix.begin();

	for (; it != matrix.end(); it++)
	{
		std::cout << *it <<std::endl ;
	}

	return 0;
}