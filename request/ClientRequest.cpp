/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientRequest.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arhallab <arhallab@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/24 12:31:16 by arhallab          #+#    #+#             */
/*   Updated: 2022/06/25 04:49:09 by arhallab         ###   ########.fr       */
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
		line = this->data.substr(0, this->data.find("\r\n"));
		this->data = this->data.substr(this->data.find("\r\n") + 2);
		//parse the line
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