/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anaselbaghdadi <anaselbaghdadi@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/25 10:16:14 by arhallab          #+#    #+#             */
/*   Updated: 2022/06/27 14:15:02 by anaselbaghd      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"


int main(int ac, char **av)
{
	//dummy servers
	parser_T parser = parse_file(ac, av);
	std::cout << parser.error_pages[404] << std::endl;
    struct sockaddr_in johnny; 
    SOCKET server_fd;
	SOCKET max_fd = 0;
	const int enable = 1;
	fd_set read_fd;
	FD_ZERO(&read_fd);
	std::map<SOCKET, Server_T> m_socket_to_server;
	//create socket for each port
	for (size_t i = 0; i < parser.servers.size(); i++)
	{
		for (size_t j = 0; j < parser.servers[i].ports.size(); j++)
		{
			server_fd = socket(AF_INET, SOCK_STREAM, 0);
			if (server_fd < 0)
			{
				std::cout << "Error opening socket" << std::endl;
				return 1;
			}
			//setting socket options to reuse address
			if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
				std::cout << "setsockopt(SO_REUSEADDR) failed" << std::endl;
			memeset(&johnny, 0, sizeof(johnny));
			johnny.sin_family = AF_INET;
			std::cout << parser.servers[i].ports[j].second << " " << parser.servers[i].ports[j].first << std::endl;
			johnny.sin_port = htons(parser.servers[i].ports[j].second);
			johnny.sin_addr.s_addr = inet_addr(parser.servers[i].ports[j].first.c_str());
			//bind socket to port
			if (bind(server_fd, (struct sockaddr *)&johnny, sizeof(johnny)) < 0)
			{
				std::cerr << strerror(errno) << std::endl;
				std::cerr << "Error binding socket" << std::endl;
				return 1;
			}
			//listen for connections
			if (listen(server_fd, 5) < 0)
			{
				std::cerr << "Error listening on socket" << std::endl;
				return 1;
			}
			//add socket to read_fd
			FD_SET(server_fd, &read_fd);
			if (server_fd > max_fd)
				max_fd = server_fd;
			//store (socket, server) in map
			m_socket_to_server[server_fd] = parser.servers[i];
		}
	}
	//safekeeping server sockets in master
	fd_set master = read_fd;
	fd_set write_fd;
	FD_ZERO(&write_fd);
	std::map<SOCKET,ClientRequest>clients;
	while (1)
	{
		//making copies of read_fd and write_fd to avoid modifying the original with select
		fd_set rcopy = read_fd;
		fd_set wcopy = write_fd;
		//selecting sockets to read/write (multiplexing)
		if (select(max_fd + 1, &rcopy, &wcopy, NULL, NULL) < 0)
		{
			std::cerr << "Error in select" << std::endl;
			return 1;
		}
		for (int i = 1; i <= max_fd; i++)
		{
			if (FD_ISSET(i, &rcopy)) //if socket is ready to read
			{
				// std::cout << "Socket " << i << " of " << m_socket_to_server[i].name << " is ready for reading" << std::endl;
				if (FD_ISSET(i, &master)) //if socket is a server socket create a new connection and add to read_fd
				{
					std::cout << "New connection" << std::endl;
					struct sockaddr_in client_addr;
					socklen_t client_addr_len = sizeof(client_addr);
					SOCKET client_fd = accept(i, (struct sockaddr *)&client_addr, &client_addr_len);
					if (client_fd < 0)
					{
						std::cerr << "Error accepting connection" << std::endl;
						return 1;
					}
					if (setsockopt(client_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
						std::cout << "setsockopt(SO_REUSEADDR) failed" << std::endl;
					FD_SET(client_fd, &read_fd);
					if (client_fd > max_fd)
						max_fd = client_fd;
					m_socket_to_server[client_fd] = m_socket_to_server[i];
					i = client_fd;
				}
				if (clients.find(i) == clients.end())
				{
					clients[i] = ClientRequest(i);
					//std::cout << i  << " fxfcv " << clients[i].getSocket() << std::endl;
				}
				////////////////////////////////////////////////////////////////////
				if (!clients[i].getIsDone())
					clients[i].storeRequest();
				////////////////////////////////////////////////////////////////////
				if (clients[i].getHasError())
				{
					std::cout << clients[i].getError() << std::endl;
					FD_CLR(i, &read_fd);
					close(i);
					clients.erase(i);
				}
				else if (clients[i].getIsDone())
				{
					std::cout << "Data: " << i << " " << clients[i].getData() << std::endl;
					FD_CLR(i, &read_fd);
					FD_SET(i, &write_fd);
				}
			}
			else if (FD_ISSET(i, &write_fd)) //if socket is ready to write, send response
			{
				// std::cout << "Socket " << i << " of " << m_socket_to_server[i].name << " is ready for writing" << std::endl;
				std::string hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
				send(i,hello.c_str(),hello.length(),0);
				std::cout << "Data sent" << std::endl;
				//remove socket from write_fd and add to read_fd
				FD_CLR(i, &write_fd);
				FD_SET(i, &read_fd);
				clients[i].setIsDone(false);
			}
		}
	}
	return 0;
}