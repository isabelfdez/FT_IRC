/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: krios-fu <krios-fu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/11 00:00:50 by krios-fu          #+#    #+#             */
/*   Updated: 2021/11/13 03:33:36 by krios-fu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
# define USER_HPP

# include <string>
# include <iostream>
# include <map>
# include <deque>

class Game;

class User
{
	std::string							_nick;
	std::string							_buffer_cmd;
	std::deque<std::string>				_answer;
	std::map<std::string, std::string> 	_table;
	public:

	User( std::string const & _nick );
	virtual ~User();

	std::string const &					getNick() const;
	std::string const &					getBufferCmd() const;
	std::map<std::string, std::string> const &	getTabla() const;

	void					setBufferCmd( std::string const & buffer_cmd );
	bool					setTabla(std::string const & coo );
	void					setAnswer(std::string const & answer);
	std::string 			getAnswer();
	void					setTableBot();
	bool					defendHorizontally();
	bool					defendVertically();

	

};


#endif 


