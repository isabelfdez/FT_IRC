/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: krios-fu <krios-fu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/22 16:35:07 by krios-fu          #+#    #+#             */
/*   Updated: 2021/11/11 13:33:05 by krios-fu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"

template<class T>
static typename std::string::iterator delimiterAdvance(T begin, T end, char c)
{
	for( ; *begin == c && begin != end; begin++ );
	return ( begin );
}

std::vector<std::string> parser( std::string buffer )
{
	std::vector<std::string>	matrix;
	size_t						pos;
	std::string					trailing;
	std::string					middle;
	std::string					tmp;

	
	if ( (pos = buffer.find('\r')  ) != std::string::npos || (pos = buffer.find('\n')  ) != std::string::npos )
		buffer[ pos ] = '\0';
	buffer  =  &(*delimiterAdvance<std::string::iterator>(buffer.begin(), buffer.end(), ' '));
	if ( buffer[0] == ':' )
		if ( (pos  = buffer.find(' ')) != std::string::npos )
			buffer = buffer.substr( pos, ( buffer.length() - pos ) );

	if ( (pos = buffer.find(':')) != std::string::npos )
	{
		middle = buffer.substr(0, pos);
		trailing = buffer.substr(pos + 1, buffer.length() - pos );

		if ( buffer.length() == pos ) { trailing = "";} 
	}
	else
		middle = buffer;

	while ( ( pos = middle.find(' ') ) != std::string::npos || middle.length() > 0 )
	{
		tmp = middle.substr(0, pos);
		if ( tmp.length() > 0 )
			matrix.push_back(tmp);
		middle.erase(0, pos);
		middle = &( *delimiterAdvance<std::string::iterator>(middle.begin(), middle.end(), ' ') );
		if ( pos == std::string::npos && middle.begin() != middle.end())
			break ;
	}
	
	if ( trailing.length() )
		matrix.push_back(trailing);
	return matrix;
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

