#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <iostream>
# include <string>

class Client
{

	public:

		Client();
		Client(int socket);
		Client(Client const & src);
		~Client();
		int getSocket();
		std::string getRequest();
		void setSocket(int socket);
		void setRequest(std::string request);
		Client &		operator=( Client const & rhs );

	private:
		int socket;
		std::string request;
};

std::ostream &			operator<<( std::ostream & o, Client const & i );

#endif /* ********************************************************** CLIENT_H */