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
#include <vector>
#include <map>

#define SOCKET int
typedef struct parsed_servers{
	std::vector<std::pair<int, int> > port;
	std::string name;
}ps;
void *memeset(void *s, int c, size_t n)
{
    char *p = (char *)s;
    while (n--)
        *p++ = c;
    return s;
}

int main()
{
	std::map<SOCKET,ps> m;
	std::vector<ps> servers;
	ps s;
	s.port.push_back(std::make_pair(0, 8080));
	s.port.push_back(std::make_pair(0, 8081));
	s.port.push_back(std::make_pair(0, 8082));
	s.name = "server1";
	servers.push_back(s);
	s.port.clear();
	s.port.push_back(std::make_pair(0, 8083));
	s.port.push_back(std::make_pair(0, 8084));
	s.port.push_back(std::make_pair(0, 8085));
	s.name = "server2";
	servers.push_back(s);
    struct sockaddr_in johnny; 
    SOCKET server_fd;
	fd_set our_sockets;
	FD_ZERO(&our_sockets);
	SOCKET max_fd = 0;
	for (int i = 0; i < servers.size(); i++)
	{
		for (int j = 0; j < servers[i].port.size(); j++)
		{
			server_fd = socket(AF_INET, SOCK_STREAM, 0);
			if (server_fd < 0)
			{
				std::cout << "Error opening socket" << std::endl;
				return 1;
			}
			memeset(&johnny, 0, sizeof(johnny));
			johnny.sin_family = AF_INET;
			johnny.sin_port = htons(servers[i].port[j].second);
			johnny.sin_addr.s_addr = INADDR_ANY;
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
			FD_SET(server_fd, &our_sockets);
			if (server_fd > max_fd)
				max_fd = server_fd;
			std::cout << server_fd << std::endl;
			m[server_fd] = servers[i];
		}
	}
	fd_set master = our_sockets;
	while (1)
	{
		fd_set copy = our_sockets;
		if (select(max_fd + 1, &copy, NULL, NULL, NULL) < 0)
		{
			std::cout << "Error in select" << std::endl;
			return 1;
		}
		for (int i = 1; i <= max_fd; i++)
		{
			if (FD_ISSET(i, &copy))
			{
				std::cout << "Socket " << i << " of server " << m[i].name << " is ready" << std::endl;
				if (FD_ISSET(i, &master))
				{
					std::cout << "New connection" << std::endl;
					struct sockaddr_in client_addr;
					socklen_t client_addr_len = sizeof(client_addr);
					SOCKET client_fd = accept(i, (struct sockaddr *)&client_addr, &client_addr_len);
					if (client_fd < 0)
					{
						std::cout << "Error accepting connection" << std::endl;
						return 1;
					}
					FD_SET(client_fd, &our_sockets);
					if (client_fd > max_fd)
						max_fd = client_fd;
					std::cout << client_fd << std::endl;
					m[client_fd] = m[i];
					i = client_fd;
				}
				std::cout << "Data received" << std::endl;
				char buffer[1024];
				memset(buffer, 0, 1024);
				int bytes_read = recv(i, buffer, 1024, 0);
				if (bytes_read < 0)
				{
					std::cout << "Error reading data" << std::endl;
					return 1;
				}
				else if (bytes_read == 0)
				{
					std::cout << "Connection closed" << std::endl;
					FD_CLR(i, &our_sockets);
					close(i);
				}
				else
				{
					std::cout << "Data: " << buffer << std::endl;
					std::string hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
        			send(i,hello.c_str(),hello.length(),0);
					std::cout << "Data sent" << std::endl;
				}
			}
		}
	}
	return 0;
}
    // while (1)
    // {
    //     int client_fd;
    //     struct sockaddr_in client_addr;
    //     socklen_t client_addr_len = sizeof(client_addr);
    //     client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len);
    //     if (client_fd < 0)
    //     {
    //         std::cout << "Error accepting connection" << std::endl;
    //         return 1;
    //     }
    //     char buffer[1024]={0};
    //     int valrecv = recv(client_fd,buffer,1024,0);
    //     if (valrecv < 0)
    //     {
    //         std::cout << "Error receiving data" << std::endl;
    //         return 1;
    //     }
    //     std::string hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
    //     send(client_fd,hello.c_str(),hello.length(),0);
    //     close(client_fd);
    // }
    // close(server_fd);
//     return 0;
// }