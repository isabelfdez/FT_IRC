/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main42.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isfernan <isfernan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/08 16:46:19 by isfernan          #+#    #+#             */
/*   Updated: 2021/10/08 19:25:06 by isfernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "user.hpp"
#include "channel.hpp"
#include "server.hpp"

void setnonblocking(int sock)
{
    int opts;
    opts = fcntl(sock, F_GETFL);
    if (opts < 0) {
        perror("fcntl(F_GETFL)");
        exit(EXIT_FAILURE);
    }
    opts = (opts | O_NONBLOCK);
    if (fcntl(sock, F_SETFL, opts) < 0) {
        perror("fcntl(F_SETFL)");
        exit(EXIT_FAILURE);
    }
    return;
}

void build_select_list(Server & serv) 
{
    int k;
    
    FD_ZERO(&serv.getSocks());
    
    FD_SET(serv.getSock(),&serv.getSocks());
	
    for (k = 0; k < 5; k++) {
        if (serv.getfds(k) != 0) {
            FD_SET(serv.getfds(k), &serv.getSocks());
            if (serv.getfds(k) > serv.getHighSock())
                serv.setHighSock(serv.getfds(k));
        }
    }
}

void handle_new_connection(Server & serv) {
    int i;
    int connection;
    connection = accept(serv.getSock(), NULL, NULL);
    if (connection < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    setnonblocking(connection);
    for (i = 0; (i < 5) && (connection != -1); i ++)
        if (serv.getfds(i) == 0) {
            printf("\nConnection accepted:   FD=%d; Slot=%d\n",
                connection,i);
            serv.getfds(i) = connection;
            connection = -1;
    		//listen(serv.getfds(i), 1);
        }
    if (connection != -1) {
        printf("\nNo room left for new client.\n");
        close(connection);
    }
}

void deal_with_data(int j, Server & serv)
{
    char buffer[1024] = {0};
    //long valread = 
	read( serv.getfds(j) , buffer, 1024);
    //if (!valread)
    //    return ;
    printf("|%s|\n", buffer);
    printf("j = %i\n", j);
    
    if (!strncmp(buffer, "close", 5))
    {
        printf("El buffer con el que ha entrado es |%s|\n", buffer);
        close(serv.getfds(j));
        serv.getfds(j) = 0;
    }
}

void read_socks(Server & serv) {
    int j;
	printf("aqui entra\n");
    if (FD_ISSET(serv.getSock(),&serv.getSocks()))
        handle_new_connection(serv);
    for (j = 0; j < 5; j++) {
        if (FD_ISSET(serv.getfds(j),&serv.getSocks()))
            deal_with_data(j, serv);
    }
}

int main()
{
	int reuse_addr = 1;
	struct timeval timeout;
    int readsocks;
	Server serv(6667);

	serv.setSock(socket(AF_INET, SOCK_STREAM, 0));
	setsockopt(serv.getSock(), SOL_SOCKET, SO_REUSEADDR, &reuse_addr, sizeof(reuse_addr));
	// SOL_SOCKET nos permite usar las opciones, y la opción SO_REUSEADDR permite tener dos socket en el mismo puerto
    //setnonblocking(serv.getSock());
	if (bind(serv.getSock(), (struct sockaddr *) &serv.getStruct(),
      sizeof(serv.getStruct())) < 0 ) {
        perror("bind");
        close(serv.getSock());
        exit(EXIT_FAILURE);
    }
    listen(serv.getSock(), MAXFD);
    serv.setHighSock(serv.getSock());
	while (1) 
	{
        build_select_list(serv);
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;
        readsocks = select(serv.getHighSock()+1, &serv.getSocks(), (fd_set *) 0, 
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