#include "util.h"
#include "stats.h"

namespace Huffman2 {
	const size_t bufSize = 1024;

	/**
	 * Writes the bit using bufferized output.
	 * Caches 8 bits, then writes
	 */
	uint8_t wBitBuff = 0;
	uint8_t wBitPtr = 0;
	void writeBit(FILE* fd, uint8_t bit) {
		uint8_t mask = (bit << wBitPtr);
		wBitBuff |= mask;
		wBitPtr++;
		if (wBitPtr >= 8) {
			fwrite(&wBitBuff, 1, 1, fd);
			wBitBuff = 0;
			wBitPtr = 0;
		}
		stats.bitsWritten++;
	}
	void finalizeWrite(FILE* fd) {
		while (wBitPtr) writeBit(fd, 0);
	}

	uint8_t rBuff = 0;
	uint8_t rPtr = 8;
	uint8_t readBit(FILE* fd) {
		if (rPtr >= 8) {
			fread(&rBuff, 1, 1, fd);
			rPtr = 0;
		}
		uint8_t mask = (uint8_t) (1 << rPtr);
		rPtr++;
		return (rBuff & mask) >> (rPtr - 1);
	}
};
