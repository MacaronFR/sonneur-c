#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sonneur.h>

#define struct_inner(field, struct_field)\
	field(int32_t, a)\
	field(float, b)

serializable_struct(inner, struct_inner);

#define struct_test(field, struct_field) \
	struct_field(inner, items)

serializable_struct(test, struct_test);

int main(void) {
	size_t size;
	test test2 = { .items = { .a = 259, .b = 158.f}};
	uint8_t *bytes = serialize(&test2, get_serializer(test), &size);
	printf("%zu\n", size);
	for (int i = 0; i < size; i++) {
		printf("%02x ", bytes[i]);
	}
	test test1 = deserialize(test, bytes);
	printf("\ntest: {\n\titems: {\n\t\ta: %d,\n\t\tb: %f\n\t}\n}", test1.items.a, test1.items.b);
	free(bytes);
	return 0;
}
