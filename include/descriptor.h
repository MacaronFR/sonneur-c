#ifndef SONNEUR_DESCRIPTOR_H
#define SONNEUR_DESCRIPTOR_H
#include <stddef.h>
#include <stdint.h>

enum type {
	INT8 = 1,
	INT16 = 2,
	INT32 = 3,
	INT64 = 4,
	FLOAT32 = 5,
	FLOAT64 = 6,
	ARRAY = 7,
	OBJECT = 8
};

struct array {
	enum type type;
	size_t size;
	void* data;
};

struct object {
	size_t size;
	struct descriptor* items;
};

union data {
	uint8_t int8;
	uint16_t int16;
	uint32_t int32;
	uint64_t int64;
	float float32;
	double float64;
	struct array* array;
	struct object* object;
};

struct descriptor {
	uint16_t id;
	enum type type;
	union data data;
};

void freeDescriptor(struct descriptor desc);

#endif //SONNEUR_DESCRIPTOR_H