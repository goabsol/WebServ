#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <iostream>
#include <time.h>
#include <string>

#define SOCKET int
void *memeset(void *s, int c, size_t n)
{
    char *p = (char *)s;
    while (n--)
        *p++ = c;
    return s;
}

int main()
{
    struct sockaddr_in johnny; 
    SOCKET server_fd;
    int server_port = 8080;
    ///add error managemement
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0)
    {
        std::cout << "Error opening socket" << std::endl;
        return 1;
    }
    memeset(&johnny, 0, sizeof(johnny));
    johnny.sin_family = AF_INET;
    johnny.sin_port = htons(server_port);
    johnny.sin_addr.s_addr = INADDR_ANY;
    //add error managemement
    if (bind(server_fd, (struct sockaddr *)&johnny, sizeof(johnny)) < 0)
    {
        std::cout << "Error binding socket" << std::endl;
        return 1;
    }
    if (listen(server_fd, 5) < 0)
    {
        std::cout << "Error listening on socket" << std::endl;
        return 1;
    }
    while (1)
    {
        int client_fd;
        struct sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);
        client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len);
        if (client_fd < 0)
        {
            std::cout << "Error accepting connection" << std::endl;
            return 1;
        }
        char buffer[1024]={0};
        int valrecv = recv(client_fd,buffer,1024,0);
        if (valrecv < 0)
        {
            std::cout << "Error receiving data" << std::endl;
            return 1;
        }
        std::string hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
        send(client_fd,hello.c_str(),hello.length(),0);
        close(client_fd);
    }
    close(server_fd);
    return 0;
}