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
};

struct SToken
{
	struct STokenType* type;
};

void InitTokenTypes();
void SumLexin(const char* code);

extern struct STokenType* types[100];
extern int token_count;

#endif