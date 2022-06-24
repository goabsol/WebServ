/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arhallab <arhallab@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/24 10:56:08 by arhallab          #+#    #+#             */
/*   Updated: 2022/06/24 11:37:34 by arhallab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>
#include <string>


class Client
{

	public:

		Client();
		Client(int socket);
		Client(Client const & src);
		~Client();
		int getSocket();
		std::string getRequest();
		void setSocket(int socket);
		void setRequest(std::string request);
		Client &		operator=( Client const & rhs );
		bool requestIsDone;

	private:
		int socket;
		std::string request;
};

std::ostream &			operator<<( std::ostream & o, Client const & i );

/* ********************************************************** CLIENT_H */