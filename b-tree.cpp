// C++ program for B-Tree insertion
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <time.h>

#include "b-tree.h"

using namespace std;

double decision_factor;

vector<double> tempo_insercao;
vector<double> tempo_rebalanceamento;
vector<double> tempo_non_full;
vector<double> tempo_metricas;
vector<double> tempo_leitura;
vector<double> tempo_calculo;
int qtd_comparacoes_insercao;
bool houve_rebalanceamento;
int f = 0;

int qtdLeituras = 0;
int qtdInsercoes = 0;

// Constructor for BTreeNode class
BTreeNode::BTreeNode(int t1, bool leaf1)
{
	// Copy the given minimum degree and leaf property
	t = t1;
	leaf = leaf1;

	// Allocate memory for maximum number of possible keys
	// and child pointers
	keys = new int[2 * t - 1];
	documents = new Document[2 * t - 1];
	C = new BTreeNode *[2 * t];

	// Initialize the number of keys as 0
	n = 0;
}

// Function to traverse all nodes in a subtree rooted with this node
void BTreeNode::traverse(ifstream &train_file)
{
	// There are n keys and n+1 children, travers through n keys
	// and first n children
	int i;
	for (i = 0; i < n; i++)
	{
		// If this is not leaf, then before printing key[i],
		// traverse the subtree rooted with child C[i].
		if (leaf == false)
			C[i]->traverse(train_file);
		//cout << " Pos: " << documents[i].index;
		vector<double> v = read_at_index(train_file, documents[i].index);
		cout << "V = ( " << v[0] << ", " << v[1] << ") " << endl;
		;
	}

	// Print the subtree rooted with last child
	if (leaf == false)
		C[i]->traverse(train_file);
}

/*
// Function to search key k in subtree rooted with this node 
BTreeNode *BTreeNode::search(double similarity) 
{ 
    // Find the first key greater than or equal to k 
    int i = 0; 
    while (i < n && similarity > documents[i].similarity)
        i++; 
  
    // If the found key is equal to k, return this node 
    if (documents[i].similarity == similarity)
        return this; 
  
    // If key is not found here and this is a leaf node 
    if (leaf == true) 
        return NULL; 
  
    // Go to the appropriate child 
    return C[i]->search(similarity);
} 
*/

Document BTreeNode::most_similar_document(Similarities &sims, int indexI, double greatest_sim_val, Document *greatest_sim_doc)
{
	// Find the first key greater than or equal to k
	int i = 0;
	double similarity;
	int indexJ;

	if (i < n)
	{
		indexJ = documents[0].index;
		similarity = sims.read_element_at(indexI, indexJ);

		while (similarity > decision_factor)
		{
			if (similarity > greatest_sim_val)
			{
				greatest_sim_val = similarity;
				greatest_sim_doc = &documents[i];
			}

			i++;

			if (i < n)
			{
				indexJ = documents[i].index;
				similarity = sims.read_element_at(indexI, indexJ);
			}
			else
			{
				break;
			}
		}
	}

	if (leaf == false)
	{
		return C[i]->most_similar_document(sims, indexI, greatest_sim_val, greatest_sim_doc);
	}
	else
	{
		return *greatest_sim_doc;
	}
}

void BTreeNode::most_similar_documents(Similarities &sims, int indexI, int k, vector<Document *> &greatest_docs, double greatest_sim_val, Document *greatest_sim_doc)
{
	// Find the first key greater than or equal to k
	int i = 0;
	double similarity;
	int indexJ;

	if (i < n)
	{
		indexJ = documents[0].index;
		similarity = sims.read_element_at(indexI, indexJ);
		documents[0].similarity = similarity;
		greatest_docs.push_back(&documents[i]);

		while (similarity > decision_factor)
		{
			if (similarity > greatest_sim_val)
			{
				greatest_sim_val = similarity;
				greatest_sim_doc = &documents[i];
			}

			i++;

			if (i < n)
			{
				indexJ = documents[i].index;
				similarity = sims.read_element_at(indexI, indexJ);
				documents[i].similarity = similarity;

				greatest_docs.push_back(&documents[i]);
			}
			else
			{
				break;
			}
		}
	}

	if (leaf == false)
	{
		C[i]->most_similar_documents(sims, indexI, k, greatest_docs, greatest_sim_val, greatest_sim_doc);
	}
	else
	{
		return;
	}
}

// The main function that inserts a new key in this B-Tree
void BTree::insert(Similarities &sims, int indexI, string doc_class)
{
	double similarity;

	clock_t Ticks[2];
	clock_t Ticks2[2];

	// If tree is empty
	if (root == NULL)
	{
		// Allocate memory for root
		root = new BTreeNode(t, true);
		//root->documents[0].similarity = similarity;  // Insert key
		root->documents[0].index = indexI;
		root->documents[0].doc_class = doc_class;
		root->n = 1; // Update number of keys in root
	}
	else // If tree is not empty
	{
		// If root is full, then tree grows in height
		if (root->n == 2 * t - 1)
		{
			Ticks[0] = clock();

			// Allocate memory for new root
			BTreeNode *s = new BTreeNode(t, false);

			// Make old root as child of new root
			s->C[0] = root;

			// Split the old root and move 1 key to the new root
			s->splitChild(0, root);

			Ticks[1] = clock();
			tempo_rebalanceamento[f] += (Ticks[1] - Ticks[0]) * 1000.0 / CLOCKS_PER_SEC;

			// New root has two children now.  Decide which of the
			// two children is going to have new key
			int i = 0;

			Ticks[0] = clock();
			Ticks2[0] = clock();

			qtd_comparacoes_insercao++;
			int indexJ = s->documents[0].index;
			qtdLeituras++;

			Ticks2[1] = clock();
			tempo_leitura[f] += (Ticks2[1] - Ticks2[0]) * 1000.0 / CLOCKS_PER_SEC;

			Ticks2[0] = clock();

			similarity = sims.read_element_at(indexI, indexJ);

			Ticks2[1] = clock();
			tempo_calculo[f] += (Ticks2[1] - Ticks2[0]) * 1000.0 / CLOCKS_PER_SEC;

			Ticks[1] = clock();
			tempo_metricas[f] += (Ticks[1] - Ticks[0]) * 1000.0 / CLOCKS_PER_SEC;

			if (similarity < decision_factor)
				i++;

			Ticks[0] = clock();

			s->C[i]->insertNonFull(sims, indexI, doc_class);

			Ticks[1] = clock();
			tempo_non_full[f] += (Ticks[1] - Ticks[0]) * 1000.0 / CLOCKS_PER_SEC;

			// Change root
			root = s;
		}
		else // If root is not full, call insertNonFull for root
		{
			Ticks[0] = clock();

			root->insertNonFull(sims, indexI, doc_class);

			Ticks[1] = clock();
			tempo_non_full[f] += (Ticks[1] - Ticks[0]) * 1000.0 / CLOCKS_PER_SEC;
		}
	}
}

// A utility function to insert a new key in this node
// The assumption is, the node must be non-full when this
// function is called
void BTreeNode::insertNonFull(Similarities &sims, int indexI, string doc_class)
{
	// Initialize index as index of rightmost element
	int i = n - 1;
	double similarity;
	vector<double> u;

	clock_t Ticks[2];
	clock_t Ticks2[2];

	// If this is a leaf node
	if (leaf == true)
	{
		if (i >= 0)
		{
			Ticks[0] = clock();
			Ticks2[0] = clock();

			qtd_comparacoes_insercao++;
			int indexJ = documents[i].index;
			qtdLeituras++;

			Ticks2[1] = clock();
			tempo_leitura[f] += (Ticks2[1] - Ticks2[0]) * 1000.0 / CLOCKS_PER_SEC;

			Ticks2[0] = clock();

			similarity = sims.read_element_at(indexI, indexJ);

			Ticks2[1] = clock();
			tempo_calculo[f] += (Ticks2[1] - Ticks2[0]) * 1000.0 / CLOCKS_PER_SEC;

			Ticks[1] = clock();
			tempo_metricas[f] += (Ticks[1] - Ticks[0]) * 1000.0 / CLOCKS_PER_SEC;

			// The following loop does two things
			// a) Finds the location of new key to be inserted
			// b) Moves all greater keys to one place ahead
			while (similarity > decision_factor)
			{
				// documents[i+1].similarity = documents[i].similarity;
				documents[i + 1].index = documents[i].index;
				documents[i + 1].doc_class = documents[i].doc_class;
				i--;
				if (i >= 0)
				{
					Ticks[0] = clock();
					Ticks2[0] = clock();

					qtd_comparacoes_insercao++;
					int indexJ = documents[i].index;
					qtdLeituras++;

					Ticks2[1] = clock();
					tempo_leitura[f] += (Ticks2[1] - Ticks2[0]) * 1000.0 / CLOCKS_PER_SEC;

					Ticks2[0] = clock();

					similarity = sims.read_element_at(indexI, indexJ);

					Ticks2[1] = clock();
					tempo_calculo[f] += (Ticks2[1] - Ticks2[0]) * 1000.0 / CLOCKS_PER_SEC;

					Ticks[1] = clock();
					tempo_metricas[f] += (Ticks[1] - Ticks[0]) * 1000.0 / CLOCKS_PER_SEC;
				}
				else
				{
					break;
				}
			}
		}
		// Insert the new key at found location
		//documents[i+1].similarity = similarity;
		documents[i + 1].index = indexI;
		documents[i + 1].doc_class = doc_class;
		n = n + 1;
	}
	else // If this node is not leaf
	{
		if (i >= 0)
		{
			Ticks[0] = clock();
			Ticks2[0] = clock();

			qtd_comparacoes_insercao++;
			int indexJ = documents[i].index;
			qtdLeituras++;

			Ticks2[1] = clock();
			tempo_leitura[f] += (Ticks2[1] - Ticks2[0]) * 1000.0 / CLOCKS_PER_SEC;

			Ticks2[0] = clock();

			similarity = sims.read_element_at(indexI, indexJ);

			Ticks2[1] = clock();
			tempo_calculo[f] += (Ticks2[1] - Ticks2[0]) * 1000.0 / CLOCKS_PER_SEC;

			Ticks[1] = clock();
			tempo_metricas[f] += (Ticks[1] - Ticks[0]) * 1000.0 / CLOCKS_PER_SEC;

			// Find the child which is going to have the new key
			while (similarity > decision_factor)
			{
				i--;

				if (i >= 0)
				{
					Ticks[0] = clock();
					Ticks2[0] = clock();

					qtd_comparacoes_insercao++;
					int indexJ = documents[i].index;
					qtdLeituras++;

					Ticks2[1] = clock();
					tempo_leitura[f] += (Ticks2[1] - Ticks2[0]) * 1000.0 / CLOCKS_PER_SEC;

					Ticks2[0] = clock();

					similarity = sims.read_element_at(indexI, indexJ);

					Ticks2[1] = clock();
					tempo_calculo[f] += (Ticks2[1] - Ticks2[0]) * 1000.0 / CLOCKS_PER_SEC;

					Ticks[1] = clock();
					tempo_metricas[f] += (Ticks[1] - Ticks[0]) * 1000.0 / CLOCKS_PER_SEC;
				}
				else
				{
					break;
				}
			}
		}

		// See if the found child is full
		if (C[i + 1]->n == 2 * t - 1)
		{
			Ticks[0] = clock();

			// If the child is full, then split it
			splitChild(i + 1, C[i + 1]);

			Ticks[1] = clock();
			tempo_rebalanceamento[f] += (Ticks[1] - Ticks[0]) * 1000.0 / CLOCKS_PER_SEC;

			Ticks[0] = clock();
			Ticks2[0] = clock();

			qtd_comparacoes_insercao++;
			int indexJ = documents[i + 1].index;
			qtdLeituras++;

			Ticks2[1] = clock();
			tempo_leitura[f] += (Ticks2[1] - Ticks2[0]) * 1000.0 / CLOCKS_PER_SEC;

			Ticks2[0] = clock();

			similarity = sims.read_element_at(indexI, indexJ);

			Ticks2[1] = clock();
			tempo_calculo[f] += (Ticks2[1] - Ticks2[0]) * 1000.0 / CLOCKS_PER_SEC;

			Ticks[1] = clock();
			tempo_metricas[f] += (Ticks[1] - Ticks[0]) * 1000.0 / CLOCKS_PER_SEC;

			// After split, the middle key of C[i] goes up and
			// C[i] is splitted into two.  See which of the two
			// is going to have the new key
			if (similarity < decision_factor)
				i++;
		}
		C[i + 1]->insertNonFull(sims, indexI, doc_class);
	}
}

// A utility function to split the child y of this node
// Note that y must be full when this function is called
void BTreeNode::splitChild(int i, BTreeNode *y)
{
	houve_rebalanceamento = true;
	// Create a new node which is going to store (t-1) keys
	// of y
	BTreeNode *z = new BTreeNode(y->t, y->leaf);
	z->n = t - 1;

	// Copy the last (t-1) keys of y to z
	for (int j = 0; j < t - 1; j++)
	{
		//z->documents[j].similarity = y->documents[j + t].similarity;
		z->documents[j].index = y->documents[j + t].index;
		z->documents[j].doc_class = y->documents[j + t].doc_class;
	}

	// Copy the last t children of y to z
	if (y->leaf == false)
	{
		for (int j = 0; j < t; j++)
			z->C[j] = y->C[j + t];
	}

	// Reduce the number of keys in y
	y->n = t - 1;

	// Since this node is going to have a new child,
	// create space of new child
	for (int j = n; j >= i + 1; j--)
		C[j + 1] = C[j];

	// Link the new child to this node
	C[i + 1] = z;

	// A key of y will move to this node. Find location of
	// new key and move all greater keys one space ahead
	for (int j = n - 1; j >= i; j--)
	{
		//documents[j + 1].similarity = documents[j].similarity;
		documents[j + 1].index = documents[j].index;
		documents[j + 1].doc_class = documents[j].doc_class;
	}

	// Copy the middle key of y to this node
	//documents[i].similarity = y->documents[t - 1].similarity;
	documents[i].index = y->documents[t - 1].index;
	documents[i].doc_class = y->documents[t - 1].doc_class;

	// Increment count of keys in this node
	n = n + 1;
}

void BTree::generate_tree(ifstream &train_file, Similarities &sims, const char *classes_file)
{
	clock_t Ticks[2];

	int index;
	vector<double> v;
	string line;
	int line_number = 0;
	vector<string> classes = read_classes(classes_file);

	//Pula a linha 1, que � de cabe�alho
	//getline(train, line);
	//pos = train.tellg();

	for (int i = 0; i < 50; i++)
	{
		tempo_insercao.push_back(0.0);
		tempo_metricas.push_back(0.0);
		tempo_non_full.push_back(0.0);
		tempo_rebalanceamento.push_back(0.0);
		tempo_leitura.push_back(0.0);
		tempo_calculo.push_back(0.0);
	}

	while (getline(train_file, line))
	{
		index = std::atoi(line.c_str());

		tempo_insercao[f] = 0.0;
		tempo_metricas[f] = 0.0;
		tempo_non_full[f] = 0.0;
		tempo_rebalanceamento[f] = 0.0;
		tempo_leitura[f] = 0.0;
		tempo_calculo[f] = 0.0;

		Ticks[0] = clock();

		qtd_comparacoes_insercao = 0;
		houve_rebalanceamento = false;

		insert(sims, index, classes[line_number]);
		qtdInsercoes++;

		Ticks[1] = clock();
		tempo_insercao[f] = (Ticks[1] - Ticks[0]) * 1000.0 / CLOCKS_PER_SEC;
		//print_times(tempo_insercao[f], tempo_rebalanceamento[f], tempo_non_full[f], tempo_metricas[f], tempo_leitura[f], tempo_calculo[f], qtdLeituras, qtdInsercoes, qtd_comparacoes_insercao, houve_rebalanceamento);
		f++;

		if (f == 50)
		{
			//write_times("tempos.txt", tempo_insercao, tempo_rebalanceamento, tempo_non_full, tempo_metricas, tempo_leitura, tempo_calculo, qtdLeituras, qtdInsercoes, qtd_comparacoes_insercao, houve_rebalanceamento);
			f = 0;
		}

		line_number++;
	}
	//traverse();
	//cout << endl;
}