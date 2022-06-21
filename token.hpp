#pragma once

#include "includes.h"

#define ID 0
#define LEFTBRACE 1
#define RIGHTBRACE 2
#define COLON 3
#define SEMICOLON 4
#define END_OF_FILE 5
#define NUMBER 6


typedef struct TOKEN_STRUCT
{
	std::string value;
	int type;
} token_T;

token_T *init_token(std::string, int type);

std::string token_type_to_string(int type);