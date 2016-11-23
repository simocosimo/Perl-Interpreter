#include "token.h"

Token *token_create(int type, ...) {
	va_list ap;
	Token *tk = (Token *) malloc(sizeof(*tk));

	tk->iType = type;
	va_start(ap, type);
	tk->cValue = va_arg(ap, int);
	va_end(ap);

	return tk;
}

void token_print(Token *tk) {
	printf("Token(%d, %c)\n", tk->iType, tk->cValue);
}

Token *token_destroy(Token *tk) {
	Token *tmp = tk;
	free(tmp);
	return tmp;
}