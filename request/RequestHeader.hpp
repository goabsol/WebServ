#ifndef REQUESTHEADER_HPP
# define REQUESTHEADER_HPP

# include <iostream>
# include <string>
# include <map>

class RequestHeader
{
	public:

		RequestHeader();
		RequestHeader( RequestHeader const & src );
		~RequestHeader();
		bool getHasError();
		std::string getError();
		std::string getData();
		void setData(std::string line);
		RequestHeader &		operator=( RequestHeader const & rhs );

	private:
		std::string method;
		std::string requestURI;
		std::string httpVersion;
		std::map<std::string, std::string> requestFields;
		std::string body;
		std::string data;
		bool hasError;
		std::string errorMessage;
		bool isDone;
};

std::ostream &			operator<<( std::ostream & o, RequestHeader const & i );

#endif /* *************************************************** REQUESTHEADER_H */