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
struct STokenType* FloatToken = NULL;
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
void ProcessTokens(const int* token_num, struct SToken* tokens, struct SStruct* structures[], int* struct_num);
void PrintTokens(const int* token_num, struct SToken(*tokens)[MAX_TOKENS]);

void Abstractise(const char* code, struct SCodeData* codeData)
{	
	SumLexin(code, &codeData->token_num, &codeData->tokens);
	ProcessTokens(&codeData->token_num, codeData->tokens, &codeData->structures, &codeData->struct_num);
}

void PrintTokens(const int* token_num, struct SToken(*tokens)[MAX_TOKENS])
{
	for (int i = 0; i < *token_num; ++i)
	{
		printf("Token: %s\n", (*tokens)[i].code);
	}
}

//todo: memory management
void ProcessTokens(const int* token_num, struct SToken* tokens, struct SStruct* structures[], int* struct_num)
{
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

	int current_token = 0;
	enum EProcessingStage tree_stage = None;

	struct SStruct* root_structure = NULL;
	struct SStruct* variable = NULL;
	for (;current_token < *token_num; ++current_token)
	{
		struct SToken* current = tokens + current_token;
		if (tree_stage == None)
		{ 
			if (current->type->type == Struct)
			{
				tree_stage = FoundStructure;
				continue;
			}
			FlagError(current->code, "a structure");
			continue;
		}

		if (tree_stage == FoundStructure)
		{
			if (current->type->type == VariableName)
			{
				root_structure = CreateStruct(current->code);
				tree_stage = StructureNamedEstablished;
				continue;
			}
			FlagError(current->code, "a name");
			continue;
		}

		if (tree_stage == StructureNamedEstablished)
		{
			if (current->type->type == OpenBracket)
			{
				tree_stage = ConstructingStruct;
				continue;
			}
			FlagError(current->code, "an opening bracket");
			continue;
		}

		if (tree_stage == ConstructingStruct)
		{
			if (current->type->type == Identifier)
			{
				tree_stage = ConstructingIdentifier;
				//todo remember identifier type
				continue;
			}
			if (current->type->type == CloseBracket)
			{
				tree_stage = None;
				//finished building structure, adding it to the list
				structures[*struct_num] = root_structure;
				root_structure = NULL;
				*struct_num += 1;
				continue;
			}
			FlagError(current->code, "an identifier or a closing bracket");
		}

		if (tree_stage == ConstructingIdentifier)
		{
			if (current->type->type == VariableName)
			{
				tree_stage = FinishingVariableDecl;
				variable = CreateStruct(current->code);
				continue;
			}
			FlagError(current->code, "a variable name");
			continue;
		}

		if (tree_stage == FinishingVariableDecl)
		{
			if (current->type->type == SemiColon)
			{
				tree_stage = ConstructingStruct;
				AddProperty(root_structure, variable);
				variable = NULL;
				continue;
			}
			FlagError(current->code, "a semicolon");
			continue;
		}
	}

	if(root_structure)
		FlagError(root_structure->name, "an closing bracket");
	return;
}

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
			int total_lenght = index + strlen(types[i]->text);
			if(total_lenght > strlen(code) && !IsWhitespace(code[total_lenght])) break;

			(*tokens)[*token_num].code = strdup(types[i]->text);
			//strncpy((*tokens)[*token_num].code, types[i]->text, lenght);
			(*tokens)[*token_num].type = types[i];
			index += lenght;

			goto endofloop;
		}

		//this token is a name token, so there is nothing to check just copy
		if (IsLetter(code[index]))
		{
			int start = index;
			while (IsLetter(code[index]) || IsDigit(code[index])) ++index;
			int lenght = index - start;
			char* temp = malloc(lenght +1);
			strncpy(temp, &code[start], lenght);
			temp[lenght] = 0;
			(*tokens)[*token_num].code = temp;
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
	FloatToken			= InitToken("float"		, Identifier	);
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
	return (character >= 'a' && character <= 'z') || (character == '_') || (character >= 'A' && character <= 'Z');;
}

inline bool IsDigit(char character)
{
	return character >= '0' && character <= '9';
}

inline bool IsWhitespace(char character)
{
	return character == ' ' || character == '\t' || character == '\n';
}