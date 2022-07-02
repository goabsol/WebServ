

#include "workshop.hpp"

std::string craftResponse(ClientRequest &request, int status_code, std::string message)
{
	std::string response = "";
	std::map<std::string, std::string> RF = request.getRequestFields();
	response += "HTTP/1.1 " + std::to_string(status_code) + " " + message + "\r\n";
	response += "Server: " + request.getServer().server_name + "\r\n";
	response += "Content-Type: " + std::string("text/plain") + "\r\n"; //change this for CGI!!!!
	response += "Content-Length: " + std::to_string(0) + "\r\n"; //for body
	response += "Connection: " + (RF.find("Connection") == RF.end() ? RF["Connection"] : "Keep-Alive") + "\r\n";
	response += "\r\n";
	return response;
}