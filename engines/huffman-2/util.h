#pragma once

#include <cstdio>
#include <stdint.h>

namespace Huffman2 {
	void writeBit(FILE* fd, uint8_t bit);
	void finalizeWrite(FILE* fd);
	uint8_t readBit(FILE* fd);
	extern const size_t bufSize;
};
