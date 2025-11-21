#include <serializer.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint8_t* descriptorToBytes(struct descriptor desc, size_t* size);

uint8_t* sizeToBytes(const size_t size) {
	uint8_t* bytes;
	if (size < 255) {
		bytes = malloc(sizeof(uint8_t));
		bytes[0] = size;
	} else {
		bytes = malloc(sizeof(uint32_t) + 1);
		bytes[0] = 255;
		*(uint32_t*)(bytes + 1) = size;
	}
	return bytes;
}

uint8_t* objectToBytes(struct object *obj, size_t* size) {
	size_t inner_size = 0;
	size_t tmp_size;
	uint8_t *tmp;
	uint8_t *buf;
	uint8_t *bytes;
	if (obj->size < 255) {
		inner_size += 1;
	} else {
		inner_size += 5;
	}
	bytes = malloc(inner_size);
	uint8_t *size_bytes = sizeToBytes(obj->size);
	memcpy(bytes, size_bytes, inner_size);
	free(size_bytes);
	for (int i = 0; i < obj->size; i++) {
		buf = descriptorToBytes(obj->items[i], &tmp_size);
		tmp = malloc(inner_size + tmp_size);
		memcpy(tmp, bytes, inner_size);
		memcpy(tmp + inner_size, buf, tmp_size);
		free(buf);
		free(bytes);
		bytes = tmp;
		inner_size += tmp_size;
	}
	*size = inner_size;
	return bytes;
}

uint8_t* arrayToBytes(struct array *arr, size_t* size) {
	if (arr->type == ARRAY || arr->type == OBJECT) {
		fprintf(stderr, "Cannot serialize array of arrays or objects\n");
		return NULL;
	}
	size_t data_size;
	switch (arr->type) {
	case INT8: data_size = 1; break;
	case INT16: data_size = 2; break;
	case INT32: data_size = 4; break;
	case INT64: data_size = 8; break;
	case FLOAT32: data_size = 4; break;
	case FLOAT64: data_size = 8; break;
	default: return NULL;
	}
	uint8_t* bytes = malloc(arr->size * data_size);
	for (int i = 0; i < arr->size; i++) {
		memcpy(bytes + i * data_size, arr->data + i * data_size, data_size);
	}
	return bytes;
}

uint8_t* dataToBytes(struct descriptor desc, size_t* size) {
	uint8_t *bytes;
	switch (desc.type) {
	case INT8: bytes = malloc(sizeof(uint8_t)); *bytes = desc.data.int8; *size = sizeof(uint8_t); break;
	case INT16: bytes = malloc(sizeof(uint16_t)); *(uint16_t*)bytes = desc.data.int16; *size = sizeof(uint16_t); break;
	case INT32: bytes = malloc(sizeof(uint32_t)); *(uint32_t*)bytes = desc.data.int32; *size = sizeof(uint32_t); break;
	case INT64: bytes = malloc(sizeof(uint64_t)); *(uint64_t*)bytes = desc.data.int64; *size = sizeof(uint64_t); break;
	case FLOAT32: bytes = malloc(sizeof(float)); *(float*)bytes = desc.data.float32; *size = sizeof(float); break;
	case FLOAT64: bytes = malloc(sizeof(double)); *(double*)bytes = desc.data.float64; *size = sizeof(double); break;
	case ARRAY: bytes = arrayToBytes(desc.data.array, size); break;
	case OBJECT: bytes = objectToBytes(desc.data.object, size); break;
	default: bytes = NULL;
	}
	return bytes;
}

uint8_t* arrayDescriptorToBytes(struct descriptor desc, size_t* size) {
	size_t inner_size = 3;
	size_t tmp_size;
	uint8_t* tmp;
	uint8_t* buf;
	uint8_t* bytes;
	if (desc.data.object->size < 255) {
		inner_size += 1;
	} else {
		inner_size += 5;
	}
	bytes = malloc(inner_size);
	*((uint16_t*)bytes) = desc.id;
	bytes[2] = desc.type;
	uint8_t * size_bytes = sizeToBytes(desc.data.object->size);
	memcpy(bytes + 3, size_bytes, inner_size - 3);
	free(size_bytes);
	for (int i = 0; i < desc.data.array->size; i++) {
		buf = dataToBytes(desc, &tmp_size);
		tmp = malloc(inner_size + tmp_size);
		memcpy(tmp, bytes, inner_size);
		memcpy(tmp + inner_size, buf, tmp_size);
		free(buf);
		free(bytes);
		bytes = tmp;
		inner_size += tmp_size;
	}
	*size = inner_size;
	return bytes;
}

uint8_t* objectDescriptorToBytes(struct descriptor desc, size_t* size) {
	uint8_t *data = objectToBytes(desc.data.object, size);
	uint8_t *bytes = malloc(3 + *size);
	memcpy(bytes + 3, data, *size);
	free(data);
	*(uint16_t*)bytes = desc.id;
	bytes[2] = desc.type;
	*size += 3;
	return bytes;
}

uint8_t* descriptorToBytes(struct descriptor desc, size_t* size) {
	uint8_t *bytes;
	uint8_t *data = dataToBytes(desc, size);
	if (data == NULL) {
		return NULL;
	}
	bytes = malloc(3 + *size);
	memcpy(bytes + 3, data, *size);
	free(data);
	*(uint16_t*)bytes = desc.id;
	*(bytes + 2) = desc.type;
	*size += 3;
	return bytes;
}

uint8_t *serialize(void *data, struct descriptor (*serializer)(const void *, char *), size_t* size) {
	struct descriptor desc = serializer(data, NULL);
	uint8_t *bytes;
	size_t tmp_size = 0;
	if (size != NULL) {
		bytes = descriptorToBytes(desc, size);
	} else {

		bytes = descriptorToBytes(desc, &tmp_size);
	}
	freeDescriptor(desc);
	return bytes;
}
