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

		bool RequestHeader::getHasError()
		{
			return hasError;
		}
		std::string RequestHeader::getError()
		{
			return errorMessage;
		}
		std::string RequestHeader::getData()
		{
			return data;
		}

		void RequestHeader::setData(std::string line)
		{
			data += line;
		}

/* ************************************************************************** */