#include "Client.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Client::Client(): socket(0), request(std::string())
{
}

Client::Client(int socket): socket(socket), request(std::string())
{
}

Client::Client( const Client & src )
{
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Client::~Client()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Client &				Client::operator=( Client const & rhs )
{
	//if ( this != &rhs )
	//{
		//this->_value = rhs.getValue();
	//}
	return *this;
}

std::ostream &			operator<<( std::ostream & o, Client const & i )
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
int Client::getSocket()
{
	return socket;
}

void Client::setSocket(int s)
{
	socket = s;
}
////////////////////////////
std::string Client::getRequest()
{
	return request;
}

void Client::setRequest(std::string req)
{
	request = req;
}
/* ************************************************************************** */