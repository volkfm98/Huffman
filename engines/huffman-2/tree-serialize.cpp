#include "tree.h"

#include <initializer_list>

namespace Huffman2 {

	static std::vector <Tree::SerializedNode> sTree;
	static void serializeNode(Tree::Node* c, int16_t i) {
		sTree[i].data = c->data;
		if (c->left) {
			sTree.push_back(Tree::SerializedNode());
			sTree[i].left = (uint16_t) (sTree.size() - 1);
			serializeNode(c->left, sTree[i].left);
		}
		if (c->right) {
			sTree.push_back(Tree::SerializedNode());
			sTree[i].right = (uint16_t) (sTree.size() - 1);
			serializeNode(c->right, sTree[i].right);
		}
	}
	void Tree::serializeIntoStream(FILE* fd) {
		sTree.push_back(SerializedNode());
		serializeNode(root, 0);
		size_t sts = sTree.size();
		fflush(fd);
		fwrite(&sts, sizeof(size_t), 1, fd);
		fflush(fd);
		fwrite(&sTree[0], sizeof(SerializedNode), sTree.size(), fd);
	}
};
