#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <iostream>


#define BUFFER  4092

void irc_send(int sock, std::string data)
{
    printf("client> %s\n", data);
    send(sock, data, strlen(data), 0);
    send(sock, "\r\n", 2, 0);
}

void ident_send(int sock, std::string ident, std::string nick)
{
    std::string data;

    asprintf(&data, "USER %s %s * * *", ident, ident);
    irc_send(sock, data);
    asprintf(&data, "NICK %s", nick);
    irc_send(sock, data);
}

int main(int argc, char **argv)
{
    int sock = 0; long valread;
    struct sockaddr_in serv_addr;
    //std::string hello = "USER gonzalo gonzalo * * *\r\nNICK ElCastigador\r\n";
    char buffer[BUFFER + 1] = {0};
    std::string response, *line;
    pthread_t   console_thread;

    std::string nick, *username, *server_host;
    int server_port;

    if (argc != 5)
    {
        printf("Use: %s nick username host port\n", *argv);
        return -1;
    }

    nick = argv[1];
    username = argv[2];
    server_host = argv[3];
    server_port = atoi(argv[4]);

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }
    
    memset(&serv_addr, '0', sizeof(serv_addr));
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(server_port);
    
    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, server_host, &serv_addr.sin_addr)<=0)
    {
        printf("Invalid address/ Address not supported \n");
        return -1;
    }
    printf("aqui\n");
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }
    ident_send(sock, nick, username);

    while (1)
    {
        line << std::cin;
        send(sock, line, strlen(line), 0);
        send(sock, "\r\n", 2, 0);
        free(line);
    }

    return 0;
}