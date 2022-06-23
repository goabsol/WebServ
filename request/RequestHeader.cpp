#include "RequestHeader.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

RequestHeader::RequestHeader()
{
}

RequestHeader::RequestHeader( const RequestHeader & src )
{
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

RequestHeader::~RequestHeader()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

RequestHeader &				RequestHeader::operator=( RequestHeader const & rhs )
{
	//if ( this != &rhs )
	//{
		//this->_value = rhs.getValue();
	//}
	return *this;
}

std::ostream &			operator<<( std::ostream & o, RequestHeader const & i )
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

		bool getHasError()
		{

		}
		std::string getError()
		{

		}
		std::string getData()
		{

		}

/* ************************************************************************** */