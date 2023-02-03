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

bool IsWhitespace(char character)
{
	return character == ' ' || character == '\t' || character == '\n';
}

void SumLexin(const char* code)
{
	static char tokens[MAX_TOKENS][MAX_TOKENS_LEN];

	int token_num = 0;

	int index = 0;
	while (true)
	{
		while (IsWhitespace(code[index]))index++;

		for (int i = 0; i < token_count; ++i)
		{
			int lenght = strlen(types[i]->text);
			if (strncmp(&code[index], types[i]->text, lenght) == 0)
			{
				if (!IsWhitespace(code[index + strlen(types[i]->text)])) break;

				strncpy(tokens[token_num], types[i]->text, lenght);
				index += lenght;
				goto endofloop;
			}
		}

		if (IsLetter(code[index]))
		{
			int start = index;
			while (IsLetter(code[index]) || IsDigit(code[index])) ++index;
			strncpy(tokens[token_num], &code[start], index - start);

			goto endofloop;
		}

		for (int i = 0; i < token_num; ++i)
		{
			printf("%s\n", tokens[i]);
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
	InitToken("buffer"	, Identifier	);
	InitToken("{"		, OpenBracket	);
	InitToken("}"		, CloseBracket	);
	InitToken(";"		, SemiColon		);
}

void InitToken(const char* text, enum EToken type)
{
	//alocate the token
	types[token_count] = (struct STokenType*)malloc(sizeof(struct STokenType));
	//copying text
	types[token_count]->text = malloc((strlen(text)+1)*sizeof(char));
	strcpy_s(types[token_count]->text, strlen(text) + 1, text);
	//setting type
	types[token_count]->type = type;
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
