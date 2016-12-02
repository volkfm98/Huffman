#include "tree.h"
#include "util.h"

namespace Huffman2 {
	void Tree::buildSymbolMap(Tree::Node *n, const std::vector<bool> &bits) {
		if (n->left == NULL && n->right == NULL) {
			symbolMap[n->data] = bits;
		} else {
			std::vector <bool> bitsCopy = bits;
			if (n->left) {
				bitsCopy.push_back(false);
				buildSymbolMap(n->left, bitsCopy);
				bitsCopy.erase(bitsCopy.end() - 1);
			}
			if (n->right) {
				bitsCopy.push_back(true);
				buildSymbolMap(n->right, bitsCopy);
				bitsCopy.erase(bitsCopy.end() - 1);
			}
		}
	}

	void Tree::writeSymbolToStream(const uint8_t &sym, FILE* fd) {
		const std::vector <bool> &bits = symbolMap[sym];
		for (int i = 0; i < bits.size(); i++) {
			writeBit(fd, (uint8_t) bits[i]);
		}
	}

	uint8_t Tree::readSymbolFromStream(FILE* fd) {
		Node *cur = root;
		while (cur->left != NULL || cur->right != NULL) {
			uint8_t bit = readBit(fd);
			if (bit) {
				cur = cur->right;
			} else {
				cur = cur->left;
			}
		}
		return cur->data;
	}
}