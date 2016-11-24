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

	intrp->cCUrr = intrp->sText[intrp->iPos];

	fclose(fp);
	return intrp;
}

void interpreter_error(const char *message) {
	printf("%s\n", message);
}

void interpreter_advance(Interpreter *intrp) {
	intrp->iPos++;
	if ((unsigned int)intrp->iPos > strlen(intrp->sText) - 1)
		intrp->cCUrr = '\0';
	else
		intrp->cCUrr = intrp->sText[intrp->iPos];
}

void interpreter_skip_whitespaces(Interpreter * intrp) {
	while (intrp->cCUrr != '\0' && isspace(intrp->cCUrr))
		interpreter_advance(intrp);
}

int interpreter_integer(Interpreter *intrp) {
	int n = 1, act, result;
	act = 2 * n;
	char *sInteger = (char *) malloc(act * sizeof(char));
	while (intrp->cCUrr != '\0' && isdigit(intrp->cCUrr)) {
		if (n != 1) {
			if (sInteger[act - 1] == '\0') {
				act = 2 * (n - 1) + 1;
				sInteger = realloc(sInteger, act * sizeof(char));
			}
		}
		sInteger[n - 1] = intrp->cCUrr;
		sInteger[n] = '\0';
		interpreter_advance(intrp);
		n++;
	}
	result = atoi(sInteger);

	free(sInteger);
	return result;
}

Token *interpreter_get_next_token(Interpreter *intrp) {
	
	while (intrp->cCUrr != '\0') {

		if (isspace(intrp->cCUrr)) {
			interpreter_skip_whitespaces(intrp);
			continue;
		}

		if (isdigit(intrp->cCUrr)) 
			return token_create(TK_INTEGER, interpreter_integer(intrp));

		if (intrp->cCUrr == '+') {
			interpreter_advance(intrp);
			return token_create(TK_PLUS, intrp->cCUrr);
		}

		if (intrp->cCUrr == '-') {
			interpreter_advance(intrp);
			return token_create(TK_MINUS, intrp->cCUrr);
		}

		if (intrp->cCUrr == '*') {
			interpreter_advance(intrp);
			return token_create(TK_MULTIPLY, intrp->cCUrr);
		}

		if (intrp->cCUrr == '/') {
			interpreter_advance(intrp);
			return token_create(TK_DIVIDE, intrp->cCUrr);
		}

		interpreter_error("GET_NEXT_TOKEN: Can't recognize the current character.");
	}

	return token_create(TK_EOF);
}

void interpreter_eat(Interpreter *intrp, int type) {
	if (intrp->tCurr->iType == type) {
		intrp->tCurr = interpreter_get_next_token(intrp);
	} else {
		interpreter_error("EAT: The type of the current token not match the pattern type.");
	}
}

float interpreter_expr(Interpreter *intrp) {
	float result = 0;
	intrp->tCurr = interpreter_get_next_token(intrp);

	Token *left = intrp->tCurr;
	interpreter_eat(intrp, TK_INTEGER);

	Token *op = intrp->tCurr;
	if(op->iType == TK_PLUS) interpreter_eat(intrp, TK_PLUS);
	else if (op->iType == TK_MINUS) interpreter_eat(intrp, TK_MINUS);
	else if (op->iType == TK_MULTIPLY) interpreter_eat(intrp, TK_MULTIPLY);
	else if (op->iType == TK_DIVIDE) interpreter_eat(intrp, TK_DIVIDE);

	Token *right = intrp->tCurr;
	interpreter_eat(intrp, TK_INTEGER);

	Token *endFile = intrp->tCurr;
	if (endFile->iType != TK_EOF)
		interpreter_error("EXPR: The expression does not match the pattern sequence.");

	switch (op->iType)
	{
	case TK_PLUS:
		result = left->iValue + right->iValue;
		break;

	case TK_MINUS:
		result = left->iValue - right->iValue;
		break;

	case TK_MULTIPLY:
		result = left->iValue * right->iValue;
		break;

	case TK_DIVIDE:
		if(right->iValue != 0) result = (float)left->iValue / right->iValue;
		else interpreter_error("EXPR: Division by zero.");
		break;

	default:
		break;
	}
	return result;
}

void interpreter_destroy(Interpreter *intrp) {
	
}