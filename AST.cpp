#include "AST.hpp"

AST_T *init_ast(int type)
{
	AST_T *ast = new AST_T;
	ast->type = type;
	
	return ast;
}