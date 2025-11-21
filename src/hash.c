#include <hash.h>

uint16_t ELFHash(char* str, const uint32_t length) {
	uint16_t hash = 0;
	uint16_t x = 0;
	for (int i = 0; i < length; str++, i++) {
		hash = (hash << 4) + *str;
		if ((x = hash & 0xF000L) != 0) {
			hash ^= x >> 24;
		}
		hash &= ~x;
	}
	return hash;
}