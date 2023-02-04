#ifndef abstractiser_header
#define abstractiser_header
#include <stdbool.h>

#define MAX_TOKENS 1000
#define MAX_TOKENS_LEN 100


enum ETokenType
{
	Type,
	Struct,
	Identifier,
	OpenBracket,
	CloseBracket,
	SemiColon,
	VariableName
};

struct STokenType
{
	char* text;
	enum EToken type;
};

struct SToken
{
	struct STokenType* type;
	char code[MAX_TOKENS_LEN];
};

void InitTokenTypes();

void Abstractise(const char* code);


extern struct STokenType* types[100];
extern int token_count;

#endif