#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <iostream>
#include <time.h>

#define SOCKET int
int main()
{
    std::cout << "Configuring local address..." << std::endl;
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    struct addrinfo *bind_address;
    getaddrinfo(0, "8080", &hints, &bind_address);
    std::cout << "Creating socket..." << std::endl;
    SOCKET socket_listen;
    socket_listen = socket(bind_address->ai_family,
    bind_address->ai_socktype, bind_address->ai_protocol);
    if (socket_listen < 0)
    {
        std::cerr << "Error creating socket: " << strerror(errno) << std::endl;
        return 1;
    }
    if (bind(socket_listen, bind_address->ai_addr, bind_address->ai_addrlen))
    {
        std::cerr << "Error binding socket: " << strerror(errno) << std::endl;
        return 1;
    }
    freeaddrinfo(bind_address);
    std::cout << "Listening..." << std::endl;
    if (listen(socket_listen, 10) < 0)
    {
        std::cerr << "Error listening: " << strerror(errno) << std::endl;
        return 1;
    }
    std::cout << "Accepting connection..." << std::endl;
    struct sockaddr_in client_address;
    socklen_t client_address_length = sizeof(client_address);
    SOCKET socket_client = accept(socket_listen, (struct sockaddr *)&client_address, &client_address_length);
    if (socket_client < 0)
    {
        std::cerr << "Error accepting connection: " << strerror(errno) << std::endl;
        return 1;
    }
    std::cout << "Client is connected" << std::endl;
    char buffer[100];
    getnameinfo((struct sockaddr *)&client_address, client_address_length, buffer, sizeof(buffer), 0, 0, NI_NUMERICHOST);
    std::cout << "Client is connected from " << buffer << std::endl;
    std::cout << "Reading request..." << std::endl;
    char request[1024];
    int bytes_read = recv(socket_client, request, sizeof(request), 0);
    printf("%.*s", bytes_read, request);
    std::cout << "sending response..." << std::endl;
    const char* response = "HTTP/1.1 200 OK\r\nConnection: close\r\nContent-Type: text/plain\r\n\r\nLocal time is: ";
    int bytes_sent = send(socket_client, response, strlen(response), 0);
    //printf("Sent %d of %d bytes.\n", bytes_sent, (int)strlen(response));
    time_t timer;
    time(&timer);
    char *time_msg = ctime(&timer);
    bytes_sent = send(socket_client, time_msg, strlen(time_msg), 0);
    //printf("Sent %d of %d bytes.\n", bytes_sent, (int)strlen(time_msg));
    close(socket_client);
    close(socket_listen);
    std::cout << "Done." << std::endl;
}