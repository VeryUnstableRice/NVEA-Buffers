#include "lexer.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "struct.h"

struct STokenType* types[100];
int token_count;


struct STokenType* VariableNameToken = NULL;
struct STokenType* IntToken = NULL;
struct STokenType* StructToken = NULL;
struct STokenType* OpenBraketToken = NULL;
struct STokenType* CloseBraketToken = NULL;
struct STokenType* SemicolonToken = NULL;


bool MatchesToken(const char* token);
bool IsDigit(char character);
bool IsLetter(char character);
bool IsWhitespace(char character);

void FlagError(char* token, char* expected)
{
	printf("Unexpected token: %s, expected %s\n", token, expected);
	fflush(stdout);
	exit(69);
}

void SumLexin(const char* code, int* token_num, struct SToken(*tokens)[MAX_TOKENS]);
void ProcessTokens(const int* token_num, struct SToken(*tokens)[MAX_TOKENS]);
void PrintTokens(const int* token_num, struct SToken(*tokens)[MAX_TOKENS]);

void Abstractise(const char* code)
{
	static struct SToken tokens[MAX_TOKENS];
	int token_num = 0;

	SumLexin(code, &token_num, &tokens);
	ProcessTokens(&token_num, &tokens);
}

void PrintTokens(const int* token_num, struct SToken(*tokens)[MAX_TOKENS])
{
	for (int i = 0; i < *token_num; ++i)
	{
		printf("Token: %s\n", (*tokens)[i].code);
	}
}

enum EProcessingStage
{
	None,

	FoundStructure,
	StructureNamedEstablished,

	OpeningStructBody,
	ConstructingStruct,

	ConstructingIdentifier,
	ExpectingVariableName,
	FinishingVariableDecl,

	ClosingStructBody
};

//todo: memory manage
void ProcessTokens(const int* token_num, struct SToken (*tokens)[MAX_TOKENS])
{
	struct SStruct* structures[10];
	int struct_num = 0;
	int current_token = 0;
	enum EProcessingStage tree_stage = None;

	struct SStruct* root_structure = NULL;
	struct SStruct* variable = NULL;
	for (;current_token < *token_num; ++current_token)
	{
		struct SToken* current = *tokens + current_token;
		if (tree_stage == None)
		{
			if (current->type->type == Struct)
			{
				tree_stage = FoundStructure;
			}
			else
			{
				FlagError(current->code, "a structure");
			}
			continue;
		}

		if (tree_stage == FoundStructure)
		{
			if (current->type->type == VariableName)
			{
				root_structure = CreateStruct(current->code);
				tree_stage = StructureNamedEstablished;
			}
			else
			{
				FlagError(current->code, "a name");
			}
			continue;
		}

		if (tree_stage == StructureNamedEstablished)
		{
			if (current->type->type == OpenBracket)
			{
				tree_stage = ConstructingStruct;
			}
			else
			{
				FlagError(current->code, "an opening bracket");
			}
			continue;
		}

		if (tree_stage == ConstructingStruct)
		{
			if (current->type->type == Identifier)
			{
				tree_stage = ConstructingIdentifier;
				//todo remember identifier type
			}
			else if(current->type->type == CloseBracket)
			{
				tree_stage = None;
				//finished building structure, adding it to the list
				structures[struct_num] = root_structure;
				root_structure = NULL;
				++struct_num;
			}
			else
			{
				FlagError(current->code, "an identifier or a closing bracket");
			}
			continue;
		}

		if (tree_stage == ConstructingIdentifier)
		{
			if (current->type->type == VariableName)
			{
				tree_stage = FinishingVariableDecl;
				variable = CreateStruct(current->code);
			}
			else
			{
				FlagError(current->code, "a variable name");
			}

			continue;
		}

		if (tree_stage == FinishingVariableDecl)
		{
			if (current->type->type == SemiColon)
			{
				tree_stage = ConstructingStruct;
				AddProperty(root_structure, variable);
				variable = NULL;
			}
			else
			{
				FlagError(current->code, "a semicolon");
			}
			continue;
		}
	}
}

/*
// TODO: Make this an actual tree, not a bunch of ifs
void ProcessTokens(const int* token_num, struct SToken(*tokens)[MAX_TOKENS])
{
	#define INC_TOKEN() if(++current_token >= *token_num) return

	struct SStruct* structures[10];
	int struct_num = 0;

	int current_token = 0;
	while (current_token < *token_num)
	{
		//we found a struct
		if ((*tokens)[current_token].type->type == Struct)
		{
			struct SStruct* structure = NULL;
			INC_TOKEN();
			//we expect a name
			if ((*tokens)[current_token].type->type == VariableName)
			{
				structure = CreateStruct((*tokens)[current_token].code);
				//now we look for the open bracket
				INC_TOKEN();
				if ((*tokens)[current_token].type->type == OpenBracket)
				{
					INC_TOKEN();
					while ((*tokens)[current_token].type->type == Identifier)
					{
						INC_TOKEN();
						if ((*tokens)[current_token].type->type == VariableName)
						{
							struct SStruct* struct_property = CreateStruct((*tokens)[current_token].code);
							INC_TOKEN();
							if ((*tokens)[current_token].type->type == SemiColon)
							{
								AddProperty(structure, struct_property);
							}
							else
							{
								DeleteStruct(struct_property);
								char error[528];
								sprintf(error, "Unexpected token: %s, expected a semicolon", (*tokens)[current_token].code);
								FlagError(error);
							}
						}
						else
						{
							char error[528];
							sprintf(error, "Unexpected token: %s, expected a name to identifier", (*tokens)[current_token].code);
							FlagError(error);
						}
					}
					structures[struct_num] = structure;
					++struct_num;
					//if ((*tokens)[current_token].type->type == )
				}
				else
				{
					char error[528];
					sprintf(error, "Unexpected token: %s, expected a open bracket", (*tokens)[current_token].code);
					FlagError(error);
				}
			}
			else
			{
				char error[528];
				sprintf(error, "Unexpected token: %s, expected a name", (*tokens)[current_token].code);
				FlagError(error);
			}
		}
		INC_TOKEN();
	}

	#undef INC_TOKEN()

}*/

void SumLexin(const char* code, int* token_num, struct SToken(*tokens)[MAX_TOKENS])
{
	int index = 0;
	while (true)
	{
		// skip all the whitespaces
		while (IsWhitespace(code[index]))index++;

		for (int i = 0; i < token_count; ++i)
		{
			if (types[i]->text == NULL)
				continue;
			int lenght = strlen(types[i]->text);

			//checking if it matches the token
			if (strncmp(&code[index], types[i]->text, lenght) != 0) continue;

			//checking if the token has anything afterwards, it could be a name that contains this
			if (!IsWhitespace(code[index + strlen(types[i]->text)])) break;


			strncpy((*tokens)[*token_num].code, types[i]->text, lenght);
			(*tokens)[*token_num].type = types[i];
			index += lenght;

			goto endofloop;
		}

		//this token is a name token, so there is nothing to check just copy
		if (IsLetter(code[index]))
		{
			int start = index;
			while (IsLetter(code[index]) || IsDigit(code[index])) ++index;
			strncpy((*tokens)[*token_num].code, &code[start], index - start);
			(*tokens)[*token_num].type = VariableNameToken;
			goto endofloop;
		}
		break;

	endofloop:
		*token_num += 1;
	}
}

//init tokens
struct STokenType* InitToken(const char* text, enum EToken type);

void InitTokenTypes()
{
	token_count = 0;
	StructToken			= InitToken("struct"	, Struct		);
	IntToken			= InitToken("int"		, Identifier	);
	OpenBraketToken		= InitToken("{"			, OpenBracket	);
	CloseBraketToken	= InitToken("}"			, CloseBracket	);
	SemicolonToken		= InitToken(";"			, SemiColon		);
	VariableNameToken	= InitToken(NULL		, VariableName  );
}

struct STokenType* InitToken(const char* text, enum EToken type)
{
	//alocate the token
	types[token_count] = (struct STokenType*)malloc(sizeof(struct STokenType));
	//setting type
	types[token_count]->type = type;

	if (text)
	{
		//copying text
		types[token_count]->text = malloc((strlen(text) + 1) * sizeof(char));
		strcpy_s(types[token_count]->text, strlen(text) + 1, text);
	}
	else 
		types[token_count]->text = NULL;

	//increasing count and returning
	return types[token_count++];
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

//functions
inline bool MatchesToken(const char* token)
{
	for (int i = 0; i < token_count; ++i)
	{
		if (strcmp(token, types[i]->text) == 0)
			return true;
	}
	return false;
}

inline bool IsLetter(char character)
{
	return (character >= 'a' && character <= 'z') || (character >= 'A' && character <= 'Z');;
}

inline bool IsDigit(char character)
{
	return character >= '0' && character <= '9';
}

inline bool IsWhitespace(char character)
{
	return character == ' ' || character == '\t' || character == '\n';
}