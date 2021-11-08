/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: krios-fu <krios-fu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/28 20:24:26 by isfernan          #+#    #+#             */
/*   Updated: 2021/11/08 16:20:48 by krios-fu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __SERVER__HPP
# define __SERVER__HPP

# include <string>
# include <stdio.h>
# include <ctype.h>
# include <sys/time.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <fcntl.h>
# include <map>
# include <list>
# include <iostream>
# include <exception>
# include <unistd.h>
# include "../user.hpp"
# include "../channel.hpp"
# include "../utils.hpp"
# include <deque>


# define PORT 49141


// Definiciones de errores

# define ERR_SINTAX                 "001"
# define ERR_NOPASSWD				"002"
# define ERR_NOTONCHANNEL			"442"
# define ERR_CHANNELISFULL			"471"
# define ERR_TOOMANYCHANNELS		"405"
# define ERR_NOSUCHCHANNEL			"403"
# define ERR_NOSUCHNICK				"401"
# define ERR_TOOMANYTARGETS			"407"
# define ERR_NORECIPIENT			"411"
# define ERR_NOTEXTTOSEND			"412"
# define ERR_UNKNOWNCOMMAND			"421"
# define ERR_NONICKNAMEGIVEN		"431"
# define ERR_ERRONEUSNICKNAME		"432"
# define ERR_NICKNAMEINUSE			"433"
# define ERR_NOTREGISTERED			"451"
# define ERR_NEEDMOREPARAMS			"461"
# define ERR_ALREADYREGISTRED		"462"
# define ERR_NEEDMOREPARAMS			"461"
# define ERR_USERNOTINCHANNEL		"441"
# define ERR_PASSWDMISMATCH			"464"
# define ERR_USERNAMEINVALID		"468"
# define ERR_BANNEDFROMCHAN			"474"
# define ERR_USERONCHANNEL			"443"
# define ERR_INVITEONLYCHAN			"473"
# define ERR_NOPRIVILEGES			"481"
# define ERR_CHANOPRIVSNEEDED		"482"
# define ERR_UMODEUNKNOWNFLAG		"501"
# define ERR_USERSDONTMATCH			"502"

// Definiciones de replies

# define RPL_WELCOME				"001"
# define RPL_YOURHOST				"002"
# define RPL_CREATE					"003"
# define RPL_MYINFO					"004"
# define RPL_UMODEIS				"221"
# define RPL_LIST					"322"
# define RPL_LISTEND				"323"
# define RPL_NOTOPIC				"331"
# define RPL_TOPIC					"332"
# define RPL_INVITING 				"341"
# define RPL_NAMREPLY				"353"
# define RPL_ENDOFNAMES				"366"
# define RPL_MOTD					"372"
# define RPL_MOTDSTART				"375"
# define RPL_ENDOFMOTD				"376"
# define RPL_USERS					"393"
# define RPL_YOUREOPER				"381"


class Server
{
	private:
		// std::map< size_t , int >	_list_connected_users;
		std::string						_password;
		int								_list_connected_user[FD_SETSIZE];
		struct sockaddr_in 				_addr_server;
		struct timeval					_time_out;
		fd_set							_reads;
		fd_set							_writes;
		int								_num_read_sock;
		int								_listen_server_sock;
		int								_highsock;

		std::deque<User *>				_send_message;
		
		std::map<int, User*>			_fd_users;
		std::map<std::string, Channel*>	_name_channel;
		std::list<std::string>			_commands;

		std::list<std::string>			_nicks;
		std::list<User *>				_connected_users;
		std::string						_password_oper;

	public:
		Server(int port);
		~Server();

		
		class GlobalServerExecption : public std::exception
		{
			public:
				virtual const char* what() const throw ();
		};

		void			build_select_list();
		void			attendClients();
		void			join_new_connection();
		void			getCustomerRequest( int  id_client );
		void			replyCustomerRequest( int & id_client, int i);
		void			parse_command(int fd, std::string buff);

		void			setNumReadSock( void );
		void			setPassword(std::string);
		int		const & getNumReadSock( void )	const ;
		int		const & getListenSockServer()	const;
		int		const & getHigthSock ()			const;
		fd_set	const & getSocks()				const;//krios-fu
		size_t			getNumChannel( void ) 	const;//krios-fu
		size_t			getNumConnections ()	const;
		size_t			getNumUser( void )		const;//krios-fu
		std::string		getPassword(void)		const;
		User			*getUserWithNick(std::string);
	
		bool			isAnswerUser( User *usr );
		bool			isUsr(std::string);
		bool			isChannel(std::string);

		void			deleteChannel( std::string );
		void			deleteUser( int const & fd );
		void			deleteBan( User *);
		void			deleteInvite( User *user);

		void			close_fd( int fd );
		void			close_all_fd();

		// Comandos
		void			nick_command    (std::vector<std::string> const& parse, User *usr);
		void			privmsg_command (std::vector<std::string> const& parse, User *usr);
		void			notice_command  (std::vector<std::string> const& parse, User *usr);
		void			user_command    (std::vector<std::string> const& parse, User *usr);
		void			quit_command    (std::vector<std::string> const& parse, User *usr);
		void			join_command    (std::vector<std::string> const& parse, User *usr);
		void			join_channel    (std::vector<std::string> const& parse, User *usr);
		void			part_command    (std::vector<std::string> const& parse, User *usr);
		void			part_channel    (std::vector<std::string> const& parse, User *usr);
		void			pong_command    (std::vector<std::string> const& parse, User *usr);
		void			mode_command    (std::vector<std::string> const& parse, User *usr);
		void			oper_command    (std::vector<std::string> const& parse, User *usr);
		void			kick_command    (std::vector<std::string> const& parse, User *usr);
		void 			list_command    (std::vector<std::string> const& parse, User *usr);
		void			invite_command  (std::vector<std::string> const& parse, User *usr);
		void			topic_command   (std::vector<std::string> const& parse, User *usr);
		void 			names_command   (std::vector<std::string> const& parse, User *usr);
		void			pass_command    (std::vector<std::string> const& parse, User *usr);






		void			sendPing(  );
		void			welcome( int const & fd );
		void			reStartSendMsg();

		void			send_message(std::string & message, int & fd, User * usr);
		void			send_message_channel(std::string & message, User * usr, Channel * chnl);
		void			send_message_channel_block(std::string & message, User * usr, Channel * chnl);
		void			send_error(std::string error, std::string str, User *dest);
		void			send_reply(std::string replay, std::string str, User * usr);
		void			sendRequest(User *user);
		void			deleteDequeUser ( User *user );


};

#endif 