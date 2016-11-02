#include <cstdio>
#include <algorithm>
#include <unordered_map>

#include "HuffmanDearchive.h"

namespace HuffmanOldDearchive {
	struct BinaryBuffer {
		unsigned char buffer, ptr;
	};

	struct Code {
		unsigned char length, chunks[32], symbol;
	};

	bool codeCmp(Code a, Code b) {
		return a.length < b.length;
	}

	static BinaryBuffer binBuff;

	static Code alphabet[256];

	static unsigned int metaSize;
	static unsigned long long symbolCnt;

	static int readBit(FILE* input, unsigned char* data, unsigned char pos) {
		unsigned char bit = (unsigned char)((unsigned char)(binBuff.buffer << binBuff.ptr) >> 7);

		if ((unsigned char)((unsigned char)(*data << pos) >> 7) != bit) {
			unsigned char mask = (unsigned char)((unsigned char)(1 << 7) >> pos);

			if ((unsigned char)(*data | mask) == *data) {
				*data = (unsigned char)(~((unsigned char)(~(*data)) | mask));
			} else {
				*data = (unsigned char)(*data | mask);
			}
		}

		++binBuff.ptr;

		if (binBuff.ptr > 7) {
			binBuff.ptr = 0;
			return fread(&binBuff.buffer, 1, 1, input);
		} else {
			return 1;
		}
	}

	static int readMetadata(FILE* input) {
		fread(&metaSize, 4, 1, input);
		fread(&symbolCnt, 8, 1, input);

		fread(&binBuff.buffer, 1, 1, input);

		for (unsigned int i = 0; i < 256; ++i) {
			for (unsigned int j = 0; j < 8; ++j) {
				readBit(input, &alphabet[i].length, j);
			}

			for (unsigned int j = 0; j < alphabet[i].length; ++j) {
				readBit(input, &alphabet[i].chunks[j / 8], j % 8);
			}

			alphabet[i].symbol = i;
		}

		std::sort(alphabet, alphabet + 256, codeCmp);
	}

	static int translate(FILE* input, FILE* output) {
		unsigned char buff[32];
		int flag;

		for (unsigned int i = 0; i < 32; ++i) {
			buff[i] = 0;
		}

		for (unsigned int i = 0; i < 256; ++i) {
			if (i != 255) {
				readBit(input, &buff[i / 8], i % 8);
			}

			flag = 1;

			for (int j = 0; j < 32; ++j) {
				flag = flag && alphabet[i].chunks[j] == buff[j];
			}

			if (flag) {
				fwrite(&alphabet[i].symbol, 1, 1, output);

				return 1;
			}
		}
	}

	int dearchive(FILE* input, FILE* output, std::unordered_map<std::string, std::string> args) {
		readMetadata(input);

		for (unsigned long long i = 0; i < symbolCnt; ++i) {
			translate(input, output);
		}
	}
};
