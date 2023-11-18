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

//recall roots()

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
	std::string name;
	unsigned __int16 id;
	std::string name_id;
};

typedef struct Node Node;

struct BiTNode {
	Node *referentNode;
	BiTNode *leftNode;
	BiTNode *rightNode;
};
//BinaryTree BT
struct BiTUser {
	User *referentUser;
	BiTUser *leftUser;
	BiTUser *rightUser
}

//make vectors binary tree
Node {
	Term *term;
	User *authors;
	Node *referenceRoot;
	unsigned __int16 num_linkedNodes;
	BiTNode *linkedNodes;//before current node
	unsigned __int16 num_linkingNodes;
	BiTNode *linkingNodes;//after current node
	int hash;
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

class treeNode {
public:
	Node *make_check_insertNode(Term term, User *author) {
		Node *node = makeNode(term, author);
		Node *foundNode = searchNodeOnTree(node->referentNode, cmpTerm_Hash, node);
		if (foundNode) return foundNode;
		else return insert(node->referenceRoot, node);
	}

	Node *insert(Node *root, Node *insertedNode) {
		if (root == nullptr) return insertedNode;

		if (insertedNode->hash <= root->hash) {
			root->leftNode = insert(root->leftNode, insertedNode);
		} else if (insertedNode->hash > root->hash) {
			root->rightNode = insert(root->rightNode, insertedNode);
		}

		return root;
	}

	Node *search(BiTNode *linkNode, bool (*cmpMethod)(Node *, Node *), Node *expectedNode) {
		Node *foundNode;
		foundNode = searchNodeOnLinkNode(linkNode, cmpMethod, expectedNode);
		if (foundNode) return foundNode;

		foundNode = searchNodeOnTree(node->referenceRoot, cmpMethod, expectedNode);
		if (foundNode) return foundNode;

		return nullptr;
	}

	Node *searchNodeOnLinkNode(BiTNode *linkRoot, bool (*cmpMethod)(Node *, Node *), Node *expectedNode) {
		if (linkRoot == nullptr || cmpMethod(linkRoot->referentNode, expectedNode)) return linkRoot;

		if (expectedNode->hash <= linkRoot->referentNode->hash) return searchNodeOnTree(linkRoot->leftNode, cmpMethod, expectedNode);
		else return searchNodeOnTree(linkRoot->rightNode, cmpMethod, expectedNode);
	}

	Node *searchNodeOnTree(Node *root, bool (*cmpMethod)(Node *, Node *), Node *expectedNode) {		
		if (root == nullptr || cmpMethod(root, expectedNode)) return root;

		if (expectedNode->hash <= root->hash) return searchNodeOnTree(root->leftNode, cmpMethod, expectedNode);
		else return searchNodeOnTree(root->rightNode, cmpMethod, expectedNode);
	}

	void processTokens(Term tokens[], User *author) {
		//root is needed to be set
		if (!node->referenceRoot) {
			node->referenceRoot = make_check_insertNode(tokens[0], author);
		}
		int jumptTimes = 0;
		Node *curNode = make_check_insertNode(tokens[0], author);
		Node *nextNode = curNode;
		for (int i = 0; i < sizeof(tokens)/sizeof(tokens[0])-1; i++) {
			curNode = nextNode;
			nextNode = make_check_insertNode(tokens[i+1], author);

			Node *foundNode;
			foundNode = searchNodeOnLinkNode(curNode->linkingNode, cmpHash_Term, nextNode);
			if (foundNode) continue;

			linkNode(curNode, nextNode);

			jumptTimes++;
			std::cout << curNode->num_linkingNode << std::endl;
			//printLinkingNodes(curNode);
		}
	}
	//this doesnt work
	void printLinkingNodes(const Node* node) {
		std::cout << "Linking Nodes for " << node->term << ": ";
		for (const Node* linkingNode : node->linkingNodes) {
			std::cout << linkingNode->term << " ";
		}
		std::cout << std::endl;
	}
	Node* getRoot() const {
		return root_;
	}

};

void printTree(Node* root) {
	if (root != nullptr) {
		printTree(root->leftNode);
		std::cout << "Term: " << root->term->str << ", Hash: " << root->hash << std::endl;
		std::cout << "Authors: ";
		for (const User* author : root->authors) {
			std::cout << author->name << " ";
		}
		std::cout << std::endl;
		std::cout << "Linking Nodes: ";
		for (const Node* linkingNode : root->linkingNodes) {
			std::cout << linkingNode->term->str << " ";
		}
		std::cout << std::endl << std::endl;
		printTree(root->rightNode);
	}
}

BiTNode *makeLinkNode(Node *referenceNode) {
	return new Node{referenceNode, nullptr, nullptr};
}

void linkNode(Node *linkingNode, Node *linkedNode) {
	insertLink(linkingNode->linkingNodes, makeLinkNode(linkedNode));
	linkingNode->num_linkingNode++;

	insertLink(linkedNode->linkedNodes, makeLinkNode(linkingNode));
	linkedNode->num_linkedNodes++;
}

BiTNode *insertLink(BiTNode *root, BiTNode *linkNode) {
	if (root == nullptr) return linkNode;

	if (linkNode->referenceNode->hash <= root->referentNode->hash) {
		root->leftNode = insertLink(root->leftNode, linkNode);
	} else if (linkNode->referenceNode->hash > root->referentNode->hash) {
		referentNode->rightNode = insertLink(root->rightNode, linkNode);
	}
	return root;
}

bool cmpHash_Term(Node *node1, Node *node2) {
	if (node1->hash != node2->hash) return false;
	return node1->term->str == node2->term->str;
}

Node *makeNode(Term *term, User *author) {
	Node *node = new Node{term, hashTerm(term), author, referenceRoot(term->classes), 0, nullptr, 0, nullptr, -1, nullptr, nullptr};
	return node;
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
	treeNode tree;

	User user1 = {"Alice", 1, "Alice_1"};
	User user2 = {"Bob", 2, "Bob_2"};

	Term input1 = [{"apple", NOUN}, {"banana", NOUN}, {"cherry", NOUN}];
	Term input2 = [{"banana", NOUN}, {"cherry", NOUN}, {"grape", NOUN}];

	//std::vector<std::string> input = ["MeCab","を","使っ","て","日本語","の","テキスト","から","名詞","を","抽出する","サンプルコード","です","。"];
	tree.processTokens(input1, &user1);
	tree.processTokens(input2, &user2);

	printTree(tree.getRoot());

	Node* curNode1 = makeNode(input1[0], &user1);
	Node* curNode2 = makeNode(input2[2], &user2);

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