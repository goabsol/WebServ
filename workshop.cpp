/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   workshop.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arhallab <arhallab@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/04 01:31:07 by arhallab          #+#    #+#             */
/*   Updated: 2022/07/04 06:00:24 by arhallab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "workshop.hpp"
#include <dirent.h>

std::string makeautoindex(std::string &root, std::string &dir)
{
	std::string autoindex_page = "<!DOCTYPE html>\
<html lang=\"en\">\
<head>\
	<meta charset=\"UTF-8\">\
	<meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">\
	<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\
	<title>AutoIndex</title>\
</head>\
<body>";
	DIR *direc;
	struct dirent *ent;
	if ((direc = opendir(dir.c_str())) != NULL)
	{
		while ((ent = readdir(direc)) != NULL)
		{
			autoindex_page += "<a href=\""  + std::string(ent->d_name) + "\">" + ent->d_name + "</a><br>";
		}
		closedir(direc);
	}
	autoindex_page += "</body>	</html>";
	return autoindex_page;
}

std::string getFileType(std::string const &file_name)
{
	MimeType mime_type;
	if (file_name.find_last_of(".") != std::string::npos)
	{
		std::string fn = file_name.substr(file_name.find_last_of("."));
		return mime_type.get_mime_type(fn);
	}
	return "text/html";
}

std::string craftResponse(ClientRequest &request, int status_code, std::string message)
{

	std::string response = "";
	std::map<std::string, std::string> RF = request.getRequestFields();
	response += "HTTP/1.1 " + std::to_string(status_code) + " " + message + "\r\n";
	response += "Server: " + request.getServer().server_name + "\r\n";

	std::fstream file;
	std::string file_name;
	message = "";
	if (status_code > 399)
	{

		if (request.current_location.error_pages.find(status_code) != request.current_location.error_pages.end())
		{
			std::cout << "Error page found" << std::endl;
			file_name = request.current_location.root + request.current_location.error_pages[status_code];
			std::cout << "Error page: " << file_name << std::endl;
		}
		else
		{
			std::cout << "yo" << std::endl;
			file_name = "./pages/" + std::to_string(status_code) + ".html";
		}
		goto end;
	}
	else if (request.current_location.redirection.first != 0)
	{
		file_name = request.current_location.redirection.second;
		request.requestURI = file_name;
		response = "HTTP/1.1 301 Moved Permanently\r\n";
		response += "Location: " + file_name + "\r\n";
		goto send;
	}
	else
	{
		file_name = request.current_location.root + request.requestURI;
	}

	if (request.method == "GET")
	{
	get:
		if (getRequestedResource(file_name, file))
		{
			if (getResourceType(file_name) == FILE)
			{
			hi:
				std::string content_type = "text/html";
				RF["Content-Type"] = getFileType(file_name);
				gotCGI(request.current_location, content_type, request.method);
			}
			else
			{
				if (*(request.requestURI.end() - 1) != '/')
				{
					std::cout << "redirecting : " << request.requestURI << std::endl;
					request.requestURI += "/";
					response = "HTTP/1.1 301 Moved Permanently\r\n";
					response += "Location: " + request.requestURI + "\r\n";
					goto send;
				}
				std::string index;
				if (indexInDir(request.current_location.index, file_name, index))
				{
					file_name += index;
					request.requestURI += index;
					std::cout << "folder : " << file_name << std::endl;
					goto get;
				}
				else if (request.method == "GET" && request.current_location.autoindex)
				{
					message = makeautoindex(request.current_location.root, file_name);
				}
				else
				{
					return (craftResponse(request, 403, "Forbidden"));
				}
			}
		}
		else
		{
			std::cout << "hi " << file_name << std::endl;
			return (craftResponse(request, 404, "Not Found"));
		}
	}
	else if (request.method == "POST")
	{
		if (request.current_location.upload_store_set)
		{
			// upload
		}
		else
		{
			goto get;
		}
	}
	else if (request.method == "DELETE")
	{
		if (getRequestedResource(file_name, file))
		{
			if (getResourceType(file_name) == FILE)
			{
				std::string content_type = "text/html";
				RF["Content-Type"] = getFileType(file_name);
				if (!gotCGI(request.current_location, content_type, request.method))
					return (craftResponse(request, 204, "No Content"));
			}
			else
			{
				if (*(request.requestURI.end() - 1) != '/')
					return (craftResponse(request, 409, "Conflict"));
				std::string content_type = "text/html";
				if (gotCGI(request.current_location, content_type, request.method))
				{
					std::string index;
					if (indexInDir(request.current_location.index, request.requestURI, index))
					{
						file_name += index;
					}
					else
					{
						return (craftResponse(request, 403, "Forbidden"));
					}
				}
				else
				{
					if (emptyDir(file_name))
						return (craftResponse(request, 204, "No Content"));
					else
					{
						if (access(file_name.c_str(), W_OK))
							return (craftResponse(request, 500, "Internal Server Error"));
						else
							return (craftResponse(request, 403, "Forbidden"));
					}
				}

				RF["Content-Type"] = content_type;
			}
		}
		else
		{
			return (craftResponse(request, 404, "Not Found"));
		}
	}
	else // maybe add 501 Not Implemented maybe not maybe just friends
	{
		return (craftResponse(request, 405, "Method Not Allowed"));
	}

end:
	file.open(file_name, std::fstream::in);
	if (file && message == "")
	{
		std::string line;
		message = "";
		while (1)
		{
			getline(file, line);
			message += line + "\r\n";
			if (file.eof())
				break;
		}
		file.close();
	}
send:
	if (RF.find("Content-Type") == RF.end())
	{
		response += "Content-Type: text/html\r\n";
	}
	else
	{
		response += "Content-Type: " + RF["Content-Type"] + "\r\n";
	}
	response += "Content-Length: " + std::to_string(message.length()) + "\r\n"; // for body
	response += "Connection: " + (RF.find("Connection") == RF.end() ? RF["Connection"] : "Keep-Alive") + "\r\n";
	response += "\r\n";
	std::cout << "Response: " << response << std::endl;
	// body
	response += message;
	
	return response;
}

bool getRequestedResource(std::string &resource, std::fstream &file)
{
	std::fstream fs;
	fs.open(resource, std::fstream::in);
	DIR *dp;
	if (fs)
	{
		fs.close();
		return true;
	}
	dp = opendir(resource.c_str());
	std::cout << "wut " << resource << std::endl;
	if (dp)
	{
		closedir(dp);
		return true;
	}
	return false;
}

bool getResourceType(std::string &path)
{
	struct stat file_info;
	lstat(path.c_str(), &file_info);
	if ((file_info.st_mode & S_IFMT) == S_IFDIR)
		return DIRECTORY;
	return FILE;
}

bool indexInDir(std::vector<std::string> &indexes, std::string &dir, std::string &found)
{
	for (std::vector<std::string>::iterator it = indexes.begin(); it != indexes.end(); it++)
	{
		std::fstream file;
		std::string file_name = dir + *it;
		if (getRequestedResource(file_name, file))
		{
			found = *it;
			return true;
		}
		
	}
	return false;
}

bool gotCGI(Location_T &location, std::string &tail, std::string &method)
{
	// CGI that shit (plz )
	
	return (true);
}

bool emptyDir(std::string &dir)
{
	struct stat file_info;
	lstat(dir.c_str(), &file_info);
	if ((file_info.st_mode & S_IFMT) == S_IFDIR)
	{
		DIR *dp;
		struct dirent *ep;
		dp = opendir(dir.c_str());
		if (dp != NULL)
		{
			while ((ep = readdir(dp)))
			{
				if (strcmp(ep->d_name, ".") != 0 && strcmp(ep->d_name, "..") != 0)
				{
					std::string file_name = dir + ep->d_name;
					if (getResourceType(file_name) == FILE)
						remove(file_name.c_str());
					else
						return false;
				}
			}
			closedir(dp);
		}
		return true;
	}
	return false;
}