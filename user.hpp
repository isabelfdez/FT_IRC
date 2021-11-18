/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isfernan <isfernan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/07 19:43:31 by isfernan          #+#    #+#             */
/*   Updated: 2021/11/18 18:41:02 by isfernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
# define USER_HPP

# include <string>
# include <sys/socket.h>
# include <netinet/in.h>
# include "channel.hpp"
#include <arpa/inet.h>
#include <deque>

# define MAX_CHANNELS 10

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

class User
{
	private:
		std::list<Channel *>		_channels;
		std::string					_username;
		std::string					_nick;
		std::string					_realName;
		std::string					_ping;
		std::string					_buffer_cmd;
		std::string					_mask;
		std::deque<std::string>		_answer;
		user_modes					_modes;
		int							_sock_fd;
		bool						_is_registered;
		bool						_max_channels;
		bool						_ping_status;
		bool						_is_send_msg;
		uint64_t					_lastTime;
		uint64_t					_time_ping;
		bool						_passState;
		

		struct sockaddr_in 			_addr;

	public:
		// Constructor && destructor
		User(int & fd, struct sockaddr_in const & addr);
		~User();

		// Getters
		std::string					getUserName() const;
		std::string	const &			getNick() const;
		bool						getmode(char mode) const;
		int							getsockfd()	const;
		bool						getRegistered() const;
		bool						getMaxChannels() const;
		std::string const &         getRealName() const;
		std::list< Channel *> &		getChannels();
		std::string					getChannelsString();
		std::string const &			getPing() const ;
		uint64_t	const &			getLastTime() const;
		uint64_t	const &			getTimePing() const;
		bool const &				getPingStatus() const;
		std::string					getIp(  ) const ;
		std::string	const &			getBufferCmd() const;
		bool const &				getIsSendMsg() const;
		std::string 				getAnswer();
		bool						getPassState()	const;
		std::string const & 		getMask() const;
		

		// Setters
		void						setUserName(std::string name);
		void						setNick(std::string nick);
		void						setmode(char mode, bool state);
		void						setRegistered(bool status);
		void                        setRealName(std::string const & realName);
		void 						setPing( std::string const & ping );
		void						setLastTime( uint64_t const & time );
		void						setPingStatus( bool const &  status );
		void						setTimePing( uint64_t const & time );
		void						setBufferCmd( std::string const & buffer_cmd );
		void						setIsSendMsg(bool const & status);
		void						setAnswer(std::string const & answer);
		void						setPassState(bool state);
		void						setMask( void );
		
		// Overload
		bool						operator==(User & obj);

		// Other functions
		std::string					showModes();
		void						init_modes();
		void						deleteChannel(Channel * channel);
		void						addChannel(Channel * channel);
};

#endif