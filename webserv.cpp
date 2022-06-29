/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anaselbaghdadi <anaselbaghdadi@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/25 10:16:14 by arhallab          #+#    #+#             */
/*   Updated: 2022/06/28 19:43:39 by anaselbaghd      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"


int main(int ac, char **av)
{
	//dummy servers
	parser_T parser = parse_file(ac, av);
	std::cout << parser.error_pages[404] << std::endl;
	
    
	return sockinit(parser);
}