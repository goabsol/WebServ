
#pragma once

#include "tools.hpp"
#include "server_parse.hpp"
#include "request/ClientRequest.hpp"

std::string craftResponse(ClientRequest &request, int status_code = 200, std::string message = "OK");