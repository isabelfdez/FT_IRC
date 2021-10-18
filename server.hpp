#ifndef __SERVER__HPP
# define __SERVER__HPP

# include <string>
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
# include "user.hpp"


# define PORT 6667

// Definiciones de errores

# define ERR_NONICKNAMEGIVEN		"431"
# define ERR_ERRONEUSNICKNAME		"432"
# define ERR_NICKNAMEINUSE			"433"
# define ERR_NOTREGISTERED			"451"
# define ERR_NEEDMOREPARAMS         "461"
# define ERR_ALREADYREGISTRED		"462"



class Server
{
	private:
		// std::map< size_t , int >	_list_connected_users;
		int								_list_connected_user[FD_SETSIZE];
		struct sockaddr_in 				_addr_server;
		struct timeval					_time_out;
		fd_set							_reads;

		int								_num_read_sock;
		int								_listen_server_sock;
		int								_highsock;
		std::map<int, User*>			_fd_users;
		std::list<std::string>			_commands;
		std::list<std::string>			_nicks;
		std::list<User *>				_connected_users;

	public:
		Server();
		~Server();

		
		class GlobalServerExecption : public std::exception
		{
			public:
				virtual const char* what() const throw ();
		};

		void			build_select_list();
		void			attendClients();
		void			join_new_connection();
		void			getCustomerRequest( int & id_client, int i);
		void			replyCustomerRequest( int & id_client, int i);
		void			parse_command(int fd, std::string buff, char * str);

		
		void			setNumReadSock(void );
		int		const & getNumReadSock( void ) const ;
		int		const & getListenSockServer()	const;
		int		const & getHigthSock ()			const;
		fd_set	const & getSocks()				const;


		// Comandos
		void			nick_command(char * str, int & fd);
		void			privmsg_command(std::string & command, int & fd);
		void			user_command( int fd, char *buffer );

};

#endif 