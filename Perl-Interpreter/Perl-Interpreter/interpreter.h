#pragma once
#ifndef _INTERPRETER_H_
#define _INTERPRETER_H_

#include <string.h>
#include <ctype.h>
#include "token.h"

// STRUCTS

typedef struct interpreter
{
	FILE *pFile;
	char *sText;
	Token *tCurr;

	int iPos;
}Interpreter;

// FUNCS

Interpreter *interpreter_create(char *path);
void interpreter_error(const char *message);
Token *interpreter_get_next_token(Interpreter *intrp);
void interpreter_eat(Interpreter *intrp, int type);
int interpreter_expr(Interpreter *intrp);

#endif
