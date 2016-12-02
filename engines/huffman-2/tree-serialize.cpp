#include "tree.h"

#include <log.h>

namespace Huffman2 {

	static std::vector <Tree::SerializedNode> sTree;
	static void serializeNode(Tree::Node* c, int16_t i) {
		sTree[i].data = c->data;
		sTree[i].left = sTree[i].right = -1;
		if (c->left) {
			sTree.push_back(Tree::SerializedNode());
			sTree[i].left = (int16_t) (sTree.size() - 1);
			serializeNode(c->left, sTree[i].left);
		}
		if (c->right) {
			sTree.push_back(Tree::SerializedNode());
			sTree[i].right = (int16_t) (sTree.size() - 1);
			serializeNode(c->right, sTree[i].right);
		}
	}
	void Tree::serializeIntoStream(FILE* fd) {
		sTree.push_back(SerializedNode());
		serializeNode(root, 0);
		size_t sts = sTree.size();
		fwrite(&sts, sizeof(size_t), 1, fd);
		eprintf("Written %u*%u=%u bytes on meta tree (+%u for size info)\n",
				sts,
				sizeof(SerializedNode),
				sts*sizeof(SerializedNode),
				sizeof(sts));
		fwrite(&sTree[0], sizeof(SerializedNode), sTree.size(), fd);
	}
};
