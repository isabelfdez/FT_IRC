/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: krios-fu <krios-fu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/11 00:10:18 by krios-fu          #+#    #+#             */
/*   Updated: 2021/11/13 05:43:39 by krios-fu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"

User::User( std::string const & nick ) 
	: _nick(nick)
{
	this->_buffer_cmd = "";
	
	this->_table["1,1"] = "💭";
	this->_table["1,2"] = "💭";
	this->_table["1,3"] = "💭";
	this->_table["2,1"] = "💭";
	this->_table["2,2"] = "💭";
	this->_table["2,3"] = "💭";
	this->_table["3,1"] = "💭";
	this->_table["3,2"] = "💭";
	this->_table["3,3"] = "💭";
}

User::~User()
{

}


std::string const & User::getBufferCmd() const { return this->_buffer_cmd; }

void	User::setBufferCmd( std::string const & buffer_cmd )
{
	this->_buffer_cmd = buffer_cmd;
}

std::map<std::string, std::string> const & User::getTabla () const { return this->_table; }

std::string const & User::getNick() const
{
	return this->_nick;
}

void	User::setAnswer(std::string const & answer)
{

	this->_answer.push_back( answer );
}

bool User::setTabla(std::string const & coo )
{
	size_t pos;
	std::string tmp_coo = coo;
	if ( (pos = tmp_coo .find('\r')  ) != std::string::npos
		|| (pos = tmp_coo .find('\n')  ) != std::string::npos )
			tmp_coo = tmp_coo.substr(0, pos );

	if ( (pos = tmp_coo .find(":") ) != std::string::npos )
		tmp_coo = &tmp_coo [1];

	if ( this->_table.count(tmp_coo) == 0 )
		return false;
	if ( this->_table[tmp_coo] != "❌" && this->_table[tmp_coo] != "⭕️" )
		this->_table[tmp_coo] = "❌";
	else
		return false;
	return true;

}

std::string getPos(size_t n)
{
	 switch (n)
	 {
		case 1:
			return "1";
		case 2:
			return "2";
		case 3:
			return "3";
		default:
			break;
	 }
	 return "" ;
}

bool User::defendHorizontally()
{
	std::string lvl[3][3] = 
	{ 
		"1,1", "1,2", "1,3",
		"2,1", "2,2", "2,3",
		"3,1", "3,2", "3,3"
	};

	int lvl3[3][3] = 
	{
		1, 2, 3,
		4, 5, 6,
		7, 8, 9,
	}; 
	
	int data = 0;
	int flag = 0;

	for( size_t i = 0; i < 3 ; i++)
	{
		for (size_t j = 0; j < 3 ; j ++ )
		{
			if ( this->_table[  lvl[i][j]  ] == "❌" )
			{	
				flag++;
				data += lvl3[i][j];
				std::cout << "[[ hor "<< data << "]]" << std::endl;

				if ( ((data + lvl3[i][0]) == 6 || (data + lvl3[i][0]) == 15 || (data + lvl3[i][0]) == 24) && flag == 2 )
				{
						std::cout << "[[ a "<< i << "]]" << std::endl;

					if (this->_table[lvl[i][0] ] == "❌" ||  this->_table[lvl[i][0] ] == "⭕️" )
						return false;
					this->_table[  lvl[i][0]  ] =  "⭕️";
					return true;
				}
				else if ( ((data + lvl3[i][1]) == 6 || (data + lvl3[i][1]) == 15 || (data + lvl3[i][1]) == 24) && flag == 2 )
				{
						std::cout << "[[ b "<< i << "]]" << std::endl;

					if (this->_table[lvl[i][1]] == "❌" ||  this->_table[lvl[i][1] ] == "⭕️" )
						return false;
					this->_table[  lvl[i][1]  ] =  "⭕️";
					return true;
				}
				else if ( ((data + lvl3[i][2]) == 6 || (data + lvl3[i][2]) == 15 || (data + lvl3[i][2]) == 24) && flag == 2)
				{
						std::cout << "[[ c "<< i << "]]" << std::endl;

					if (this->_table[lvl[i][2] ] == "❌" ||  this->_table[lvl[i][2] ] == "⭕️" )
						return false;
					this->_table[  lvl[i][2]  ] =  "⭕️";
					return true;
				}
				
			}
		}
		flag = 0;
		data =0;
	}
	return false ;
}

bool User::defendVertically()
{
	std::string lvl[3][3] = 
	{ 
		"1,1", "1,2", "1,3",
		"2,1", "2,2", "2,3",
		"3,1", "3,2", "3,3"
	};

	int lvl3[3][3] = 
	{
		1, 2, 3,
		4, 5, 6,
		7, 8, 9,
	}; 
	
	int data = 0;
	int flag = 0;


	for( size_t i = 0; i < 3 ; i++)
	{
		for (size_t j = 0; j < 3 ; j ++ )
		{
			if ( this->_table[  lvl[j][i]  ] == "❌" )
			{
				flag++;

				data += lvl3[j][i];
				std::cout << "[[ ver "<< data << " i : " << i << " f : " << flag << "]]" << std::endl;
				if ( ((data + lvl3[0][i]) == 12 || (data + lvl3[0][i]) == 15 || (data + lvl3[0][i]) == 18) && flag == 2 )
				{
						std::cout << "[[ *** a"<< i << "]]" << std::endl;

					if (this->_table[lvl[0][i] ] != "❌" &&  this->_table[lvl[0][i] ] != "⭕️" )
					{
						this->_table[  lvl[0] [i] ] =  "⭕️";
						return true;
					}
				}
				if ( ((data + lvl3[1][i]) == 12 || (data + lvl3[1][i]) == 15 || (data + lvl3[1][i]) == 18) && flag == 2  )
				{
						std::cout << "[[ *** b"<< i << "]]" << std::endl;

					if (this->_table[lvl[1][i]] != "❌" &&  this->_table[lvl[1][i] ] != "⭕️" )
					{
						this->_table[  lvl[1][i]  ] =  "⭕️";
						return true;
					}
				}
				if ( ((data + lvl3[2][i]) == 12 || (data + lvl3[2][i]) == 15 || (data + lvl3[2][i]) == 18) && flag == 2 )
				{
						std::cout << "[[ *** c"<< i << "]]" << std::endl;

					if (this->_table[lvl[2][i] ] != "❌" &&   this->_table[lvl[2][i] ] != "⭕️" )
					{
						this->_table[  lvl[2][i]  ] =  "⭕️";
						return true;
					}
				}
				
			}
		}
		flag = 0;
		data = 0;
	}
	return false ;
}

void	User::setTableBot()
{
	size_t x,y;
	std::string coo;
	if(this->defendHorizontally())
	{
		std::cout << "horlol" << std::endl;
		return ;
	}
	else if(this->defendVertically())
	{
		std::cout << "Verlol" << std::endl;
		
		return ;
	}
	
	srand( time( NULL ) );
	for (;;)
	{
		x = 1 + rand() % ( 4 - 1);
		y = 1 + rand() % ( 4 - 1);

		coo.append(getPos(x));
		coo.append(",");
		coo.append(getPos(y));
		if ( this->_table.count(coo) != 0 && this->_table[coo] != "❌" && this->_table[coo] != "⭕️" )
		{
			this->_table[coo] = "⭕️";
			break ;
		}
		coo.clear();
	}
	std::cout <<  getPos(x) << " " << getPos(y)<< std::endl;

	// shee

	// while ()
}


std::string User::getAnswer()
{
	std::string tmp;

	if ( this->_answer.size() > 0)
	{
		tmp = this->_answer[0];
		this->_answer.pop_front();
	}
	
	return tmp;
}