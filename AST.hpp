#pragma once

#define A_COMPOUND 	1
#define A_LOCATION 	2
#define A_VARIABLE 	3 
#define A_STATEMENT 4
#define A_NOOP 		5

typedef struct AST_STRUCT
{
	public:
		int type;
} AST_T;

AST_T *init_ast(int type);
