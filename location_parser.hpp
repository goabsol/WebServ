#include "parsing_tools.hpp"
#include "token.hpp"

class Location_T
{
	public:
		Location_T(std::vector<token_T> tokens, size_t &i);
		Location_T();
		Location_T(const Location_T& location);
		Location_T& operator=(const Location_T& location);
		std::string root;
		std::vector<std::string> cgi;
		std::vector<std::string> allowed_methods;
		int body_size_limit;
		std::vector<std::string> index;
		bool autoindex;
		std::string upload_store;
		std::map<std::string, Location_T> locations;
};