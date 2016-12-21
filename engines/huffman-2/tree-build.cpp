#include "tree.h"

#include <algorithm>
#include <set>

namespace Huffman2 {
	/**
	 * The arg is [ ( byte => occurences ) ]
	 */
	void Tree::build(std::vector < std::pair <uint8_t, uint32_t> > &freq) {
//		std::sort(freq.begin(), freq.end(), [](const std::pair <uint8_t, uint32_t> &a, const std::pair <uint8_t, uint32_t> &b) {
//			return (a.second < b.second);
//		});
		std::set <std::pair<int, Node*>> tree;
		for (int i = 0; i < freq.size(); i++) {
			tree.insert({ freq[i].second, new Node(freq[i].first) });
		}

		while (tree.size() > 1) {
			auto a = *tree.begin();
			tree.erase(tree.begin());
			auto b = *tree.begin();
			tree.erase(tree.begin());
			Node *t = new Node();
			t->left = a.second;
			t->right = b.second;
			tree.insert({ a.first + b.first, t });
		}
		root = tree.begin()->second;
		this->buildSymbolMap(this->root, std::vector<bool>());
	}
};
