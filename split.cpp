/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: krios-fu <krios-fu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/22 16:35:07 by krios-fu          #+#    #+#             */
/*   Updated: 2021/11/05 18:02:14 by krios-fu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"

template<class T>
static typename std::string::iterator delimiterAdvance(T begin, T end, char c)
{
	for( ; *begin == c && begin != end; begin++ );
	return ( begin );
}

std::vector<std::string> split(char * buff, char c)
{
	size_t						pos;
	std::string					tmp;
	std::vector<std::string>	matrix;
	char *						tmp2;
	if ( *buff == 0 )
		return matrix;
	if ( ( tmp2 = strchr( buff, '\r' ) ) || ( tmp2 = strchr( buff, '\n' ) ) )
	 	*tmp2 = 0;
	std::string buffer( buff );
	std::string::iterator begin = buffer.begin();
	std::string::iterator end = buffer.end();
	buffer = &( *delimiterAdvance<std::string::iterator>( begin, end, c ) );
	while ( ( pos = buffer.find(c) ) != std::string::npos || buffer.length() > 0 )
	{
		tmp = buffer.substr(0, pos);
		if ( tmp.length() > 0 )
			matrix.push_back(tmp);
		buffer.erase(0, pos);
		buffer = &( *delimiterAdvance<std::string::iterator>(begin, end, c) );
		if ( pos == std::string::npos )
			break ;
	}
	return matrix;
}

