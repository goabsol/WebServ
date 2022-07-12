/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arhallab <arhallab@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/09 07:34:47 by arhallab          #+#    #+#             */
/*   Updated: 2022/07/10 01:14:06 by arhallab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "socket.hpp"

int    sockinit(parser_T parser)
{
    std::map<SOCKET,ClientRequest>clients;
    std::map<SOCKET,ClientRequest>clts_ongoing_requests;
    struct sockaddr_in johnny; 
    SOCKET server_fd;
	SOCKET max_fd = 0;
	const int enable = 1;
	fd_set read_fd;
	FD_ZERO(&read_fd);
	std::map<SOCKET, Server_T> m_socket_to_server;
	std::map<SOCKET, std::string> m_socket_to_response;
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
			// std::cout << parser.servers[i].ports[j].second << " " << parser.servers[i].ports[j].first << std::endl;
			johnny.sin_addr.s_addr = inet_addr(parser.servers[i].ports[j].first.c_str());
			johnny.sin_port = htons(parser.servers[i].ports[j].second);
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
	while (1)
	{
		//making copies of read_fd and write_fd to avoid modifying the original with select
		fd_set rcopy = read_fd;
		fd_set wcopy = write_fd;
		struct timeval timeout;
		SOCKET socket_to_close;
		
		get_nearest_timeout(timeout, socket_to_close, clts_ongoing_requests);
		std::cout << "TO test " << clts_ongoing_requests.empty() << " if not empty the closest to be closed: " << socket_to_close;
		std::cout << " in " << timeout.tv_sec << "," << timeout.tv_usec << std::endl;
		//selecting sockets to read/write (multiplexing)
		int select_return = select(max_fd + 1, &rcopy, &wcopy, NULL, (clts_ongoing_requests.empty() ? NULL : &timeout));
		if (select_return < 0)
		{
			std::cerr << "Error in select" << std::endl;
			return 1;
		}
		else if(select_return == 0)
		{
			std::cout << "Connection closedddd"  << std::endl;
			FD_CLR(socket_to_close, &read_fd);
			FD_CLR(socket_to_close, &write_fd);
			close(socket_to_close);
			clients.erase(socket_to_close);
			clts_ongoing_requests.erase(socket_to_close);
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
					// std::cout << i << " " << client_fd << std::endl;
					clients[client_fd] = ClientRequest(client_fd, m_socket_to_server[i]);
					break;
				}
				else {
					// if (clients.find(i) == clients.end())
					// {
					// 	clients[i] = ClientRequest(i);
					// 	std::cout << i  << " fxfcv " << clients[i].getSocket() << std::endl;
					// }
					// else
					// {
					// }
					////////////////////////////////////////////////////////////////////
					if (!clients[i].getIsDone())
					{
						try
						{
							if (clts_ongoing_requests.find(i) == clts_ongoing_requests.end())
							{
								gettimeofday(&(clients[i].start_time), NULL);
								clts_ongoing_requests[i] = clients[i];
								std::cout << "Reading from socket " << i << std::endl;
								std::cout << "Start time: " << clients[i].start_time.tv_sec << "," << clients[i].start_time.tv_usec << std::endl;
							}
							clients[i].storeRequest();
							if (clients[i].getIsDone())
							{
							m_socket_to_response[i] = craftResponse(clients[i]);
							}
						}
						catch(http_error_exception& e)
						{
							std::cout << e.what() << std::endl;
							clients[i].setIsDone(true);
							clients[i].clearData();
							m_socket_to_response[i] = craftResponse(clients[i], e.code, e.message);

						}

					}
					////////////////////////////////////////////////////////////////////
					// std::cout << client.getHasError() << " " << client.getIsDone() << std::endl;
					if (clients[i].getHasError() || clients[i].getConnectionClosed())
					{
						std::cout << (clients[i].getHasError()?clients[i].getError() : "Connection closed")  << std::endl;
						FD_CLR(i, &read_fd);
						FD_CLR(i, &write_fd);
						close(i);
						clients.erase(i);
						clts_ongoing_requests.erase(i);
					}
					else if (clients[i].getIsDone())
					{
						std::cout << "Data: " << i << " " << clients[i].getData() << std::endl;
						clts_ongoing_requests.erase(i);
						FD_CLR(i, &read_fd);
						FD_SET(i, &write_fd);
					}
				}
			}
			else if (FD_ISSET(i, &wcopy)) //if socket is ready to write, send response
			{
				// std::cout << "Socket " << i << " of " << m_socket_to_server[i].name << " is ready for writing" << std::endl;
				std::string hello = m_socket_to_response[i];
				int sent_bytes = send(i,hello.c_str(),hello.size(),0);
				std::cout << "Data sent ---> sent: " << sent_bytes << " ----- total: " << hello.size() << std::endl;
				std::cout << clients[i].getData() << std::endl;
				//remove socket from write_fd and add to read_fd
				FD_CLR(i, &write_fd);
				FD_SET(i, &read_fd);
				clients[i].setIsDone(false);
			}
		}
	}
}
