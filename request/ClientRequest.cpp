/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientRequest.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arhallab <arhallab@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/24 12:31:16 by arhallab          #+#    #+#             */
/*   Updated: 2022/06/30 18:16:51 by arhallab         ###   ########.fr       */
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
}

ClientRequest::ClientRequest(int socket) : Socket(socket), data(""), requestPosition(0), hasError(false), isDone(false), closeConnection(false)
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

void ClientRequest::setIsDone(bool isDone)
{
	this->isDone = isDone;
	this->requestPosition = 0;
}
/* ************************************************************************** */

void ClientRequest::parseRequest(std::string &line)
{
	//need to redo this loop
	std::cout << "line: " << line << std::endl;
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
		if (requestline.size() != 3 || countChr(line, ' ') != 2)
		{
			this->hasError = true;
			this->errorMessage = "Error: Request line wrong number of parameters";
			return ;
		}
		if (!validMethod(requestline[0]))
		{
			this->hasError = true;
			this->errorMessage = "Error: Request line method not valid";
			return ;
		}
		else
		{
			this->method = requestline[0];
		}
		this->requestURI = requestline[1];
		if (requestline[2] != "HTTP/1.1")
		{
			this->hasError = true;
			this->errorMessage = "Error: Request line HTTP version not valid";
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
		char *l = strdup(line.c_str());
		std::string p(strtok(l, ": "));
		std::string v(strtok(NULL, ": "));
		requestFields[p]= v;
		// std::cout << p << " : " << v << std::endl;
	}
	else
	{
		if (this->data.find("\r\n") != std::string::npos)
		{
			this->data = this->data.substr(this->data.find("\r\n") + 2);
		}
		if (requestFields.find("Transfer-Encoding") != requestFields.end() && requestFields["Transfer-Encoding"] == "chunked")
		{
			while(1)
			{
			int length = std::stoi(line, nullptr, 16);
			this->data = this->data.substr(line.length() + 2);
			this->body += this->data.substr(0, length);
			}
			
		}
		else if (requestFields.find("Content-Length") != requestFields.end())
		{
			long length = std::stoi(requestFields["Content-Length"]);
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
		}
		if (this->hasError == true)
		{
			return ;
		}
	// std::cout << this->data << std::endl
}
