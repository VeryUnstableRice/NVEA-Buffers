#include <stdlib.h>
#include "struct.h"
#include "string.h"

void AddProperty(struct SStruct* structure, struct SProperty* property)
{
	//if capacity exceded, allocate more
	if (structure->property_num + 1 >= structure->property_capacity)
	{
		structure->property_capacity = (structure->property_num + 1) * 2;
		structure->properties = (struct SProperty**)realloc(structure->properties, structure->property_capacity * sizeof(struct SProperty*));
	}
	//add to the array
	structure->properties[structure->property_num] = property;
	++structure->property_num;
}

struct SProperty* CreateProperty(char* name, struct SStruct* type)
{
	struct SProperty* output = malloc(sizeof(struct SProperty));
	output->name = strdup(name);
	output->type = type;
	return output;
}

struct SStruct* CreateStruct(char* name, bool generate_code)
{
	struct SStruct* output = malloc(sizeof(struct SStruct));
	//strcpy(output->name, name);
	output->property_num = 0;
	output->generate_code = generate_code;
	output->properties = (struct SProperty **)malloc(sizeof(struct SProperty**));
	output->name = strdup(name);
	return output;
}

void DeleteStruct(struct SStruct* object)
{
	for (int i = 0; i < object->property_num; ++i)
		free(object->properties[i]);
	free(object->properties);
	free(object->name);
	free(object);
}
