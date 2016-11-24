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
	int n = 1, result;
	char *sInteger = (char *) malloc(sizeof(char));
	while (intrp->cCUrr != '\0' && isdigit(intrp->cCUrr)) {
		/* if (n != 1) {
			if (sInteger + (n - 1) == NULL) {
				sInteger = realloc(sInteger, (2 * (n - 1) + 1) * sizeof(char));
			}
		} */
		sInteger[n - 1] = intrp->cCUrr;
		interpreter_advance(intrp);
		n++;
	}
	sInteger[n - 1] = '\0';
	result = atoi(sInteger);
	sInteger = realloc(sInteger, n * sizeof(char));

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