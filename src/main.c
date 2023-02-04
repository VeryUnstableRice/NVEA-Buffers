#include <stdio.h>

int main(int argc, char** argv)
{
	char* code = " \
		struct test	\
		{\
			int test;\
		}\
		";


	InitTokenTypes();
	Abstractise(code);
	FreeTokens();
}