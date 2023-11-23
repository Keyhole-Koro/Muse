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
    BPTreeNode() : nextLeaf(nullptr), children() {}

};

struct BiTUser {
	User *referentUser;
	BiTUser *leftUser;
	BiTUser *rightUser;
};

struct BiTNode {
	Node *referentNode;
	BiTNode *leftNode;
	BiTNode *rightNode;
};

struct Node {
	int key;
	Term *term;
	BiTUser *authors;
	BPTreeNode *referenceRoot;
	short unsigned int num_previousNodes;
	BiTNode *previousNodes;//before current node
	short unsigned int num_nextNodes;
	BiTNode *nextNodes;//after current node
	Node *leftNode;
	Node *rightNode;
};

void linkNode(Node *linkingNode, Node *linkedNode);
bool cmpHash_Term(Node *node1, Node *node2);

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


BPTreeNode *rootReferent(Classes c);
Node *makeNode(const Term *term, BiTUser *bituser);
User *makeUser(std::string name, short unsigned int id, std::string name_id);
BiTUser *makeBiTUser(User *user);
BiTUser *insertAuthor(BiTUser *root, bool (cmpMethod)(User*, User*), BiTUser *expectedbituser);
void linkNodes(Node *node, Node *linkedNode);

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

bool cmpAuthorKey_name(User *user1, User *user2) {
	if (!user1 || !user2) return false;
	
	if (user1->key != user2->key) return false;
	return user1->name == user2->name;
}

class BPTree {
public:
	Node* make_check_insertNode(const Term* term, User* author) {
		BiTUser* bituser = makeBiTUser(author);
		Node* node = makeNode(term, bituser);
		Node* nodeOnTree = insert(node);
		insertAuthor(nodeOnTree->authors, cmpAuthorKey_name, bituser);
		return nodeOnTree;
	}

	Node *insert(Node *node) {
		BPTreeNode *root = node->referenceRoot;
		if (root == nullptr){
			root = new BPTreeNode();
			root->keys.push_back(node->key);
			return node;
		}

		Node *foundNode = searchNodeOnTree(cmpHash_Term, node);
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

	void traverse(BPTreeNode *root) {
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
		if (linkRoot == nullptr || cmpMethod(linkRoot->referentNode, expectedNode)) return expectedNode;

		if (expectedNode->key <= linkRoot->referentNode->key) return searchNodeOnLinkNode(linkRoot->leftNode, cmpMethod, expectedNode);
		else return searchNodeOnLinkNode(linkRoot->rightNode, cmpMethod, expectedNode);
	}

	Node *searchNodeOnTree(bool (*cmpMethod)(Node *, Node *), Node *expectedNode) {
   	 	return searchInternal(expectedNode->referenceRoot, cmpMethod, expectedNode);
	}

	Node* searchInternal(BPTreeNode* bptreenode, bool (*cmpMethod)(Node*, Node*), Node *expectedNode) {
		if (bptreenode == nullptr) return nullptr;

		int i = 0;
		while (i < bptreenode->keys.size() && expectedNode->key > bptreenode->keys[i]) {
			i++;
		}
		if (i < bptreenode->keys.size() && expectedNode->key == bptreenode->keys[i]) {
			for (size_t j = 0; j < bptreenode->values.size(); j++) {
				if (cmpMethod(expectedNode, bptreenode->values[j])) return bptreenode->values[j];
			}
		} else if (!bptreenode->children.empty()) return searchInternal(bptreenode->children[i], cmpMethod, expectedNode);
		return nullptr;
	}

	int processTokens(const std::vector<Term>& tokens, User* author) {
		int jumptTimes = 0;
		Node* nextNodeOnTree = make_check_insertNode(&tokens[0], author);
		for (int i = 0; i < tokens.size() - 1; i++) {
			//if (jumptTimes >= ) return;
			Node *curNodeOnTree = nextNodeOnTree;
			nextNodeOnTree = make_check_insertNode(&tokens[i+1], author);

			Node *foundNextNode = nullptr;

			linkNodes(curNodeOnTree, nextNodeOnTree);

			jumptTimes++;
		}
		return jumptTimes;
	}
};

//make this abstract
BiTNode *insertLink(BiTNode *root, bool (cmpMethod)(Node*, Node*), BiTNode *linkNode) {
	if (root == nullptr) return linkNode;
	if (cmpMethod(root->referentNode, linkNode->referentNode)) return nullptr;//if it exists already, return nullptr to stop excess calculations

	if (!linkNode || !root) return nullptr;
	if (linkNode->referentNode->key <= root->referentNode->key) {
		root->leftNode = insertLink(root->leftNode, cmpMethod, linkNode);
	} else if (linkNode->referentNode->key > root->referentNode->key) {
		root->rightNode = insertLink(root->rightNode, cmpMethod, linkNode);
	}
	return root;
}

BiTNode *makeLinkNode(Node *referentNode) {
	return new BiTNode{referentNode, nullptr, nullptr};
}

void linkNodes(Node *node, Node *linkedNode) {
	insertLink(node->nextNodes, cmpHash_Term, makeLinkNode(linkedNode));
	node->num_nextNodes++;

	insertLink(linkedNode->previousNodes, cmpHash_Term, makeLinkNode(node));
	linkedNode->num_previousNodes++;
}

BiTUser *insertAuthor(BiTUser *root, bool (cmpMethod)(User*, User*), BiTUser *expectedbituser) {
	if (!root->referentUser) return expectedbituser;

	if (cmpMethod(root->referentUser, expectedbituser->referentUser)) return nullptr;//if it exists already, return nullptr to stop excess calculations
	
	if (!expectedbituser || !root) return nullptr;
	if (expectedbituser->referentUser->key <= root->referentUser->key) {

		root->leftUser = insertAuthor(root->leftUser, cmpMethod, expectedbituser);

	} else if (expectedbituser->referentUser->key > root->referentUser->key) {
		root->rightUser = insertAuthor(root->rightUser, cmpMethod, expectedbituser);

	}
	return root;
}

Node *makeNode(const Term *term, BiTUser *bituser) {
    Node *node = new Node {
        hashStr(term->str),
        const_cast<Term*>(term),
        bituser,
        rootReferent(term->word_class),
        0,
        nullptr,
        0,
        nullptr,
        nullptr,
        nullptr
    };
    return node;
}

User *makeUser(std::string name, short unsigned int id, std::string name_id) {
	User *user = new User{hashStr(name), name, id, name_id};
	return user;
}
BiTUser *makeBiTUser(User *user) {
	return new BiTUser{user, nullptr, nullptr};
}

void errorMsg(std::string msg) {
	std::cout << msg << std::endl;
	exit(1);
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

int main() {
    BPTree tree;

    std::vector<Term> input1 = {{"apple", NOUN}, {"banana", NOUN}, {"cherry", NOUN}};
    std::vector<Term> input2 = {{"banana", NOUN}, {"cherry", NOUN}, {"grape", NOUN}};

	User *user1 = makeUser("Alice", 1, "Alice_1");
    User *user2 = makeUser("Bob", 2, "Bob_2");

	// std::vector<std::string> input = {"MeCab","を","使っ","て","日本語","の","テキスト","から","名詞","を","抽出する","サンプルコード","です","。"};
    tree.processTokens(input1, user1);
    tree.processTokens(input2, user2);

	Node *curNode1 = makeNode(&input1[0], makeBiTUser(user1));
    Node *curNode2 = makeNode(&input2[2], makeBiTUser(user2));

	Node *foundNode = tree.searchNodeOnTree(cmpHash_Term, curNode1);

	if (foundNode) printf("true\n");
	else printf("false\n");
	
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