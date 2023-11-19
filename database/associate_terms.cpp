#include <iostream>
#include <string>
#include <vector>

/*
implement cache by num_linked later
*/

//recall roots()

int ORDER = 3;

Node *root_verb = nullptr;
Node *root_adjective = nullptr;
Node *root_subject = nullptr;
Node *root_object = nullptr;
Node *root_adverb = nullptr;
Node *root_pronouce = nullptr;
Node *root_interjunciton = nullptr;
Node *root_conjunction = nullptr;
Node *root_determiner = nullptr;
Node *root_preposition  = nullptr;
Node *root_particle = nullptr;
Node *root_auxiliaryverb = nullptr;
Node *root_symbol = nullptr;
Node *root_other;

typedef enum {
	NOUN = 0,
	VERB,
	ADJECTIVE,
	SUBJECT,
	OBJECT,
	ADVERB,
	PRONOUNCE
	INTERJECTION,
	CONJUCTION,
	DETERMINER,
	PREPOSITON,
	PARTICLE,
	AUXILIARYVERB,
	SYMBOL,
} Classes;

struct Term {
	std::string str;
	Classes word_class;
};

struct User {
	int key;
	std::string name;
	unsigned __int16 id;
	std::string name_id;
};

typedef struct Node Node;

struct BPTreeNode {
	std::vector<Node*> keys;
	std::vector<BPTreeNode*> children;
	BPTreeNode *nextLeaf

	BPTreeNode() : nextLeaf(nullptr) {}
};

struct BiTUser {
	User *referentUser;
	BiTUser *leftUser;
	BiTUser *rightUser
}

Node {
	int key;
	Term *term;
	BitUser *authors;
	Node *referenceRoot;
	unsigned __int16 num_previousNodes;
	BiTNode *previousNodes;//before current node
	unsigned __int16 num_nextNodes;
	BiTNode *previousNodes;//after current node
	Node *leftNode;
	Node *rightNode;
};

void linkNode(Node *linkingNode, Node *linkedNode);
bool cmpHash_Term(Node *node1, Node *node2);

//calculate the sum of characters
int hashTerm(const std::string &term) {
	int sum = 0;
	for (char ch : term) {
		sum += static_cast<int>(ch);
	}
	return sum;
}

class BPTree {
public:
	void make_check_insertNode(Term term, User *author) {
\		Node *node = makeNode(term, author);
		Node *foundNode = searchNodeOnTree(cmpTerm_Hash, node);
		if (foundNode) return;
		insertToBPT(node);
	}

	void insert(Node *node) {
		Node *root = node->referenceRoot;
		if (root == nullptr){
			root = new BPTreeNode();
			root->keys.push_back(node->key);
			return;
		}

		if (root->keys.size() == (2 * ORDER - 1)) {
			BPTreeNode *newRoot = new BPTreeNode();
			newRoot->children.push_back(root);
			splitChild(newRoot, 0);
			root = newRoot;
		}
		insertInternal(root, node);
	}

	void insertInternal(BPTreeNode *root, Node *node) {
		int i = root->keys.size()-1;

		while (i >= 0 && node->key < root->keys[i]) {
			i--;
		}
		i++;

		if (root->children.empty()) {
			root->keys.insert(root->keys.begin() + i, node->key);
			if (root->keys.size() == (2 * ORDER)) {
				BPTreeNode *newLeaf = splitChild(root, i);
				newLeaf->nextLeaf = root->nextLeaf;
				root->nextLeaf = newLeaf;
			}
			return;
		}
		if (root->children[i]->keys.size() == (2 * ORDER - 1)) {
			splitChild(root, i);
			if (node->key > root->keys[i]) i++;
		}
		insertInternal(root->children[i], node);
	}

	BPTreeNode *splitChild(BPTreeNode *parent, int index) {
		BPTreeNode *child = parent->children[index];
		BPTreeNode *newChild = new BPTreeNode();

		parent->keys.insert(parent->keys.begin() + index, child->keys[ORDER - 1]);
		parent->children.insert(parent->children.begin() + index + 1, newChild);

		newChild->keys.assign(child->keys.begin() + ORDER, child->keys.end());
		child->keys.erase(child->keys.begin() + ORDER, child->keys.end());

		if (!child->children.empty()) {
			newChild->children.assign(child->children.begin() + ORDER, child->children.end());
			child->children.erase(child->children.begin() + ORDER, child->children.end());
		}
		return newChild;
	}

	

	void traverse(Node *root) {
		if (root != nullptr) {
			traverseLeafNodes(root);
		}
		std::cout << std::endl;
	}

	void traverseLeafNodes(BPTreeNode *node) {
		while (node != nullptr) {
			for (int key : node->keys) {
				std::cout << key << " ";
			}
			node = node->nextLeaf;
		}
	}

	Node *search(BiTNode *linkNode, bool (*cmpMethod)(Node *, Node *), Node *expectedNode) {
		Node *foundNode;
		foundNode = searchNodeOnLinkNode(linkNode, cmpMethod, expectedNode);
		if (foundNode) return foundNode;

		foundNode = searchNodeOnTree(cmpMethod, expectedNode);
		if (foundNode) return foundNode;

		return nullptr;
	}

	Node *searchNodeOnLinkNode(BiTNode *linkRoot, bool (*cmpMethod)(Node *, Node *), Node *expectedNode) {
		if (linkRoot == nullptr || cmpMethod(linkRoot->referentNode, expectedNode)) return linkRoot;

		if (expectedNode->key <= linkRoot->referentNode->key) return searchNodeOnLinkNode(linkRoot->leftNode, cmpMethod, expectedNode);
		else return searchNodeOnLinkNode(linkRoot->rightNode, cmpMethod, expectedNode);
	}

	Node *searchNodeOnTree(bool (*cmpMethod)(Node *, Node *), Node *expected_node) {
   	 	return searchInternal(cmpMethod, expected_node);
	}

	Node *searchInternal(bool (*cmpMethod)(Node *, Node *), Node *expectedNode) {
		BPTreeNode *root = expectedNode->referenceRoot;
  	  	if (root == nullptr) return -1;

    	int i = 0;
    	while (i < root->keys.size() && expectedNode->key > root->keys[i]) {
        	i++;
    	}

    	if (i < root->keys.size() && expectedNode->key == root->keys[i]) {
        	if (cmpMethod(expectedNode, root->nextLeaf[i])) return root->nextLeaf[i];
    	} else if (root->children.empty()) {
       	 	return nullptr;
    	} else {
    	    return searchInternal(root->children[i], key);
    	}
	}

	void processTokens(Term tokens[], User *author) {
		//root is needed to be set
		if (!node->referenceRoot) {
			node->referenceRoot = make_check_insertNode(tokens[0], author);
		}
		int jumptTimes = 0;
		Node *nextNodeOnTree = make_check_insertNode(tokens[0], author);
		for (int i = 0; i < sizeof(tokens)/sizeof(tokens[0])-1; i++) {
			Node *curNodeOnTree = nextNodeOnTree;
			nextNodeOnTree = make_check_insertNode(tokens[i+1], author);

			Node *foundNextNode = nullptr;
			if (nodeOnTree) curNodeOnTree = searchNodeOnLinkNode(curNodeOnTree->linkingNode, cmpHash_Term, nextNodeOnTree);
			if (curNodeOnTree) continue;

			linkNode(NodeOnTree, nextNodeOnTree);

			jumptTimes++;
			std::cout << curNodeOnTree->num_nextNodes << std::endl;
		}
	}
};

BiTNode *makeLinkNode(Node *referenceNode) {
	return new Node{referenceNode, nullptr, nullptr};
}

void linkNode(Node *linkingNode, Node *linkedNode) {
	insertLink(linkingNode->nextNodes, makeLinkNode(linkedNode));
	linkingNode->num_nextNodes++;

	insertLink(linkedNode->previousNodes, makeLinkNode(linkingNode));
	linkedNode->num_previousNodes++;
}

//make this abstract
BiTNode *insertLink(BiTNode *root, BiTNode *linkNode) {
	if (root == nullptr) return linkNode;

	if (linkNode->referenceNode->key <= root->referentNode->key) {
		root->leftNode = insertLink(root->leftNode, linkNode);
	} else if (linkNode->referenceNode->key > root->referentNode->key) {
		referentNode->rightNode = insertLink(root->rightNode, linkNode);
	}
	return root;
}

BitUser *insertAuthor(BitUser *root, BitUser *user) {
	if (root = nullptr) return user;

	if (user->key <= root->key) {
		root->leftNode = insertAuthor(root->leftNode, user);
	} else if (user->key > root->key) {
		referentNode->rightNode = insertAuthor(root->rightNode, user);
	}
	return root;
}

bool cmpHash_Term(Node *node1, Node *node2) {
	if (node1->key != node2->key) return false;
	return node1->term->str == node2->term->str;
}

Node *makeNode(Term *term, BitUser *bituser) {
	Node *node = new Node{hashTerm(term), term, bituser, referenceRoot(term->classes), 0, nullptr, 0, nullptr, -1, nullptr, nullptr};
	return node;
}

User *makeUser(std::string name, int id, std::string name_id) {
	User *user = new User{hashTerm(name+name_id), name, id, name_id};
	return user;
}
BitUser *makeBiTUser(User *user) {
	BiTUser *bituser = new BitUser{user, nullptr, nullptr};
}

void errorMsg(std::string msg) {
	std::cout << msg << std::endl;
	exit(1);
}

Node *referenceRoot(Classes c) {
	if (c == NOUN) return root_noun;
	else if (c == PREPOSITON) return root_preposition;
	else if (c == PARTICLE) return root_particle;
	else if (c == AUXILIARYVERB) return root_auxiliaryverb;
	else if (c == DETERMINER) return root_determiner;
    else if (c == VERB) return root_verb;
	else if (c == ADJECTIVE) return root_adjective;
    else if (c == SUBJECT) return root_subject;
    else if (c == OBJECT) return root_object;
    else if (c == ADVERB) return root_adverb;
    else if (c == PRONOUNCE) return root_pronouce;
    else if (c == INTERJECTION) return root_interjunciton;
    else if (c == CONJUCTION) return root_conjunction;
    else if (c == SYMBOL) return root_symbol;
	return root_other;
	errorMsg("any root doesnt match");
}

int main() {
	BPTree tree;

	Term input1 = [{"apple", NOUN}, {"banana", NOUN}, {"cherry", NOUN}];
	Term input2 = [{"banana", NOUN}, {"cherry", NOUN}, {"grape", NOUN}];

	User *user1 = makeUser("Alice", 1, "Alice_1");
	User *user2 = makeUser("Bob", 2, "Bob_2");

	//std::vector<std::string> input = ["MeCab","を","使っ","て","日本語","の","テキスト","から","名詞","を","抽出する","サンプルコード","です","。"];
	tree.processTokens(input1, user1);
	tree.processTokens(input2, user2);

	Node* curNode1 = makeNode(input1[0], user1);
	Node* curNode2 = makeNode(input2[2], user2);

	NodeData* searchNode = tree.search(curNode1, cmpHash_Term, instance_intializedNode());

	if (searchNode) {
		std::cout << "Found Node: " << searchNode->term << std::endl;
	} else {
		std::cout << "Node not found." << std::endl;
	}

	NodeData* nodeToLink = tree.search(curNode2, cmpHash_Term, instance_intializedNode());
	if (nodeToLink) {
		NodeData* linkingNode = tree.search(curNode2, cmpHash_Term, instance_intializedNode());

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

//tokenize