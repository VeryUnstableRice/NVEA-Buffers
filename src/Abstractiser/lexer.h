#ifndef abstractiser_header
#define abstractiser_header
#include <stdbool.h>

#define MAX_TOKENS 1000
#define MAX_TOKENS_LEN 100
#define MAX_STRUCTURES 100


enum ETokenType
{
	type,
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
	char *code;
};

struct SCodeData
{
	struct SToken tokens[MAX_TOKENS];
	struct SStruct* structures[MAX_STRUCTURES];
	int struct_num, token_num;
};

void InitTokenTypes(struct SCodeData* codeData);

void Abstractise(const char* code, struct SCodeData* codeData);


extern struct STokenType* types[100];
extern int token_count;

#endif