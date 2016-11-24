#include "token.h"

Token *token_create(int type, ...) {
	va_list ap;
	Token *tk = (Token *) malloc(sizeof(*tk));

	tk->iType = type;
	va_start(ap, type);
	tk->iValue = va_arg(ap, int);
	va_end(ap);

	// token_print(tk);
	return tk;
}

void token_print(Token *tk) {
	printf("Token(%d, %c)\n", tk->iType, tk->iValue);
}

void token_destroy(Token *tk) {
	free(tk);
}