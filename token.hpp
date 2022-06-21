#pragma once

#include "includes.h"

#define ID 0
#define LBRACE 1
#define RBRACE 2
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