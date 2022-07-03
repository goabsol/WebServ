

#include "workshop.hpp"
#include <fstream>
#include <sys/stat.h>
#include <sys/types.h>

std::string craftResponse(ClientRequest &request, int status_code, std::string message)
{
	
	std::string response = "";
	std::map<std::string, std::string> RF = request.getRequestFields();
	response += "HTTP/1.1 " + std::to_string(status_code) + " " + message + "\r\n";
	response += "Server: " + request.getServer().server_name + "\r\n";

	std::fstream file;
	std::string file_name;
	if (status_code > 399)
	{

		if (request.getServer().error_pages.find(status_code) != request.getServer().error_pages.end())
		{
			file_name = request.current_location.root + request.getServer().error_pages[status_code];
			std::cout << "Error page: " << file_name << std::endl;
		}
		else
		{
			file_name = "./pages/" + std::to_string(status_code) + ".html";
		}
	}
	else if (request.current_location.redirection.first != 0)
	{
		file_name = request.current_location.redirection.second;
	}
	else
	{
		struct stat file_info;
		std::cout << request.current_location.root+request.current_location_path << std::endl;
		lstat((request.current_location.root + request.current_location_path).c_str(), &file_info);
		if ( (file_info.st_mode & S_IFMT) == S_IFDIR)
		{
			file_name = request.current_location.root+request.requestURI + "index.html";
		}
		else
		{
			file_name = request.current_location.root+request.requestURI;
		}  
		
	}
	file.open(file_name, std::ios::in);
	std::cout << "file : " << file_name << std::endl;
	if (file.is_open())
	{
		std::string line;
		message = "";
		while (getline(file, line))
		{
			message += line + "\n";
		}
		file.close();
	}
	if (RF.find("Content-Type") == RF.end())
	{
		response += "Content-Type: text/html\r\n";
	}
	else
	{
		response += "Content-Type: " + RF["Content-Type"] + "\r\n";
	}
	std::cout << "messages : " << message << std::endl;
	response += "Content-Length: " + std::to_string(message.length()) + "\r\n"; //for body
	response += "Connection: " + (RF.find("Connection") == RF.end() ? RF["Connection"] : "Keep-Alive") + "\r\n";
	response += "\r\n";
	//body
	response += message;
	return response;
}