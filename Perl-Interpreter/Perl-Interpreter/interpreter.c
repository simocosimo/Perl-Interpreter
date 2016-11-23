#include "interpreter.h"

Interpreter *interpreter_create(char *path) {
	int n = 0;
	FILE *fp;
	Interpreter *intrp = (Interpreter *) malloc(sizeof(*intrp));

	if (!(fp = fopen(path, "r")))
		return NULL;

	intrp->pFile = fp;
	intrp->iPos = 0;
	intrp->tCurr = NULL;

	// counting the number of characters in the file
	do {
		getc(intrp->pFile); n++;
	} while (!feof(intrp->pFile));
	
	rewind(intrp->pFile);

	intrp->sText = (char *) malloc(n * sizeof(char));
	for (int i = 0; i < n - 1; i++)
		intrp->sText[i] = getc(intrp->pFile);
	intrp->sText[n - 1] = '\0';

	fclose(fp);
	return intrp;
}

void interpreter_error(const char *message) {
	printf("%s\n", message);
}

Token *interpreter_get_next_token(Interpreter *intrp) {
	char cCurr;
	
	if ((unsigned int)intrp->iPos > strlen(intrp->sText) - 1)
		return token_create(TK_EOF);

	cCurr = intrp->sText[intrp->iPos];

	if (isdigit(cCurr)) {
		intrp->iPos++;
		return token_create(TK_INTEGER, cCurr);
	}

	if (cCurr == '+') {
		intrp->iPos++;
		return token_create(TK_PLUS, cCurr);
	}

	if (cCurr == '-') {
		intrp->iPos++;
		return token_create(TK_MINUS, cCurr);
	}

	interpreter_error("GET_NEXT_TOKEN: Can't recognize the current character.");
	return NULL;
}

void interpreter_eat(Interpreter *intrp, int type) {
	if (intrp->tCurr->iType == type) {
		if (intrp->tCurr->iType == TK_INTEGER) intrp->tCurr->cValue -= '0';
		intrp->tCurr = interpreter_get_next_token(intrp);
	} else {
		interpreter_error("EAT: The type of the current token not match the pattern type.");
	}
}

int interpreter_expr(Interpreter *intrp) {
	int result = 0;
	intrp->tCurr = interpreter_get_next_token(intrp);

	Token *left = intrp->tCurr;
	interpreter_eat(intrp, TK_INTEGER);

	Token *op = intrp->tCurr;
	if(op->iType == TK_PLUS) interpreter_eat(intrp, TK_PLUS);
	else if (op->iType == TK_MINUS) interpreter_eat(intrp, TK_MINUS);

	Token *right = intrp->tCurr;
	interpreter_eat(intrp, TK_INTEGER);

	Token *endFile = intrp->tCurr;
	if (endFile->iType != TK_EOF)
		interpreter_error("EXPR: The expression does not match the pattern sequence.");

	if (op->iType == TK_PLUS) result = left->cValue + right->cValue;
	else if (op->iType == TK_MINUS) result = left->cValue - right->cValue;
	return result;
}