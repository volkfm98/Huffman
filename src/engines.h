#pragma once

#include "../engines/huffman-old/HuffmanArchive.h"
#include "../engines/huffman-old/HuffmanDearchive.h"

#include "../engines/huffman-2/export.h"

#include <unordered_map>
#include <functional>
#include <string>
#include <initializer_list>
#include <cstdio>

std::unordered_map <
	std::string,
	std::pair <
		std::function <int (
			FILE*,
			FILE*,
			std::unordered_map <std::string, std::string>
		)>,
		std::function <int (
			FILE*,
			FILE*,
			std::unordered_map <std::string, std::string>
		)>
	>
> engines = {
	{
		"huffman-old", 
		{
			HuffmanOldArchive::archive,
			HuffmanOldDearchive::dearchive
		}
	},
	{
		"huffman-2",
		{
			Huffman2::deflate,
			Huffman2::inflate
		}
	}
};

const std::string defaultEngine = "huffman-old";
