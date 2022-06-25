/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientRequest.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-bagh <ael-bagh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/24 12:31:16 by arhallab          #+#    #+#             */
/*   Updated: 2022/06/25 07:27:41 by ael-bagh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ClientRequest.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

ClientRequest::ClientRequest()
{
}

ClientRequest::ClientRequest( const ClientRequest & src )
{
}

ClientRequest::ClientRequest(int &socket) : Socket(socket), data("")
{
	requestPosition = 0;
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
	//if ( this != &rhs )
	//{
		//this->_value = rhs.getValue();
	//}
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
bool ClientRequest::getIsDone()
{
	return this->isDone;
}

/* ************************************************************************** */

void ClientRequest::parseRequest()
{
	std::string line;
	
	while(this->data.find("\r\n\r\n") != std::string::npos)
	{
		if (this->data.find("\r\n") != std::string::npos)
		{
			if (requestPosition < 2)
				requestPosition++;
			line = this->data.substr(0, this->data.find("\r\n"));
			this->checkLineValidity(line);
		}
		this->data = this->data.substr(this->data.find("\r\n") + 2);
		//parse the line
	}
	requestPosition = 3;
}

void ClientRequest::checkLineValidity(std::string line)
{
	if (requestPosition == 1)
	{
		std::vector<std::string> requestline = split_white_space(line);
		if (requestline.size() != 3)
		{
			hasError = 1;
			errorMessage = "Error: Request line wrong number of parameters";
			return ;
		}
		if (requestline[0] != "POST" && requestline[0] != "GET" && requestline[0] != "DELETE")
		{
			hasError = 1;
			errorMessage = "Error: Request method is wrong";
			return ;
		}
		// im reading about what an URI should be like so meh
		//if (requestline[1] )
	}
}

void ClientRequest::storeRequest()
{
	char buffer[1024];
	memeset(buffer, 0, 1024);
    int bytes_read;
    std::vector<std::string> lines;
    bytes_read = recv(this->Socket, buffer, 1024, 0);
    this->data += buffer;
    if (this->data.find("\r\n\r\n") != std::string::npos)
    {
		this->parseRequest();
    }
}