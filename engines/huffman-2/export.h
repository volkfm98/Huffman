#pragma once

namespace Huffman2 {
	int deflate(FILE* in, FILE* out, std::unordered_map<std::string, std::string> args);
	int inflate(FILE* in, FILE* out, std::unordered_map<std::string, std::string> args);
}