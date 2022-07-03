

#include "workshop.hpp"
#include <fstream>

std::string craftResponse(ClientRequest &request, int status_code, std::string message)
{
	
	std::string response = "";
	std::map<std::string, std::string> RF = request.getRequestFields();
	response += "HTTP/1.1 " + std::to_string(status_code) + " " + message + "\r\n";
	response += "Server: " + request.getServer().server_name + "\r\n";
	if (status_code != 200)
	{
		std::fstream file;
		std::string file_name;
		if (request.getServer().error_pages.find(status_code) != request.getServer().error_pages.end())
		{
			file_name = request.getServer().error_pages[status_code];
			std::cout << "Error page: " << file_name << std::endl;
		}
		else
		{
			file_name = "/" + std::to_string(status_code) + ".html";
		}
		file.open("."+file_name, std::ios::in);
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
	}
	response += "Content-Type: " + (status_code != 200 ? std::string("text/html") : std::string("text/plain")) + "\r\n"; //change this for CGI!!!!
	response += "Content-Length: " + std::to_string(message.length()) + "\r\n"; //for body
	response += "Connection: " + (RF.find("Connection") == RF.end() ? RF["Connection"] : "Keep-Alive") + "\r\n";
	response += "\r\n";
	//body
	response += message;
	return response;
}