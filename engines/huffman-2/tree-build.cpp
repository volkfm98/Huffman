#include "tree.h"

#include <algorithm>

namespace Huffman2 {
	/**
	 * The arg is [ ( byte => occurences ) ]
	 */
	void Tree::build(std::vector < std::pair <uint8_t, uint32_t> > &freq) {
		std::sort(freq.begin(), freq.end(), [](const std::pair <uint8_t, uint32_t> &a, const std::pair <uint8_t, uint32_t> &b) {
			return (a.second > b.second);
		});
		std::vector <Node*> tree;
		tree.resize(freq.size());
		for (int i = 0; i < freq.size(); i++) {
			tree[i] = new Node();
			tree[i]->data = freq[i].first;
		}

		while (tree.size() > 1) {
			for (int i = 0; i < tree.size(); i += 2) {
				if (i != tree.size() - 1) {
					Node* t = new Node();
					t->left = tree[i];
					t->right = tree[i + 1];
					tree[i] = t;
					tree.erase(tree.begin() + i + 1);
					i--;
				}
			}
		}
		root = tree[0];
		this->buildSymbolMap(this->root, std::vector<bool>());
	}
};
