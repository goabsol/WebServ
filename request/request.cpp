#include "RequestHeader.cpp"

RequestHeader parse_request(Client client)
{
    RequestHeader ret;
    char buffer[1024];
	memset(buffer, 0, 1024);
    int bytes_read;
    std::string rest = "";
    std::vector<std::string> lines;
    bytes_read = recv(client.socket, buffer, 1024, 0);
    client.buffer += buffer;
    if (client.buffer.find("\r\n\r\n") == std::string::npos)
    {
        
        return really_parse_request(client.buffer);
    }
    else
        return ;
	//while((bytes_read = recv(socket, buffer, 1024, 0)) > 0)
    //{
    //    std::string text = rest + buffer;
    //    if (text.find("\r\n\r\n") == std::string::npos)
    //        break ;
    //    std::string delimiter = "\r\n";
    //    std::cout << text << std::endl;
    //    size_t pos = 0;
    //    while (((pos = text.find(delimiter)) != std::string::npos)) 
    //    {
    //        lines.push_back(text.substr(0, pos));
    //        ret.setData(text.substr(0, pos + 2));
    //        if (pos != text.length())
    //            rest = text.substr(pos, text.length());
    //        std::cout << text.substr(0, pos) << std::endl;
    //        text.erase(0, pos + 2);
    //    }
    //    lines.push_back(text);
    //    ret.setData(text);
    //}
    //TODO :
    // CHECK IF THIS IS WORKING XD
    // if line 1 correct fill request line
    // then fill the fiels in the map
    // then fill the body
    // all error management 
    return ret;
}