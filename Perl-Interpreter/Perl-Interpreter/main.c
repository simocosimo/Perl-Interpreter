#include "interpreter.h"

int main(int argc, char **argv)
{
	if (argc != 2) return -1;

	int result;
	Interpreter *intrp = interpreter_create(argv[1]);
	if (intrp == NULL) return -2;

	result = interpreter_expr(intrp);
	printf("%d\n", result);

	system("PAUSE");
	return 0;
}