#include <cstdio>
#include <algorithm>

#include <vector>
#include <queue>
#include <unordered_map>

#include "HuffmanArchive.h"
#include "HuffmanDearchive.h"

using std::priority_queue;
 
extern size_t eprintf(const char* format, ...);
extern bool outputTree;
 
namespace HuffmanOldArchive {
	struct BinatyBuffer {
		unsigned char ptr, buffer;
	};
	 
	struct Node {
		Node *l, *r, *parent;
		unsigned long long weight;
		unsigned char sym;
	};

	struct NodeCmpStruct {
		bool operator()(Node*& a, Node*& b) {
			return (*a).weight > (*b).weight;
		}
	};
	 
	struct HuffmanTree {
		Node tree[511];
	};

	struct Code {
		unsigned char length;
		unsigned char chunks[32];
	};
 
	static std::priority_queue <Node*, std::vector<Node*>, NodeCmpStruct> que;
	static HuffmanTree hufTree;

	static Code codeTable[256];

	static BinatyBuffer binBuff;

	static unsigned int metadataSize = 256 * 8;
	static unsigned long long symbolCnt = 0;
	static unsigned long long bitsWritten = 0;
	 
	static int buildStats(FILE* input) {
		unsigned char buff;
	 
		for (int i = 0; i < 256; ++i) {
			hufTree.tree[i].sym = i;
			hufTree.tree[i].weight = 0;
			hufTree.tree[i].l = NULL;
			hufTree.tree[i].r = NULL;
			hufTree.tree[i].parent = NULL;
		}

		while (fread(&buff, 1, 1, input)) {
			++hufTree.tree[buff].weight;
		}

		for (int i = 0; i < 256; ++i) {
			if (hufTree.tree[i].weight != 0) {
				que.push(&hufTree.tree[i]);
			}

			symbolCnt += hufTree.tree[i].weight;
		}
	 
		eprintf("Stats has been built\n");

		return 0;
	}
	 
	static int buildHuffmanTree(FILE* input) {
		buildStats(input);
		
		Node *firstMin, *secondMin;
		unsigned int ptr = 256;

		while (que.size() > 1) {
			firstMin = que.top();
			que.pop();
			secondMin = que.top();
			que.pop();

			hufTree.tree[ptr].l = firstMin;
			hufTree.tree[ptr].r = secondMin;
			hufTree.tree[ptr].parent = NULL;
			hufTree.tree[ptr].weight = (*firstMin).weight + (*secondMin).weight;

			(*firstMin).parent = &(hufTree.tree[ptr]);
			(*secondMin).parent = &(hufTree.tree[ptr]);

			que.push(&(hufTree.tree[ptr]));

			++ptr;
		}

		fseek(input, 0, SEEK_SET);

		eprintf("Tree has been built\n");
	}
	
	static int buildCodeTable() {
		Node* curRoot;
		unsigned char mask;

		for (unsigned int i = 0; i < 256; ++i) {
			curRoot = &hufTree.tree[i];
			mask = (unsigned char)(1 << 7);

			while ((*curRoot).parent != NULL) {
				if (curRoot == (*(*curRoot).parent).r) {
					codeTable[i].chunks[codeTable[i].length / 8] = (unsigned char)((unsigned char)codeTable[i].chunks[codeTable[i].length / 8] | mask);
				}

				++codeTable[i].length;
			
				curRoot = (*curRoot).parent;

				mask >>= 1;

				if (mask == 0) {
					mask = (unsigned char)(1 << 7);
				}
			}

			metadataSize += codeTable[i].length;
		}

		eprintf("Table has been built\n");
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

	static int writeCodeLength(FILE* output, unsigned char c) {
		for (int i = 0; i < 8; ++i) {
			writeBit(output, (unsigned char)((unsigned char)(codeTable[c].length << i) >> 7));
		}
	}

	static int writeCode(FILE* output, unsigned char c) {
		for (int i = codeTable[c].length - 1; i >= 0; --i) {
			writeBit(output, (unsigned char)((unsigned char)(codeTable[c].chunks[i / 8] << i % 8) >> 7));
		}
	}

	static int writeMetadata(FILE* output) {
		fwrite(&metadataSize, 4, 1, output);
		fwrite(&symbolCnt, 8, 1, output);

		for (unsigned int i = 0; i < 256; ++i) {
			writeCodeLength(output, (unsigned char)i);
			writeCode(output, (unsigned char)i);
		}

		eprintf("Metadata has been written\n");
	}
	 
	int archive(FILE* input, FILE* output, std::unordered_map<std::string, std::string> arg) {
		buildHuffmanTree(input);
		buildCodeTable();

		binBuff.buffer = 0;
		binBuff.ptr = 0;

		writeMetadata(output);
		eprintf("Metadata bits: %d\n", bitsWritten);
		bitsWritten = 0;
	
		eprintf("Starting translation...   ");

		unsigned char buff;
			
		while (fread(&buff, 1, 1, input)) {
			writeCode(output, buff);
		}
		eprintf("Code bits: %d\n", bitsWritten);
		bitsWritten = 0;

		if (binBuff.ptr != 0) {
			writeLastBuffer(output);
		}
		eprintf("Last bits: %d\n", bitsWritten);
		bitsWritten = 0;

		eprintf("Done\n");
	}
};
