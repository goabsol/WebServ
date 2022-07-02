

#include "workshop.hpp"

std::string craftResponse(ClientRequest &request, int status_code = 200, std::string message)
{
	std::string response = "";
	response += "HTTP/1.1 " + std::to_string(status_code) + " " + message + "\r\n";
	response += "Content-Type: " + request.content_type + "\r\n";
	response += "Content-Length: " + std::to_string(request.content.length()) + "\r\n";
	response += "\r\n";
	response += request.content;
	return response;
}