#ifndef abstractiser_header
#define abstractiser_header
#include <stdbool.h>

#define MAX_TOKENS 100
#define MAX_TOKENS_LEN 100


enum ETokenType
{
	Type,
	Identifier,
	OpenBracket,
	CloseBracket,
	SemiColon
};

struct STokenType
{
	char* text;
	enum EToken type;
	bool substring;
};

void ReadBuffers(const char* code);

void InitTokenTypes();
void SumLexin();

extern struct STokenType* types[100];
extern int token_count;

#endif