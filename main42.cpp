/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main42.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isfernan <isfernan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/08 16:46:19 by isfernan          #+#    #+#             */
/*   Updated: 2021/10/08 17:11:56 by isfernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "user.hpp"
#include "channel.hpp"
#include "server.hpp"

void setnonblocking(int sock)
{
    int opts;
    opts = fcntl(sock,F_GETFL);
    if (opts < 0) {
        perror("fcntl(F_GETFL)");
        exit(EXIT_FAILURE);
    }
    opts = (opts | O_NONBLOCK);
    if (fcntl(sock,F_SETFL,opts) < 0) {
        perror("fcntl(F_SETFL)");
        exit(EXIT_FAILURE);
    }
    return;
}

void build_select_list(Server & serv) 
{
    int k;
    
    FD_ZERO(&serv._socks);
    
    FD_SET(serv._sock,&serv._socks);
	
    for (k = 0; k < 5; k++) {
        if (serv._connectList[k] != 0) {
            FD_SET(serv._connectList[k],&serv._socks);
            if (serv._connectList[k] > serv._highSock)
                serv._highSock = serv._connectList[k];
        }
    }
}

void handle_new_connection(Server & serv) {
    int i;
    int connection;
    connection = accept(serv._sock, NULL, NULL);
    if (connection < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    setnonblocking(connection);
    for (i = 0; (i < 5) && (connection != -1); i ++)
        if (serv._connectList[i] == 0) {
            printf("\nConnection accepted:   FD=%d; Slot=%d\n",
                connection,i);
            serv._connectList[i] = connection;
            connection = -1;
        }
    if (connection != -1) {
        printf("\nNo room left for new client.\n");
        close(connection);
    }
}

void deal_with_data(int listnum, Server & serv)
{
    char buffer[1024] = {0};
    long valread = read( serv._connectList[listnum] , buffer, 1024);
    if (!valread)
        return ;
    printf("|%s|\n", buffer);
    printf("listnum = %i\n", listnum);
    
    if (!strncmp(buffer, "close", 5))
    {
        printf("El buffer con el que ha entrado es |%s|\n", buffer);
        close(serv._connectList[listnum]);
        serv._connectList[listnum] = 0;
    }
}

void read_socks(Server & serv) {
    int j;
    if (FD_ISSET(serv._sock,&serv._socks))
        handle_new_connection(serv);
    for (j = 0; j < 5; j++) {
        if (FD_ISSET(serv._connectList[j],&serv._socks))
            deal_with_data(j, serv);
    }
}

int main()
{
	int reuse_addr = 1;
	struct timeval timeout;
    int readsocks;
	Server serv(6667);

	serv._sock = socket(AF_INET, SOCK_STREAM, 0);
	setsockopt(serv._sock, SOL_SOCKET, SO_REUSEADDR, &reuse_addr,
        sizeof(reuse_addr));
    setnonblocking(serv._sock);
	if (bind(serv._sock, (struct sockaddr *) &serv._server_address,
      sizeof(serv._server_address)) < 0 ) {
        perror("bind");
        close(serv._sock);
        exit(EXIT_FAILURE);
    }
    listen(serv._sock, MAXFD);
    serv._highSock = serv._sock;
	while (1) 
	{
        build_select_list(serv);
        timeout.tv_sec = 2;
        timeout.tv_usec = 0;
        readsocks = select(serv._highSock+1, &serv._socks, (fd_set *) 0, 
          (fd_set *) 0, &timeout);
        if (readsocks < 0) {
            perror("select");
            exit(EXIT_FAILURE);
        }
        if (readsocks == 0) {
            printf(".");
            fflush(stdout);
        } else
            read_socks(serv);
    }

}