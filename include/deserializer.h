#ifndef SONNEUR_DESERIALIZER_H
#define SONNEUR_DESERIALIZER_H

#include <stdint.h>
#include <stdlib.h>
#include <descriptor.h>

struct descriptor *descriptorFromBytes(uint8_t* bytes, size_t*size);

#define deserialize(type, data) type##FromDescriptor(descriptorFromBytes(data, NULL));

#endif //SONNEUR_DESERIALIZER_H