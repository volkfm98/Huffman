#include "tree.h"
#include "util.h"
#include "stats.h"

#include <unordered_map>

namespace Huffman2 {
	int inflate(FILE *in, FILE *out, std::unordered_map<std::string, std::string> args) {
		Tree *ht = new Tree();
		ht->deserializeFromStream(in);

		size_t len;
		fread(&len, sizeof(len), 1, in);

		uint8_t sym;
		for (int i = 0; i < len; i++) {
			sym = ht->readSymbolFromStream(in);
			fwrite(&sym, 1, 1, out);
		}

		return 0;
	}
}