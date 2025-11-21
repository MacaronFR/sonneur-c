#ifndef SONNEUR_SERIALIZER_H
#define SONNEUR_SERIALIZER_H

#include <descriptor.h>
#include <hash.h>

uint8_t *serialize(void *data, struct descriptor (*serializer)(const void *, char *), size_t* size);

#define get_serializer(name) name##ToDescriptor

#define TYPE_int8_t INT8
#define TYPE_uint8_t INT8
#define TYPE_int16_t INT16
#define TYPE_uint16_t INT16
#define TYPE_int INT32
#define TYPE_int32_t INT32
#define TYPE_uint32_t INT32
#define TYPE_int64_t INT64
#define TYPE_uint64_t INT64
#define TYPE_float FLOAT32
#define TYPE_double FLOAT64
#define TYPE_char INT8

#define ACCESS_int8_t int8
#define ACCESS_uint8_t int8
#define ACCESS_int16_t int16
#define ACCESS_uint16_t int16
#define ACCESS_int int32
#define ACCESS_int32_t int32
#define ACCESS_uint32_t int32
#define ACCESS_int64_t int64
#define ACCESS_uint64_t int64
#define ACCESS_float float32
#define ACCESS_double float64
#define ACCESS_char int8

#define test(a) a

#define serializable_struct(name, field_list) \
	const enum type TYPE_##name = OBJECT;\
	typedef struct {\
		field_list(FIELD_DECL, FIELD_DECL)\
	} name;\
	struct descriptor name##ToDescriptor(const void *data, char *desc_name) {\
		char *n = desc_name == NULL ? "root" : desc_name;\
		const name *s = (const name *)data;\
		struct object *obj = malloc(sizeof(struct object));\
		obj->size = field_list(COUNT_FIELDS, COUNT_FIELDS);\
		obj->items = malloc(sizeof(struct descriptor) * obj->size);\
		size_t idx = 0; \
		field_list(FIELD_SERIALIZE, STRUCT_FIELD_SERIALIZE) \
		return (struct descriptor){ \
			.id = ELFHash(n, strlen(n)), \
			.type = OBJECT, \
			.data.object = obj \
		}; \
	}\
	name name##FromDescriptor(struct descriptor *desc) {\
		name result = {};\
		for(int i = 0; i < desc->data.object->size; ++i) {\
			field_list(ASSIGN_FIELD, ASSIGN_STRUCT_FIELD)\
		}\
		free(desc);\
		return result;\
	}\

#define FIELD_DECL(type, name) type name;

#define COUNT_FIELDS(type, name) +1

#define FIELD_SERIALIZE(t, name) \
	obj->items[idx].id = ELFHash(#name, strlen(#name));\
	obj->items[idx].type = TYPE_##t; \
	obj->items[idx].data.ACCESS_##t = s->name;\
	idx++;

#define STRUCT_FIELD_SERIALIZE(t, name)\
	obj->items[idx].id = ELFHash(#name, strlen(#name));\
	obj->items[idx].type = OBJECT;\
	obj->items[idx].data.object = t##ToDescriptor(&s->name, #name).data.object;\
	idx++;

#define ASSIGN_FIELD(type, name) \
	if(ELFHash(#name, strlen(#name)) == desc->data.object->items[i].id) result.name = desc->data.object->items[i].data.ACCESS_##type;

#define ASSIGN_STRUCT_FIELD(type, name)\
	if(ELFHash(#name, strlen(#name)) == desc->data.object->items[i].id) {\
		result.name = type##FromDescriptor(&desc->data.object->items[i]);\
	}

#define CAST_INT8(val) (uint8_t)(val)
#define CAST_INT16(val) (uint16_t)(val)
#define CAST_INT32(val) (uint32_t)(val)
#define CAST_INT64(val) (uint64_t)(val)
#define CAST_FLOAT32(val) (float)(val)
#define CAST_FLOAT64(val) (double)(val)

#endif //SONNEUR_SERIALIZER_H