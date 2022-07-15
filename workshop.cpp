/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   workshop.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arhallab <arhallab@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/04 01:31:07 by arhallab          #+#    #+#             */
/*   Updated: 2022/07/15 12:39:06 by arhallab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "workshop.hpp"

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

std::string getFileExtension(std::string const &file_name)
{
	if(file_name.find_last_of(".") != std::string::npos)
		return file_name.substr(file_name.find_last_of("."));
	return "";
}

std::string craftResponse(ClientRequest &request, int status_code, std::string message)
{
	std::string response = "";
	std::map<std::string, std::string> RF = request.getRequestFields();
	response += "HTTP/1.1 " + std::to_string(status_code) + " " + message + "\r\n";
	response += "Server: " + request.getServer().server_name + "\r\n";

	request.file_name = "";
	std::fstream file;
	message = "";
	if (status_code > 399)
	{
		if (request.current_location.error_pages.find(status_code) != request.current_location.error_pages.end())
			request.file_name = request.current_location.root + request.current_location.error_pages[status_code];
		else
			request.file_name = "./pages/" + std::to_string(status_code) + ".html";
		goto end;
	}
	else if (request.current_location.redirection.first != 0)
	{
		request.file_name = request.current_location.redirection.second;
		request.requestURI = request.file_name;
		response = "HTTP/1.1 301 Moved Permanently\r\n";
		response += "Location: " + request.file_name + "\r\n";
		goto send;
	}
	else
		request.file_name = request.current_location.root + request.requestURI;

	if (request.method == "GET")
	{
		get:
		if (getRequestedResource(request.file_name, file))
		{
			if (getResourceType(request.file_name) == true)
			{
				//RF["Content-Type"] = getFileType(file_name);
				gotCGI(request, request.file_name, request.method);
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
				if (indexInDir(request.current_location.index, request.file_name, index))
				{
					request.file_name += index;
					request.requestURI += index;
					std::cout << "folder : " << request.file_name << std::endl;
					goto get;
				}
				else if (request.method == "GET" && request.current_location.autoindex)
				{
					message = makeautoindex(request.current_location.root, request.file_name);
					goto send;
				}
				else
					return (craftResponse(request, 403, "Forbidden"));
			}
		}
		else
			return (craftResponse(request, 404, "Not Found"));
	}
	else if (request.method == "POST")
	{
		if (request.current_location.upload_store_set)
		{
			// upload
		}
		else
			goto get;
	}
	else if (request.method == "DELETE")
	{
		if (getRequestedResource(request.file_name, file))
		{
			if (getResourceType(request.file_name) == true)
			{
				RF["Content-Type"] = getFileType(request.file_name);
				if (!gotCGI(request, RF["Content-Type"], request.method))
					return (craftResponse(request, 204, "No Content"));
			}
			else
			{
				if (*(request.requestURI.end() - 1) != '/')
					return (craftResponse(request, 409, "Conflict"));
				std::string content_type = "text/html";
				if (gotCGI(request, content_type, request.method))
				{
					std::string index;
					if (indexInDir(request.current_location.index, request.requestURI, index))
						request.file_name += index;
					else
						return (craftResponse(request, 403, "Forbidden"));
				}
				else
				{
					if (emptyDir(request.file_name))
						return (craftResponse(request, 204, "No Content"));
					else
					{
						if (access(request.file_name.c_str(), W_OK))
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
	file.open(request.file_name,  std::ios::in| std::ios::binary| std::ios::ate);
	if (file.is_open())
	{
		request.body_present = true;
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
	
	// response += "Connection: " + (RF.find("Connection") == RF.end() ? RF["Connection"] : "Keep-Alive") + "\r\n";
	
	// body
	if (request.body_present)
	{
		long long size_f = file.tellg();
		if (size_f < 64000)
		{
		
			char *buffer;
			buffer = new char[size_f + 1];
			buffer[size_f] = '\0';
			file.seekg (0, std::ios::beg);
			file.read(buffer, size_f);
			
			std::cout << "size : " << size_f << std::endl;
			std::cout << "hello" << std::endl;
				// for body
			response += "Content-Length: " + std::to_string(size_f) + "\r\n\r\n";
			response += std::string(buffer, size_f);
			std::cout << buffer << std::endl;
			request.body_present = false;
			request.size_body = size_f;
		}
		else
		{
			response += "Content-Length: " + std::to_string(size_f) + "\r\n\r\n";
			request.bytes_read = 0;
			request.size_body = size_f;
		}
		file.close();
	}
	else
	{
		response += "Content-Length: " + std::to_string(message.length()) + "\r\n\r\n";
		request.size_body = message.length();
		response += message;
	}
	// std::cout << "Response: "<< std::endl << response << std::endl;
	
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
	return true;
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

bool gotCGI(ClientRequest &request, std::string &file_name, std::string &method)
{
	// CGI that shit (plz )
	//set headers
	//path
	std::string path = request.requestURI;
	std::map<std::string, std::string> cgi = request.current_location.cgi;
	std::string extension = getFileExtension(file_name);
	if (cgi.find(extension) != cgi.end())
	{
		char** args = new char*[3];
		args[0] = strdup(cgi[extension].c_str());
		args[1] = strdup(path.c_str());
		args[2] = NULL;
		//set environement variables
		char**env = new char*[2];
		env[0] = strdup(std::string("QUERY_STRING="+request.queryString).c_str());
		env[1] = NULL;
		/////////////////////////////
		pid_t pid = fork();
		int fd[2];
		if (pipe(fd) < 0)
			throw("pipeError");
		if (pid == 0)
		{
			if (method == "POST")
			{
				int body = open(request.rq_name.c_str(), O_RDWR);
				dup2(body, 0);
			}
			close(fd[0]);
			dup2(fd[1], 1);
			close(fd[1]);
			execve(cgi[extension].c_str(),args,env);
		}
		else
		{
			char buff[1024];
			std::string response = "";
			int count = 0;
			close(fd[1]);
			FILE *f = fdopen(fd[0],"r");
			int sum = 0;
			while ((count = read(fd[0], buff, 1024)) != 0)
			{
				sum += count;
				response += std::string(buff);
			}
			std::string header = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: "+ std::to_string(count)+"\r\n"+"Server: tobechanged"+"\r\n\r\n"+response;
		}
	}
	else
		return false;
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
					if (getResourceType(file_name) == true)
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