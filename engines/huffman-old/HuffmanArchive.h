#ifndef HUFFMANARCHIVE_H_
#define HUFFMANARCHIVE_H_

#include <unordered_map>

namespace HuffmanOldArchive {
	int archive(FILE* input, FILE* output, std::unordered_map<std::string, std::string> args);
}

#endif
