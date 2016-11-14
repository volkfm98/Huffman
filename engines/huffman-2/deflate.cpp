#include "tree.h"
#include "util.h"
#include "stats.h"
#include <util.h>

#include <unordered_map>

namespace Huffman2 {
	int deflate(FILE *in, FILE *out, std::unordered_map<std::string, std::string> args) {
		clock_t deflateBegin = clock();
		std::unordered_map <uint8_t, uint32_t> freq;

		clock_t firstReadBegin = clock();
		eprintf("Reading data from input stream... ");
		char buff[bufSize];
		size_t bytes;
		size_t fullBytes = 0;
		while (bytes = fread(buff, 1, bufSize, in)) {
			fullBytes += bytes;
			for (int i = 0; i < bytes; i++) {
				freq[(uint8_t) buff[i]]++;
			}
		}
		fseek(in, 0, SEEK_SET);
		double firstReadSeconds = (double)(clock() - firstReadBegin) / CLOCKS_PER_SEC;
		eprintf("done, spent %.1lf s, read performance %.1lf MB/s\n", firstReadSeconds, fullBytes / ((1 << 20) * firstReadSeconds));

		std::vector < std::pair<uint8_t, uint32_t> > freqV;
		for (auto i : freq) {
			freqV.push_back(i);
		}

		clock_t hufBegin = clock();
		eprintf("Building huffman tree...\n");
		Tree* ht = new Tree();
		ht->build(freqV);
		ht->serializeIntoStream(out);
		eprintf("Done building tree, spent %.2lf s\n", (double)(clock() - hufBegin) / CLOCKS_PER_SEC);

		fwrite(&fullBytes, sizeof(fullBytes), 1, out);

		clock_t readBegin = clock();
		eprintf("Reading & transpiling... ");
		for (int k = 0; k < fullBytes; k++) {
			bytes = fread(buff, 1, bufSize, in);
			for (int i = 0; i < bytes; i++) {
				ht->writeSymbolToStream((uint8_t) buff[i], out);
			}
		}
		eprintf("done, spent %.2lf s\n", (double)(clock() - readBegin) / CLOCKS_PER_SEC);

		finalizeWrite(out);

		double deflateSeconds = (double)(clock() - deflateBegin) / CLOCKS_PER_SEC;
		eprintf("Deflate done in %.2lf seconds, archiving speed %.1lf MB/s\n", deflateSeconds, fullBytes /((1 << 20) * deflateSeconds));

		return 0;
	}
}