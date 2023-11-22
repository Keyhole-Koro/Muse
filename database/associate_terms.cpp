#include <iostream>
#include <string>
#include <vector>

//implement cache by num_linked later
//recall roots()

int ORDER = 3;

typedef enum {
	NOUN = 0,
	VERB,
	ADJECTIVE,
	SUBJECT,
	OBJECT,
	ADVERB,
	PRONOUNCE,
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
	short unsigned int id;
	std::string name_id;
};

typedef struct Node Node;

struct BPTreeNode {
    std::vector<int> keys;
    std::vector<BPTreeNode*> children;
    BPTreeNode *nextLeaf;
    std::vector<Node*> values;  // Vector to store multiple values for each key
    BPTreeNode() : nextLeaf(nullptr) {}
};

struct BiTUser {
	User *referentUser;
	BiTUser *leftUser;
	BiTUser *rightUser;
};

struct NodeLink {
	Node *referentNode;
	NodeLink *leftNode;
	NodeLink *rightNode;
};

struct Node {
	int key;
	Term *term;
	BiTUser *authors;
	Node *referenceRoot;
	short unsigned int num_previousNodes;
	NodeLink *previousNodes;//before current node
	short unsigned int num_nextNodes;
	NodeLink *previousNodes;//after current node
	Node *leftNode;
	Node *rightNode;
};

void linkNode(Node *linkingNode, Node *linkedNode);
bool cmpHash_Term(Node *node1, Node *node2);

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
Node *root_other = nullptr;

//calculate the sum of characters
int hashStr(const std::string &term) {
	int sum = 0;
	for (char ch : term) {
		sum += static_cast<int>(ch);
	}
	return sum;
}

bool cmpHash_Term(Node *node1, Node *node2) {
	if (node1->key != node2->key) return false;
	return node1->term->str == node2->term->str;
}

bool cmpAuthorKey_name(User *user1, User *user2) {
	if (user1->key != user2->key) return false;
	return user1->name == user2->name;
}

class BPTree {
public:
	Node *make_check_insertNode(Term term, User *author) {
		Node *node = makeNode(term, author);
		Node *nodeOnTree = insert(node);
		insertAuthor(nodeOnTree->BiTUser, cmpAuthorKey_name, author);
		return nodeOnTree;
	}

	Node *insert(Node *node) {
		Node *root = node->referenceRoot;
		if (root == nullptr){
			root = new BPTreeNode();
			root->keys.push_back(node->key);
			return;
		}

		Node *foundNode = searchNodeOnTree(cmpTerm_Hash, node);
		if (foundNode) return foundNode;

		if (root->keys.size() == (2 * ORDER - 1)) {
			BPTreeNode *newRoot = new BPTreeNode();
			newRoot->children.push_back(root);
			splitChild(newRoot, 0);
			root = newRoot;
		}
		insertInternal(root, node);
		return node;
	}

	void insertInternal(BPTreeNode *root, Node *node) {
		int i = root->keys.size() - 1;

		while (i >= 0 && node->key < root->keys[i]) {
			i--;
		}
		i++;

		if (root->children.empty()) {
			root->keys.insert(root->keys.begin() + i, node->key);
			root->values.insert(root->values.begin() + i, node);

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

	Node* searchInternal(bool (*cmpMethod)(Node*, Node*), Node* expectedNode) {
		BPTreeNode* root = expectedNode->referenceRoot;
		if (root == nullptr) return nullptr;

		int i = 0;
		while (i < root->keys.size() && expectedNode->key > root->keys[i]) {
			i++;
		}
		if (i < root->keys.size() && expectedNode->key == root->keys[i]) {
			for (size_t j = 0; j < root->values.size(); j++) {
				if (cmpMethod(expectedNode, root->values[j])) return root->values[j];
			}
		} else if (!root->children.empty()) return searchInternal(cmpMethod, root->children[i]);
		return nullptr;
	}

	int processTokens(Term tokens[], User *author) {
		int jumptTimes = 0;
		Node *nextNodeOnTree = make_check_insertNode(tokens[0], author);
		for (int i = 0; i < sizeof(tokens)/sizeof(tokens[0])-1; i++) {
			//if (jumptTimes >= ) return;
			Node *curNodeOnTree = nextNodeOnTree;
			nextNodeOnTree = make_check_insertNode(tokens[i+1], author);

			Node *foundNextNode = nullptr;

			linkNode(curNodeOnTree, nextNodeOnTree);

			jumptTimes++;
			std::cout << curNodeOnTree->num_nextNodes << std::endl;
		}
		return jumptTimes;
	}
};

//make this abstract
BiTNode *insertLink(BiTNode *root, bool (cmpMethod)(Node*, Node*), BiTNode *linkNode) {
	if (root == nullptr) return linkNode;
	if (cmpMethod(root->referentNode, linkedNode->referentNode)) return nullptr;//if it exists already, return nullptr to stop excess calculations

	if (linkNode->referenceNode->key <= root->referentNode->key) {
		root->leftNode = insertLink(root->leftNode, linkNode);
	} else if (linkNode->referenceNode->key > root->referentNode->key) {
		root->rightNode = insertLink(root->rightNode, linkNode);
	}
	return root;
}

BiTNode *makeLinkNode(Node *referenceNode) {
	return new Node{referenceNode, nullptr, nullptr};
}

void linkNode(Node *linkingNode, Node *linkedNode) {
	insertLink(linkingNode->nextNodes, cmpHash_Term, makeLinkNode(linkedNode));
	linkingNode->num_nextNodes++;

	insertLink(linkedNode->previousNodes, cmpHash_Term, makeLinkNode(linkingNode));
	linkedNode->num_previousNodes++;
}

BiTUser *insertAuthor(BiTUser *root, bool (cmpMethod)(User*, User*), User *user) {
	if (root->referentUser = nullptr) return user;
	if (cmpMethod(root->referentUser, user->referentUser)) return nullptr;//if it exists already, return nullptr to stop excess calculations

	if (user->key <= root->referentUser->key) {
		root->leftNode = insertAuthor(root->leftNode, cmpMethod, user);
	} else if (user->key > root->referentUser->key) {
		referentNode->rightNode = insertAuthor(root->rightNode, cmpMethod, user);
	}
	return root;
}

Node *makeNode(Term *term, BiTUser *bituser) {
	Node *node = new Node{hashStr(term->str), term, bituser, rootReferent(term->classes), 0, nullptr, 0, nullptr, nullptr, nullptr};
	return node;
}

User *makeUser(std::string name, short unsigned int id, std::string name_id) {
	User *user = new User{hashStr(name), name, id, name_id};
	return user;
}
BitUser *makeBiTUser(User *user) {
	BiTUser *bituser = new BitUser{user, nullptr, nullptr};
}

void errorMsg(std::string msg) {
	std::cout << msg << std::endl;
	exit(1);
}

Node *rootReferent(Classes c) {
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

	Node* curNode1 = makeNode(&input1[0], user1);
	Node* curNode2 = makeNode(&input2[2], user2);

	return 0;
}

//tokenize
/*
#include <iostream>
#include <fstream>

// Function to save Term data to a text file
void saveTermToFile(const Term& term, std::ofstream& file) {
    file << term.str << " " << term.word_class << std::endl;
}

// Function to save User data to a text file
void saveUserToFile(const User& user, std::ofstream& file) {
    file << user.key << " " << user.name << " " << user.id << " " << user.name_id << std::endl;
}

// Function to save BPTreeNode data to a text file
void saveBPTreeNodeToFile(const BPTreeNode& node, std::ofstream& file) {
    for (size_t i = 0; i < node.keys.size(); ++i) {
        file << node.keys[i] << " ";
    }
    file << std::endl;

    // Recursively save children
    for (size_t i = 0; i < node.children.size(); ++i) {
        saveBPTreeNodeToFile(*(node.children[i]), file);
    }

    // Save Node pointers
    for (size_t i = 0; i < node.values.size(); ++i) {
        file << node.values[i]->key << " ";
    }
    file << std::endl;
}

// Function to save BiTUser data to a text file
void saveBiTUserToFile(const BiTUser& bitUser, std::ofstream& file) {
    file << bitUser.referentUser->key << " " << bitUser.leftUser->referentUser->key << " "
         << bitUser.rightUser->referentUser->key << std::endl;
}

// Function to save NodeLink data to a text file
void saveNodeLinkToFile(const NodeLink& nodeLink, std::ofstream& file) {
    file << nodeLink.referentNode->key << " " << nodeLink.leftNode->referentNode->key << " "
         << nodeLink.rightNode->referentNode->key << std::endl;
}

// Function to save Node data to a text file
void saveNodeToFile(const Node& node, std::ofstream& file) {
    file << node.key << " ";
    saveTermToFile(*(node.term), file);
    saveBiTUserToFile(*(node.authors), file);
    file << node.referenceRoot->key << " ";
    file << node.num_previousNodes << " ";
    saveNodeLinkToFile(*(node.previousNodes), file);
    file << node.num_nextNodes << " ";
    saveNodeLinkToFile(*(node.previousNodes), file);
    file << node.leftNode->key << " " << node.rightNode->key << std::endl;
}

int main() {
    // Example data
    Term term1 = {"apple", Noun};
    User user1 = {1, "John", 123, "john123"};
    BPTreeNode bpNode;
    BiTUser bitUser = {&user1, nullptr, nullptr};
    NodeLink nodeLink = {nullptr, nullptr, nullptr};
    Node node = {10, &term1, &bitUser, nullptr, 2, &nodeLink, 3, &nodeLink, nullptr, nullptr};

    // Save data to a text file
    std::ofstream outFile("data.txt");
    saveTermToFile(term1, outFile);
    saveUserToFile(user1, outFile);
    saveBPTreeNodeToFile(bpNode, outFile);
    saveBiTUserToFile(bitUser, outFile);
    saveNodeLinkToFile(nodeLink, outFile);
    saveNodeToFile(node, outFile);

    outFile.close();

    return 0;
}
*/