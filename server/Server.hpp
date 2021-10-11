/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: krios-fu <krios-fu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/11 14:23:18 by krios-fu          #+#    #+#             */
/*   Updated: 2021/10/11 19:17:32 by krios-fu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __SERVER__HPP
# define __SERVER__HPP

#include <string>
# include <ctype.h>
# include <sys/time.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <fcntl.h>
# include <map>
# include <iostream>
# include <exception>
#include <unistd.h>

#define PORT 6667

class Server
{
	private:
		std::map< size_t , int >	_list_connected_users;
		struct sockaddr_in 			_addr_server;
		struct timeval				_time_out;
		fd_set						_socks;
		int							_num_read_sock;
		int							_listen_server_sock;
		int							_highsock;

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
		void			getCustomerRequest( int const & id_client );
		
		void			setNumReadSock(void );
		int		const & getNumReadSock( void ) const ;
		int		const & getListenSockServer()	const;
		int		const & getHigthSock ()			const;
		fd_set	const & getSocks()				const;
};

#endif 