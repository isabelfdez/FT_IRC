#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 6667

int ft_error(char const *str)
{
    if (str)
        std::cout << str << std::endl;
    return (-1);
}

int main(int argc, char **argv)
{
    struct sockaddr_in address;

    int server_fd, new_socket;
    int addrlen = sizeof(address);
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        return (ft_error("Socket failed"));


    memset((char *)&address, 0, sizeof(address));
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = htonl(INADDR_ANY); 
    address.sin_port = htons(PORT);
    
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
        return (ft_error("Bind failed"));

    if (listen(server_fd, 100) < 0) 
    {
        std::cout << "In listen\n";
        exit(EXIT_FAILURE);
    }

    while (42)
    {
        std::cout << "***WAITING FOR A CONEXION***\n";

        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0)
        {
            std::cout << "In accept\n";
            exit(EXIT_FAILURE);        
        }
        
    }
}