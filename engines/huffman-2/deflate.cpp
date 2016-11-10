#include "tree.h"
#include "util.h"
#include "stats.h"
#include <util.h>

#include <unordered_map>

namespace Huffman2 {
	int deflate(FILE *in, FILE *out, std::unordered_map<std::string, std::string> args) {
		std::unordered_map <uint8_t, uint32_t> freq;

		char buff[512];
		size_t bytes;
		size_t fullBytes = 0;
		while (bytes = fread(buff, 1, 512, in)) {
			fullBytes += bytes;
			for (int i = 0; i < bytes; i++) {
				freq[(uint8_t) buff[i]]++;
			}
		}
		fseek(in, 0, SEEK_SET);

		std::vector < std::pair<uint8_t, uint32_t> > freqV;
		for (auto i : freq) {
			freqV.push_back(i);
		}

		Tree* ht = new Tree();
		ht->build(freqV);
		ht->serializeIntoStream(out);

		fwrite(&fullBytes, sizeof(fullBytes), 1, out);

		for (int k = 0; k < fullBytes; k++) {
			bytes = fread(buff, 1, 512, in);
			for (int i = 0; i < bytes; i++) {
				ht->writeSymbolToStream((uint8_t) buff[i], out);
			}
		}

		finalizeWrite(out);

		return 0;
	}
}