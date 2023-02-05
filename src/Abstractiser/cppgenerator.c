#include "cppgenerator.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "buffer.h"
#include "Abstractiser/lexer.h"
#include "Abstractiser/struct.h"

void GenerateHeaderData(struct SBuffer* buffer, const struct SCodeData* data);
void GenerateCPPData(struct SBuffer* buffer, const struct SCodeData* data);
void PutDataToFile(const char* code, char* path);
char* GeneratePath(const char* path, char* extension);

void GenerateCPPFiles(const char* path, struct SCodeData* data)
{
	struct SBuffer header_buffer, cpp_buffer;
	InitBuffer(&header_buffer);
	InitBuffer(&cpp_buffer);

	GenerateHeaderData(&header_buffer	, data);
	GenerateCPPData(&cpp_buffer		, data);

	char* path_to_header = GeneratePath(path, "generated.h");
	char* path_to_cpp	 = GeneratePath(path, "generated.cpp");

	PutDataToFile(header_buffer.string	, path_to_header);
	PutDataToFile(cpp_buffer.string		, path_to_cpp);

	free(path_to_header);
	free(path_to_cpp);
}

char* GeneratePath(const char* path, char* extension)
{
	size_t path_len = strnlen(path, 258);
	size_t ext_len = strnlen(extension, 258);
	if (path_len == 258 || ext_len == 258)
		return NULL;

	size_t lenght = path_len + ext_len+2;
	char* output = (char*)malloc(lenght);
	snprintf(output, lenght+1, "%s.%s", path, extension);
	return output;
}

void PutDataToFile(const char* code, char* path)
{
	FILE *file = fopen(path, "w");
	int results = fputs(code, file);
	if (results == EOF) {
		printf("Failed to print file: %s\n", path);
	}
	fclose(file);
}

void GenerateHeaderData(struct SBuffer* buffer, const struct SCodeData* data)
{
	AppendToBuffer(buffer, "#include <stddef.h>\n");

	//generating the structure itself
	for (int i = 0; i < data->struct_num; ++i)
	{
		//normal struct things
		struct SStruct* current = data->structures[i];
		
		if (!current->generate_code) continue;

		AppendToBuffer(buffer, "class ");
		AppendToBuffer(buffer, current->name);
		AppendToBuffer(buffer, " : CObject ");
		AppendToBuffer(buffer, "{");

		AppendToBuffer(buffer, "public:");
		for (int j = 0; j < current->property_num; ++j)
		{
			struct SProperty *property = current->properties[j];
			AppendToBuffer(buffer, property->type->name);
			AppendToBuffer(buffer, " ");
			AppendToBuffer(buffer, property->name);
			AppendToBuffer(buffer, "; ");
		}


		//reflection code
		AppendToBuffer(buffer, "private: static class ");
		AppendToBuffer(buffer, current->name);
		AppendToBuffer(buffer, "_class { virtual void Init() override { ");\
		for (int j = 0; j < current->property_num; ++j)
		{
			struct SProperty* property = current->properties[j];
			AppendToBuffer(buffer, "AddToReflection(\"");
			AppendToBuffer(buffer, property->type->name);
			AppendToBuffer(buffer, "\",offsetof(class ");
			AppendToBuffer(buffer, current->name);
			AppendToBuffer(buffer, ",");
			AppendToBuffer(buffer, property->name);
			AppendToBuffer(buffer, ");");
		}
		AppendToBuffer(buffer, "}");
		AppendToBuffer(buffer, "}class_type;");

		AppendToBuffer(buffer, "static CClass* StructInfo() { return class_type; }");
		AppendToBuffer(buffer, "virtual CClass* GetClass() const override { return StructInfo(); }");

		AppendToBuffer(buffer, "virtual std::uint64 GetTypeID() override { return ");

		char IDStr[5];
		sprintf(IDStr, "%d", i);
		AppendToBuffer(buffer, IDStr);
		AppendToBuffer(buffer, ";}");



		AppendToBuffer(buffer, "}");
		AppendToBuffer(buffer, "\n");
	}
}

void GenerateCPPData(struct SBuffer* buffer, const struct SCodeData* data)
{

}
