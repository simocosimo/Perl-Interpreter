#pragma once
#ifndef _TOKEN_H_
#define _TOKEN_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "token_types.h"

// STRUCTS

typedef struct token
{
	int iType;
	int cValue;
}Token;

// FUNCS

Token *token_create(int type, ...);
void token_print(Token *tk);
Token *token_destroy(Token *tk);


#endif