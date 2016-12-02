#include "tree.h"

namespace Huffman2 {
	static Tree::SerializedNode sTree[512];
	void deserializeNode(Tree::Node *n, size_t i) {
		n->data = sTree[i].data;
		if (sTree[i].left >= 0) {
			n->left = new Tree::Node();
			deserializeNode(n->left, (size_t) sTree[i].left);
		} else {
			n->left = 0;
		}
		if (sTree[i].right >= 0) {
			n->right = new Tree::Node();
			deserializeNode(n->right, (size_t) sTree[i].right);
		} else {
			n->right = 0;
		}
	}
	void Tree::deserializeFromStream(FILE* fd) {
		size_t len;
		fread(&len, sizeof(len), 1, fd);
		fread(sTree, sizeof(SerializedNode), len, fd);
		root = new Node();
		deserializeNode(root, 0);
	}
}