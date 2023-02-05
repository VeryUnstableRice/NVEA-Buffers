#include "buffer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void InitBuffer(struct SBuffer* buffer)
{
	buffer->capacity = 10;
	buffer->size = 0;
	buffer->string = (char*)malloc(buffer->capacity * sizeof(char));
	memset(buffer->string, 0, buffer->capacity * sizeof(char));
	if (!buffer->string)
	{
		printf("Error allocating memory.\n");
		exit(1);
	}
}

void AppendToBuffer(struct SBuffer* buffer, const char* string)
{
	size_t lenght = strlen(string);
	if (buffer->size + lenght >= buffer->capacity)
	{
		buffer->capacity = (buffer->size + lenght)*2;
		buffer->string = (char*)realloc(buffer->string, buffer->capacity * sizeof(char));
	}
	strncpy(buffer->string + buffer->size, string, lenght+1);
	buffer->size += lenght;
}

void FreeBuffer(struct SBuffer* buffer)
{
	free(buffer->string);
}
