#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sys/socket.h>
#include "../tools.hpp"
#include "../server_parse.hpp"

class ClientRequest
{
	public:

		ClientRequest();
		ClientRequest(ClientRequest const & src);
		ClientRequest (int, Server_T &, long addr, long port);
		~ClientRequest();
		bool getHasError();
		bool getIsDone();
		std::string getError();
		std::string getData();
		int getSocket();
		void setData(std::string line);
		ClientRequest &	operator=( ClientRequest const & rhs );
		void checkLineValidity(std::string line);
		void parseRequest();
		void storeRequest();
		bool isValidURI(std::string &);
		void setIsDone(bool);
		bool getConnectionClosed();
		bool locationExists(std::string &request);
		bool autorised_method(std::string &method);
		std::map<std::string, std::string> getRequestFields();
		Server_T &getServer();
		void clearData();
		int Socket;
		std::string method;
		std::string requestURI;
		std::string httpVersion;
		std::map<std::string, std::string> requestFields;
		std::fstream req_file;
		std::fstream resp_file;
		std::string rq_name;
		std::string rp_name;
		size_t rq_size;
		size_t rp_size;
		std::string data;
		bool hasError;
		bool needHost;
		std::string errorMessage;
		int requestPosition;
		bool isDone;
		bool closeConnection;
		Server_T server;
		Location_T current_location;
		std::string current_location_path;
		timeval start_time;
		size_t size;
		bool size_set;
		bool expect_newline;
		bool new_data;
		size_t content_len;
		std::string queryString;
		bool body_present;
		std::string file_name;
		long		bytes_read;
		bool next_is_zero;
		std::streamsize cursor;
		size_t size_body;
		long	client_port;
		long	client_host;
		bool 	cgied;
		pid_t	cgi_pid;
		int 	cgi_fd;
};

std::ostream &			operator<<( std::ostream & o, ClientRequest const & i );

void	get_nearest_timeout(struct timeval &, SOCKET &, std::map<SOCKET,ClientRequest> &);
