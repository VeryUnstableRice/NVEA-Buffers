#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Abstractiser/lexer.h"


char help[] =
"-cpp -> produces code for c++ \n\
-f -> \"/path/to/file\"\n\
-h -> help\n\
";

int fakemain(int argc, char** argv);

int main(int argc, char** argv)
{
	char* tempargc[] =
	{
		argv[0],
		"-cpp",
		"-f",
		"C:\\Users\\Truncated\\source\\repos\\NVEABuffers\\example.nveab"
	};

	fakemain(4, tempargc);
}

int fakemain(int argc, char** argv)
{
	FILE* file = NULL;

	enum ELanguageType
	{
		LT_None,
		LT_CPP,
	};

	enum ELanguageType language = LT_None;

	enum ECommandTree
	{
		CT_None,
		CT_SelectFile,
	}current_tree = CT_None;

	for (int i = 0; i < argc; ++i)
	{
		if (current_tree == CT_None)
		{
			if (argc == 1 || strcmp(argv[i], "-h") == 0)
			{
				printf("%s\n", help);
				return;
			}

			if (strcmp(argv[i], "-cpp") == 0)
			{
				current_tree = CT_None;
				language = LT_CPP;
			}
			else if (strcmp(argv[i], "-f") == 0)
			{
				current_tree = CT_SelectFile;
			}
			continue;
		}

		if (current_tree == CT_SelectFile)
		{
			file = fopen(argv[i], "r");
			if (file == NULL)
			{
				printf("File: %s is not valid\n", argv[i]);
				return;
			}
			else {
				printf("File: %s has been opened successfully\n", argv[i]);
			}
			current_tree = CT_None;
		}
	}

	if (!file)
	{
		return;
	}

	char* code = NULL;
	fseek(file, 0, SEEK_END);
	int file_size = ftell(file);
	rewind(file);
	//fseek(file, 0, SEEK_SET);

	code = malloc(file_size * sizeof(char));
	fread(code, sizeof(char), file_size, file);
	fclose(file);

	InitTokenTypes();
	struct SCodeData codeData;
	memset(&codeData, 0, sizeof codeData);
	Abstractise(code, &codeData);
	free(code);
	for (int i = 0; i < codeData.token_num; ++i)
	{
		free(codeData.tokens[i].code);
	}
	FreeTokens();
}