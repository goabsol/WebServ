/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientRequest.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arhallab <arhallab@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/24 12:31:16 by arhallab          #+#    #+#             */
/*   Updated: 2022/07/10 01:23:02 by arhallab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ClientRequest.hpp"

const std::string tmp[] = {"GET",
						   "POST",
						   "PUT",
						   "DELETE",
						   "HEAD",
						   "OPTIONS",
						   "CONNECT",
						   "TRACE"};

const std::vector<std::string> v_methods(tmp,tmp+8);
/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

ClientRequest::ClientRequest()
{
}

ClientRequest::ClientRequest( const ClientRequest & src )
{
	*this = src;
}

ClientRequest::ClientRequest(int socket, Server_T &server) : Socket(socket), data(""),
			requestPosition(0), hasError(false), isDone(false), closeConnection(false),
			server(server), current_location(Location_T())
{
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

ClientRequest::~ClientRequest()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

ClientRequest &				ClientRequest::operator=( ClientRequest const & rhs )
{
	if ( this != &rhs )
	{
		this->Socket = rhs.Socket;
		this->method = rhs.method;
		this->requestURI = rhs.requestURI;
		this->httpVersion = rhs.httpVersion;
		this->requestFields = rhs.requestFields;
		this->body = rhs.body;
		this->data = rhs.data;
		this->hasError = rhs.hasError;
		this->needHost = rhs.needHost;
		this->errorMessage = rhs.errorMessage;
		this->requestPosition = rhs.requestPosition;
		this->isDone = rhs.isDone;
		this->closeConnection = rhs.closeConnection;
		this->server = rhs.server;
		this->current_location = rhs.current_location;
		this->current_location_path = rhs.current_location_path;
		this->start_time = rhs.start_time;
	}
	return *this;
}

std::ostream &			operator<<( std::ostream & o, ClientRequest const & i )
{
	//o << "Value = " << i.getValue();
	return o;
}


/*
** --------------------------------- METHODS ----------------------------------
*/


/*
** --------------------------------- ACCESSOR ---------------------------------
*/

bool ClientRequest::getHasError()
{
	return this->hasError;
}
std::string ClientRequest::getError()
{
	return this->errorMessage;
}
std::string ClientRequest::getData()
{
	return this->data;
}
int ClientRequest::getSocket()
{
	return this->Socket;
}
bool ClientRequest::getIsDone()
{
	return this->isDone;
}

bool ClientRequest::getConnectionClosed()
{
	return this->closeConnection;
}

std::map<std::string, std::string> ClientRequest::getRequestFields()
{
	return this->requestFields;
}

Server_T &ClientRequest::getServer()
{
	return this->server;
}

void ClientRequest::setIsDone(bool isDone)
{
	this->isDone = isDone;
	this->requestPosition = 0;
	this->hasError = false;
}

void ClientRequest::clearData()
{
	this->data.clear();
}
/* ************************************************************************** */


bool sortlocation(std::pair<std::string, Location_T>  &a, std::pair<std::string, Location_T> &b)
{
	return a.first < b.first;
}

bool ClientRequest::locationExists(std::string &request)
{
	// if (this->server.locations.find(request) != this->server.locations.end())
	// 	return true;
	std::vector<std::pair<std::string, Location_T> > valid_locations;
	
	for (std::map<std::string, Location_T>::iterator it = this->server.locations.begin(); it != this->server.locations.end(); it++)
	{
		std::cout << "location: " << it->first << " match " << request << std::endl;
		if (!strncmp(it->first.c_str(), request.c_str(), it->first.length()))
		{
			std::cout << "Found location: " << it->first << std::endl;
			valid_locations.push_back(std::make_pair(it->first, it->second));
		}
	}
	if (valid_locations.size())
	{
		sort(valid_locations.begin(), valid_locations.end(), sortlocation); 
		this->current_location = valid_locations.back().second;
		this->current_location_path = valid_locations.back().first;
		std::cout << "Taken location :" << valid_locations.back().first << std::endl;
		return true;
	}
	return false;
}

bool ClientRequest::autorised_method(std::string &method)
{
		
	if (this->current_location.allowed_methods_inh == false && this->current_location.allowed_methods_set == false)
		return true;
	std::vector<std::string> current_methods = this->current_location.allowed_methods;
	if (std::find(current_methods.begin(), current_methods.end(), method) != current_methods.end())
		return true;
	
	return false;
}

void ClientRequest::parseRequest(std::string &line)
{
	// std::cout << "line: " << line << std::endl;
	if (line == "")
	{
		this->requestPosition = 2;
		line = this->data;
	}
	this->checkLineValidity(line);
		// if (this->data.find("\r\n") != std::string::npos)
		// {
		// 	if (this->requestPosition < 2)
		// 		this->requestPosition++;
		// 	line = this->data.substr(0, this->data.find("\r\n"));
		// 	this->checkLineValidity(line);
		// }
		// //parse the line
		// if (this->requestPosition == 3 || (this->requestPosition == 2  && this->data.find("\r\n")==0))
		// {
		// 	this->requestPosition = 3;
		// 	std::map<std::string, std::string>::iterator contentLength = requestFields.find("Content-Length");
		// 	//std::map<std::string, std::string>::iterator transferEncoding = requestFields.find("Transfer-Encoding");
		// 	if (contentLength != requestFields.end())
		// 	{
		// 		std::cout << this->data << " ------l-o" << std::endl;
		// 		size_t length = std::stoi(contentLength->second);
		// 		char *buff = new char[length];
		// 		size_t bytes_read = recv(Socket, buff, length, 0);
			// 	std::cout << bytes_read << std::endl;
			// 	if (bytes_read > 0)
			// 	{
			// 		this->body += std::string(buff, buff + length);
			// 		this->setIsDone(true);
			// 	}
			// 	else if(!bytes_read)
			// 	{
			// 		this->closeConnection = true;
			// 	}
			// 	else
			// 	{
			// 		this->hasError = true;
			// 		this->errorMessage = "Error: recv() failed";
			// 	}
			// }
			// else
			// {
			// 	this->setIsDone(true);
			// }
		// }
}

void ClientRequest::checkLineValidity(std::string line)
{
	if (this->requestPosition == 0)
	{
		std::vector<std::string> requestline = split(line, ' ');
		if (requestline.size() != 3 || countChr(line, ' ') != 2 || !validMethod(requestline[0]) || !validURI(requestline[1]))
		{
			throw http_error_exception(400, "Bad Request");
			return ;
		}
		else
		{
			this->method = requestline[0];
		}
		hexaToAscii(requestline[1]);
		if (requestline[1].length() > 2048)
		{
			/* ERROR 414 */
			throw http_error_exception(414, "Request-URI Too Long");
			return ;
		}
		else if (!locationExists(requestline[1]))
		{
			/* ERROR 404 */
			
			throw http_error_exception(404, "Not Found");
			return ;
		}

		// this->current_location = this->server.locations[requestline[1]];
		// this->current_location_path = requestline[1];
		if (this->server.locations[this->current_location_path].redirection_set)
		{
			
			throw http_error_exception(301, "Moved Permanently");
					
			// throw http_redirect_exception((this->server.locations[requestline[1]].redirection).first, this->server.locations[requestline[1]].redirection.second, line + "\r\n" + this->data);
			return ;
		}
		
		else if ( !autorised_method(this->method))
		{
			this->hasError = true;
			this->errorMessage = "Error: Request line method not autorised";
			/* ERROR 405 */
			
			throw http_error_exception(405, "Method Not Allowed");
			return ;
		}
		else
		{
		
			this->requestURI = requestline[1];
		}
		
		if (requestline[2] != "HTTP/1.1")
		{
			this->hasError = true;
			this->errorMessage = "Error: Request line HTTP version not valid";
			/* ERROR 505 */
			throw http_error_exception(505, "HTTP Version Not Supported");
			return ;
		}
		else
		{
			this->httpVersion = requestline[2];
		}
		requestPosition = 1;
	}
	else if (requestPosition == 1)
	{
		if (line.find(": ") != std::string::npos)
		{
			char *l = strdup(line.c_str());
			std::string p(strtok(l, ": "));
			std::string v(strtok(NULL, ": "));
			// CHECK V WITH P
			requestFields[p]= v;
		}
		else
		{
			this->hasError = true;
			this->errorMessage = "Error: Request line field not valid";
			/* ERROR 400 */
			throw http_error_exception(400, "Bad Request");
			return ;
		}
		// std::cout << p << " : " << v << std::endl;
	}
	else
	{
		if (this->data.find("\r\n") != std::string::npos)
		{
			this->data = this->data.substr(this->data.find("\r\n") + 2);
		}
		if (requestFields.find("Transfer-Encoding") != requestFields.end())
		{
			if (requestFields["Transfer-Encoding"] != "chunked")
			{
				this->hasError = true;
				this->errorMessage = "Error: Transfer-Encoding not chunked";
				/* 501 ERROR */
				throw http_error_exception(501, "Not Implemented");
				return ;
			}
			bool size_found = false;
			int size;
			while(line != "0")
			{
				if (this->data.find("\r\n") != std::string::npos)
				{
					line = this->data.substr(0, this->data.find("\r\n"));
					std::cout << "hihi " << line << std::endl;
					this->data = this->data.substr(this->data.find("\r\n") + 2);
					if (!size_found)
					{
						size_found = true;
						size = std::stoi(line, nullptr, 16);
						std::cout << "size " << size << std::endl;
					}
					else
					{
						size -= line.length();
						if (size <= 0)
							size_found = false;
						this->body += line;
					}
				}
				else if (size > 0 || !size_found)
				{
					char *buff = new char[1024];
					size_t bytes_read = recv(Socket, buff, 1024, 0);
					std::cout << "buff: " << buff << std::endl;
					if (bytes_read > 0)
					{
						this->data += std::string(buff, buff + bytes_read);
						size -= bytes_read;
					}
					else if(!bytes_read)
					{
						this->closeConnection = true;
						return;
					}
					else
					{
						this->hasError = true;
						this->errorMessage = "Error: recv() failed";
						return;
					}
				}
			}
			std::cout << "body: " << this->body << std::endl;
		}
		else if (requestFields.find("Content-Length") != requestFields.end())
		{
			long length = std::stoi(requestFields["Content-Length"]);
			if (length > this->server.body_size_limit)
			{
				this->hasError = true;
				this->errorMessage = "Error: Content-Length too long";
				/* 413 ERROR */
				throw http_error_exception(413, "Request Entity Too Large");
				return ;
			}
			this->body = this->data.substr(0, std::min(length, (long)this->data.length()));
			this->data = this->data.substr(std::min(length, (long)this->data.length()));
			length -= this->body.length();
			while(length > 0)
			{
				char *buff = new char[length];
				size_t bytes_read = recv(Socket, buff, length, 0);
				if (bytes_read > 0)
				{
					this->body += std::string(buff, buff + length);
					length -= bytes_read;
				}
				else if(!bytes_read)
				{
					this->closeConnection = true;
					return;
				}
				else
				{
					this->hasError = true;
					this->errorMessage = "Error: recv() failed";
					return;
				}
			}
			std::cout << "body : " << this->body << std::endl;
		}
		this->setIsDone(true);
	}
}


void ClientRequest::storeRequest()
{
	if (this->hasError == true)
	{
		return ;
	}
	char *buffer = new char[1024];
	memeset(buffer, 0, 1024);
    size_t bytes_read = 0;
    std::vector<std::string> lines;
    bytes_read = recv(this->Socket, buffer, 1024, 0);
	if (bytes_read > 0)
	{
		this->data += std::string(buffer, buffer + bytes_read);
		if (this->requestPosition == 0)
		{
			trimwspace(this->data);
		}
	}
	else if (bytes_read == 0)
	{
		this->closeConnection = true;
	}
	else
	{
		this->hasError = true;
		this->errorMessage = "Error: recv() failed";
		std::cout << "<-------->" << bytes_read << std::endl;

		// 
	}
    while (this->data.find("\r\n") != std::string::npos)
    {
		std::string line;
		if (this->data.find("\r\n") != std::string::npos)
			line = this->data.substr(0, this->data.find("\r\n"));
		else
			line = this->data;
		parseRequest(line);
		try
		{
			if (this->data.find("\r\n") != std::string::npos)
			{
				this->data = this->data.substr(this->data.find("\r\n") + 2);
			}
		}
		catch(std::exception e)
		{
			std::cerr << "Error: " << this->data << std::endl;
		}
		if (this->hasError == true)
		{
			return ;
		}
		std::cout << "line: |" << line << "|"<< std::endl;
		if (line == "")
		{
			
			return ;
		}
	}
	// std::cout << this->data << std::endl
}

void	get_nearest_timeout(struct timeval &timeout, SOCKET &stc, std::map<SOCKET,ClientRequest> &clients)
{
	gettimeofday(&timeout, NULL);
	struct timeval min_timeout;
	
	for (std::map<SOCKET,ClientRequest>::iterator it = clients.begin(); it != clients.end(); ++it)
	{
		if (it->second.isDone == false)
		{
			struct timeval diff;
			diff.tv_sec = 15 - (timeout.tv_sec - it->second.start_time.tv_sec);
			diff.tv_usec = it->second.start_time.tv_usec - timeout.tv_usec;
			if (diff.tv_usec < 0)
			{
				diff.tv_sec--;
				diff.tv_usec += 1000000;
			}
			if (diff.tv_sec < 0)
			{
				diff.tv_sec = 0;
				diff.tv_usec = 0;
			}
			// std::cout << "diff: " << diff.tv_sec << " " << diff.tv_usec << std::endl;
			// std::cout << "timeout: " << timeout.tv_sec << " " << timeout.tv_usec << std::endl;
			// std::cout << "start_time: " << it->second.start_time.tv_sec << " " << it->second.start_time.tv_usec << std::endl;
			
			if (it == clients.begin() || diff.tv_sec < min_timeout.tv_sec || (diff.tv_sec == min_timeout.tv_sec && diff.tv_usec < min_timeout.tv_usec))
			{
				min_timeout = diff;
				stc = it->first;
			}
		}
	}
	timeout = min_timeout;
}
