/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arhallab <arhallab@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/24 10:55:23 by arhallab          #+#    #+#             */
/*   Updated: 2022/06/24 11:50:15 by arhallab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "request.hpp"

RequestHeader parse_request(Client client)
{
    RequestHeader ret;
    char buffer[1024];
	memset(buffer, 0, 1024);
    int bytes_read;
    std::string rest = "";
    std::vector<std::string> lines;
    bytes_read = recv(client.getSocket(), buffer, 1024, 0);
    client.setRequest(client.getRequest() + buffer);
    if (client.getRequest().find("\r\n\r\n") != std::string::npos)
    {
		client.requestIsDone = true;
        return really_parse_request(client.getRequest());
    }
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

RequestHeader really_parse_request(std::string req)
{
	
}