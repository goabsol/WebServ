/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientRequest.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anaselbaghdadi <anaselbaghdadi@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/24 12:31:16 by arhallab          #+#    #+#             */
/*   Updated: 2022/06/27 17:11:35 by anaselbaghd      ###   ########.fr       */
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

ClientRequest::ClientRequest(int socket) : Socket(socket), data(""), requestPosition(0), hasError(false), isDone(false)
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

void ClientRequest::setIsDone(bool isDone)
{
	this->isDone = isDone;
	this->requestPosition = 0;
}
/* ************************************************************************** */

void ClientRequest::parseRequest()
{
	std::string line;
	
	while(this->data.find("\r\n\r\n") != std::string::npos)
	{
		if (this->data.find("\r\n") != std::string::npos)
		{
			if (this->requestPosition < 2)
				this->requestPosition++;
			line = this->data.substr(0, this->data.find("\r\n"));
			this->checkLineValidity(line);
		}
		this->data = this->data.substr(this->data.find("\r\n") + 2);
		//parse the line
		if (this->requestPosition == 2 && this->data.find("\r\n")==0)
		{
			this->requestPosition = 3;
			if (requestFields.find("Content-Length") == requestFields.end()
			&& requestFields.find("Transfer-Encoding") == requestFields.end() )
			{
				this->isDone = true;
			}
			//std::cout << "hi I happen" << std::endl;
			//this->data.clear();
		}
		
	}
}

void ClientRequest::checkLineValidity(std::string line)
{
	if (this->requestPosition == 1)
	{
		std::vector<std::string> requestline = split_white_space(line);
		if (requestline.size() != 3 || countChr(line, ' ') != 2)
		{
			this->hasError = true;
			this->errorMessage = "Error: Request line wrong number of parameters";
			return ;
		}
		if (std::find(v_methods.begin(), v_methods.end(), requestline[0]) == v_methods.end())
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
		requestPosition = 2;
	}
	else if (requestPosition == 2)
	{
		char *l = strdup(line.c_str());
		std::string p(strtok(l, ": "));
		std::string v(strtok(NULL, ": "));
		requestFields[p]= v;
		std::cout << p << " : " << v << std::endl;
	}
	else
	{
		
		if (requestFields.find("Content-Length") != requestFields.end())
		{
			this->data = this->data.substr(this->data.find("\r\n") + 2);
			if (this->data.length() != std::stoi(requestFields["Content-Length"]) + 4)
			{
				this->hasError = true;
				//error here
				return ;
			}
			this->body = this->data.substr(0, std::stoi(requestFields["Content-Length"]));
		}
		else
		{
			//To do: parse chunked;
		}
	}
}


void ClientRequest::storeRequest()
{
	char *buffer;
	memeset(buffer, 0, 1);
    int bytes_read;
    std::vector<std::string> lines;
    bytes_read = recv(this->Socket, buffer, 1, 0);
	if (bytes_read > 0)
	{
		this->data += std::string(buffer);;
	}
	else
	{
		this->hasError = true;
		this->errorMessage = "Error: recv() failed";
	}
    if (this->data.find("\r\n\r\n") != std::string::npos
	&& this->data.substr(this->data.find("\r\n\r\n") + 4).find("\r\n\r\n") != std::string::npos)
    {
		std::cout << "found" << std::endl;
		std::cout << this->data << std::endl;
		this->parseRequest();
    }
	std::cout << this->data << std::endl;
}