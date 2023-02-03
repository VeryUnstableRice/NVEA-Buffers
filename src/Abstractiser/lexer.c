#include "lexer.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

struct STokenType* types[100];
int token_count;



bool MatchesToken(const char* token)
{
	for (int i = 0; i < token_count; ++i)
	{
		if (strcmp(token, types[i]->text) == 0) 
			return true;
	}
	return false;
}

bool IsLetter(char character)
{
	return (character >= 'a' && character <= 'z') || (character >= 'A' && character <= 'Z');;
}

bool IsDigit(char character)
{
	return character >= '0' && character <= '9';
}

void SumLexin(const char* code)
{
	static char tokens[MAX_TOKENS][MAX_TOKENS_LEN];

	int token_num = 0;

	int index = 0;
	while (true)
	{
		while (code[index] == ' ' || code[index] == '\t' || code[index] == '\n') {
			index++;
		}

		for (int i = 0; i < token_count; ++i)
		{
			int lenght = strlen(types[i]->text);
			if (strncmp(&code[index], types[i]->text, lenght) == 0)
			{
				strncpy(tokens[token_num], types[i]->text, lenght);
				index += lenght;
				goto endofloop;
			}
		}

		if (IsLetter(code[index]))
		{
			int start = index;
			int j = 0;
			while (IsLetter(code[index]) || IsDigit(code[index]))
			{
				tokens[token_num][j++] = code[index];
				++index;
			}
			tokens[token_num][j] = 0;
			goto endofloop;
		}
		return;

	endofloop:
		++token_num;
	}
}

//init tokens
void InitToken(const char* text, enum EToken type);

void InitTokenTypes()
{
	token_count = 0;
	InitToken("buffer"	, Identifier	, false);
	InitToken("{"		, OpenBracket	, true);
	InitToken("}"		, CloseBracket	, true);
	InitToken(";"		, SemiColon		, true);
}

void InitToken(const char* text, enum EToken type, bool substring)
{
	//alocate the token
	types[token_count] = (struct STokenType*)malloc(sizeof(struct STokenType));
	//copying text
	types[token_count]->text = malloc((strlen(text)+1)*sizeof(char));
	strcpy_s(types[token_count]->text, strlen(text) + 1, text);
	//setting type
	types[token_count]->type = type;
	types[token_count]->substring = substring;
	//increasing count
	++token_count;
}

void FreeTokens()
{
	for (int i = 0; i < token_count; ++i)
	{
		if (!types[i]) continue;
		free(types[i]->text);
		free(types[i]);
	}
	token_count = 0;
}
