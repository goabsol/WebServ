#include "RequestHeader.hpp"

RequestHeader parse_request(int socket)
{
    RequestHeader ret;
    char buffer[1024];
	memset(buffer, 0, 1024);
    int bytes_read;
    std::string rest = "";
    std::vector<std::string> lines;
	while((bytes_read = recv(socket, buffer, 1024, 0)))
    {
        std::string text = rest + buffer;
        std::string delimiter = "\r\n";
        size_t pos = 0;
        while ((pos = text.find(delimiter)) != std::string::npos) 
        {
            lines.push_back(text.substr(0, pos));
            if (pos != text.length())
                rest = text.substr(pos, text.length());
        }
    }
    //TODO :
    // CHECK IF THIS IS WORKING XD
    // if line 1 correct fill request line
    // then fill the fiels in the map
    // then fill the body
    // all error management
    return ret;
}