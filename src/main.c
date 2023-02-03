#include <stdio.h>

int main(int argc, char** argv)
{
	char* code = " \
		buffer test	\
		{\
			int test;\
		}\
		";


	InitTokenTypes();
	ReadBuffers(code);
	FreeTokens();
}