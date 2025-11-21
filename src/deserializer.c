#include <deserializer.h>
#include <stdio.h>

union data dataFromBytes(uint8_t* bytes, enum type type, size_t *size);

size_t sizeForType(enum type type) {
	switch (type) {
		case INT8: return sizeof(uint8_t);
		case INT16: return sizeof(uint16_t);
		case INT32: return sizeof(uint32_t);
		case INT64: return sizeof(uint64_t);
		case FLOAT32: return sizeof(float);
		case FLOAT64: return sizeof(double);
		case ARRAY: return sizeof(void *);
		case OBJECT: return sizeof(struct descriptor *);
		default: return 0;
	}
}

size_t getSize(uint8_t *bytes) {
	if (bytes[0] < 255) {
		return bytes[0];
	}
	return *(uint32_t*)(bytes + 1);
}

struct descriptor *descriptorFromBytes(uint8_t* bytes, size_t *size) {
	struct descriptor *desc = malloc(sizeof(struct descriptor));
	desc->id = *(uint16_t*)bytes;
	desc->type = bytes[2];
	desc->data = dataFromBytes(bytes + 3, desc->type, size);
	if (size != NULL) {
		*size += 3;
	}
	return desc;
}

void assignToArray(void *array, enum type type, union data data) {
	switch (type) {
		case INT8: *(uint8_t*)array = data.int8; break;
		case INT16: *(uint16_t*)array = data.int16; break;
		case INT32: *(uint32_t*)array = data.int32; break;
		case INT64: *(uint64_t*)array = data.int64; break;
		case FLOAT32: *(float*)array = data.float32; break;
		case FLOAT64: *(double*)array = data.float64; break;
		case ARRAY: *(void**)array = data.array; break;
		case OBJECT: *(struct object**)array = data.object; break;
	}
}

union data dataFromBytes(uint8_t* bytes, enum type type, size_t *size) {
	size_t start;
	if (size != NULL) {
		*size += sizeForType(type);
	}
	switch (type) {
	case INT8: return (union data){.int8 = *bytes};
	case INT16: return (union data){.int16 = *(uint16_t*)bytes};
	case INT32: return (union data){.int32 = *(uint32_t*)bytes};
	case INT64: return (union data){.int64 = *(uint64_t*)bytes};
	case FLOAT32: return (union data){.float32 = *(float*)bytes};
	case FLOAT64: return (union data){.float64 = *(double*)bytes};
	case ARRAY: {
		if (size != NULL) {
			*size -= sizeForType(ARRAY);
		}
		struct array *arr = malloc(sizeof(struct array));
		size_t sizeOfType = sizeForType(arr->type);
		start = 1 + (arr->size < 255 ? 1 : 5);
		arr->type = bytes[0];
		arr->size = getSize(bytes + 1);
		arr->data = malloc(arr->size * sizeOfType);
		for (int i = 0; i < arr->size; i++) {
			assignToArray(arr->data + i * sizeOfType, arr->type, dataFromBytes(bytes + start + i * sizeOfType, arr->type, size));
		}
		return (union data){.array = arr};
	}
	case OBJECT: {
		if (size != NULL) {
			*size -= sizeForType(OBJECT);
		}
		struct object *obj = malloc(sizeof(struct object));
		obj->size = getSize(bytes);
		obj->items = malloc(obj->size * sizeof(struct descriptor));
		start = obj->size < 255 ? 1 : 5;
		for (int i = 0; i < obj->size; i++) {
			size_t delta = 0;
			struct descriptor *tmp = descriptorFromBytes(bytes + start, &delta);
			start += delta;
			obj->items[i] = *tmp;
			free(tmp);
		}
		if (size != NULL) {
			*size += start;
		}
		return (union data){.object = obj};
	}
	}
}