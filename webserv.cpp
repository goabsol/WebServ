/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arhallab <arhallab@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/25 10:16:14 by arhallab          #+#    #+#             */
/*   Updated: 2022/07/14 16:40:43 by arhallab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"


int main(int ac, char **av)
{
	//dummy servers
	signal(SIGPIPE, SIG_IGN);
	parser_T parser = parse_file(ac, av);
	std::cout << parser.error_pages[404] << std::endl;
	
    
	return sockinit(parser);
}