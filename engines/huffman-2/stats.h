#pragma once

#include <stdint.h>

namespace Huffman2 {
	struct {
		uint32_t bitsWritten;
		uint32_t metaBytes;
		uint32_t dataBytes;
	} stats;
};
