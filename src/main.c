#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Abstractiser/lexer.h"
#include "Abstractiser/struct.h"
#include "Abstractiser/cppgenerator.h"


char help[] =
"-cpp -> produces code for c++ \n\
-f -> \"/path/to/input\"\n\
-o -> \"path/to/output\"\n\
-h -> help\n\
";

int fakemain(int argc, char** argv);
char* GetCodeFromFile(FILE* file);

int main(int argc, char** argv)
{
	char* tempargc[] =
	{
		argv[0],
		"-cpp",
		"-f",
		"C:\\Users\\Truncated\\source\\repos\\NVEABuffers\\example.nveab"
	};

	return fakemain(4, tempargc);
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

	if (!file || language == LT_None)
	{
		return 1;
	}

	//reading the code from file to char array
	char* code = GetCodeFromFile(file);

	//turning the code into abstracted data

	//initialising data
	struct SCodeData codeData;
	memset(&codeData, 0, sizeof codeData);

	InitTokenTypes(&codeData);

	Abstractise(code, &codeData);
	
	//turning the abstracted data into generated code
	switch (language)
	{
	case LT_CPP:
		GenerateCPPFiles("C:\\Users\\Truncated\\source\\repos\\NVEABuffers\\example", &codeData);
		break;
	}
	
	//freeing memory
	free(code);
	for (int i = 0; i < codeData.token_num; ++i)
		free(codeData.tokens[i].code);
	for (int i = 0; i < codeData.struct_num; ++i)
		free(codeData.structures[i]);
	FreeTokens();
	return 0;
}

char* GetCodeFromFile(FILE* file)
{
	char* code = NULL;
	fseek(file, 0, SEEK_END);
	int file_size = ftell(file);
	rewind(file);

	code = malloc(file_size * sizeof(char));
	fread(code, sizeof(char), file_size, file);
	fclose(file);
	return code;
}
