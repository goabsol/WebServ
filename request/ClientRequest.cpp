/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientRequest.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arhallab <arhallab@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/24 12:31:16 by arhallab          #+#    #+#             */
/*   Updated: 2022/07/06 18:59:34 by arhallab         ###   ########.fr       */
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

ClientRequest::ClientRequest(int socket, Server_T &server) : Socket(socket), data(""), requestPosition(0), hasError(false), isDone(false), closeConnection(false), server(server), current_location(Location_T()), body(""), size(0), size_set(false)
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
	this->body = "";
	this->size = 0;
	this->size_set = false;
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
	if (line == "")
	{
		this->requestPosition = 2;
		std::cout << "Request is done " << this->data << std::endl;
		
		line = this->data.substr(this->data.find("\r\n") + 2);
		this->data = this->data.substr(this->data.find("\r\n") + 2);
	}
	if (this->requestPosition == 2)
	{
		std::cout << "DATA " << this->data << std::endl;

	}
	this->checkLineValidity(line);
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
		// if (this->data.find("\r\n") != std::string::npos)
		// {
		// 	this->data = this->data.substr(this->data.find("\r\n") + 2);
		// }
		if (requestFields.find("Transfer-Encoding") != requestFields.end())
		{
			if (requestFields["Transfer-Encoding"] != "chunked")
			{
				throw http_error_exception(501, "Not Implemented");
			}
			if (this->size_set && line.length() > this->size)
			{
				this->size = 0;
				this->size_set = false;
				throw http_error_exception(400, "Bad Request");
			}
			std::cout << "hihi1 |" << line << "|" <<  std::endl;
			while (line != "0" && line != "")
			{
				std::cout << "hihi2 " << line << " hihi " << this->data << std::endl;
				if (this->data.find("\r\n") != std::string::npos)
				{
					std::cout << "hihi3 " << line << std::endl;
					line = this->data.substr(0, this->data.find("\r\n"));
					this->data = this->data.substr(this->data.find("\r\n") + 2);
					if (!this->size_set)
					{
						this->size_set = true;
						this->size = std::stoi(line, nullptr, 16);
						std::cout << "size " << size << std::endl;
					}
					else
					{
						this->size -= line.length();
						if (this->size <= 0)
							this->size_set = false;
						this->body += line;
					}
				}
				else if (this->size > 0 || !this->size_set)
				{
					break;
				}
				else
				{
					this->setIsDone(true);
				}
			}
			if (line == "0")
			{
				this->setIsDone(true);
			}
			std::cout << "body: " << this->body << std::endl;
		}
		else if (requestFields.find("Content-Length") != requestFields.end())
		{
			long length = std::stoi(requestFields["Content-Length"]) - this->body.length();
			if (length > this->server.body_size_limit)
			{
				this->hasError = true;
				this->errorMessage = "Error: Content-Length too long";
				/* 413 ERROR */
				throw http_error_exception(413, "Request Entity Too Large");
				return ;
			}
			this->body += this->data.substr(0, std::min(length, (long)this->data.length()));
			this->data = this->data.substr(std::min(length, (long)this->data.length()));
			length -= line.length();
			if (length <= 0 || std::stoi(requestFields["Content-Length"]) == 0)
			{
				this->setIsDone(true);
				return ;
			}
			// std::cout << "body : " << this->body << std::endl;
		}
		else
		{
			this->setIsDone(true);
		}
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
			std::cerr << "Error: " << e.what() << std::endl;
		}
		if (this->hasError == true)
		{
			return ;
		}
		if (this->isDone)
		{
			
			return ;
		}
	}
	// std::cout << this->data << std::endl
}
