#include <cstdio>
#include <algorithm>

#include "HuffmanArchive.h"
 
extern size_t eprintf(const char* format, ...);
 
struct BinatyBuffer {
	unsigned char ptr, buffer;
};
 
struct Symbol {
	unsigned long long weight;
	unsigned char sym;
};
 
bool symbolCmp(Symbol a, Symbol b) {
	return a.weight < b.weight;
}
 
struct HuffmanTree {
	unsigned int tree[511][3];
	unsigned long long weight[511];
};
 
static unsigned long long symbolToWeight[256];
static Symbol stats[256];
 
static HuffmanTree hufTree;

static BinatyBuffer binBuff;

static unsigned long long symbolCnt = 0;

static unsigned long long bitsWritten = 0;
 
static int buildStats(FILE* input) {
	unsigned char buff;
 
	for (int i = 0; i < 256; ++i) {
		stats[i].sym = i;
		stats[i].weight = 0;
	}

	while (fread(&buff, 1, 1, input)) {
		++stats[buff].weight;
	}
 
	for (int i = 0; i < 256; ++i) {
		symbolToWeight[i] = stats[i].weight;
		symbolCnt += stats[i].weight;
	}
 
	std::sort(stats, stats + 256, symbolCmp);
 
	return 0;
}
 
static int buildHuffmanTree(FILE* input) {
	buildStats(input);
 
	hufTree.tree[0][0] = 0;
	hufTree.tree[0][1] = 0;
	hufTree.tree[0][2] = (unsigned char)stats[0].sym;
	hufTree.weight[0] = stats[0].weight;
 
	for (int i = 1; i < 511; i += 2) {
		hufTree.tree[i][0] = 0;
		hufTree.tree[i][1] = 0;
		hufTree.tree[i][2] = (unsigned char)stats[i / 2 + 1].sym;

		hufTree.weight[i] = stats[i / 2 + 1].weight;
 
		hufTree.tree[i + 1][0] = i - 1;
		hufTree.tree[i + 1][1] = i;
		hufTree.weight[i + 1] = hufTree.weight[i - 1] + hufTree.weight[i];
	}

	fseek(input, 0, SEEK_SET);
}
 
static int writeBit(FILE* output, unsigned char bit) {
	if ((unsigned char)((unsigned char)(binBuff.buffer << binBuff.ptr) >> 7) != bit) {
		unsigned char mask = (unsigned char)((unsigned char)((unsigned char)1 << 7) >> binBuff.ptr);

		if ((unsigned char)(binBuff.buffer | mask) == binBuff.buffer) {
			binBuff.buffer = (unsigned char)~((unsigned char)((unsigned char)~binBuff.buffer) | mask);
		} else {
			binBuff.buffer = (unsigned char)(binBuff.buffer | mask);
		}
	}

		++binBuff.ptr;
 	
	if (binBuff.ptr > 7) {
		fwrite(&binBuff.buffer, 1, 1, output);

		binBuff.ptr = 0;
	}
	bitsWritten++;
 
	return 1;
}

static int writeLastBuffer(FILE* output) {
	for (unsigned char i = 0; i < 8; ++i) {
		writeBit(output, 0);
	}
}

static int writeSym(FILE* output, unsigned char c) {
	for (unsigned char i = 0; i < 8; ++i) {
		writeBit(output, (i >> (7 - i)) << 7);
	}
}

static int writeCodeLength(FILE* output, unsigned char c) {
	unsigned char length = 0;

	for (unsigned int i = 0; i < 256; ++i) {
		if (stats[i].sym == c) {
			length = (unsigned char)(256 - i);

			if (i == 0) {
				length = 255;
			}

			for (unsigned int j = 0; j < 8; ++j) {
				writeBit(output, (unsigned char)(((unsigned char)(length << j)) >> 7));
			}
		}
	}
}

static int writeCode(FILE* output, unsigned char c) {
	unsigned int curNode = 510;
	unsigned char leaf = 1;
	
	while (!(hufTree.tree[curNode][0] == 0 && hufTree.tree[curNode][1] == 0)) {
		if ((unsigned char)hufTree.tree[hufTree.tree[curNode][leaf]][2] == c) {
			curNode = hufTree.tree[curNode][leaf];
			
			writeBit(output, leaf);
		} else {
			curNode = hufTree.tree[curNode][1 - leaf];

			writeBit(output, 1 - leaf);
		}
	}
}

static int writeMetadata(FILE* output) {
	unsigned int metadataSize = 35039;
	fwrite(&metadataSize, 4, 1, output);
	fwrite(&symbolCnt, 8, 1, output);

	for (unsigned int i = 0; i < 256; ++i) {
		writeCodeLength(output, (unsigned char)i);
		writeCode(output, (unsigned char)i);
	}
}
 
int archive(FILE* input, FILE* output) {
	buildHuffmanTree(input);
	
	binBuff.buffer = 0;
	binBuff.ptr = 0;

	writeMetadata(output);
	eprintf("Metadata bits: %d\n", bitsWritten);
 
	unsigned char buff;
		
	while (fread(&buff, 1, 1, input)) {
		writeCode(output, buff);
	}
	eprintf("Code bits: %d\n", bitsWritten);

	if (binBuff.ptr != 0) {
		writeLastBuffer(output);
	}
	eprintf("Last bits: %d\n", bitsWritten);
}
