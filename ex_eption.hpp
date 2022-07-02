

#include <exception>
#include <iostream>
#include <string>


class http_error_exception : public std::exception
{
public:
	http_error_exception(int code, const std::string message)
	{
		this->code = code;
		this->message = message;
	}
	int code;
	std::string message;
	const char* what() const noexcept
	{
		return message.c_str();
	}
};