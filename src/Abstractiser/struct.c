#include <stdlib.h>
#include "struct.h"
#include "string.h"

void AddProperty(struct SStruct* structure, struct SStruct* property)
{
	structure->Properties[structure->PropertyNum] = property;
	++structure->PropertyNum;
}

struct SStruct* CreateStruct(char* name)
{
	struct SStruct* output = malloc(sizeof(struct SStruct));
	//strcpy(output->name, name);
	output->PropertyNum = 0;
	strcpy_s(output->name, 100, name);
	return output;
}

void DeleteStruct(struct SStruct* object)
{
	for (int i = 0; i < object->PropertyNum; ++i)
		free(object->Properties[i]);
	free(object);
}
