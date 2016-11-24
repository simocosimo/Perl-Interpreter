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
	char cCUrr;
	Token *tCurr;

	int iPos;
}Interpreter;

// FUNCS

Interpreter *interpreter_create(char *path);
void interpreter_error(const char *message);
void interpreter_advance(Interpreter *intrp);
int interpreter_integer(Interpreter *intrp);
void interpreter_skip_whitespaces(Interpreter * intrp);
Token *interpreter_get_next_token(Interpreter *intrp);
void interpreter_eat(Interpreter *intrp, int type);
float interpreter_expr(Interpreter *intrp);
void interpreter_destroy(Interpreter *intrp);

#endif
