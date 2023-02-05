#ifndef struct_header
#define struct_header
#include <stdbool.h>

struct SProperty
{
	char* name;
	struct SStruct* type; // if it's null then it defines a type, if not then it's a type
};

struct SStruct
{
	char *name;

	bool generate_code;
	struct SProperty** properties;
	int property_num;
	int property_capacity;
};

struct SProperty* CreateProperty(char* name, struct SStruct* type);
void AddProperty(struct SStruct* structure, struct SProperty* property);
struct SStruct* CreateStruct(char* name, bool generate_code);
void DeleteStruct(struct SStruct* object);


#endif