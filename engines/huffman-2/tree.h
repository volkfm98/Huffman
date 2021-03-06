#pragma once

#include <stdint.h>
#include <vector>
#include <cstdio>
#include <unordered_map>

namespace Huffman2 {
	struct Tree {
		struct Node {
			Node *left, *right;
			uint8_t data;
			Node() {}
			Node(uint8_t data) : data(data) {
				left = right = NULL;
			}
			bool isLeaf() {
				return left == NULL && right == NULL;
			}
			bool operator < (const Node &arg) const {
				return data < arg.data;
			}
		};
		struct SerializedNode {
			int16_t left, right;
			uint8_t data;
			SerializedNode() {
				left = right = -1;
			}
			SerializedNode(uint8_t data) {
				left = right = -1;
				this->data = data;
			}
			SerializedNode(uint16_t l, uint16_t r, uint8_t d) {
				left = l;
				right = r;
				data = d;
			}
		};

		Node* root;

		void build(std::vector < std::pair <uint8_t, uint32_t > > &freq);
		void serializeIntoStream(FILE* fd);
		void deserializeFromStream(FILE* fd);

		//std::unordered_map <char, std::vector<bool> > symbolMap;
		std::vector <bool> symbolMap[256];
		void buildSymbolMap(Tree::Node *n, const std::vector<bool> &bits);
		void writeSymbolToStream(const uint8_t &sym, FILE* fd);
		uint8_t readSymbolFromStream(FILE* fd);
	};
};
