/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isfernan <isfernan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/07 19:43:31 by isfernan          #+#    #+#             */
/*   Updated: 2021/10/08 16:52:54 by isfernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
# define USER_HPP

# include <string>
# include <sys/socket.h>
# include <netinet/in.h>
# include "Channel.hpp"

//Command: MODE
//   Parameters: <nickname>
//               *( ( "+" / "-" ) *( "i" / "w" / "o" / "O" / "r" ) )
//
//   The user MODE's are typically changes which affect either how the
//   client is seen by others or what 'extra' messages the client is sent.
//
//   A user MODE command MUST only be accepted if both the sender of the
//   message and the nickname given as a parameter are both the same.  If
//   no other parameter is given, then the server will return the current
//   settings for the nick.
//
//      The available modes are as follows:
//
//           a - user is flagged as away;
//           i - marks a users as invisible;
//           w - user receives wallops;
//           r - restricted user connection;
//           o - operator flag;
//           O - local operator flag;
//           s - marks a user for receipt of server notices.

// ESTOS SON LOS UNICOS MODOS QUE SALEN EN EL ULTIMO RFC Y POR TANTO NO HAY QUE
// IMPLEMENTAR MAS

class User;
class Channel;

typedef struct	user_modes
{
	bool	a;
	bool	i;
	bool	w;
	bool	r;
	bool	small_o;
	bool	big_o;
	bool	s;
}				user_modes;

//class IUser
//{
//	public:
//		// Virtual destructor
//		virtual ~IUser() = 0;
//	
//		// Getters
//		virtual std::string			getUserName() const = 0;
//		virtual std::string			getNick() const = 0;
//		virtual bool				getmode(char mode) const = 0;		// Pasamos el caracter del que queremos obtener el mode
//		virtual int					getsockfd()	const = 0;
//		virtual struct sockaddr_in	getstruct()	const = 0;
//
//		// Setters
//		virtual void				setUserName(std::string name) = 0;
//		virtual void				setNick(std::string nick) = 0;
//		virtual void				setmode(char mode, bool state) = 0;		// Pasamos el caracter de mode que quieremos cambiar y el estado al que lo queremos cambiar
//		
//		// Other functions
//		virtual void				init_modes();	
//};

class User
{
	private:
		std::string					_username;
		std::string					_nick;
		user_modes					_modes;
		int							_sock_fd;
		std::list<Channel *>		_channels;
		bool						_is_registered;

	public:
		// Constructor && destructor
		User(int & fd);
		~User();

		// Getters
		std::string					getUserName() const;
		std::string					getNick() const;
		bool						getmode(char mode) const;		// Pasamos el caracter del que queremos obtener el mode
		int							getsockfd()	const;
		bool						getRegistered() const;
		

		// Setters
		void						setUserName(std::string name);
		void						setNick(std::string nick);
		void						setmode(char mode, bool state);
		void						setRegistered(bool status);

		// Overload
		bool						operator==(User & obj);

		// Other functions
		void						init_modes();
		void						deleteChannel(Channel & channel);
		void						addChannel(Channel * channel);
};

#endif