#include <iostream>
#include <string>
#include <vector>

/*
B tree is not adopted but binary tree
cuz the more letters have the more bigger hash
you can search short one(interms of character code), for example, ha,wo(frequently used) faster
but you gotta set the top properly
*/

/*
implement cache later
*/
struct User {
	std::string name;
	int id;
	std::string name_id;
};

struct Node {
	std::string term;
	int hash;
	std::vector<User*> authors;
	std::vector<Node*> linkingNodes;
	Node *leftNode;
	Node *rightNode;
};

void linkNodes(Node *linkingNode, Node *linkedNode);
bool cmpTerm_Hash(Node *node1, Node *node2);
void initializeNode(Node *node);

typedef User UserData;
typedef Node NodeData;

int sum_charNode(const std::string &term) {
	int sum = 0;
	for (char ch : term) {
		sum += static_cast<int>(ch);
	}
	return sum;
}

class treeNode {
private:
	int num_globalNodes;
	Node *root = nullptr;
public:
	Node *make_insertNode(const std::string &term, User *author) {
		Node *node = new Node{term, sum_charNode(term), {author}, {}, nullptr, nullptr};
		insert(root, node);
		return node;
	}

	Node *insert(Node *root, Node *insertedNode) {
		if (root == nullptr) {
			return insertedNode;
		}

		if (insertedNode->hash <= root->hash) {
			root->leftNode = insert(root->leftNode, insertedNode);
		} else if (insertedNode->hash > root->hash) {
			root->rightNode = insert(root->rightNode, insertedNode);
		}

		return root;
	}

	Node *search(Node *referentNode, bool (*cmpMethod)(Node *, Node *), Node *expectedNode) {
		Node *foundNode;
		foundNode = searchNodeOnLinkingNode(referentNode, cmpMethod, expectedNode);
		if (foundNode) return foundNode;

		foundNode = searchNodeOnTree(root, cmpMethod, expectedNode);
		if (foundNode) return foundNode;
		
		return nullptr;
	}

	Node *searchNodeOnLinkingNode(Node *referentNode, bool (*cmpMethod)(Node *, Node *), Node *expectedNode) {
		for (int i = 0; i < referentNode->linkingNodes.size(); i++) {
			Node *node = referentNode->linkingNodes[i];
			if (cmpMethod(node, expectedNode)) std::cout << "true" << std::endl;
			if (cmpMethod(node, expectedNode)) return node;
		}
		return nullptr;
	}

	Node *searchNodeOnTree(Node *root, bool (*cmpMethod)(Node *, Node *), Node *expectedNode) {		
		if (root == nullptr || cmpMethod(root, expectedNode)) return root;

		if (expectedNode->hash <= root->hash) return searchNodeOnTree(root->leftNode, cmpMethod, expectedNode);
		else return searchNodeOnTree(root->rightNode, cmpMethod, expectedNode);
		
	}

	void linkNode(Node *linkingNode, Node *linkedNode) {
		linkingNode->linkingNodes.push_back(linkedNode);
	}

	void processTerms(const std::vector<std::string>& input, User *author) {
		//root is needed to be set
		if (!root) {
			root = make_insertNode(input[0], author);
		}
		for (int i = 0; i < input.size() - 1; i++) {
			Node* curNode = make_insertNode(input[i], author);
			Node* expected_node = make_insertNode(input[i+1], author);

			Node *foundNode;

			foundNode = searchNodeOnLinkingNode(curNode, cmpTerm_Hash, expected_node);
			std::cout << "link " << foundNode << std::endl;
			if (foundNode) continue;

			foundNode = searchNodeOnTree(root, cmpTerm_Hash, expected_node);
			if (foundNode) {
				linkNode(curNode, foundNode);
			} else {
				insert(root, curNode);
			}
			printLinkingNodes(curNode);
		}
	}
	
	void printLinkingNodes(const Node* node) {
		std::cout << "Linking Nodes for " << node->term << ": ";
		for (const Node* linkingNode : node->linkingNodes) {
			std::cout << linkingNode->term << " ";
		}
		std::cout << std::endl;
}
};


void linkNodes(Node *linkingNode, Node *linkedNode) {
	linkingNode->linkingNodes.push_back(linkedNode);
}

bool cmpTerm_Hash(Node *node1, Node *node2) {
	if (node1->hash != node2->hash) return false;
	return node1->term == node2->term;
}

void initializeNode(Node *node) {
	node->term = "";
	node->authors.clear();
	node->linkingNodes.clear();
}

Node *instance_intializedNode() {
	Node *node = new Node{"", -1, {}, {}, nullptr, nullptr};
	return node;
}

Node *makeNode(const std::string &term, User* author) {
	Node *node = new Node{term, sum_charNode(term), {author}, {}, nullptr, nullptr};
	return node;
}

int main() {
	treeNode tree;

	User user1 = {"Alice", 1, "Alice_1"};
	User user2 = {"Bob", 2, "Bob_2"};

	std::vector<std::string> input1 = {"apple", "banana", "cherry"};
	std::vector<std::string> input2 = {"banana", "cherry", "grape"};

	//std::vector<std::string> input = ["MeCab","を","使っ","て","日本語","の","テキスト","から","名詞","を","抽出する","サンプルコード","です","。"];
	tree.processTerms(input1, &user1);
	tree.processTerms(input2, &user2);

	Node* curNode1 = makeNode(input1[0], &user1);
	Node* curNode2 = makeNode(input2[2], &user2);

	NodeData* searchNode = tree.search(curNode1, cmpTerm_Hash, instance_intializedNode());

	if (searchNode) {
		std::cout << "Found Node: " << searchNode->term << std::endl;
	} else {
		std::cout << "Node not found." << std::endl;
	}

	NodeData* nodeToLink = tree.search(curNode2, cmpTerm_Hash, instance_intializedNode());
	if (nodeToLink) {
		NodeData* linkingNode = tree.search(curNode2, cmpTerm_Hash, instance_intializedNode());

		if (linkingNode) {
			tree.linkNode(linkingNode, nodeToLink);
			std::cout << "Nodes linked successfully." << std::endl;
		} else {
			std::cout << "Linking node not found." << std::endl;
		}
	} else {
		std::cout << "Node to link not found." << std::endl;
	}

	return 0;
}

