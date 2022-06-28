/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anaselbaghdadi <anaselbaghdadi@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/09 07:33:11 by arhallab          #+#    #+#             */
/*   Updated: 2022/06/28 19:44:00 by anaselbaghd      ###   ########.fr       */
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
#include "./request/ClientRequest.hpp"
#define SOCKET int

int sockinit(parser_T);


