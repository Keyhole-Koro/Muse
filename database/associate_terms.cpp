#include <iostream>
#include <string>
#include <vector>

//implement cache by num_linked later
//recall roots()

int ORDER = 3;

typedef struct BPTreeNode BPTRreeNode;
typedef struct Node Node;
typedef struct User User;

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


struct BPTreeNode {
    std::vector<int> keys;
    std::vector<BPTreeNode*> children;
    BPTreeNode *nextLeaf;
    std::vector<Node*> values;  // Vector to store multiple values for each key
    BPTreeNode() : nextLeaf(nullptr), children() {}

};

struct BinaryTreeUser {
	User *referentUser;
	BinaryTreeUser *leftUser;
	BinaryTreeUser *rightUser;
};

struct BinaryTreeNode {
	Node *referentNode;
	BinaryTreeNode *leftNode;
	BinaryTreeNode *rightNode;
};

struct User {
	int key;
	std::string name;
	short unsigned int id;
	std::string name_id;
};

struct Node {
	int key;
	Term *term;
	BinaryTreeUser *users;
	BPTreeNode *referenceRoot;
	short unsigned int num_previousNodes;
	BinaryTreeNode *previousNodes;//before current node
	short unsigned int num_nextNodes;
	BinaryTreeNode *nextNodes;//after current node
};

void linkNode(Node *linkingNode, Node *linkedNode);
bool cmpHash_Term(Node *node1, Node *node2);

Node *makeNode(const Term *term, BinaryTreeUser *BinaryTreeuser);
User *makeUser(std::string name, short unsigned int id, std::string name_id);
BinaryTreeUser *makeBinaryTreeUser(User *user);
BinaryTreeUser *insertuser(BinaryTreeUser *root, bool (cmpMethod)(User*, User*), BinaryTreeUser *expectedBinaryTreeuser);
void linkNodes(Node *node, Node *linkedNode);
void printValueInVector(const std::vector<Node*> &v);

//calculate the sum of characters
int hashStr(const std::string &term) {
	int sum = 0;
	for (char ch : term) {
		sum += static_cast<int>(ch);
	}
	return sum;
}

bool cmpHash_Term(Node *node1, Node *node2) {
	if (!node1 || !node2) return false;
	
	if (node1->key != node2->key) return false;
	return node1->term->str == node2->term->str;
}

bool cmpUserKey_name(User *user1, User *user2) {
	if (!user1 || !user2) return false;
	
	if (user1->key != user2->key) return false;
	return user1->name == user2->name;
}

User *makeUser(std::string name, short unsigned int id, std::string name_id) {
	User *user = new User{hashStr(name), name, id, name_id};
	return user;
}
BinaryTreeUser *makeBinaryTreeUser(User *user) {
	return new BinaryTreeUser{user, nullptr, nullptr};
}

void errorMsg(std::string msg) {
	std::cout << msg << std::endl;
	exit(1);
}

BinaryTreeNode *makeLinkNode(Node *referentNode) {
	return new BinaryTreeNode{referentNode, nullptr, nullptr};
}

//make this abstract
BinaryTreeNode *insertLink(BinaryTreeNode *BiTroot, bool (cmpMethod)(Node*, Node*), BinaryTreeNode *linkedNode) {
	if (BiTroot == nullptr) return linkedNode;
	if (cmpMethod(BiTroot->referentNode, linkedNode->referentNode)) return nullptr;//if it exists already, return nullptr to stop excess calculations

	if (!BiTroot || !linkedNode) return nullptr;
	if (linkedNode->referentNode->key <= BiTroot->referentNode->key) {
		BiTroot->leftNode = insertLink(BiTroot->leftNode, cmpMethod, linkedNode);
	} else if (linkedNode->referentNode->key > BiTroot->referentNode->key) {
		BiTroot->rightNode = insertLink(BiTroot->rightNode, cmpMethod, linkedNode);
	}
	return BiTroot;
}

void linkNodes(Node *linkingNode, Node *linkedNode) {
	insertLink(linkingNode->nextNodes, cmpHash_Term, makeLinkNode(linkedNode));
	linkingNode->num_nextNodes++;

	insertLink(linkedNode->previousNodes, cmpHash_Term, makeLinkNode(linkingNode));
	linkedNode->num_previousNodes++;
}

BinaryTreeUser *insertuser(BinaryTreeUser *BiTroot, bool (cmpMethod)(User*, User*), BinaryTreeUser *expectedBiTuser) {
	if (!BiTroot->referentUser) return expectedBiTuser;

	if (cmpMethod(BiTroot->referentUser, expectedBiTuser->referentUser)) return nullptr;//if it exists already, return nullptr to stop excess calculations
	
	if (!BiTroot || !expectedBiTuser) return nullptr;
	if (expectedBiTuser->referentUser->key <= BiTroot->referentUser->key) {

		BiTroot->leftUser = insertuser(BiTroot->leftUser, cmpMethod, expectedBiTuser);

	} else if (expectedBiTuser->referentUser->key > BiTroot->referentUser->key) {
		BiTroot->rightUser = insertuser(BiTroot->rightUser, cmpMethod, expectedBiTuser);

	}
	return BiTroot;
}

class BPTree {
private:
	BPTreeNode *root_noun = nullptr;
	BPTreeNode *root_verb = nullptr;
	BPTreeNode *root_adjective = nullptr;
	BPTreeNode *root_subject = nullptr;
	BPTreeNode *root_object = nullptr;
	BPTreeNode *root_adverb = nullptr;
	BPTreeNode *root_pronouce = nullptr;
	BPTreeNode *root_interjunciton = nullptr;
	BPTreeNode *root_conjunction = nullptr;
	BPTreeNode *root_determiner = nullptr;
	BPTreeNode *root_preposition  = nullptr;
	BPTreeNode *root_particle = nullptr;
	BPTreeNode *root_auxiliaryverb = nullptr;
	BPTreeNode *root_symbol = nullptr;
	BPTreeNode *root_other = nullptr;

public:
	Node* make_check_insertNode(const Term* term, User* user) {
		BinaryTreeUser* BiTuser = makeBinaryTreeUser(user);
		Node* node = makeNode(term, BiTuser);
		Node* nodeOnTree = insert(node);
		insertuser(nodeOnTree->users, cmpUserKey_name, BiTuser);
		return nodeOnTree;
	}

	Node *insert(Node *node) {
		printf("insert\n");
		BPTreeNode *root = node->referenceRoot;
		if (!root){
			printf("!root\n");
			root = new BPTreeNode();
			root->keys.push_back(node->key);
			root->values.push_back(node);
			return node;
		}
		printf("insert1\n");

		Node *foundNode = searchNodeOnTree(cmpHash_Term, node);
		if (foundNode) {
			printf("Node with key %d already exists. Returning found node.\n", node->key);
			return foundNode;
		}

		printf("insert2\n");

		if (root->keys.size() == (2 * ORDER - 1)) {
			printf("Root is full. Creating a new root.\n");
			BPTreeNode *newRoot = new BPTreeNode();
			newRoot->children.push_back(root);
			splitChild(newRoot, 0);
			root = newRoot;
		}
		printf("just before insertInternal\n");
		insertInternal(root, node);
		return node;
	}

	void insertInternal(BPTreeNode *root, Node *node) {
		printf("insertInternal\n");
		int i = root->keys.size() - 1;

		while (i >= 0 && node->key < root->keys[i]) {
			i--;
		}
		i++;

		if (root->children.empty()) {
			root->keys.insert(root->keys.begin() + i, node->key);
			root->values.insert(root->values.begin() + i, node);

			printValueInVector(root->values);

			if (root->keys.size() == (2 * ORDER)) {
				printf("Leaf node is full. Splitting.\n");
				BPTreeNode *newLeaf = splitChild(root, i);
				newLeaf->nextLeaf = root->nextLeaf;
				root->nextLeaf = newLeaf;
			}
			return;
		}

		if (root->children[i]->keys.size() == (2 * ORDER - 1)) {
			printf("Child node at index %d is full. Splitting.\n", i);
			splitChild(root, i);
			if (node->key > root->keys[i]) i++;
		}
		insertInternal(root->children[i], node);
	}

	BPTreeNode *splitChild(BPTreeNode *parent, int index) {
		printf("splitChild\n");
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


	void traverse(BPTreeNode *parent) {
		if (parent != nullptr) {
			traverseLeafNodes(parent);
		}
		std::cout << std::endl;
	}

	void traverseLeafNodes(BPTreeNode *parent) {
		while (parent != nullptr) {
			for (int key : parent->keys) {
				std::cout << key << " ";
			}
			parent = parent->nextLeaf;
		}
	}

	Node *search(BinaryTreeNode *BiTroot, bool (*cmpMethod)(Node *, Node *), Node *expectedNode) {
		Node *foundNode;
		foundNode = searchNodeOnLinkNode(BiTroot, cmpMethod, expectedNode);
		if (foundNode) return foundNode;

		foundNode = searchNodeOnTree(cmpMethod, expectedNode);
		if (foundNode) return foundNode;

		return nullptr;
	}

	Node *searchNodeOnLinkNode(BinaryTreeNode *BiTroot, bool (*cmpMethod)(Node *, Node *), Node *expectedNode) {
		if (BiTroot == nullptr || cmpMethod(BiTroot->referentNode, expectedNode)) return expectedNode;

		if (expectedNode->key <= BiTroot->referentNode->key) return searchNodeOnLinkNode(BiTroot->leftNode, cmpMethod, expectedNode);
		else return searchNodeOnLinkNode(BiTroot->rightNode, cmpMethod, expectedNode);
	}

	Node *searchNodeOnTree(bool (*cmpMethod)(Node *, Node *), Node *expectedNode) {
   	 	return searchInternal(expectedNode->referenceRoot, cmpMethod, expectedNode);
	}

	Node* searchInternal(BPTreeNode* parent, bool (*cmpMethod)(Node*, Node*), Node *expectedNode) {
		if (parent == nullptr) return nullptr;

		int i = 0;
		while (i < parent->keys.size() && expectedNode->key > parent->keys[i]) {
			i++;
		}
		if (i < parent->keys.size() && expectedNode->key == parent->keys[i]) {
			for (size_t j = 0; j < parent->values.size(); j++) {
				if (cmpMethod(expectedNode, parent->values[j])) return parent->values[j];
			}
		} else if (!parent->children.empty()) return searchInternal(parent->children[i], cmpMethod, expectedNode);
		return nullptr;
	}
	
	BPTreeNode *rootReferent(Classes c) {
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
	
	Node *makeNode(const Term *term, BinaryTreeUser *BiTuser) {
		Node *node = new Node {
			hashStr(term->str),
			const_cast<Term*>(term),
			BiTuser,
			rootReferent(term->word_class),
			0,
			nullptr,
			0,
			nullptr
		};
		return node;
	}
	
	int processTokens(const std::vector<Term>& tokens, User* user) {
		int jumptTimes = 0;
		Node* nextNodeOnTree = make_check_insertNode(&tokens[0], user);
		for (int i = 0; i < tokens.size() - 1; i++) {
			//if (jumptTimes >= ) return;
			Node *curNodeOnTree = nextNodeOnTree;
			nextNodeOnTree = make_check_insertNode(&tokens[i+1], user);

			Node *foundNextNode = nullptr;

			linkNodes(curNodeOnTree, nextNodeOnTree);

			jumptTimes++;
		}
		return jumptTimes;
	}
	
};

void printValueInVector(const std::vector<Node*>& values) {
		printf("Values in vector: ");
		for (const auto& value : values) {
			printf("%d ", value->key);
		}
		printf("\n");
	}



int main() {
    BPTree tree;

    std::vector<Term> input1 = {{"apple", NOUN}, {"banana", NOUN}, {"cherry", NOUN}};
    std::vector<Term> input2 = {{"banana", NOUN}, {"cherry", NOUN}, {"grape", NOUN}};

    User *user1 = makeUser("Alice", 1, "Alice_1");
    User *user2 = makeUser("Bob", 2, "Bob_2");

    tree.processTokens(input1, user1);
    tree.processTokens(input2, user2);

    Node *testNode1 = tree.makeNode(&input1[0], makeBinaryTreeUser(user1));
    Node *testNode2 = tree.makeNode(&input2[2], makeBinaryTreeUser(user2));

    Node *foundNode1 = tree.searchNodeOnTree(cmpHash_Term, testNode1);
    Node *foundNode2 = tree.searchNodeOnTree(cmpHash_Term, testNode2);

    if (foundNode1) {
        std::cout << "Node 1 found in the tree!" << std::endl;
    } else {
        std::cout << "Node 1 not found in the tree." << std::endl;
    }

    if (foundNode2) {
        std::cout << "Node 2 found in the tree!" << std::endl;
    } else {
        std::cout << "Node 2 not found in the tree." << std::endl;
    }

    return 0;
}
//tokenize