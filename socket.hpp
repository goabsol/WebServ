/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arhallab <arhallab@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/09 07:33:11 by arhallab          #+#    #+#             */
/*   Updated: 2022/07/14 14:54:40 by arhallab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <poll.h>
#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>
#include <fcntl.h>
#include "tools.hpp"
#include "conf_file.hpp"
#include <sstream>
#include "./request/ClientRequest.hpp"
#include "workshop.hpp"

int sockinit(parser_T);


