/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isfernan <isfernan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/11 00:10:18 by krios-fu          #+#    #+#             */
/*   Updated: 2021/11/18 18:42:14 by isfernan         ###   ########.fr       */
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

	this->_win = false;
	this->_lose = false;
	this->_tie  =false;
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

bool	User::defendDiagonal(std::string target)
{
	std::string lvl[3][3] = 
	{ 
		{"1,1", "1,2", "1,3"},
		{"2,1", "2,2", "2,3"},
		{"3,1", "3,2", "3,3"}
	};

	int lvl3[3][3] = 
	{
		{1, 2, 3},
		{4, 5, 6},
		{7, 8, 9},
	};

	int data = 0;
	
	if ( this->_table[  lvl[0][0]  ] == target )
		data += lvl3[0][0];
	if ( this->_table[  lvl[1][1]  ] == target )
		data += lvl3[1][1];
	if ( this->_table[  lvl[2][2]  ] == target )
		data += lvl3[2][2];
	
	if ( data == 6 )
		if (this->_table[lvl[2][2] ] != "❌" &&  this->_table[lvl[2][2] ] != "⭕️" )
		{
			this->_table[  lvl[2] [2] ] =  "⭕️";
			if (target == "⭕️" )
				return gameOver(target);
			return true;
		}

	if ( data == 14)
		if (this->_table[ lvl[0][0] ] != "❌" &&  this->_table[ lvl[0][0] ] != "⭕️" )
		{
			this->_table[  lvl[0] [0] ] =  "⭕️";
			if (target == "⭕️" )
				return gameOver(target);
			return true;
		}
	if ( data == 10)
		if (this->_table[lvl[1][1] ] != "❌" &&  this->_table[ lvl[1][1] ] != "⭕️" )
		{
			this->_table[  lvl[1] [1] ] =  "⭕️";
			if (target == "⭕️" )
				return gameOver(target);
			return true;
		}
	if( data == 15 )
		return gameOver(target);

	data = 0;

	if ( this->_table[  lvl[0][2]  ] == target )
		data += lvl3[0][2];
	if ( this->_table[  lvl[1][1]  ] == target )
		data += lvl3[1][1];
	if ( this->_table[  lvl[2][0]  ] == target )
		data += lvl3[2][0];
	
	if ( data == 8 )
		if (this->_table[lvl[2][0] ] != "❌" &&  this->_table[ lvl[2][0] ] != "⭕️" )
		{
			this->_table[  lvl[2] [0] ] =  "⭕️";
			if (target == "⭕️" )
				return gameOver(target);
			return true;
		}

	if ( data == 12)
		if (this->_table[ lvl[0][2] ] != "❌" &&  this->_table[ lvl[0][2] ] != "⭕️" )
		{
			this->_table[  lvl[0] [2] ] =  "⭕️";
			if (target == "⭕️" )
				return gameOver(target);
			return true;
		}
	if ( data == 10)
		if (this->_table[lvl[1][1] ] != "❌" &&  this->_table[ lvl[1][1] ] != "⭕️" )
		{
			this->_table[  lvl[1] [1] ] =  "⭕️";
			if (target == "⭕️" )
				return gameOver(target);
			return true;
		}
	if( data == 15)
		return gameOver(target);

	if (data == 5 && this->_table[ lvl[1][1] ] == "❌" )
		if (this->_table[lvl[0][2] ] != "❌" &&  this->_table[ lvl[0][2] ] != "⭕️" )
			{
				this->_table[  lvl[0] [2] ] =  "⭕️";
				return true;
			}
	return false;
}


bool User::defendHorizontally(std::string target)
{
	std::string lvl[3][3] = 
	{ 
		{"1,1", "1,2", "1,3"},
		{"2,1", "2,2", "2,3"},
		{"3,1", "3,2", "3,3"}
	};

	int lvl3[3][3] = 
	{
		{1, 2, 3},
		{4, 5, 6},
		{7, 8, 9},
	}; 
	
	int data = 0;
	int flag = 0;

	for( size_t i = 0; i < 3 ; i++)
	{
		for (size_t j = 0; j < 3 ; j ++ )
		{
			if ( this->_table[  lvl[i][j]  ] == target )
			{	
				flag++;
				data += lvl3[i][j];

				if ( ((data + lvl3[i][0]) == 6 || (data + lvl3[i][0]) == 15 || (data + lvl3[i][0]) == 24) && flag == 2 )
				{

					if (this->_table[lvl[i][0] ] != "❌" &&  this->_table[lvl[i][0] ] != "⭕️" )
					{
						this->_table[  lvl[i][0]  ] =  "⭕️";
						if (target == "⭕️" )
							return gameOver(target);
						return true;
					}
				}
				else if ( ((data + lvl3[i][1]) == 6 || (data + lvl3[i][1]) == 15 || (data + lvl3[i][1]) == 24) && flag == 2 )
				{

					if (this->_table[lvl[i][1]] !="❌" &&  this->_table[lvl[i][1] ] != "⭕️" )
					{
						this->_table[  lvl[i][1]  ] =  "⭕️";
						if (target == "⭕️" )
							return gameOver(target);
						return true;
					}
				}
				else if ( ((data + lvl3[i][2]) == 6 || (data + lvl3[i][2]) == 15 ||  (data + lvl3[i][2]) == 24) && flag == 2)
				{

					if (this->_table[lvl[i][2] ] != "❌" &&  this->_table[lvl[i][2] ] != "⭕️" )
					{
						this->_table[  lvl[i][2]  ] =  "⭕️";
						if (target == "⭕️" )
							return gameOver(target);
						return true;
					}
				}
				if ( flag == 3 ) 
					return gameOver(target);
			}
		}
		flag = 0;
		data =0;
	}
	return false ;
}

bool User::check()
{
	std::string lvl[3][3] = 
	{ 
		{"1,1", "1,2", "1,3"},
		{"2,1", "2,2", "2,3"},
		{"3,1", "3,2", "3,3"}
	};
	
	int over = 0;

	for( size_t i = 0; i < 3 ; i++)
	{
		for (size_t j = 0; j < 3 ; j ++ )
		{
			if (this->_table[  lvl[j][i]  ] == "❌")
				over++;
			if (over == 3)
				return gameOver("❌");
			
		}
		over = 0;
	}
	over = 0;
	for( size_t i = 0; i < 3 ; i++)
	{
		for (size_t j = 0; j < 3 ; j ++ )
		{
			if (this->_table[  lvl[i][j]  ] == "❌")
				over++;
			if (over == 3)
				return gameOver("❌");
			
		}
		over = 0;
	}
	return false;
	
}

bool User::defendVertically(std::string target)
{
	std::string lvl[3][3] = 
	{ 
		{"1,1", "1,2", "1,3"},
		{"2,1", "2,2", "2,3"},
		{"3,1", "3,2", "3,3"}
	};

	int lvl3[3][3] = 
	{
		{1, 2, 3},
		{4, 5, 6},
		{7, 8, 9},
	}; 
	
	int data = 0;
	int flag = 0;


	for( size_t i = 0; i < 3 ; i++)
	{
		for (size_t j = 0; j < 3 ; j ++ )
		{

			if ( this->_table[  lvl[j][i]  ] == target )
			{
				flag++;

				data += lvl3[j][i];
				if ( ((data + lvl3[0][i]) == 12 || (data + lvl3[0][i]) == 15 || (data + lvl3[0][i]) == 18) && flag == 2 )
				{
					if (this->_table[lvl[0][i] ] != "❌" &&  this->_table[lvl[0][i] ] != "⭕️" )
					{
						this->_table[  lvl[0] [i] ] =  "⭕️";

						if (target == "⭕️" )
							return gameOver(target);
						return true;
					}
				}
				if ( ((data + lvl3[1][i]) == 12 || (data + lvl3[1][i]) == 15 || (data + lvl3[1][i]) == 18) && flag == 2  )
				{
					if (this->_table[lvl[1][i]] != "❌" &&  this->_table[lvl[1][i] ] != "⭕️" )
					{
						this->_table[  lvl[1][i]  ] =  "⭕️";
						if (target == "⭕️" )
							return gameOver(target);
						return true;
					}
				}
				if ( ((data + lvl3[2][i]) == 12 || (data + lvl3[2][i]) == 15 || (data + lvl3[2][i]) == 18) && flag == 2 )
				{
					if (this->_table[lvl[2][i] ] != "❌" &&   this->_table[lvl[2][i] ] != "⭕️" )
					{
						this->_table[  lvl[2][i]  ] =  "⭕️";
						if (target == "⭕️" )
							return gameOver(target);
						return true;
					}
				}
				if ( flag == 3 ) 
					return gameOver( target);

			}
		}
		flag = 0;
		data = 0;
	}
	return false ;
}

bool User::gameOver( std::string target)
{
	if ( target == "⭕️" )
		this->_lose = true;
	else
		this->_win = true;

	return true;
}

bool	User::getWin()
{
	return this->_win;
}
bool	User::getLose()
{
	return this->_lose;
}

bool	User::tie()
{
	std::string lvl[3][3] = 
	{ 
		{"1,1", "1,2", "1,3"},
		{"2,1", "2,2", "2,3"},
		{"3,1", "3,2", "3,3"}
	};

	for (size_t i = 0; i < 3; i++)
		for (size_t j = 0; j < 3; j++)
			if (this->_table[lvl[i][j] ] != "❌" &&  this->_table[lvl[i][j] ] != "⭕️")
				return false;
	this->_tie = true;
	return true;
}

void	User::setTableBot()
{
	size_t x,y;
	std::string coo;
	if (this->check())
		return ;

	if(this->defendDiagonal("⭕️"))
		return ;
	else if(this->defendHorizontally("⭕️"))
		return ;
	else if(this->defendVertically("⭕️"))
		return ;
	else if(this->defendDiagonal("❌"))
		return ;
	else if(this->defendHorizontally("❌"))
		return ;
	else if(this->defendVertically("❌"))
		return ;
	else if (this->tie())
		return ;
	
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
	if (this->tie())
		return ;
}


bool User::getTie()
{
	return this->_tie;
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