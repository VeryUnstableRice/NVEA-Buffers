#ifndef buffer_header
#define buffer_header

struct SBuffer
{
	char* string;
	size_t size;
	size_t capacity;
};

void InitBuffer(struct SBuffer* buffer);
void AppendToBuffer(struct SBuffer* buffer, const char* string);
void FreeBuffer(struct SBuffer* buffer);

#endif