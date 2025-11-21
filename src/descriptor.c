#include <descriptor.h>
#include <stdlib.h>

void freeObject(struct object obj);

void freeArray(struct array arr) {
	for (int i = 0; i < arr.size; i++) {
		if (arr.type == ARRAY) {
			freeArray(((struct array*)arr.data)[i]);
		} else if (arr.type == OBJECT) {
			freeObject(((struct object*)arr.data)[i]);
		}
	}
}

void freeObject(struct object obj) {
	for (int i = 0; i < obj.size; i++) {
		freeDescriptor(obj.items[i]);
	}
	free(obj.items);
}

void freeDescriptor(struct descriptor desc) {
	switch (desc.type) {
	case INT8:
	case INT16:
	case INT32:
	case INT64:
	case FLOAT32:
	case FLOAT64: break;
	case ARRAY: freeArray(*desc.data.array); free(desc.data.array); break;
	case OBJECT: freeObject(*desc.data.object); free(desc.data.object); break;
	}
}