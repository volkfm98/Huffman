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

struct Node {
	Node *l, *r;
	unsigned char symbol;
};

static Node bor[511];

bool codeCmp(Code a, Code b) {
	return a.length < b.length;
}

static BinaryBuffer binBuff;

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

	unsigned char length = 0;
	Node *curNode;
	int ptr = 1;

	unsigned char bit = 0;

	for (unsigned int i = 0; i < 256; ++i) {
		for (unsigned int j = 0; j < 8; ++j) {
			readBit(input, &length, j);
		}

		curNode = &bor[0];

		for (unsigned int j = 0; j < length; ++j) {
			readBit(input, &bit, 7);

			if (bit) {
				if ((*curNode).r == NULL) {
					(*curNode).r = &bor[ptr];
					++ptr;
				}

				curNode = (*curNode).r;
			} else {
				if ((*curNode).l == NULL) {
					(*curNode).l = &bor[ptr];
					++ptr;
				}

				curNode = (*curNode).l;
			}
		}

		(*curNode).symbol = i;
	}
}

static int translate(FILE* input, FILE* output) {
	unsigned char bit;
	Node* curNode = &bor[0];

	for (unsigned int i = 0; i < 257; ++i) {		
		if ((*curNode).l == NULL && (*curNode).r == NULL) {
			fwrite(&(*curNode).symbol, 1, 1, output);

			return 0;
		}

		readBit(input, &bit, 7);

		if (bit) {
			curNode = (*curNode).r;
		} else {
			curNode = (*curNode).l;
		}
	}
}

int dearchive(FILE* input, FILE* output, std::unordered_map<std::string, std::string> args) {
	readMetadata(input);

	for (unsigned long long i = 0; i < symbolCnt; ++i) {
		translate(input, output);
	}
}
}
