// C++ program for B-Tree insertion 
#include<iostream> 
#include<vector>
#include<string>
#include <tuple>
#include <fstream>

#include "reading.h"
#include "metrics.h"

using namespace std;

//Limiar para decidir se vai para a esquerda ou direita
extern double decision_factor;

typedef struct document
{
	//Posicao no arquivo de treinos em que o dado se encontra
	int index;
	string doc_class;
	double similarity = 0;
} Document;

// A BTree node 
class BTreeNode
{
	Document *documents;
	int *keys;  // An array of keys 
	int t;      // Minimum degree (defines the range for number of keys) 
	BTreeNode **C; // An array of child pointers 
	int n;     // Current number of keys 
	bool leaf; // Is true when node is leaf. Otherwise false 
public:
	BTreeNode(int _t, bool _leaf);   // Constructor 

									 // A utility function to insert a new key in the subtree rooted with 
									 // this node. The assumption is, the node must be non-full when this 
									 // function is called 
	void insertNonFull(ifstream& train_file, vector<double> v, int index, string doc_class);

	// A utility function to split the child y of this node. i is index of y in 
	// child array C[].  The Child y must be full when this function is called 
	void splitChild(int i, BTreeNode *y);

	// A function to traverse all nodes in a subtree rooted with this node 
	void traverse(ifstream& train_file);

	// A function to search a key in subtree rooted with this node. 
	//BTreeNode *search(double similarity);   // returns NULL if k is not present. 

								// Make BTree friend of this so that we can access private members of this 
								// class in BTree functions 

	Document most_similar_document(ifstream& train_file, vector<double> v, double maior, Document* greatest_sim_doc);

	void most_similar_documents(ifstream& train_file, vector<double> v, int k, vector<Document*>& greatest_docs, double maior = 0, Document* greatest_sim_doc = nullptr);

	friend class BTree;
};

// A BTree 
class BTree
{
	BTreeNode *root; // Pointer to root node 
	int t;  // Minimum degree 
public:
	BTree() {}
	// Constructor (Initializes tree as empty) 
	BTree(int _t, double decision_factor)
	{
		root = NULL;  
		t = _t;
		decision_factor = decision_factor;
	}

	// function to traverse the tree 
	void traverse(ifstream& train_file)
	{
		if (root != NULL) root->traverse(train_file);
	}

	/*
	// function to search a key in this tree 
	BTreeNode* search(double similarity)
	{
		return (root == NULL) ? NULL : root->search(similarity);
	}
	*/

	Document most_similar_document(ifstream& train_file, vector<double> v, double maior = 0, Document* greatest_sim_doc = nullptr)
	{
		vector<double> u = read_at_index(train_file, root->documents[0].index);
		double similarity = cosine_similarity(v, u);

		return root->most_similar_document(train_file, v, similarity, &root->documents[0]);
	}

	void most_similar_documents(ifstream& train_file, vector<double> v, int k, vector<Document*>& greatest_docs, double maior = 0, Document* greatest_sim_doc = nullptr)
	{
		vector<double> u = read_at_index(train_file, root->documents[0].index);

		double similarity = cosine_similarity(v, u);

		root->documents[0].similarity = similarity;
		greatest_docs.push_back(&root->documents[0]);

		return root->most_similar_documents(train_file, v, k, greatest_docs, similarity, &root->documents[0]);
	}

	//Document BTree::most_similar_document(char* train_file, vector<double> v);

	// The main function that inserts a new key in this B-Tree 
	void insert(ifstream& train_file, vector<double> v, int index, string doc_class);

	void generate_tree(ifstream& train_file, const char* classes_file);
};
