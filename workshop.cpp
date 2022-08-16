/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   workshop.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arhallab <arhallab@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/04 01:31:07 by arhallab          #+#    #+#             */
/*   Updated: 2022/07/17 23:09:51 by arhallab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "workshop.hpp"

std::string makeautoindex(std::string &root, std::string &dir)
{
	(void)root;
	std::string autoindex_page = "<!DOCTYPE html>\
<html lang=\"en\">\
<head>\
	<meta charset=\"UTF-8\">\
	<meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">\
	<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\
	<title>AutoIndex</title>\
	<script src=\"https://kit.fontawesome.com/c88d0d8eda.js\" crossorigin=\"anonymous\"></script>\
	  <script src=\"https://cdn.tailwindcss.com\"></script>\
</head>\
<body>\
<div class =\"flex flex-col mt-20 mb-4 overflow-auto border-2 bg-black-300 rounded-md divide-y-2 divide-dashed divide-black-400\">";
	DIR *direc;
	struct dirent *ent;
	if ((direc = opendir(dir.c_str())) != NULL)
	{
		while ((ent = readdir(direc)) != NULL)
		{
			std::string file_name = ent->d_name;
			std::string icon = "";
			if (getResourceType(file_name) == false)
				icon =  "<i class=\"cursor-pointer text-xs m-2 p-2 bg-red-500 border-2 rounded-full fa-solid fa-folder\"></i> ";
			else if (getResourceType(file_name) == true )
				icon =  "<i class=\"cursor-pointer text-xs m-2 p-2 bg-blue-500 border-2 rounded-full fa-solid fa-file\"></i> ";
			autoindex_page += "<a class=\"cursor-pointer text-base border\" href=\""  + std::string(ent->d_name) + "\">"+ icon + ent->d_name + "</a>";
		}
		closedir(direc);
	}
	autoindex_page += "</div></body>	</html>";
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
		if (getRequestedResource(request.file_name))
		{
			if (getResourceType(request.file_name) == true)
			{
				RF["Content-Type"] = getFileType(request.file_name);
				if(gotCGI(request, request.file_name))
					return (response);
			}
			else
			{
				if (*(request.requestURI.end() - 1) != '/')
				{
					// std::cout << "redirecting : " << request.requestURI << std::endl;
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
					// std::cout << "folder : " << request.file_name << std::endl;
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
			std::vector<std::string> dirs = split(request.requestURI, '/');
			std::string dir = request.current_location.upload_store;
			for (std::vector<std::string>::iterator it = dirs.begin(); it != dirs.end() - 1; it++)
			{
				std::string i = *it;
				dir += i + "/";
				if (dir.size() > 0)
				{
					if (access(dir.c_str(), F_OK) != 0)
						mkdir(dir.c_str(), 0777);
				}
			}
			request.file_name = request.current_location.upload_store + request.requestURI;
			std::ifstream  src(request.rq_name, std::ios::binary);
			std::ofstream  dst(request.file_name,   std::ios::binary);

			dst << src.rdbuf();
			
			src.close();
			dst.close();
			status_code = 201;
			message = "Created";
			request.body_present = false;
			response = "HTTP/1.1 " + std::to_string(status_code) + " " + message + "\r\n";
			response += "Server: " + request.getServer().server_name + "\r\n";
			response += "Content-Type: text/html\r\n";
			response += "Content-Length: 0\r\n";
			response += "Location: " + request.requestFields["Host"] + request.requestURI + "\r\n\r\n";
			return(response);
			
		}
		else
			goto get;
	}
	else if (request.method == "DELETE")
	{
		if (getRequestedResource(request.file_name))
		{
			if (getResourceType(request.file_name) == true)
			{
				RF["Content-Type"] = getFileType(request.file_name);
				if (!gotCGI(request, request.file_name))
				{
					remove(request.file_name.c_str());
				C204:
					status_code = 204;
					message = "No Content";
					response = "HTTP/1.1 " + std::to_string(status_code) + " " + message + "\r\n";
					response += "Server: " + request.getServer().server_name + "\r\n";
					response += "Content-Type: text/html\r\n";
					response += "Content-Length: 0\r\n";
					response += "Location: " + request.requestFields["Host"] + request.requestURI + "\r\n\r\n";
					return(response);
				}
			}
			else
			{
				if (*(request.requestURI.end() - 1) != '/')
					return (craftResponse(request, 409, "Conflict"));
				std::string content_type = "text/html";
				if (gotCGI(request, request.file_name))
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
						goto C204;
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
	if (RF.find("Content-Type") == RF.end() || status_code > 399)
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
			
			// std::cout << "size : " << size_f << std::endl;
			// std::cout << "hello" << std::endl;
				// for body
			response += "Content-Length: " + std::to_string(size_f) + "\r\n\r\n";
			response += std::string(buffer, size_f);
			// std::cout << buffer << std::endl;
			request.body_present = false;
			request.size_body = size_f;
			delete [] buffer;
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
	// // std::cout << "Response: "<< std::endl << response << std::endl;
	
	return response;
}

bool getRequestedResource(std::string &resource)
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
	// std::cout << "wut " << resource << std::endl;
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
		if (getRequestedResource(file_name))
		{
			found = *it;
			return true;
		}
		
	}
	return false;
}

std::string http_toupp(std::string &str)
{
	std::string response = "HTTP_";

	// turn lowercase to uppercase and - to _ and add it to response
	for (size_t i = 0; i < str.length(); i++)
	{
		if (str[i] >= 'a' && str[i] <= 'z')
			response += str[i] - 32;
		else if (str[i] == '-')
			response += '_';
		else
			response += str[i];
	}
	return response;
}

bool gotCGI(ClientRequest &request, std::string &file_name)
{
	// CGI that shit (plz )
	//set headers
	//path
	std::string path = file_name;
	std::map<std::string, std::string> cgi = request.current_location.cgi;
	std::string extension = getFileExtension(file_name);
	if (cgi.find(extension) != cgi.end())
	{
		char** args = new char*[3];
		args[0] = strdup(cgi[extension].c_str());
		args[1] = strdup(path.c_str());
		args[2] = NULL;
		int res = open(request.rp_name.c_str(), O_RDWR | O_CREAT);
		request.cgi_fd = res;
		request.cgied = true;
		pid_t pid = fork();
		if (pid == 0)
		{
			std::vector<char *> env;
			env.push_back(strdup("GATEWAY_INTERFACE=CGI/1.1"));
			env.push_back(strdup(("QUERY_STRING=" + request.queryString).c_str()));
			if (request.requestFields.find("Content-Length") != request.requestFields.end())
				env.push_back(strdup(("CONTENT_LENGTH=" + request.requestFields["Content-Length"]).c_str()));
			 if (request.requestFields.find("Content-Type") != request.requestFields.end())
				env.push_back(strdup(("CONTENT_TYPE=" + request.requestFields["Content-Type"]).c_str()));
			env.push_back(strdup(("PATH_INFO=" + request.requestURI).c_str()));
			env.push_back(strdup(("PATH_TRANSLATED=" + request.requestURI).c_str()));
			env.push_back(strdup(("SCRIPT_NAME=")));
			env.push_back(strdup(("SCRIPT_FILENAME=" + file_name).c_str()));
			env.push_back(strdup(("REQUEST_METHOD=" + request.method).c_str()));
			env.push_back(strdup(("REDIRECT_STATUS=200")));
			env.push_back(strdup(("SERVER_PROTOCOL=HTTP/1.1")));
			env.push_back(strdup(("SERVER_SOFTWARE=WEBSERV/1.1")));
			env.push_back(strdup(("SERVER_NAME=" + request.server.server_name).c_str()));
			env.push_back(strdup(("PATH=" + std::string(getenv("PATH"))).c_str()));
			env.push_back(strdup(("SERVER_PORT=" + std::to_string(request.client_port)).c_str()));
			for (std::map<std::string, std::string>::iterator it = request.requestFields.begin(); it != request.requestFields.end(); it++)
			{
				std::string key = it->first;
				std::string value = it->second;
				env.push_back(strdup((http_toupp(key)+ "=" + value).c_str()));
			}
			env.push_back(NULL);
			int body = open(request.rq_name.c_str(), O_RDONLY);
			dup2(body, 0);
			close(body);
			if (dup2(res, 1) == - 1)
				std::cerr << "duperror" << std::endl;
			close(res);
			execve(cgi[extension].c_str(),args, env.data());
			exit(1);
		}
		else
		{
			request.cgi_pid = pid;
			// waitpid(pid, NULL, 0);
			// char *buff = new char[1025];
			// std::string message = "";
			// int count;	
			// int sum = 0;
			// lseek(res, 0, SEEK_SET);
			// while ((count = read(res, buff, 1024)) > 0)
			// {
			// 	buff[count] = '\0';
			// 	sum += count;
			// 	message += std::string(buff);
			// }
			// delete [] buff;
			// close(res);
			// remove(request.rp_name.c_str());
			// //if php add 
			// if (extension == ".php")
			// {
			// 	size_t pos = message.find("\r\n\r\n");
			// 	if (pos != std::string::npos)
			// 	{
			// 		sum -= pos + 4;
			// 	}
			// }
			// std::string status = "HTTP/1.1 200 OK\r\n";
			// size_t p = message.find("\r\n\r\n");
			// if (p != std::string::npos)
			// {
			// 	std::string header = message.substr(0, p);
			// 	std::vector<std::string> lines = split(header, '\n');
			// 	for (std::vector<std::string>::iterator it = lines.begin(); it != lines.end(); it++)
			// 	{
			// 		std::string line = *it;
			// 		if (line.find("Status:") != std::string::npos)
			// 		{
			// 			std::vector<std::string> status_line = split(line, ' ');
			// 			status = "HTTP/1.1 " + status_line[1] + status_line[2] + "\r\n";
			// 		}
			// 	}
			// }
			// std::string header = status  + "Content-Length: "+ std::to_string(sum)+"\r\n"+"Server: "+request.server.server_name+"\r\n"+"Connection: "+request.requestFields["Connection"]+"\r\n";
			// if (extension == ".py")
			// 	header += "Content-Type: text/html\r\n\r\n";
			// //split
			// response = header + message;
		}
	}
	else
	{
		return (false);
	}
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
