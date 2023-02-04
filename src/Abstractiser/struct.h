#ifndef struct_header
#define struct_header

extern struct SStruct* IntPrimitive;
 
struct SStruct
{
	char *name;
	struct SStruct* Properties[100];
	int PropertyNum;
};

void AddProperty(struct SStruct* structure, struct SStruct* property);
struct SStruct* CreateStruct(char* name);
void DeleteStruct(struct SStruct* object);


#endif